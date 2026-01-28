#version 450
#extension GL_ARB_separate_shader_objects : enable
/// NOTE :: i hate that I cant find the way for arrays atm, to manually add vars by index is dumb...
///			i have tried, and am able to pass uniform arrays, but I cannot seem to call the correct index....
///			HELP ...... i need somebody....help.....not just anybody......HEEEEEEEE EEEEE LLLLP *insert guitar intro*
layout(location = 0) out vec4 fragColor;			/// color 

layout(location = 0) in vec3 vertNormal;
layout(location = 1) in vec3 light_direction_one;
layout(location = 2) in vec3 eyeDir; 
layout(location = 3) in vec2 textureCoords; 
// lIGHTS
layout(location = 5) in vec3 LIGHT_DIRECTION[2];   /// light direction - ARRAY
/// Color uniforms
			/// Array
uniform vec4 SPEC[4];
uniform vec4 DIFF[4];
uniform vec4 AMB[4];

uniform int index_0;
uniform int index_1;

vec3 REFLECTION_INCEDENT[4];
float DIFFUSE_DOT_PRODUCT[4];
float SPECULAR_DOT_PRODUCT[4];


/// color specular exponent values
uniform float spec_exponent_value_0;
uniform float spec_exponent_value_1;
uniform float spec_exponent_value_2;
uniform float spec_exponent_value_3;
uniform float spec_exponent_value_4;
uniform float SPEC_EXPONENT_VALUE[4];

//uniform sampler2D myTexture; 

void main() {
//  vec4 ks = vec4(0.3, 0.3, 0.3, 0.0);
//	vec4 kd = vec4(0.30, 0.30, 0.30, 0.0);
//	vec4 ka = 0.50 * kd;
//	vec4 kt = texture(myTexture,textureCoords); 
			
				/// Reflection is based incedent which means a vector from the light source
				/// not the direction to the light source so flip the sign
							// Array 0
	DIFFUSE_DOT_PRODUCT[0] = max(dot(vertNormal,LIGHT_DIRECTION[0]), 0.0);
	REFLECTION_INCEDENT[0] = normalize(reflect(-LIGHT_DIRECTION[0], vertNormal));		// ARRAY TEST
	SPECULAR_DOT_PRODUCT[0] = max(dot(eyeDir, REFLECTION_INCEDENT[0]), 0.0);			// ARRAY TEST
	SPECULAR_DOT_PRODUCT[0] = pow(SPECULAR_DOT_PRODUCT[0],14.0);						// ARRAY TEST
	
	float DIFFUSE_DOT = max(dot(vertNormal,LIGHT_DIRECTION[0]), 0.0);
	vec3 REFLECTION_INC = normalize(reflect(-LIGHT_DIRECTION[0], vertNormal));			// inside shader test
	float SPECULAR_DOT = max(dot(eyeDir, REFLECTION_INC), 0.0);							// inside shader test
	SPECULAR_DOT = pow(SPECULAR_DOT,14.0);							// inside shader test
	
						 // Array 1
	DIFFUSE_DOT_PRODUCT[1] = max(dot(vertNormal,LIGHT_DIRECTION[1]), 0.0);
	REFLECTION_INCEDENT[1] = normalize(reflect(-LIGHT_DIRECTION[1], vertNormal));
	SPECULAR_DOT_PRODUCT[1] = max(dot(eyeDir,REFLECTION_INCEDENT[1]), 0.0);
	SPECULAR_DOT_PRODUCT[1] = pow(SPECULAR_DOT_PRODUCT[1], 14.0);

	// turn these var(s) into arrays ...
	float DIFF_DOT = max(dot(vertNormal,LIGHT_DIRECTION[1]), 0.0);
	vec3 REF_INC = normalize(reflect(-LIGHT_DIRECTION[1], vertNormal));
	float SPEC_DOT = max(dot(eyeDir,REF_INC), 0.0);
	SPEC_DOT = pow(SPEC_DOT , 5.0);

	

	//	NOTE:: Im using the <SPECULAR_DOT_PRODUCT && DIFFUSE_DOT_PRODUCT> as an array , pay f******* attention !!
	
	/// dRAW
	vec4 color0 = (AMB[0] + (DIFFUSE_DOT_PRODUCT[0] * DIFF[0]) + (SPECULAR_DOT_PRODUCT[0] * SPEC[0]));
	vec4 color1 = (AMB[1] + (DIFFUSE_DOT_PRODUCT[1] * DIFF[1]) + (SPECULAR_DOT_PRODUCT[1] * SPEC[1]));
	
	
	fragColor = color0 + color1;
						/// Texture
//	fragColor =  (ka + (diff * kd) + (spec * ks)) * kt; 

}