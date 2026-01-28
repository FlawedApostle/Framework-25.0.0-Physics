#include <glew.h>
#include <iostream>
#include <SDL.h>
#include "Scene2p.h"			/// scene2p.h
#include <MMath.h>
#include "Debug.h"
#include "Mesh.h"
#include "Shader.h"
#include "Body.h"
#include <QMath.h>
#include "Quaternion.h"

//Body bodyAdmin;
// initializer
Scene2p::Scene2p() :

	  drawInWireMode{ false }
	, shader		{ nullptr }
	, sphereMesh	{ nullptr }
	, sphereBody	{ nullptr }
	, planeBody		{ nullptr }
	, planeMesh		{ nullptr }
	, planeNormal	{ 0,1,0 }
	, trackball		{nullptr}
	//upVector{ 0, 1, 0 }
{
	Debug::Info("Created Scene2p: ", __FILE__, __LINE__);
}

/// destroy
Scene2p::~Scene2p() 
{
	Debug::Info("Deleted Scene2p: ", __FILE__, __LINE__);
}

bool Scene2p::OnCreate() {
	Debug::Info("Loading assets Scene2p: ", __FILE__, __LINE__);

	/// Trackball
	trackball = new Trackball();

	/// UNIFORMS
	// NOTE:: LIGHT_POSITION[0] = some reason the plane is flipped , so i point the light -90 , rather than the plane, which would affect the physics
	LIGHT_POSITION[0] = Vec3(0.0f, -90.0f, 0.0f);			// plane
	LIGHT_POSITION[1] = Vec3(0.0f, 0.0f, 0.0f);				// ball
	// Plane
	SPECULAR[0] = Vec4(0.0f, 0.0f, 0.7f, 0.0f);
	DIFFUSE[0] = Vec4(0.0f, 0.0f, 0.4f, 0.0f);
	AMBIENT[0] = 0.4f * DIFFUSE[0];
	index_0 = 0;
	// Sphere
	SPECULAR[1] = Vec4(0.7f, 0.0f, 0.0f, 0.0f);
	DIFFUSE[1] = Vec4(0.4f, 0.0f, 0.0f, 0.0f);
	AMBIENT[1] = 0.2f * DIFFUSE[1];
	index_1 = 1;
	/// I cannot see the plane from above , if I rotate to -90 the plane is visible..... wtf - check note on LIGHT_POSITION[0]
	/// Plane
	planeBody = new Body();
	planeBody->OnCreate();
	planeBody->orientation = QMath::angleAxisRotation(90, Vec3(1, 0, 0)); 
	//planeBody->orientation *= QMath::angleAxisRotation(45, Vec3(0, 0, 1));
	planeBody->radius = 2.0f;
	planeNormal = Vec3(0.0f, 0.0f, -1.0f);
	planeNormal = QMath::rotate(planeNormal, planeBody->orientation);

	/// Sphere
	// V = W X N (velocity = angular velocity cross normal -> (assume each letter is a vector)
	sphereBody = new Body();
	sphereBody->OnCreate();
	sphereBody->pos = Vec3(0.0f, 1.0, 0.0f);
	sphereBody->angularVelocity = Vec3(0.0f, 0.0f, 0.0f);			// starts at 0 for rest
	sphereBody->radius = 1;
	// Speed of the ball
	sphereBody->angularAcceleration = Vec3(0.0f, 0.0f, 0.0f);		// starts at 0 for rest

	/// Mesh(s)
	// Mesh -> plane
	planeMesh = new Mesh("meshes/Plane.obj");
	planeMesh->OnCreate();
	// Mesh -> sphere
	sphereMesh = new Mesh("meshes/Sphere.obj");
	sphereMesh->OnCreate();
	/// Shader
	shader = new Shader("shaders/defaultPhong/phongVert.glsl", "shaders/defaultPhong/phongFrag.glsl");
	if (shader->OnCreate() == false) {
		std::cout << "Shader failed ... we have a problem\n";
	}

	projectionMatrix = MMath::perspective(45.0f, (16.0f / 9.0f), 0.5f, 100.0f);
	//viewMatrix = MMath::lookAt(Vec3(0.0f, 0.0f, 20.0f), Vec3(0.0f, 0.0f, 0.0f), Vec3(0.0f, 1.0f, 0.0f));
	cameraPosition = sphereBody->pos + Vec3(0.0f, 0.0f, 10.0f);
	/// define this line ! 
	cameraOrientation = QMath::angleAxisRotation(0, Vec3(1.0f, 0.0f, 0.0f));
	Matrix4 T = MMath::translate(cameraPosition);
	Matrix4 R = MMath::toMatrix4(cameraOrientation);
	viewMatrix = MMath::inverse(R) * MMath::inverse(T);		// why inverse ? - check slides , check cat slides
	
	return true;
}
/// Clean Up
void Scene2p::OnDestroy() {
	Debug::Info("Deleting assets Scene2p: ", __FILE__, __LINE__);
	/// sphere
	sphereBody->OnDestroy();
	delete sphereBody;
	sphereMesh->OnDestroy();
	delete sphereMesh;
	/// plane
	planeBody->OnDestroy();
	delete planeBody;
	planeMesh->OnDestroy();
	delete planeMesh;
	/// shader
	shader->OnDestroy();
	delete shader;
	/// Trackball
	delete trackball;
}
/// Controls !!		Draw in wire mode is now M
void Scene2p::HandleEvents(const SDL_Event& sdlEvent) {
	/// Trackball
	// Explanation
	initialTrackballOrientation = trackball->getQuat();
	trackball->HandleEvents(sdlEvent);
	finalTrackballOrientation = trackball->getQuat();
	
	switch (sdlEvent.type) {
	case SDL_KEYDOWN:
		switch (sdlEvent.key.keysym.scancode) {
		case SDL_SCANCODE_M:
			drawInWireMode = !drawInWireMode;
			break;

		case SDL_SCANCODE_P:
			planeNormal.print("planeNormal"); 
			upVector.print("upVector"); 
			sphereBody->vel.print("sphereBody->vel");
			printf("torqueMagnitude %f\n", torqueMagnitude);
			printf("velocityMagnitutde %f\n", velocityMagnitutde);
			printf("velocityDirection (%f,%f,%f)\n", velocityDirection.x , velocityDirection.y , velocityDirection.z);
			break;

			/// W , A , S , D
		case SDL_SCANCODE_W:
			printf("UP\n");
			rotation = QMath::angleAxisRotation(2.0f, axis_PitchUp);
			planeBody->orientation = rotation * planeBody->orientation;
			planeNormal = QMath::rotate(planeNormal, rotation);
			break;
		case SDL_SCANCODE_A:
			printf("LEFT\n");
			rotation = QMath::angleAxisRotation(2.0f, axis_Left);
			planeBody->orientation = rotation * planeBody->orientation;
			planeNormal = QMath::rotate(planeNormal, rotation);
			break;
		case SDL_SCANCODE_D:
			printf("RIGHT\n");
			rotation = QMath::angleAxisRotation(2.0f, axis_Right);
			planeBody->orientation = rotation * planeBody->orientation;
			planeNormal = QMath::rotate(planeNormal, rotation);
			break;
		case SDL_SCANCODE_S:
			printf("DOWN\n"); planeNormal.print();
			rotation = QMath::angleAxisRotation(2.0f, axis_PitchDown);
			planeBody->orientation = rotation * planeBody->orientation;
			planeNormal = QMath::rotate(planeNormal, rotation);
			break;
		}
		break;

	case SDL_MOUSEMOTION:
		break;

	case SDL_MOUSEBUTTONDOWN:
		break;

	case SDL_MOUSEBUTTONUP:
		break;

	default:
		break;
	}
}

