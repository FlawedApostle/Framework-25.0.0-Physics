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


Scene2p::Scene2p() :

	  drawInWireMode	{ false }
	, drawInNormalsFace	{ false }
	, shader				{ nullptr }
	, shader_normals_face	{ nullptr }
	, shader_normals_line	{ nullptr }
	, sphereMesh	{ nullptr }
	, sphereBody	{ nullptr }
	, planeBody		{ nullptr }
	, planeMesh		{ nullptr }
	, planeNormal	{ 0,1,0 }															// Normal
	, trackball		{nullptr}
	//upVector{ 0, 1, 0 }																// Up Vector
{
	Debug::Info("Created Scene2p: ", __FILE__, __LINE__);
}
Scene2p::~Scene2p() 
{
	Debug::Info("Deleted Scene2p: ", __FILE__, __LINE__);
}

bool Scene2p::OnCreate() {
	Debug::Info("Loading assets Scene2p: ", __FILE__, __LINE__);

	/// Trackball
	trackball = new Trackball();

	/// UNIFORMS
	lightPos = Vec3(10.0f, 10.0f, 0.0f);												// light position for shader
	normalScale = 2.0f;																	// normal scale for shader
	//lightPosLoc = glGetUniformLocation(shader->GetProgram(), "lightPos");				// Cache the uniform location for the light position in the shader
	
	upVector = { 0.0f,1.0f,0.0f };														/// generate the upVector

	/// Plane
	planeBody = new Body();
	planeBody->OnCreate();
	planeBody->radius = 2.0f;
	planeBody->orientation = QMath::angleAxisRotation(90, Vec3(-1, 0, 0)); 
	planeNormal = Vec3(0.0f, 1.0f, 0.0f);
	/*planeNormal = QMath::rotate(planeNormal, planeBody->orientation);*/

	/// Sphere
	// V = W X N (velocity = angular velocity cross normal -> (assume each letter is a vector)
	sphereBody = new Body();
	sphereBody->OnCreate();
	sphereBody->pos = Vec3(0.0f, 1.0, 0.0f);
	sphereBody->angularVelocity = Vec3(1.0f, 0.0f, 0.0f);								// starts at 0 for rest
	sphereBody->radius = 1;
	sphereBody->angularAcceleration = Vec3(10.0f, 0.0f, 0.0f);							// SPEED  - starts at 0 for rest
	planeNormal = QMath::rotate(Vec3(0,1,0), planeBody->orientation);					// No Drift - Fixed Base to start off

	// ----- 3D
	// PLANE
	planeMesh = new Mesh("meshes/Plane.obj");
	planeMesh->OnCreate();
	// SPHERE
	sphereMesh = new Mesh("meshes/Sphere.obj");
	sphereMesh->OnCreate();
	
	/// SHADER
	shader = new Shader("shaders/defaultPhong/phongVert.glsl", "shaders/defaultPhong/phongFrag.glsl");
	shader->CheckShader(shader);						// added shader CHECK function to Shader.h
	// SHADER - NORMALS 
	shader_normals_face = new Shader("shaders/Normals/normalsVert.glsl", "shaders/Normals/normalsFrag.glsl");
	shader_normals_face->CheckShader(shader_normals_face);		// added shader CHECK function to Shader.h
	// SHADER - NORMAL LINES
	shader_normals_line = new Shader(
		"shaders/NormalsDraw/drawNormals.vert" , 
		"shaders/NormalsDraw/drawNormals.frag" ,
		nullptr, nullptr,								
		"shaders/NormalsDraw/drawNormals.geom"
	);
	shader_normals_line->CheckShader(shader_normals_line);

	// CAMERA
	projectionMatrix = MMath::perspective(45.0f, (16.0f / 9.0f), 0.5f, 100.0f);
	//viewMatrix = MMath::lookAt(Vec3(0.0f, 0.0f, 20.0f), Vec3(0.0f, 0.0f, 0.0f), Vec3(0.0f, 1.0f, 0.0f));
	cameraPosition = sphereBody->pos + Vec3(0.0f, 0.0f, 10.0f);
	cameraOrientation = QMath::angleAxisRotation(0, Vec3(1.0f, 0.0f, 0.0f));
	Matrix4 T = MMath::translate(cameraPosition);
	Matrix4 R = MMath::toMatrix4(cameraOrientation);
	viewMatrix = MMath::inverse(R) * MMath::inverse(T);		// why inverse ? - check slides , check cat slides
	
	return true;
}

