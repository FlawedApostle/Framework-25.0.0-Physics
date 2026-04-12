#include <iostream>
#include "Plane.h"

Plane::Plane() : 
	Plane_Body	{nullptr}	,
	Plane_Mesh	{nullptr}
{
	std::cout << "Plane::Class::Load\n" << std::endl;
}

Plane::~Plane()
{
	printf("PLANE CLASS::DESTRUCTOR\n");
	// When Plane Obj is deleted - destroy the Body	
	if (Plane_Body)
		delete Plane_Body; 
	printf("PLANE CLASS::DESTRUCTOR::DELETED - BODY\n");
	if (Plane_Mesh)
		delete Plane_Mesh;
	printf("PLANE CLASS::DESTRUCTOR::DELETED - MESH\n");
}

// BODY - SETTER
void Plane::SetPlane_Body(Body* NB)
{
	//if (Plane_Body)					// if plane_body has been made somewhere delete it and remake a new one. - this i dont know yet as I may want more planes in the scene
		//delete Plane_Body;
	Plane_Body = NB;
}
// MESH - SETTER
void Plane::SetPlane_Mesh(Mesh* NM)
{
	Plane_Mesh = NM;
}
