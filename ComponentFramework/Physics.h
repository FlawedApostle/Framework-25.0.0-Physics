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
	// TORQUE
	Vec3 TORQUE_DIRECTION(const Vec3& UpVector, const Vec3& Normal);
	void TORQUE_DIRECTION_UPDATE(const Vec3& upVector, const Vec3& Normal);
	
	Vec3 TORQUE(Vec3 UpVector, Vec3 Normal, float Distance_to_Pivot, Body* Body);






	//--------------------------GETTERS/SETTERS---------------------------------
	//Torque Direction
	Vec3 Get_TourqueDir() const { return _torqueDir; }
	void Set_TourqueDir(Vec3 TD) { _torqueDir = TD; }
	//Distance To Pivot
	float Get_DistanceToPivot() { return _distanceToPivot; }
	void Set_DistanceToPivot(float DP) { _distanceToPivot = DP; }
	//-----------------------------------------------------------




};
//#endif PHYSICS_H