void Scene2p::Update(const float deltaTime)
{	
	// TODO for YOU
	/*
	// Calculate torqueMag using forceMag * distance to pivot
	// The force is the weight of the sphere
	// The distance to the pivot relies on the angle 
	// between the weight and the normal
	*/

	/// ROTATION , BEGIN ROTATION
	// UpVector is set in INIT - it is a const , should it be ?
	// we know the perpendicular distance between pivot and force
	/* 
	// Set the upVector
	// Get the cos(angle) - planeNormal DOT up = cos(angle)
	// Get perpendicular distance between pivot & force
	// force = torqueMagnitude
	// pivot is the upVector || is it the planeNormal
	*/
	upVector = { 0.0f,1.0f,0.0f };														/// generate the upVector
	angle = VMath::dot(planeNormal, upVector);					/// acos(planeNormal DOT upVector)	/// not the right angle set to avoid error
	float distanceToPivot = sphereBody->radius * sin(angle);

	/// DIRECTION , FIND DIRECTION USING CROSS PRODUCT
	// Find mag of torque
	// find axis of rotation
	// torque = (UP CROSS planeNormal)
	// torqueMagnitude is the weight of the ball - we need a direction and a Magnitude 
	// weight * distance to pivot
	torque = VMath::cross(upVector, planeNormal);
	torqueMagnitude = VMath::mag(torque);
	torqueMagnitude = torqueMagnitude * distanceToPivot;
	//printf("Torque Magnitude = %f\n", torqueMagnitude);

	/// BALL MOVING
	sphereBody->ApplyTourque(torque);
	sphereBody->UpdateAngularVelocity(deltaTime);

	/// Change the orientation using quaternion.
	sphereBody->UpdateOrientation(deltaTime);
	/// velocityMag = angularVelocity  * radius
	velocityMagnitutde = VMath::mag(sphereBody->angularVelocity * sphereBody->radius);
	/// Velocity Direction
	// velocityDirection = angularVelocityDirection CROSS planeNormal
	velocityDirection = VMath::cross(sphereBody->angularVelocity, planeNormal);
	/// set the sphereBody velocity to velocityMagnitude * velocityDirection
	// mag is the speed , velocity is the direction
	sphereBody->vel = velocityMagnitutde * velocityDirection;
	sphereBody->Update(deltaTime);


	/// Starting camera position , the vector location of the sphere's body's position
	cameraPosition = cameraPosition - sphereBody->pos;

	// WHY INVERSE ! ...... looking down the neg z axis !
	// initial is getQuat() in handle events, gets the inital position of the orientation of the quat
	// trackball.HandleEvents(sdlEvent) is sandwiched in between to gather controler input
	// final is is getQuat() is the orientation after movement 
	Quaternion changeInTrackballOrientation = finalTrackballOrientation * QMath::inverse(initialTrackballOrientation);
	// cam orientation will equal the finalOrientaion *= inverseOrientaion(initial) 
	// then correct the rotate of the cam position in relation to the change in trackball orientaion 
	cameraOrientation *= changeInTrackballOrientation;
	cameraPosition = QMath::rotate(cameraPosition, changeInTrackballOrientation);
	/// Explanation
	cameraPosition = cameraPosition + sphereBody->pos;
	/// MATRIX
	Matrix4 T = MMath::translate(cameraPosition);
	Matrix4 R = MMath::toMatrix4(cameraOrientation);
	// will place at the origin
	viewMatrix = MMath::inverse(R) * MMath::inverse(T);		// why inverse ? - check slides , check cat slides


}

