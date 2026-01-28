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
layout(location = 4) in vec3 light_direction_two;  /// light direction
layout(location = 5) in vec3 LIGHT_DIRECTION[2];   /// light direction - ARRAY
layout(location = 8) in vec3 light_direction_three;    /// light direction
layout(location = 9) in vec3 light_direction_four;    /// light direction
/// Color uniforms
uniform vec4 k_specular0;
uniform vec4 k_diffuse0;
uniform vec4 k_ambient0;
uniform vec4 k_specular1;
uniform vec4 k_diffuse1;
uniform vec4 k_ambient1;
uniform vec4 k_specular2;
uniform vec4 k_diffuse2;
uniform vec4 k_ambient2;
uniform vec4 k_specular3;
uniform vec4 k_diffuse3;
uniform vec4 k_ambient3;
/// color specular exponent values
uniform float spec_exponent_value_0;
uniform float spec_exponent_value_1;
uniform float spec_exponent_value_2;
uniform float spec_exponent_value_3;
uniform float spec_exponent_value_4;

//uniform sampler2D myTexture; 

void main() {
//  vec4 ks = vec4(0.3, 0.3, 0.3, 0.0);
//	vec4 kd = vec4(0.30, 0.30, 0.30, 0.0);
//	vec4 ka = 0.50 * kd;
//	vec4 kt = texture(myTexture,textureCoords); 

	float diffuse_one = max(dot(vertNormal, light_direction_one), 0.0);
	float diffuse_two = max(dot(vertNormal, light_direction_two), 0.0);
	float diffuse_three = max(dot(vertNormal, light_direction_three), 0.0);
	float diffuse_four = max(dot(vertNormal, light_direction_four), 0.0);


	/// Reflection is based incedent which means a vector from the light source
	/// not the direction to the light source so flip the sign
	vec3 reflection_incedent_one = normalize(reflect(-light_direction_one, vertNormal));
	vec3 reflection_incedent_two = normalize(reflect(-light_direction_two, vertNormal));
	vec3 reflection_incedent_three = normalize(reflect(-light_direction_three, vertNormal));
	vec3 reflection_incedent_four = normalize(reflect(-light_direction_four, vertNormal));

	float specular_one = max(dot(eyeDir, reflection_incedent_one), 0.0);
	specular_one = pow(specular_one,spec_exponent_value_1);
	float specular_two = max(dot(eyeDir, reflection_incedent_two), 0.0);
	specular_two = pow(specular_two,spec_exponent_value_2);
	float specular_three = max(dot(eyeDir, reflection_incedent_three), 0.0);
	specular_three = pow(specular_three,spec_exponent_value_2);
	float specular_four = max(dot(eyeDir, reflection_incedent_four), 0.0);
	specular_four = pow(specular_four,spec_exponent_value_3);

	
//	vec4 color1 = (ka + (diffuse_one * kd) + (specular_one * ks));
	vec4 color2 = (k_ambient0 + (diffuse_one * k_diffuse0) + (specular_one * k_specular0));
	vec4 color3 = (k_ambient1 + (diffuse_two * k_diffuse1) + (specular_two * k_specular1));
	vec4 color4 = (k_ambient2 + (diffuse_three * k_diffuse2) + (specular_three * k_specular2));
	vec4 color5 = (k_ambient3 + (diffuse_four * k_diffuse3) + (specular_four * k_specular3));
	fragColor =   color2 + color3 + color4 + color5;

//	fragColor =  (ka + (diffuse_one * kd) + (specular_one * ks) + (diffuse_two * kd) + (specular_two * ks));
//	fragColor =  (ka + (diffuse_one * kd + diffuse_two * kd) + (specular_one * ks + specular_two * ks)); 
//	fragColor =  (k_ambient0 + (diffuse_two * k_diffuse0) + (specular_two * k_specular0)); 
// kt is for the texture
//	fragColor =  (ka + (diff * kd) + (spec * ks)) * kt; 



/////////////////////////////////////////////////////////////////////////////////////////////
							/// Arrays , dont work...
//	float diffuse_three = max(dot(vertNormal, LIGHT_DIRECTION[0]), 0.0);
//	float diffuse_four = max(dot(vertNormal, LIGHT_DIRECTION[1]), 0.0);
							/// Arrays , dont work...
//	vec3 reflection_incedent_three = normalize(reflect(-LIGHT_DIRECTION[0], vertNormal));
//	vec3 reflection_incedent_four = normalize(reflect(-LIGHT_DIRECTION[1], vertNormal));
							/// Arrays , dont work...
//	float specular_three = max(dot(eyeDir, reflection_incedent_three), 0.0);
//	specular_three = pow(specular_three,14.0);
//	float specular_four = max(dot(eyeDir, reflection_incedent_four), 0.0);
//	specular_four = pow(specular_four,14.0);
/////////////////////////////////////////////////////////////////////////////////////////////
}