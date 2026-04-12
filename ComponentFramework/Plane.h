//#pragma once
#ifndef PLANE_H
#define PLANE_H

#include <Vector.h> /// This is in GameDev
#include "Matrix.h"
#include "Quaternion.h"
#include "Body.h"


class Plane
{
protected:
	Body* Plane_Body;

public:
	

	void SetPlane_Body(Body* NB) { Plane_Body = NB; };
	Body* GetPlane_Body() { return Plane_Body; };

};

#endif PLANE_H