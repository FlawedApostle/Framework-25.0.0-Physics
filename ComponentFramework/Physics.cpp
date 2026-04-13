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

// TORQUE - ANGLE / DISTANCE TO PIVOT
// NOTES -
/*
	//float cosTheta = VMath::dot(planeNormal, upVector);
	////cosTheta = MMath::clamp(cosTheta, -1.0f, 1.0f);
	//float theta = acos(cosTheta);
	//float distanceToPivot = sphereBody->radius * sin(theta);
*/
float Physics::Angle_DistanceToPivot(Vec3 Normal, Vec3 UpVec)
{
	float cosTheta = VMath::dot(Normal, UpVec);
	float theta = acos(cosTheta);
	float sinTheta = sin(theta);
	
	return sinTheta;
}


Vec3 Physics::TORQUE_DIRECTION(Vec3 UpVector, Vec3 Normal)
{
	return VMath::cross(UpVector, Normal);
}

// TORQUE [ FUNCTION RETURNS A VEC3 TORQUE VALUE ] 
// - THIS FUNCTION WORKS IN RELATION TO [ ANGLE_DISTANCETOPIVOT ]
// - PURPOSE IS TO YEILD A VALUE THAT CAN BE MANIPULATED IN THE SCENE
// NOTES -
/*
	torque = VMath::cross(upVector, planeNormal);									// Vec3 Torque( vec3 , Vec3, Body*)
	if (VMath::mag(torque) > VERY_SMALL) {
		
		Vec3 torqueDir = VMath::normalize(torque);
		torqueMagnitude = sphereBody->mass * 9.8f * distanceToPivot;
		Vec3 torqueFinal = torqueDir * torqueMagnitude;
		
		sphereBody->ApplyTourque(torqueFinal);
		
		
		// -- Deprecated
		//torqueMagnitude = distanceToPivot * sphereBody->mass;						// simple proportional model
		//torqueMagnitude = torqueMagnitude * distanceToPivot;						// simple proportional model
	

*/
Vec3 Physics::TORQUE(Vec3 UpVector, Vec3 Normal, float Distance_to_Pivot,  Body* Body)
{
	Vec3 TORQUE , TORQUE_DIRECTION , TORQUE_FINAL;
	float COS_THETA , THETA , TORQUE_MAGNITUDE;

	// find the direction first
	TORQUE = VMath::cross(UpVector, Normal);
	// if (MAG)TORQUE > VERY_SMALL. DO THE FOLLOWING
	//if (VMath::mag(TORQUE) > VERY_SMALL)
	//{
		TORQUE_DIRECTION = VMath::normalize(TORQUE);
		TORQUE_MAGNITUDE = Body->mass * 9.8f * Distance_to_Pivot;
		TORQUE_FINAL = TORQUE_DIRECTION * TORQUE_MAGNITUDE;

		//return Body->ApplyTourque(TORQUE_FINAL);
	//}


	return TORQUE_FINAL;




}
