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
	Mesh* Plane_Mesh;
	//std::unique_ptr<Body> Plane_Body;
public:
	
	Plane();
	~Plane();
	// Body
	Body* GetPlane_Body() { return Plane_Body; }
	void SetPlane_Body(Body* NB);
	// Mesh
	Mesh* GetPlane_Mesh() { return Plane_Mesh; }
	void SetPlane_Mesh(Mesh* NM);
};

#endif PLANE_H