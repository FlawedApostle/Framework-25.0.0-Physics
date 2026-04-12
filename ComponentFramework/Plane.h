//#pragma once
#ifndef PLANE_H
#define PLANE_H

#include <Vector.h> /// This is in GameDev
#include "Matrix.h"
#include "Quaternion.h"
#include "Body.h"

#include "memory.h" // unique pointer

class Plane
{
protected:
	Body* Plane_Body;
	//std::unique_ptr<Body> Plane_Body;
public:
	
	Plane(); // : Plane_Body(nullptr) {}
	~Plane()
	{
		printf("Plane ... Deleting \n");
		// When Plane Obj is deleted - destroy the Body	
		if(Plane_Body)
		delete Plane_Body; printf("Plane_Body::DELETED\n");
	}

	Body* GetPlane_Body() { return Plane_Body; }
	void SetPlane_Body(Body* NB) 
	{ 
		Plane_Body = NB; 
	}

};

#endif PLANE_H