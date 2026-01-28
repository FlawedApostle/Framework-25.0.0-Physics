#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) out vec4 fragColor;			/// color 

layout(location = 2) in vec3 vertNormal;
layout(location = 3) in vec3 eyeDir; 


uniform samplerCube skybox;

in vec3 final_Angle_Of_Reflection;

//vec3 REFLECTION_INCEDENT[4];

void main() {

	fragColor = vec4(texture(skybox, final_Angle_Of_Reflection ).rgb,1.0);

}