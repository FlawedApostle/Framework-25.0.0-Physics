#version 450
#extension GL_ARB_separate_shader_objects : enable
/// NOTE :: i hate that I cant find the way for arrays atm, to manually add vars by index is dumb...
///			i have tried, and am able to pass uniform arrays, but I cannot seem to call the correct index....
///			HELP ...... i need somebody....help.....not just anybody......HEEEEEEEE EEEEE LLLLP *insert guitar intro*
layout(location = 0) out vec4 fragColor;			/// color 

layout(location = 0) in vec3 vertNormal;
layout(location = 1) in vec3 eyeDir; 
layout(location = 2) in vec2 textureCoords; 
// lIGHTS
layout(location = 3) in vec3 LIGHT_DIRECTION[2];   /// light direction - ARRAY
/// Color uniforms
			/// Array
uniform vec4 SPEC[4];
uniform vec4 DIFF[4];
uniform vec4 AMB[4];

vec3 REFLECTION_INCEDENT[4];
float DIFFUSE_DOT_PRODUCT[4];
float SPECULAR_DOT_PRODUCT[4];


/// color specular exponent values
uniform float SPEC_EXPONENT_VALUE[4];

//uniform sampler2D myTexture; 

void main() {
							// Array 0
	DIFFUSE_DOT_PRODUCT[0] = max(dot(vertNormal,LIGHT_DIRECTION[0]), 0.0);
	REFLECTION_INCEDENT[0] = normalize(reflect(-LIGHT_DIRECTION[0], vertNormal));		// ARRAY TEST
	SPECULAR_DOT_PRODUCT[0] = max(dot(eyeDir, REFLECTION_INCEDENT[0]), 0.0);			// ARRAY TEST
	SPECULAR_DOT_PRODUCT[0] = pow(SPECULAR_DOT_PRODUCT[0],75.0);						// ARRAY TEST
	
	
						 // Array 1
	DIFFUSE_DOT_PRODUCT[1] = max(dot(vertNormal,LIGHT_DIRECTION[1]), 0.0);
	REFLECTION_INCEDENT[1] = normalize(reflect(-LIGHT_DIRECTION[1], vertNormal));
	SPECULAR_DOT_PRODUCT[1] = max(dot(eyeDir,REFLECTION_INCEDENT[1]), 0.0);
	SPECULAR_DOT_PRODUCT[1] = pow(SPECULAR_DOT_PRODUCT[1], 75.0);

	

	//	NOTE:: Im using the <SPECULAR_DOT_PRODUCT && DIFFUSE_DOT_PRODUCT> as an array , pay f******* attention !!
	
	/// dRAW
	vec4 color0 = (AMB[0] + (DIFFUSE_DOT_PRODUCT[0] * DIFF[0])  + (SPECULAR_DOT_PRODUCT[0] * SPEC[0]));
	vec4 color1 = (AMB[1] + (DIFFUSE_DOT_PRODUCT[1] * DIFF[1])  + (SPECULAR_DOT_PRODUCT[1] * SPEC[1]));
	
	
	fragColor = color0 + color1;
						/// Texture
//	fragColor =  (ka + (diff * kd) + (spec * ks)) * kt; 

}