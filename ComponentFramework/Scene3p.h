//#pragma once
#ifndef SCENE3p_H
#define SCENE3p_H
#include "Scene.h"
#include "Vector.h"
#include <Matrix.h>
#include "Quaternion.h"
#include "Trackball.h"

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
	bool drawInNormalsFace;
	bool drawInNormalsLine = true;
	Shader* shader;
	Shader* shader_normals_face;
	Shader* shader_normals_line;
	/// SPHERE OG
	Body* sphereBody;
	Mesh* sphereMesh;
	Vec3 sphereNormal;
	/// SPHERE COLLISION
	Body* sphereCollision0_Body;
	Mesh* sphereCollision0_Mesh;
	Vec3 sphereCollision0_Normal;
	/// PLANE
	Body* planeBody;
	Mesh* planeMesh;
	Vec3  planeNormal;

	/// CAMERA - PHYSICS
	Vec3 cameraPosition;
	Quaternion cameraOrientation;
	Quaternion rotation;
	/// GRAPHICS
	Matrix4 projectionMatrix;
	Matrix4 viewMatrix;
	Matrix4 modelMatrix;
	/// TRACKBALL
	Trackball* trackball;
	//	QUATERNION - changeInTrackballOrientation;
	Quaternion initialTrackballOrientation;
	Quaternion finalTrackballOrientation;
	/// CONTROLS - AXIS EVENTS - MOVEMENT
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
	Vec3 linearVelocity;

	/// Uniforms
	Vec3 lightPos;
	float normalScale;
	float lightPosLoc;


public:	/// again this up vector is set public for speed...


public:
	explicit Scene3p();
	virtual ~Scene3p();

	virtual bool OnCreate() override;
	virtual void OnDestroy() override;
	virtual void Update(const float deltaTime) override;
	virtual void Render() const override;
	virtual void HandleEvents(const SDL_Event& sdlEvent) override;

};


#endif // SCENE3p_P

