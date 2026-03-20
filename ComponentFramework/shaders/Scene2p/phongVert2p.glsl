#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec4 vVertex;
layout(location = 1) in vec3 vNormal;
layout(location = 2) in vec2 uvCoord;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;
uniform vec3 LIGHT_POSITION[2];             // uniform name , call the correct uniform name including index
uniform vec3 Light_Position0;

layout(location = 0) out vec3 vertNormal;
layout(location = 1) out vec3 Light_Direction0;
layout(location = 2) out vec3 eyeDir;
layout(location = 3) out vec2 textureCoords; 
layout(location = 4) out vec3 LIGHT_DIRECTION[2];


void main() {
    textureCoords = uvCoord;
    mat3 normalMatrix = mat3(transpose(inverse(modelMatrix)));
    vertNormal = normalize(normalMatrix * vNormal); /// Rotate the normal to the correct orientation 
    vec3 vertPos = vec3(viewMatrix * modelMatrix * vVertex);
    vec3 vertDir = normalize(vertPos);
    eyeDir = -vertDir;
    
    Light_Direction0 = normalize(vec3(Light_Position0) - vertPos); 
    LIGHT_DIRECTION[0] = normalize(vec3(LIGHT_POSITION[0]) - vertPos); 
    LIGHT_DIRECTION[1] = normalize(vec3(LIGHT_POSITION[1]) - vertPos); 
   
   
   gl_Position = projectionMatrix * viewMatrix * modelMatrix * vVertex;
    
}
