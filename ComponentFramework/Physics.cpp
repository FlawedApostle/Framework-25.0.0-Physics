#include "Physics.h"
#include "Matrix.h"
#include "vector.h"
#include "MMath.h"
#include "QMath.h"
#include "Scene1p.h"


Physics::Physics() :

	_torque				{}					,
	_torqueDir			{}					,
	_distanceToPivot	{}					,
	_gravity			{0.0,-9.8,0.0}
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
// Pure math
/*
Seperate computation from state
TORQUE_DIRECTION - computes a value [ the direction ]
UpdateTorque     - modifies members state _torqueDir
*/
Vec3 Physics::Torque_Direction(const Vec3& UpVector, const Vec3& Normal)
{
	return VMath::cross(UpVector, Normal);
}
void Physics::Torque_Direction_Update(const Vec3& upVector, const Vec3& Normal)
{
	_torqueDir = Torque_Direction(upVector, Normal);
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
Vec3 Physics::Torque(const Vec3& TorqueDir, float mass, float distanceToPivot)
{
	float TorqueDirMag = VMath::mag(TorqueDir);
	// check validity before using this vector
	if (TorqueDirMag <= VERY_SMALL)
		return Vec3(0.0f,0.0f,0.0f); // no torque

	Vec3 TorqueDirNormal = TorqueDir / TorqueDirMag;
	float torqueMag = mass * 9.8f * distanceToPivot;

	return TorqueDirNormal * torqueMag;

}
