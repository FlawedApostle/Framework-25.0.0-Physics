#pragma once
#ifndef SCENE4_H
#define SCENE4_H
//#include "Body.h"		/// - More clarification - If i add this header file, I can use the sdl_image Texture obj
#include "Scene.h"
#include "Vector.h"
#include <Matrix.h>
#include "Texture.h"
#include "Trackball.h"
//#include <SDL_image.h>
using namespace MATH;

/// Forward declarations 
union SDL_Event;
class Body;
class Mesh;
class Shader;
class Texture;		/// - More clarification - Forward declaring Texture allows me to use the SDL_image Texture pointer obj

class Scene4g : public Scene {
private:
	bool drawInWireMode;
	Shader* shader;
	/// Trackball
	Trackball* trackball;
	Vec3 cameraPosition;						/// world space
	Quaternion cameraPositionOrientation;		/// world spcae
	Quaternion rotation;
	Matrix4 trackball_translate;
	Matrix4 trackball_rotate;
	Matrix4 trackball_view;
	/// SKULL
	Body* body_Skull;
	Mesh* mesh_Skull;
	Texture* texture_Skull;
	/// EYE RIGHT
	Body* body_EyeR;
	Mesh* mesh_EyeR;
	Texture* texture_Eye;				// global var , using same texture for both spheres (eyes)
	/// EYE RIGHT
	Body* body_EyeL;
	Mesh* mesh_EyeL;
	/// UNIFORMS
	int SPECULAR_EXPONENT[4];
	Vec3 LIGHT_POSITION[4];
	Vec4 SPEC[4];
	Vec4 DIFF[4];
	Vec4 AMB[4];
	/// MATRIX - default
	Matrix4 projectionMatrix;
	Matrix4 viewMatrix;
	Matrix4 modelMatrix;
	/// MATRIX Skull		T * R * S
	Matrix4 skull_modelMat;
	Matrix4 skull_modelTranslate;
	Matrix4 skull_modelRotate;
	Matrix4 skull_modelScale;
	/// MATRIX EYE GLOBAL VARS
	Matrix4 eye_modelScale;
	Matrix4 eye_modelRotate;
	/// MATRIX EYE RIGHT	T * R * S
	Matrix4 eyeRight_modelMat;
	Matrix4 eyeRight_modelTranslate;
	Matrix4 eyeRight_modelRotate;
	Matrix4 eyeRight_modelScale;
	/// MATRIX EYE LEFT
	Matrix4 eyeLeft_modelMat;
	Matrix4 eyeLeft_modelTranslate;
public:
	explicit Scene4g();
	virtual ~Scene4g();

	virtual bool OnCreate() override;
	virtual void OnDestroy() override;
	virtual void Update(const float deltaTime) override;
	virtual void Render() const override;
	virtual void HandleEvents(const SDL_Event& sdlEvent) override;

	/// spec function
	//virtual void specPow(float pow);
};


#endif // SCENE4G_H