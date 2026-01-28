#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec4 vVertex;           /// object surface vertex
layout(location = 1) in vec3 vNormal;           /// object surface vertex normal
//layout(location = 2) in vec2 uvCoord;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

layout(location = 2) out vec3 vertNormal;
layout(location = 3) out vec3 eyeDir;
//layout(location = 2) out vec2 textureCoords; 

out vec3 final_Angle_Of_Reflection;

//uniform vec3 LIGHT_POSITION[2];

                /// NOTES
/// snells law = n1(sin(0)) = n2(sin(0)) 
/// Incidence (n1) = LIGHT_POSITION
/// Incidence Ray (n2) = inverse(cross(eyeDir , Normal))
/// Angle_Of_Incidence = dot(LIGHT_POSITION , eyedir)
/// Angle_Of_Reflection = dot(Light_Position , -eyeDir);
/// color = Incidence * Angle_Of_Incidence *= Incidence Ray * Angle_Of_Reflection

void main() {
//    textureCoords = uvCoord;
    mat3 normalMatrix = mat3(transpose(inverse(modelMatrix)));
    vertNormal = normalize(normalMatrix * vNormal); /// Rotate the normal to the correct orientation 
    vec3 vertPos = vec3(viewMatrix * modelMatrix * vVertex);
    vec3 vertDir = normalize(vertPos);
    eyeDir = -vertDir;

    /// FIX THE MATH, ITS WRONG !
    /// n1(sin0) = n2(sin0) - (0 == theta)
    /// Taking the eyeDir and normalize , this is now the original incident ray
    /// Finding the angle (Dot product) Of the newly created incident ray and the surface Normal
    ///  - Updating the incident ray vector with the incidence theta angle
    /// Taking the original incidence ray and using the GLSL reflect function to find the reflect incidence ray
    /// Finding the angle (Dot product) of the newly created reflection incident ray and the surface Normal
    ///  - Updating the reflecting incident ray vector by multipling with the reflection theta
    /// Final Angle Of Reflection is passed out from the vert to the frag shader , (reflection final * incidence final)
                    /// Incidence
    vec3  incidence_ray = normalize(eyeDir);
    float incidence_theta = dot(incidence_ray , vertNormal);
    vec3  incidence_final = incidence_ray * incidence_theta;
                    /// Reflection
    vec3  incidence = normalize(reflect(incidence_ray, vertNormal));
    vec3  reflection = -incidence;
    float reflection_theta = dot(reflection , vertNormal);
    vec3 reflection_final = reflection * reflection_theta;
                    /// Final
	final_Angle_Of_Reflection = reflection_final + incidence_final;
    gl_Position = projectionMatrix * viewMatrix * modelMatrix * vVertex;

                               
    
}
