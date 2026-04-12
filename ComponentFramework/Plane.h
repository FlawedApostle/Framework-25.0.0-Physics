//#pragma once
#ifndef PLANE_H
#define PLANE_H

#include <Vector.h> /// This is in GameDev
#include "Matrix.h"
#include "Quaternion.h"
#include "Body.h"
#include "Mesh.h"

#include "memory.h" // unique pointer

class Plane
{
protected:
	Body* Plane_Body;
	//std::unique_ptr<Body> Plane_Body;
public:
	
	Plane();
	~Plane();

	Body* GetPlane_Body() { return Plane_Body; }
	void SetPlane_Body(Body* NB);

};

#endif PLANE_H