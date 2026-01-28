#pragma once
#ifndef SCENE2_H
#define SCENE2_H
//#include "Body.h"		/// - More clarification - If i add this header file, I can use the sdl_image Texture obj
#include "Scene.h"
#include "Vector.h"
#include <Matrix.h>
//#include <SDL_image.h>
using namespace MATH;

/// Forward declarations 
union SDL_Event;
class Body;
class Mesh;	
class Shader;
class Texture;		/// - More clarification - Forward declaring Texture allows me to use the SDL_image Texture pointer obj

class Scene2g : public Scene {
private:
	/// <summary>
	///  scancode
	float scancode_specular_ = 0.0f;	
	float scancode_diffuse_ = 0.0f;	
	Shader* shader;
	/// SPHERE
	Body* body_sphere;
	Mesh* mesh_sphere;
	//Texture* texture_moon;
	/// UNIFORMS
	Vec3 l1Pos_;
	Vec3 lightPos;
	Vec3 lightPos_Two;
	Vec3 lightPos_0;

	//Vec4 specularOne;
	//Vec4 diffuseOne;
	//Vec4 ambientOne;
	/// Default ks , kd , ka variables for array
	/// Light 0 - original lighting
	// spec , diff , ambient	
	/// phongVert - Copy vars
	Vec4 spec_;
	Vec4 diff_;
	Vec4 amb_;	
	Vec4 specular0;
	Vec4 diffuse0;
	Vec4 ambient0;
	/// <summary>
	/// PhongFrag - copy - copy
	/// </summary>
	Vec3 light_position_one;
	Vec3 light_position_two;
	Vec3 light_position_three;
	Vec3 light_position_four;
	Vec3 LIGHT_POSITION[2];
	Vec4 k_specular0;
	Vec4 k_diffuse0;
	Vec4 k_ambient0;
	Vec4 k_specular1;
	Vec4 k_diffuse1;
	Vec4 k_ambient1;
	Vec4 k_specular2;
	Vec4 k_diffuse2;
	Vec4 k_ambient2;
	Vec4 k_specular3;
	Vec4 k_diffuse3;
	Vec4 k_ambient3;
	float spec_exponent_value_0;
	float spec_exponent_value_1;
	float spec_exponent_value_2;
	float spec_exponent_value_3;
	float spec_exponent_value_4;
	
	/// <summary>
	/// PhongFrag - copy
	/// </summary>
	Vec4 kspecular[2];
	Vec4 kdiffuse[2];
	Vec4 kambient[2];
	float specular_uniform[2];
	int specular_uniform_index;
	float specular_uniform_pow[2];
	//Vec3 LIGHT_DIRECTION[2];
	//Vec3 LIGHT_POSITION[2];
	/// Light 1
	Vec4 l1specular_;
	Vec4 l1diffuse_;
	Vec4 l1ambient_;
	// Specular exponent uniform , defins how wet the obj looks
	float specExponent0_;
	float specExponent1_;
	/// Shader array
	Vec4 speculars_[2];
	int specularsIndex0_;
	int specularsIndex1_;
	int specularsIndex2_;
	/// Lights array
	Vec4 Light_[6];
	/// MATRIX
	Matrix4 projectionMatrix;
	Matrix4 viewMatrix;
	Matrix4 modelMatrix;
	/// Sphere Matrix
	Matrix4 projectionMatrix_sphere;
	Matrix4 viewMatrix_sphere;
	Matrix4 translate_sphere;
	Matrix4 rotate_sphere;
	Matrix4 scale_sphere;
	Matrix4 modelMatrix_sphere;
	bool drawInWireMode;

public:
	explicit Scene2g();
	virtual ~Scene2g();

	virtual bool OnCreate() override;
	virtual void OnDestroy() override;
	virtual void Update(const float deltaTime) override;
	virtual void Render() const override;
	virtual void HandleEvents(const SDL_Event& sdlEvent) override;

	/// spec function
	//virtual void specPow(float pow);
};


#endif // SCENE2G_H