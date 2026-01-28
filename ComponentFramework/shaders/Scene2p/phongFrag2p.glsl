#version 450
#extension GL_ARB_separate_shader_objects : enable

/// NOTE :: I finally figured the array syntax to call the index
///			I am however, still don't feel confidant and would like some clarification on the syntax in the glsl (c++ file Scene2p.cpp).....
///			I built a Spec , Diff , Ambient each is a respective array of size two.
///			There is an index for each array index; So if there is array[2] then there will be index_0 & index_1 ....
///			.... This is to allow in the c++ file to call the correct index in the shader file fragColor ....
///			.... I don't like how I have to set a uniform index, but it works for now .....
///			.... Perhaps there is an easier way, or a way to loop through, however ....
///			.... I don't like the idea of loop either, it would be far better to use a <vector>array
///			Coming in via layout is LIGHTDIRECTION via an array.......what a pain that was to figure.....
layout(location = 0) out vec4 fragColor;

layout(location = 0) in vec3 vertNormal;
layout(location = 1) in vec3 Light_Direction0;
layout(location = 2) in vec3 eyeDir; 
layout(location = 3) in vec2 textureCoords; 
layout(location = 4) in vec3 LIGHT_DIRECTION[2];

/// Spec , Diffuse , Ambient are built into seperate arrays 
//  the naming convention is self explanitory
//  uniform i is for the user to define the array size

layout(location = 7)uniform vec4 SPECULAR[2];
layout(location = 10)uniform vec4 DIFFUSE[2];
layout(location = 13)uniform vec4 AMBIENT[2];

uniform int index_0;
uniform int index_1;


//uniform sampler2D myTexture; 

void main() {
//    vec4 ks = vec4(0.0, 0.3, 1.0, 0.0);
//	vec4 kd = vec4(0.0, 0.0, 0.0, 0.0);
//	vec4 ka = 0.1 * DIFFUSE_ONE;
//	vec4 kt = texture(myTexture,textureCoords); 


	float diff_two = max(dot(vertNormal, LIGHT_DIRECTION[0]), 0.0);
    float diff_three = max(dot(vertNormal, LIGHT_DIRECTION[1]), 0.0);
	
	vec3 reflection_two = normalize(reflect(-LIGHT_DIRECTION[0], vertNormal));
	vec3 reflection_three = normalize(reflect(-LIGHT_DIRECTION[1], vertNormal));

	float spec_two = max(dot(eyeDir, reflection_two), 0.0);
	spec_two = pow(spec_two,14.0);
	float spec_three = max(dot(eyeDir, reflection_three), 0.0);
	spec_three = pow(spec_three,14.0);


//	vec4 Color1 =  (AMBIENT[0] + (diff_two * DIFFUSE[0]) + (spec_two * SPECULAR[0]));
//	vec4 Color2 =  (AMBIENT[1] + (diff_three * DIFFUSE[1]) + (spec_three * SPECULAR[1]));

	vec4 Color3 =  (AMBIENT[index_0] + (diff_two * DIFFUSE[index_0]) + (spec_two * SPECULAR[index_0]));
	vec4 Color4 =  (AMBIENT[index_1] + (diff_three * DIFFUSE[index_1]) + (spec_three * SPECULAR[index_1]));
//	fragColor =  Color1 + Color2;
	fragColor =  Color3 + Color4;

// kt is for the texture
//	fragColor =  (ka + (diff * kd) + (spec * ks)) * kt; 
}