void Scene2p::Render() const {
	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_CULL_FACE);

	/// Set the background color then clear the screen
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (!drawInWireMode) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	else {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

	/// have to edit the shader for the plane !!!!
	/// Fragcolor = {1.0,1.0,1.0,0.0}; -> FragShader

	/// Render Shader

	glUseProgram(shader->GetProgram());

	glUniform3fv(glGetUniformLocation(shader->GetProgram(), "LIGHT_POSITION[0]"), 1, LIGHT_POSITION[0]);		// LIGHT 
	glUniformMatrix4fv(shader->GetUniformID("projectionMatrix"), 1, GL_FALSE, projectionMatrix);
	glUniformMatrix4fv(shader->GetUniformID("viewMatrix"), 1, GL_FALSE, viewMatrix);
	/// Plane Arrays Finally !! ....
	glUniform4fv(glGetUniformLocation(shader->GetProgram(), "SPECULAR[0]"), 2, SPECULAR[index_0]);
	glUniform4fv(glGetUniformLocation(shader->GetProgram(), "DIFFUSE[0]"), 2, DIFFUSE[index_0]);
	glUniform4fv(glGetUniformLocation(shader->GetProgram(), "AMBIENT[0]"), 2, AMBIENT[index_0]);
	/// Render plane mesh
	glUniformMatrix4fv(shader->GetUniformID("modelMatrix"), 1, GL_FALSE, planeBody->getModelMatrix());
	planeMesh->Render(GL_TRIANGLES);
	/// Sphere Arrays Finally !! ....
	glUniform3fv(glGetUniformLocation(shader->GetProgram(), "LIGHT_POSITION[0]"), 2, LIGHT_POSITION[1]);		// LIGHT
	glUniform4fv(glGetUniformLocation(shader->GetProgram(), "SPECULAR[0]"), 2, SPECULAR[index_1]);
	glUniform4fv(glGetUniformLocation(shader->GetProgram(), "DIFFUSE[0]"), 2, DIFFUSE[index_1]);
	glUniform4fv(glGetUniformLocation(shader->GetProgram(), "AMBIENT[0]"), 2, AMBIENT[index_1]);
	/// Render Sphere mesh
	glUniformMatrix4fv(shader->GetUniformID("modelMatrix"), 1, GL_FALSE, sphereBody->getModelMatrix());
	sphereMesh->Render(GL_TRIANGLES);
	
	
	glUseProgram(0);
}




