#pragma once
#ifndef SCENE7_H
#define SCENE7_H
//#include "Body.h"		/// - More clarification - If i add this header file, I can use the sdl_image Texture obj
#include "Scene.h"
#include "Vector.h"
#include <Matrix.h>
#include "SkyBox.h"		/// Skybox 
#include "Camera.h"		/// Camera , I only want to use the cam settings, why I made a pointer to Camera class named CN_Camera why load the skyboax in there? what if I want to load other skyboxes ?? fk this is why I suck, my mind wonders..and I end up going down pointless rabbit holes....maybe on my own time I do this...
#include <SDL_image.h>
using namespace MATH;

/// Forward declarations 
union SDL_Event;
class Body;
class Mesh;
class Shader;
class Texture;		/// - More clarification - Forward declaring Texture allows me to use the SDL_image Texture pointer obj
class SkyBox;
class Scene7g : public Scene {
private:
	bool drawInWireMode;
	Shader* shader;
	Shader* shader_skyBox;
	/// Trackball
	Trackball trackball;
	///  Trackball Mats Translate , Rotate , View (Matrixs)
	Matrix4 trackball_T;
	Matrix4 trackball_R;
	Matrix4 trackball_V;
	/// Camera Update Scene5g
	Quaternion cameraOrientationWorldSpace;
	Vec3 cameraWorldSpace;
	/// Camera
	Camera* CN_Camera;
	/// SkyBox
	SkyBox* CN_SkyBox;
	/// SPHERE
	Body* body_cube;
	Mesh* mesh_cube;
	//Texture* texture_moon;
	/// MATRIX - CUBE
	Matrix4 cube_modelMat;
	Matrix4 cube_Translate;
	Matrix4 cube_rotate;
	Matrix4 cube_scale;
	/// UNIFORMS - Phong shadwer
	Vec3 LIGHT_POSITION[2];
	Vec4  SPEC[4];
	Vec4  DIFF[4];
	Vec4  AMB[4];
	float SPEC_EXPONENT_VALUE[4];
	int index_0 = 0;
	int index_1 = 1;
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

public:
	explicit Scene7g();
	virtual ~Scene7g();

	virtual bool OnCreate() override;
	virtual void OnDestroy() override;
	virtual void Update(const float deltaTime) override;
	virtual void Render() const override;
	virtual void HandleEvents(const SDL_Event& sdlEvent) override;
};


#endif // SCENE7G_H