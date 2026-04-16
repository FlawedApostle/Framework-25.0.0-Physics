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
	Vec3 Torque_Direction(const Vec3& UpVector, const Vec3& Normal);
	void Torque_Direction_Update(const Vec3& upVector, const Vec3& Normal);
	
	Vec3 Torque(const Vec3& TorqueDir, float mass, float distanceToPivot);





	//--------------------------GETTERS/SETTERS---------------------------------
	//Torque Direction
	Vec3 Get_TourqueDir() const { return _torqueDir; }
	void Set_TourqueDir(Vec3 TD) { _torqueDir = TD; }
	// Gravity
	Vec3 Get_Gravity() const { return _gravity; }
	void Set_Gravity(Vec3 NG) { _gravity = NG; }
	//Distance To Pivot
	float Get_DistanceToPivot() { return _distanceToPivot; }
	void Set_DistanceToPivot(float DP) { _distanceToPivot = DP; }

	//-----------------------------------------------------------




};
//#endif PHYSICS_H
