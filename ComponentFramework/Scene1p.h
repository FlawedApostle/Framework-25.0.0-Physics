//#pragma once
#ifndef SCENE1p_H
#define SCENE1p_H
#include "Scene.h"
#include "Vector.h"
#include <Matrix.h>
using namespace MATH;

/// Forward declarations 
union SDL_Event;
class Body;
class Mesh;
class Shader;

class Scene1p : public Scene
{
	private:
		bool drawInWireMode;
		Shader* shader;
		/// Sphere
		Body*	sphereBody;
		Mesh*	sphereMesh;				// ball
		/// Plane
		Body*	planeBody;				// plane
		Mesh*	planeMesh;
		Vec3	planeNormal;

		/// Graphics
		Matrix4 projectionMatrix;
		Matrix4 viewMatrix;
		Matrix4 modelMatrix;

		/// UNIFORMS LIGHTS POSITIONS
		Vec3 Light_Position0;
		
		/// Toruqe
		Vec3 upVector;		// Up Vector -> for rolling the ball
		float angle;
		float distancetoPivot;		// to roll the ball 
		float torqueMagnitude;
		Vec3 torqueDirection;
		Vec3 torque;
		Vec3 velocityDirection;
		float velocityMagnitutde;


	public:	/// again this up vector is set public for speed...
		
		//Texture* mario_main;
		// Physics 3 equations
		//Vec3 torqueDir;

	public:
		explicit Scene1p();
		virtual ~Scene1p();

		virtual bool OnCreate() override;
		virtual void OnDestroy() override;
		virtual void Update(const float deltaTime) override;
		virtual void Render() const override;
		virtual void HandleEvents(const SDL_Event& sdlEvent) override;
	};


#endif // SCENE1_P

