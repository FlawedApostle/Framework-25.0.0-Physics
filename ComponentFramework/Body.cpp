#include "Body.h"
#include "Matrix.h"
#include "vector.h"
#include "MMath.h"
#include "QMath.h"
#include "Scene1p.h"
/// INIT
Body::Body(): 
	mesh{nullptr},
	texture{nullptr},
	upVector{0.0f, 1.0f, 0.0f},
	pos{}, 
	vel{}, 
	accel{}, 
	mass{1.0f},		// mass set from 0 to 1.0
	radius{1.0f},
	rotationalInertia{ },
	angularAcceleration{},
	angularVelocity {},
	displacement {} ,
	angularDisplacement {}
{ 
}
/// Destructor
Body::~Body() {}

void Body::Update(float deltaTime) {
	/// From 1st semester physics class
	pos += vel * deltaTime + 0.5f * accel * deltaTime * deltaTime;
	vel += accel * deltaTime;
}

void Body::ApplyForce(Vec3 force) {
	accel = force / mass;
}

/// Physics Functions - Update !
void Body::ApplyTourque(Vec3 Torque)
{
	/// Hollow sphere = 2.3mr^2
	// tourque slides rotational inertia
	rotationalInertia[0] = 2.3f * pow(2,mass + radius) + mass + radius;
	rotationalInertia[1] = 0.0f;
	rotationalInertia[2] = 0.0f;
	
	rotationalInertia[3] = 0.0f;
	rotationalInertia[4] = 2.3f * pow(2, mass + radius) + mass + radius;
	rotationalInertia[5] = 0.0f;
						 
	rotationalInertia[6] = 0.0f;
	rotationalInertia[7] = 0.0f;
	rotationalInertia[8] = 2.3f * pow(2, mass + radius) + mass + radius;
	
	/// Solid sphere = 2.5mr^2
	/*
	rotationalInertia[0] = 2.5f * pow(2, mass + radius) + mass + radius;
	rotationalInertia[1] = 0.0f;
	rotationalInertia[2] = 0.0f;

	rotationalInertia[3] = 0.0f;
	rotationalInertia[4] = 2.5f * pow(2, mass + radius) + mass + radius;
	rotationalInertia[5] = 0.0f;

	rotationalInertia[6] = 0.0f;
	rotationalInertia[7] = 0.0f;
	rotationalInertia[8] = 2.5f * pow(2, mass + radius) + mass + radius;
	*/

}
void Body::UpdateOrientation(float deltatime)
{
	// Update the orientation using angular velocity
	// need angle and axis	-> slides Velocity
	float angularSpeed = VMath::mag(angularVelocity);
	// Failsafe obj does not have a 'FULL' vector only direction, but no Mag , therefore it will crash
	/// I want more clarification on this !! - Vector requers a direction and a Mag , with no Mag its has nothing, therefore it will fail
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
/// AngularVelocity
// angularVel = theta / time
void Body::UpdateAngularVelocity(float deltaTime)
{
	// AV = W = theta / t 
	/// final angularVel = initial angular Vel + angular accell * deltaTime 
	//angularVelocity = angularVelocity + angularAcceleration * deltaTime;

	/// NO WAY ITS JUST THIS....
	//angularVelocity = angularDisplacement / deltaTime;
	angularVelocity = angularVelocity + angularAcceleration * deltaTime;
}
/// Angular Acceleration - angularAccel = change in angular Vel / time
// linear Acceleration	 - angularAccel * radius 
void Body::UpdateAngularAcceleration(float deltaTime)
{
	// TODO for YOU
	// Code up Umer's scribbles (also in circularMotion slides)
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
	return translate * rotation * scale;
}

/// Physics 1
void Body::UpdatePos(float deltaTime)
{
	pos += vel * deltaTime;
}
/// LINEAR VELOCITY		-- test !
void Body::UpdateVel(float deltaTime)
{
	/// Why Angular_Acceleration and not Theta ? - clarification please ! 
	/// ... is it bcuz we do not have a starting angle (theta) so we will start based off its acceleration ?
	/// ... these questions sound dumb, but these things throw me off.. in the slides its different - w = theta / time
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

