#include "Physics.h"
#include "Matrix.h"
#include "vector.h"
#include "MMath.h"
#include "QMath.h"
#include "Scene1p.h"


Physics::Physics() :

	_torque				{}			,
	_torqueDir			{}			,
	_distanceToPivot	{}
{

	std::cout << "Physics Class" << std::endl;
}

// Angle Torque
float Physics::Angle_DistanceToPivot(Vec3 Normal, Vec3 UpVec)
{
	float cosTheta = VMath::dot(Normal, UpVec);
	float theta = acos(cosTheta);
	float sinTheta = sin(theta);
	
	return sinTheta;
}


Vec3 Physics::TORQUE(Vec3 Up, Vec3 Normal, Body* Body)
{
	Vec3 TORQUE, TORQUE_FINAL , TORQUE_DIRECTION;
	float COS_THETA , THETA, DISTANCE_TO_PIVOT , TORQUE_MAGNITUDE;

	COS_THETA = VMath::dot(Normal, Up);
	THETA = acos(COS_THETA);
	DISTANCE_TO_PIVOT = Body->radius * sin(THETA);

	TORQUE = VMath::cross(Up, Normal);
	TORQUE_DIRECTION = VMath::normalize(TORQUE);
	
	
	TORQUE_MAGNITUDE = Body->mass * 9.8f * DISTANCE_TO_PIVOT;

	TORQUE_FINAL = TORQUE_DIRECTION * TORQUE_MAGNITUDE;

	return TORQUE_FINAL;



	//float cosTheta = VMath::dot(planeNormal, upVector);
	////cosTheta = MMath::clamp(cosTheta, -1.0f, 1.0f);
	//float theta = acos(cosTheta);
	//float distanceToPivot = sphereBody->radius * sin(theta);
	
	//torque = VMath::cross(upVector, planeNormal);				// Vec3 Torque( vec3 , Vec3, Body*)
	//if (VMath::mag(torque) > VERY_SMALL) {
	//	Vec3 torqueDir = VMath::normalize(torque);
	//	//torqueMagnitude = distanceToPivot * sphereBody->mass;						// simple proportional model
	//	torqueMagnitude = sphereBody->mass * 9.8f * distanceToPivot;
	//	//torqueMagnitude = torqueMagnitude * distanceToPivot;						// simple proportional model
	//	Vec3 torqueFinal = torqueDir * torqueMagnitude;
	//
	//	sphereBody->ApplyTourque(torqueFinal);
}
