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
	// Torque
	Vec3 Get_Tourque() { return _torque; }
	void Set_Tourque(Vec3 T) {  _torque = T; }
	//Torque Direction
	Vec3 Get_TourqueDir() { return _torqueDir; }
	void Set_TourqueDir(Vec3 TD) { _torqueDir = TD; }
	//Distance To Pivot
	float Get_DistanceToPivot() { return _distanceToPivot; }
	void Set_DistanceToPivot(float DP) { _distanceToPivot = DP; }

	// ANGLE
	float Angle_DistanceToPivot(Vec3 Normal, Vec3 UpVec);

	Physics();
	//~Physics();

	/// AXIS OF ROTATION : UP x NORMAL
	/* Find mag of torque
	 find axis of rotation
	 torque = (UP CROSS planeNormal)
	 torqueMagnitude is the weight of the ball - we need a direction and a Magnitude
	 weight * distance to pivot
	 */

	Vec3 TORQUE(Vec3 Up, Vec3 Normal, Body* Body);

};
//#endif PHYSICS_H
