//#pragma once
#ifndef SCENE3p_H
#define SCENE3p_H
#include "Scene.h"
#include "Vector.h"
#include <Matrix.h>
#include "Quaternion.h"
#include "Trackball.h"			/// Trackball .. why you scared  ..

using namespace MATH;

/// Forward declarations 
union SDL_Event;
class Body;
class Mesh;
class Shader;

class Scene3p : public Scene
{
private:
	bool drawInWireMode;
	Shader* shader;
	/// Sphere
	Body* sphereBody;
	Mesh* sphereMesh;
	/// Plane
	Body* planeBody;
	Mesh* planeMesh;
	Vec3  planeNormal;
	/// CAMERA - PHYSICS
	Vec3 cameraPosition;
	Quaternion cameraOrientation;
	Quaternion rotation;
	/// Graphics
	Matrix4 projectionMatrix;
	Matrix4 viewMatrix;
	Matrix4 modelMatrix;
	/// TrackBall
	Trackball* trackball;
	//Quaternion changeInTrackballOrientation;
	Quaternion initialTrackballOrientation;
	Quaternion finalTrackballOrientation;
	/// HANDLE EVENTS AXIS MOVEMENT
	Vec3 axis_Left = Vec3(0.0f, 0.0f, 1.0f);
	Vec3 axis_Right = Vec3(0.0f, 0.0f, -1.0f);
	Vec3 axis_PitchUp = Vec3(-1.0f, 0.0f, 0.0f);
	Vec3 axis_PitchDown = Vec3(1.0f, 0.0f, 0.0f);
	


	/// PHYSICS - Toruqe
	Vec3 upVector;				/// Up Vector - for rolling the ball
	float angle;
	float distancetoPivot;		/// force of the ball 
	float torqueMagnitude;		/// speed of the ball 
	Vec3 torqueDirection;		/// direction of the ball
	Vec3 torque;
	Vec3 velocityDirection;
	float velocityMagnitutde;

	/// Uniforms
	Vec4 SPECULAR[2];
	Vec4 DIFFUSE[2];
	Vec4 AMBIENT[2];
	Vec3 LIGHT_POSITION[2];

	int index_0;
	int index_1;


public:	/// again this up vector is set public for speed...

	//Texture* mario_main;
	// Physics 3 equations
	//Vec3 torqueDir;

public:
	explicit Scene3p();
	virtual ~Scene3p();

	virtual bool OnCreate() override;
	virtual void OnDestroy() override;
	virtual void Update(const float deltaTime) override;
	virtual void Render() const override;
	virtual void HandleEvents(const SDL_Event& sdlEvent) override;
};


#endif // SCENE2p_P

