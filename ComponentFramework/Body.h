#ifndef BODY_H
#define BODY_H
#include <Vector.h> /// This is in GameDev
#include "Matrix.h"
#include "vector.h"
#include "Quaternion.h"
using namespace MATH; 

/// Just forward declair these classes so I can define a pointer to them
/// Used later in the course
class Mesh;
class Texture;

class Body {
public:
    Body();
    ~Body();
public:	/// making Physics public for now, cause I want to get it done....howver yuck !!
	Vec3 pos;
	Vec3 vel;
	Vec3 accel;
	float mass;

	Quaternion orientation;
	//Quaternion position;
	Vec3 upVector;								/// Up vector for the ball, to solve the angle
	Vec3 angularVelocity;
	Vec3 angularAcceleration;
	Vec3 displacement;							/// track the ball ? can this determine velocity && acceleration ?
	Vec3 angularDisplacement;					/// track the ball ? can this determine velocity && acceleration ?
	float radius;
	Matrix3 rotationalInertia;

private: /// Physics stuff
	/// put physics vars private, and set getters for them


private: /// Graphics stuff	- Note its in Private, so Getters && Setters ! 
	Mesh *mesh;
	Texture *texture;
	// V = W * N	(Normals - all)

public:
	bool OnCreate();
	void OnDestroy();
	void Update(float deltaTime);
	void Render() const;

	void ApplyForce(Vec3 force);
	void setAccel(const Vec3 &accel_) { accel = accel_;}
	
	void ApplyTourque(Vec3 Torque);
	void UpdateOrientation(float deltatime);
	void UpdateAngularVelocity(float deltaTime);
	void UpdateAngularAcceleration(float deltaTime);
	/// get Model Matrix
	const Matrix4 getModelMatrix() const;
	
	void UpdatePos(float deltaTime);
	void UpdateVel(float deltaTime);
	void UpdateAccel(float deltaTime);

	/// Calculator test to ensure the radians_to_degrees is working...
	void converttoRads(float omega , float deltatime)
	{
		float angleRadians = omega * deltatime;
		float angleDegrees = angleRadians * RADIANS_TO_DEGREES;
		printf("Radians = %f\n", angleDegrees);
	}


};

#endif