// DESTROY
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
	/// Trackball
	delete trackball;
	/// ----------- SHADERS
	shader->OnDestroy();
	delete shader;									// Shader 1
	shader_normals_face->OnDestroy();				// Shader 2
	delete shader_normals_face;
	shader_normals_line->OnDestroy();				// Shader 3
	delete shader_normals_line;
}

/// CONTROLLS
void Scene2p::HandleEvents(const SDL_Event& sdlEvent) {
	/// Trackball
	initialTrackballOrientation = trackball->getQuat();
	trackball->HandleEvents(sdlEvent);
	finalTrackballOrientation = trackball->getQuat();
	
	switch (sdlEvent.type) {
	case SDL_KEYDOWN:
		switch (sdlEvent.key.keysym.scancode) {
		
		// WIREFRAME
		case SDL_SCANCODE_M:
			drawInWireMode = !drawInWireMode;
			break;

		// NORMALS - FACE
		case SDL_SCANCODE_N:
			drawInNormalsFace = !drawInNormalsFace;
			break;

		// NORMALS - LINE
		case SDL_SCANCODE_B:
			drawInNormalsLine = !drawInNormalsLine;
			break;

			// PLANE DETAILS
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
			//planeNormal = QMath::rotate(planeNormal, rotation);
			planeNormal = QMath::rotate(Vec3(0, 1, 0), planeBody->orientation);

			break;
		case SDL_SCANCODE_A:
			printf("LEFT\n");
			rotation = QMath::angleAxisRotation(2.0f, axis_Left);
			planeBody->orientation = rotation * planeBody->orientation;
			//planeNormal = QMath::rotate(planeNormal, rotation);
			planeNormal = QMath::rotate(Vec3(-1, 0, 0), planeBody->orientation);

			break;
		case SDL_SCANCODE_D:
			printf("RIGHT\n");
			rotation = QMath::angleAxisRotation(2.0f, axis_Right);
			planeBody->orientation = rotation * planeBody->orientation;
			//planeNormal = QMath::rotate(planeNormal, rotation);
			planeNormal = QMath::rotate(Vec3(1, 0, 0), planeBody->orientation);

			break;
		case SDL_SCANCODE_S:
			printf("DOWN\n"); planeNormal.print();
			rotation = QMath::angleAxisRotation(2.0f, axis_PitchDown);
			planeBody->orientation = rotation * planeBody->orientation;
			//planeNormal = QMath::rotate(planeNormal, rotation);
			planeNormal = QMath::rotate(Vec3(0, -1, 0), planeBody->orientation);

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
	planeNormal = QMath::rotate(planeNormal, planeBody->orientation);				// ----- PLANE NORMAL ------		MUST update the plane normal for 'punctual' rolling
	float cosTheta = VMath::dot(planeNormal, upVector);								// ----- ANGULAR ACCELERATION ------ FIND the angle between PLANE normal and UPVECTOR to distinguish angular acceleration
	float theta = acos(cosTheta);
	float distanceToPivot = sphereBody->radius * sin(theta);

	/// DIRECTION , FIND DIRECTION USING CROSS PRODUCT
	// Find mag of torque
	// find axis of rotation
	// torque = (UP CROSS planeNormal)
	// torqueMagnitude is the weight of the ball - we need a direction and a Magnitude 
	// weight * distance to pivot
	torque = VMath::cross(upVector, planeNormal);
	Vec3 torqueDir = VMath::normalize(torque);
	torqueMagnitude = VMath::mag(torque);
	torqueMagnitude = torqueMagnitude * distanceToPivot;
	Vec3 torqueFinal = torqueDir * torqueMagnitude;
	//printf("Torque Magnitude = %f\n", torqueMagnitude);

	/// BALL MOVING
	sphereBody->ApplyTourque(torqueFinal);
	sphereBody->UpdateAngularVelocity(deltaTime);

	/// Change the orientation using quaternion.
	sphereBody->UpdateOrientation(deltaTime);
	/// velocityMag = angularVelocity  * radius
	velocityMagnitutde = VMath::mag(sphereBody->angularVelocity * sphereBody->radius);
	/* Velocity Direction
	// - velocityDirection = angularVelocityDirection CROSS planeNormal
	// - set the sphereBody velocity to velocityMagnitude * velocityDirection mag is the speed , velocity is the direction */
	velocityDirection = VMath::normalize(VMath::cross(sphereBody->angularVelocity, planeNormal));
	sphereBody->vel = velocityMagnitutde * velocityDirection;
	//sphereBody->Update(deltaTime);


	// TRACKBALL - SYNTHETIC CAMERA
	// Starting camera position
	//cameraPosition = cameraPosition - sphereBody->pos;
	cameraPosition = cameraPosition - planeBody->pos;

	// WHY INVERSE ! ...... looking down the neg z axis !
	// initial is getQuat() in handle events, gets the inital position of the orientation of the quat
	// trackball.HandleEvents(sdlEvent) is sandwiched in between to gather controler input - final is is getQuat() is the orientation after movement 
	Quaternion changeInTrackballOrientation = finalTrackballOrientation * QMath::inverse(initialTrackballOrientation);
	// cam orientation will equal the finalOrientaion *= inverseOrientaion(initial) 
	// then correct the rotate of the cam position in relation to the change in trackball orientaion 
	cameraOrientation *= changeInTrackballOrientation;
	cameraPosition = QMath::rotate(cameraPosition, changeInTrackballOrientation);
	

	/// MATRIX
	Matrix4 T = MMath::translate(cameraPosition);
	Matrix4 R = MMath::toMatrix4(cameraOrientation);
	// will place at the origin
	viewMatrix = MMath::inverse(R) * MMath::inverse(T);		// why inverse ? - check slides , check cat slides


}

void Scene2p::Render() const {
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	// -- FACE WINDING
	glFrontFace(GL_CCW);
	// glFrontFace(GL_CW);

	/// Set the background color then clear the screen
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// =========================
	// WIREFRAME TOGGLE
	// =========================
	if (!drawInWireMode) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	else {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

	// ============================
	// PASS 1 — BASE GEOMETRY
	// ============================

	if (drawInNormalsFace) {
		glUseProgram(shader_normals_face->GetProgram());
		// 2. Send matrices to Face Shader
		glUniformMatrix4fv(shader_normals_face->GetUniformID("projectionMatrix"), 1, GL_FALSE, projectionMatrix);
		glUniformMatrix4fv(shader_normals_face->GetUniformID("viewMatrix"), 1, GL_FALSE, viewMatrix);
		// 3. Render Plane with Face Shader
		glUniformMatrix4fv(shader_normals_face->GetUniformID("modelMatrix"), 1, GL_FALSE, planeBody->getModelMatrix());
		planeMesh->Render(GL_TRIANGLES);

	}
	else 
	{
		// Default Shader
		glUseProgram(shader->GetProgram());
		glUniform3fv(glGetUniformLocation(shader->GetProgram(), "lightPos"), 1, &lightPos.x);

		glUniformMatrix4fv(shader->GetUniformID("projectionMatrix"), 1, GL_FALSE, projectionMatrix);
		glUniformMatrix4fv(shader->GetUniformID("viewMatrix"), 1, GL_FALSE, viewMatrix);
		
		// PLANE
		glUniformMatrix4fv(shader->GetUniformID("modelMatrix"), 1, GL_FALSE, planeBody->getModelMatrix());
		planeMesh->Render(GL_TRIANGLES);

		// SPHERE
		glUniformMatrix4fv(shader->GetUniformID("modelMatrix"), 1, GL_FALSE, sphereBody->getModelMatrix());
		sphereMesh->Render(GL_TRIANGLES);

	}
	// ============================
	// PASS 2 — NORMAL LINES
	// ============================

	if (drawInNormalsLine) {
		glUseProgram(shader_normals_line->GetProgram());

		glUniformMatrix4fv(shader_normals_line->GetUniformID("projectionMatrix"), 1, GL_FALSE, projectionMatrix);
		glUniformMatrix4fv(shader_normals_line->GetUniformID("viewMatrix"), 1, GL_FALSE, viewMatrix);

		// PLANE NORMALS
		glUniformMatrix4fv(shader_normals_line->GetUniformID("modelMatrix"), 1, GL_FALSE, planeBody->getModelMatrix());
		planeMesh->Render(GL_TRIANGLES);

		// SPHERE NORMALS
		glUniformMatrix4fv(shader_normals_line->GetUniformID("modelMatrix"), 1, GL_FALSE, sphereBody->getModelMatrix());
		sphereMesh->Render(GL_TRIANGLES);
	}

	glUseProgram(0);
}






