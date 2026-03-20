#version 450
#extension GL_ARB_separate_shader_objects : enable
/// This is the new Vertex Shader file - The Evaluator it is sent from the TessVert
layout (triangles, equal_spacing, ccw) in;
/// Mat(s) for Phong Lighting
uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;
/// Map(s) Tessellation - normal , height
layout(binding = 0) uniform sampler2D heightMapText;
layout(binding = 2) uniform sampler2D normal_Map;

/// LightPosition
uniform vec3 lightposition;             // ADD A LIGHT POSITION

/// Shader - Frag Shader
out vec3 light_Dir_from_eval;
out vec3 eye_Dir_from_eval;
out float fog_val_from_eval;

// Shader - Control
in vec2 uvCoordFromCtrl[];
in vec3 normalFromCtrl[];

/// Shader - Evaluator
out vec2 uvCoordFromEval;
out vec3 normalFromEval;


vec2 interpolateVec2(vec2 v0, vec2 v1, vec2 v2) {
    return gl_TessCoord.x * v0 + gl_TessCoord.y * v1 + gl_TessCoord.z * v2;
}

vec3 interpolateVec3(vec3 v0, vec3 v1, vec3 v2) {
    return  gl_TessCoord.x * v0 + gl_TessCoord.y * v1 + gl_TessCoord.z * v2;
}

vec4 interpolateVec4(vec4 v0, vec4 v1, vec4 v2) {
    return  gl_TessCoord.x * v0 + gl_TessCoord.y * v1 + gl_TessCoord.z * v2;
}

const float fog_density = 25.0;
const float fog_gradiant =10.0;

void main() {
   
   // Coord coming from the evaluator which is equal to the inerpolate vec3(s)
    uvCoordFromEval = interpolateVec2(uvCoordFromCtrl[0], uvCoordFromCtrl[1], uvCoordFromCtrl[2]);
    normalFromEval =  interpolateVec3(normalFromCtrl[0], normalFromCtrl[1], normalFromCtrl[2]);
    /// Interpolate position across the triangle
    vec4 position = interpolateVec4(gl_in[0].gl_Position, gl_in[1].gl_Position, gl_in[2].gl_Position);

    vec4 height = vec4(texture(heightMapText,uvCoordFromEval));
    position.z =  height.r * 3.0;       /// add uniform to scale height

     // calculate phong
     // NOTE: Taking in a vec4 position (line 50) the vertices interpolated amongst the plane (obj used)
     vec3 vertPos = vec3(viewMatrix * modelMatrix * position);
     vec3 vertDir = normalize(vertPos);
     eye_Dir_from_eval = -vertDir;
     light_Dir_from_eval = normalize(lightposition - vertPos);
        
    /// Fog
    /// ||A||^2 - ||B||^2
//     float distance = length(vertPos);
     float vertPos_Distance = length(vertPos);
     light_Dir_from_eval = normalize(lightposition - vertPos);

     // fog equation = d * p / gradient
     // -d = distance 
     //  p = density
     //  gradient  
//    fog_val_from_eval = clamp(pow(vertPos_Distance / fog_gradiant, fog_density), 0.0, 1.0);             // working 12:58 2025-07-04         
    fog_val_from_eval = clamp(pow(vertPos_Distance * fog_gradiant, fog_density), 0.0, 1.0);
//    fog_val_from_eval = clamp(1.0 - exp(-pow(vertPos_Distance * fog_density, fog_gradiant)), 0.0, 1.0);

//    float fogStart = -12.0;
//    float fogEnd = 100.0;
//    fog_val_from_eval = clamp((vertPos_Distance - fogStart) / (fogEnd - fogStart), 0.0, 1.0);
    
    gl_Position =  projectionMatrix * viewMatrix * modelMatrix * position ;
}