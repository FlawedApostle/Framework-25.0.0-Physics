#version 450
#extension GL_ARB_separate_shader_objects : enable

in vec2 uvCoordFromEval;
in vec3 normalFromEval;
out vec4 fragColor;

// bring in from evaluator
in vec3 eye_Dir_from_eval;
in vec3 light_Dir_from_eval;
in float fog_val_from_eval;

// sending to the evaluator
uniform vec3 lightposition;             /// uniform light position 

layout(binding = 1)uniform sampler2D terrainDiffuse_map;
layout(binding = 2)uniform sampler2D normal_Map;

/// terrain color
//uniform vec4 dColor ;
//uniform vec4 color_diffuse;


void main() {
    // uniform this 
    vec4 dColor = vec4(0.1, 0.1, 0.1, 1.0);                // fog color
    vec4 color_diffuse = vec4(0.53, 0.81, 0.92, 1.0);         // terrain color
    vec4 color_specular = 0.2 * color_diffuse;
    vec4 Color_ambient = 0.5 * color_diffuse;
    vec4 kt = texture(terrainDiffuse_map,uvCoordFromEval);
    vec3 normal = texture(normal_Map,uvCoordFromEval).xyz * 2.0 - 1.0;
    /// LIGHTING
    float diff = max(dot(light_Dir_from_eval, normal),0.0); 
    vec3 reflection = reflect(-light_Dir_from_eval , normal);
//    vec3 reflection = normalize(reflect(-light_Dir_from_eval,normal));    // OG
    float spec = pow(max(dot(eye_Dir_from_eval, normal) , 0.0) , 25.0);
//    float spec = max(dot(eye_Dir_from_eval, normal),0.0);                 // OG
//    spec = pow(spec,25.0);                                                // OG
    vec4 color = (Color_ambient + (diff * color_diffuse) + (spec * color_specular)) * kt;
    
    // bring in the fog value from the evaluator coming from the evaluator where it is set
    fragColor = mix(dColor, color, fog_val_from_eval);

//    fragColor = vec4(vec3(fog_val_from_eval), 1.0);
    
}