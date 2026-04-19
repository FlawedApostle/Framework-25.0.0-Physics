//#ifdef PHYSICS_H
//#define PHYSICS_H
#pragma once
#include <Vector.h> /// This is in GameDev
//#include "vector.h"
#include "Matrix.h"
#include "Quaternion.h"
#include "Body.h"


class Physics
{
protected:

	Vec3 _torque;
	Vec3 _torqueDir;
	Vec3 _gravity;
	
	Vec3 _gravityNormal;
	Vec3 _torqueDirectionAndSpeed;

	float angSpeed;
	float speed;
	float planeDist;

	float _distanceToPivot;

public:
	Physics();
	//~Physics();

	// TORQUE - ANGLE - [DISTANCE TO PIVOT - TORQUE]
	float Angle_DistanceToPivot(Vec3 Normal, Vec3 UpVec);


	/* Find mag of torque
	 AXIS OF ROTATION : UP x NORMAL
	 find axis of rotation
	 torque = (UP CROSS planeNormal)
	 torqueMagnitude is the weight of the ball - we need a direction and a Magnitude
	 weight * distance to pivot
	 */
	// TORQUE - Distance to pivot / rotation
	Vec3 Torque_Direction(const Vec3& UpVector, const Vec3& Normal);
	void Torque_Direction_Update(const Vec3& upVector, const Vec3& Normal);
	
	// TORQUE - Apply Rotation using angle and direction force calculations - Notes below	function 1.
	/*
	/// ------------------------------- ORIGINAL OLD CODE - DO NOT REMOVE

	float cosTheta = VMath::dot(planeNormal, upVector);
	//cosTheta = MMath::clamp(cosTheta, -1.0f, 1.0f);
	float theta = acos(cosTheta);
	float distanceToPivot = sphereBody->radius * sin(theta);

	//torque = VMath::cross(planeNormal, downhill) * sphereBody->radius * sphereBody->mass;			// NEW :: using downhill to apply gravity direction in relation to the
	torque = VMath::cross(upVector, planeNormal);									// Vec3 Torque( vec3 , Vec3, Body*)
	if (VMath::mag(torque) > VERY_SMALL) {

	Vec3 torqueDir = VMath::normalize(torque);
	torqueMagnitude = sphereBody->mass * 9.8f * distanceToPivot;
	Vec3 torqueFinal = torqueDir * torqueMagnitude;

	sphereBody->ApplyTourque(torqueFinal);

	}

	*/
	Vec3 Torque(const Vec3& TorqueDir, float mass, float distanceToPivot);
	
	// TORQUE - Apply Rotation using angle and direction force calculations - Notes below	function 2. - current using gravity mag for speed in rotational rolling
	/*
	/// VER # 1.
		Vec3 torque = VMath::cross(planeNormal, downhill) * sphereBody->mass * sphereBody->radius;

		if (VMath::mag(torque) > VERY_SMALL)
		{
			sphereBody->ApplyTourque(torque);
		}
	*/
	Vec3 Torque(const Vec3& planeNormal, Vec3& const downHill, float mass, float radius);


	//--------------------------GETTERS/SETTERS---------------------------------
	//Torque Direction
	Vec3 Get_TourqueDir() const { return _torqueDir; }
	void Set_TourqueDir(Vec3 TD) { _torqueDir = TD; }
	// Gravity
	Vec3 Get_Gravity() const { return _gravity; }
	void Set_Gravity(Vec3 NG) { _gravity = NG; }
	//Distance To Pivot
	float Get_DistanceToPivot()			{ return _distanceToPivot; }
	void Set_DistanceToPivot(float DP)	{ _distanceToPivot = DP; }
	// GravityNormal
	Vec3 Get_GravityNormal() const	{ return _gravityNormal; }
	void Set_GravityNormal(Vec3 GN) { _gravityNormal = GN; };
	// Torque Axis Direction And Speed
	Vec3 Get_TorqueDirectionAndSpeed() { return _torqueDirectionAndSpeed; }
	void Set_TorqueDirectionAndSpeed(Vec3 NTDS) { _torqueDirectionAndSpeed = NTDS; }
	// Angular Speed
	// Plane Distance
	//-----------------------------------------------------------




};
//#endif PHYSICS_H
