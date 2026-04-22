#pragma once
#ifndef SCENE5p_H
#define SCENE5p_H
#include "Scene.h"
#include "Vector.h"
#include <Matrix.h>
#include "Quaternion.h"
#include "Trackball.h"
#include "Plane.h"

#include "Physics.h"

using namespace MATH;

/// Forward declarations 
union SDL_Event;
class Body;
class Mesh;
class Shader;

class Scene5p : public Scene
{
private:
	bool drawInWireMode;
	bool drawInNormalsFace;
	bool drawInNormalsLine = true;
	bool bWantsToJump;
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
	/// ANGLE		
	Vec3 upVector;				/// Up Vector - for rolling the ball
	float angle;
	float distancetoPivot;		/// force of the ball 
	/// TORQUE
	float torqueMagnitude;		/// speed of the ball 
	Vec3 torqueDirection;		/// direction of the ball
	Vec3 torque;
	/// VELOCITY
	Vec3 velocityDirection;
	float velocityMagnitutde;
	Vec3 linearVelocity;
	/// Uniforms
	Vec4 color_specular;
	Vec4 color_diffuse;
	float color_ambient_exponent = 0.1f;
	Vec3 lightPos;
	float normalScale;
	float lightPosLoc;
	
	/// BOUNDARIES			--- MOVED TO A NEW CPP
	float baseHalfSize;
	const float CONTACT_EPS = 0.01f;;

	/// CAMERA
	Vec3 offset;		// = Vec3(0, 0, 30);
	Vec3 target;		//  = sphereBody->pos;
	Vec3 rotatedOffset; // = trackball->getRot() * offset;
public:	/// again this up vector is set public for speed...

	// Plane 
	Plane* plane_plane1;
	//Physics physics;
	Physics PHYSICS;

public:
	explicit Scene5p();
	virtual ~Scene5p();

	virtual bool OnCreate() override;
	virtual void OnDestroy() override;
	virtual void Update(const float deltaTime) override;
	virtual void Render() const override;
	virtual void HandleEvents(const SDL_Event& sdlEvent) override;

	/// ----- LINEAR VELOCITY -- THESE SHOULD BE IN BODY
	Vec3 ComputeRollingVelocity_Cross(const Vec3& planeNormal);
	Vec3 ComputeFreeFallVelocity(Body* body, const Vec3& gravity, float dt);

	/// ----- ARCADE ---- ANGULAR DAMPNING
	void ApplyAngularDamping(float deltaTime);



	/// ----- DEPRECATED
	Vec3 ComputeRollingVelocity(const Vec3& downhill);
	Vec3 IfOnPlane(bool onPlane, Body* _body1, Vec3 _gravity, Vec3 _downHill, Vec3 _linearVelocity, float _angSpeed, float _speed, const float _time);

	Vec3 ComputeLinearVelocity(
		bool onPlane,
		Body* body,
		const Vec3& gravity,
		const Vec3& downhill,
		float deltaTime);

	void UpdateLinearVelocity(
		bool onPlane,
		Body* body,
		const Vec3& gravity,
		const Vec3& downhill,
		float deltaTime,
		Vec3& outLinearVelocity);

};
#endif // SCENE5_P




