#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) out vec4 fragColor;


layout(location = 0) in vec3 vertNormal;
layout(location = 3) in vec3 eyeDir; 
layout(location = 4) in vec2 textureCoords; 
layout(location = 5) in vec3 LIGHT_DIRECTION[2];

uniform sampler2D myTexture; 

uniform vec4 SPEC[4];
uniform vec4 DIFF[4];
uniform vec4 AMB[4];

float DIFFUSE_DOT[4];
vec3  REFLECTION_INCEDENT[4];
float SPECULAR_DOT[4];
uniform int SPECULAR_EXPONENT[4];

void main() {
//    vec4 ks = vec4(0.3, 0.3, 0.3, 0.0);
//	vec4 kd = vec4(0.6, 0.6, 0.3, 0.0);
//	vec4 ka = 0.1 * kd;
	vec4 kt = texture(myTexture,textureCoords); 


//	float diff = max(dot(vertNormal, LIGHT_DIRECTION[0]), 0.0);
//	vec3 reflection = normalize(reflect(-LIGHT_DIRECTION[0], vertNormal));
//	float spec = max(dot(eyeDir, reflection), 0.0);
//	spec = pow(spec,14.0);

	DIFFUSE_DOT[0] = max(dot(vertNormal, LIGHT_DIRECTION[0]), 0.0);
	REFLECTION_INCEDENT[0] = normalize(reflect(-LIGHT_DIRECTION[0], vertNormal));
	SPECULAR_DOT[0] = max(dot(eyeDir, REFLECTION_INCEDENT[0]), 0.0);
	SPECULAR_DOT[0] = pow(SPECULAR_DOT[0],SPECULAR_EXPONENT[0]);

	DIFFUSE_DOT[1] = max(dot(vertNormal, LIGHT_DIRECTION[1]), 0.0);
	REFLECTION_INCEDENT[1] = normalize(reflect(-LIGHT_DIRECTION[1], vertNormal));
	SPECULAR_DOT[1] = max(dot(eyeDir, REFLECTION_INCEDENT[1]), 0.0);
	SPECULAR_DOT[1] = pow(SPECULAR_DOT[1],SPECULAR_EXPONENT[0]);

		/// colors
	vec4 color0 = (AMB[0] + (DIFFUSE_DOT[0]  * DIFF[0]) + (SPECULAR_DOT[0] * SPEC[0])) * kt;	
	vec4 color1 = (AMB[1] + (DIFFUSE_DOT[1]  * DIFF[1]) + (SPECULAR_DOT[1] * SPEC[1])) * kt;	
//	vec4 color1 = (AMB[1] + (diff * DIFF[1]) + (spec * SPEC[1])) * kt;	
		/// draw
	fragColor =  color0 + color1;
}