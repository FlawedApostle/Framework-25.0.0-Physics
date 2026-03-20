#include "Body.h"
#include "Matrix.h"
#include "vector.h"
#include "MMath.h"
#include "QMath.h"
#include "Scene1p.h"

// tHEta : is the total angle of distance							- DISPLACEMENT
// oMEGa : is how fast you're rotating								- ANGULAR VELOCITY
// aLPHa : is how much rout rotation is spinning fast or slow		- ANGULAR ACCELERATION

Body::Body(): 
	mesh{nullptr},
	texture{nullptr},
	upVector{0.0f, 1.0f, 0.0f},
	pos{}, 
	vel{}, 
	accel{}, 
	mass{1.0f},														// mass set from 0 to 1.0
	radius{1.0f},
	rotationalInertia{ },
	angularAcceleration{},
	angularVelocity {},
	displacement {} ,
	angularDisplacement {}
{ 
}

Body::~Body() {}

void Body::Update(float deltaTime) {
	/// From 1st semester physics class
	pos += vel * deltaTime + 0.5f * accel * deltaTime * deltaTime;
	vel += accel * deltaTime;
}

/// ------- TEST
void Body::ApplyForce(Vec3 force) {
	accel = force / mass;
}

// ----- NEW PHYSICS
void Body::ApplyTourque(Vec3 Torque)
{
	/* TORQUE SLDES - rotational inertia
	 Hollow sphere = I = 2/3(mass * radi) ^ 2			 2 / 3 = 0.666667
	 Solid sphere =  I = 2/5(mass * radi) ^ 2			 2 / 5 = 0.4													*/
	
	float Svalue = 0.6666667 * mass * (radius * radius);
	float Hvalue = 0.4 * mass * (radius * radius);
	rotationalInertia[0] = Svalue;		rotationalInertia[1] = 0.0f;		rotationalInertia[2] = 0.0f;
	rotationalInertia[3] = 0.0f;		rotationalInertia[4] = Svalue;		rotationalInertia[5] = 0.0f;
	rotationalInertia[6] = 0.0f;		rotationalInertia[7] = 0.0f;		rotationalInertia[8] = Svalue;

	/* 2. Calculate Angular Acceleration
	// alpha = Inverse(I) * Torque
	// Since this is a diagonal matrix where all diagonal values are the same (iValue),
	// the inverse matrix simply has (1.0f / iValue) on the diagonals.													*/

	// Failsafe to prevent divide by zero if mass or radius is 0
	if (Hvalue < VERY_SMALL || Svalue < VERY_SMALL) {
		angularAcceleration = Vec3(0.0f, 0.0f, 0.0f);
		return;
	}

	float inverseIValue = 1.0f / Svalue;


	/* Multiply the Inverse Matrix by the Torque Vector
	// Because all non-diagonal elements are 0, the matrix multiplication simplifies to:								*/
	angularAcceleration.x = inverseIValue * Torque.x;
	angularAcceleration.y = inverseIValue * Torque.y;
	angularAcceleration.z = inverseIValue * Torque.z;

}
void Body::UpdateOrientation(float deltatime)
{
	// 1. Rotate your orientation quaternion using the angular velocity  [slides Velocity]
	// 2. Find the axis you are rotating about by normalizing the angular velocity vector
	// 3. Calculate the angle you need to rotate using angle = magnitude of the angular velocity *  deltaTime.
	//Vec3 angularVelocityNormalize = VMath::normalize(angularVelocity);																				// Using this I get weird results
	float angularSpeed = VMath::mag(angularVelocity);
	// Failsafe obj does not have a 'FULL' vector only direction, but no Mag , therefore it will crash
	/// I want more clarification on this - Vector requers a direction and a Mag , with no Mag its has nothing, therefore it will fail
	if (angularSpeed < VERY_SMALL)
	{
		return;
	}

	float angleRadians = angularSpeed * deltatime;
	float angleDegrees = angleRadians * RADIANS_TO_DEGREES;
	Vec3 axisOfRotation = VMath::normalize(angularVelocity);
	Quaternion rotation = QMath::angleAxisRotation(angleDegrees, axisOfRotation);

	// Combine quaternions my multiplying
	orientation *= rotation;
}


void Body::UpdateAngularVelocity(float deltaTime)
{
	// AV = W = theta / t 
	angularVelocity = angularVelocity + angularAcceleration * deltaTime;
}
void Body::UpdateAngularAcceleration(float deltaTime)
{
	// TODO for YOU
	// circularMotion slides
	// final angular_vel = initial angular_vel + angular_acc * deltaTime
	// angularVel = s(theta) / t

	angularVelocity = angularVelocity + angularAcceleration * deltaTime;
}

/// Get the model matrix
const Matrix4 Body::getModelMatrix() const
{
	Matrix4 translate = MMath::translate(pos);
	Matrix4 rotation = MMath::toMatrix4(orientation);
	Matrix4 scale = MMath::scale(radius, radius, radius);
	// scale * rotate * translate == translate * rotate * scale
	// read right to left Hebrew style
	//return translate * rotation * scale;

	// PRO ORDER (Hebrew Style / Right-to-Left):
	// Translate * Rotate * Scale
	return MMath::translate(pos) * MMath::toMatrix4(orientation) * MMath::scale(radius, radius, radius);
}

/// Physics 1
void Body::UpdatePos(float deltaTime)
{
	pos += vel * deltaTime;
}
/// LINEAR VELOCITY
void Body::UpdateVel(float deltaTime)
{
	
	// velocity = d / t - displacement / time 
	vel += accel * deltaTime;

	/// Linear Velocity:: V = || W || x || N || (multiply) 
	//float velocity = VMath::mag(vel);
	//float velocity_angular = VMath::mag(angularVelocity);
	//vel = angularAcceleration * deltaTime

	//vel = displacement / deltaTime;

	/// Linear Velocity = V = W X N NOTE:: each variable is NORMALIZED	- Clarification
	// direction of the linear velocity = direction of the angularVelocity CROSS normal
	//float av = VMath::mag(angularVelocity);
	//Vec3 AV = Vec3(av, av, av);
	//AV = VMath::normalize(AV);
	//Vec3 UV = VMath::normalize(upVector);
	//vel = VMath::cross(AV, UV);
	


}
/// LINEAR ACCELERATION
void Body::UpdateAccel(float deltaTime)
{
	accel += vel * deltaTime;
}


/// Physics default Body
bool Body::OnCreate() {
	return true;
}
void Body::OnDestroy() {
}
void Body::Render() const {
}

