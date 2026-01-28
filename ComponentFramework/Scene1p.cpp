#include <glew.h>
#include <iostream>
#include <SDL.h>
#include "Scene1p.h"			/// scene1g.h
#include <MMath.h>
#include "Debug.h"
#include "Mesh.h"
#include "Shader.h"
#include "Body.h"
#include <QMath.h>

Body bodyAdmin;
// initializer
Scene1p::Scene1p() :
	
	drawInWireMode{ false }, 
	shader{ nullptr }, 
	sphereMesh{ nullptr },
	sphereBody{ nullptr }, 
	planeBody{ nullptr },
	planeMesh {nullptr},
	planeNormal {0,1,0}
	//upVector{ 0, 1, 0 }
{	Debug::Info("Created Scene1p: ", __FILE__, __LINE__);	}

/// destructor
Scene1p::~Scene1p() {
	Debug::Info("Deleted Scene1p: ", __FILE__, __LINE__);
}

bool Scene1p::OnCreate() {
	Debug::Info("Loading assets Scene1p: ", __FILE__, __LINE__);

	/// UNIFORMS - LIGHTS
	Light_Position0 = Vec3(-15.0f, 15.0f,0.0f);
	
	/// Plane
	planeBody = new Body();
	planeBody->OnCreate();
	planeBody->orientation = QMath::angleAxisRotation(90, Vec3(1, 0, 0));
	planeBody->orientation *= QMath::angleAxisRotation(45, Vec3(0, 0, 1));
	planeNormal = Vec3(0, 0, -1);
	planeNormal = QMath::rotate(planeNormal, planeBody->orientation);
	printf("Plane Normal \n"); planeNormal.print();	/// Handle Events -> p to print the planeNormal

	/// Sphere
	// V = W X N (velocity = angular velocity cross normal -> (assume each letter is a vector)
	sphereBody = new Body();
	sphereBody->OnCreate();
	sphereBody->pos = Vec3(0, 1.3, 0);
	sphereBody->angularVelocity = Vec3(0, 0, 0);
	sphereBody->radius = 1;
	// Speed of the ball
	sphereBody->angularAcceleration = Vec3(0, 0, 1);

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
	viewMatrix = MMath::lookAt(Vec3(0.0f, 0.0f, 12.0f), Vec3(0.0f, 0.0f, 0.0f), Vec3(0.0f, 1.0f, 0.0f));

	//projectionMatrix_sphere = MMath::perspective(45.0f, (16.0f / 9.0f), 0.5f, 100.0f);;
	//viewMatrix_sphere = MMath::lookAt(Vec3(0.0f, 0.0f, 5.5f), Vec3(0.0f, 0.0f, 0.0f), Vec3(0.0f, 1.0f, 0.0f));
	//rotate_sphere = MMath::rotate(0.0f, Vec3(0.0f, 0.0f, 0.0f));
	//translate_sphere = MMath::translate(Vec3(0.0f,0.0f,0.0f));
	//modelMatrix_sphere = translate_sphere * rotate_sphere * scale_sphere;

	return true;

	/*
	/// Physics obj manipulation
	// when rotating the obj rotate the normal
	// check class notes , Discord , email Umer || book appointment
	// force = mass * Accel

	/// Physics Equations
	// Equation 1 Torque = Force * Dist to Pivot == Torque = Force * Weight
	// Equation 2 Torque = Rotational Inertia * Angular Accel

	// finalV = initialV + acc * t
	// FinalW = initialW * angularAccel * t		// -> w == Angular Velocity

	// find the Angle theta
	// find the distance
	// find the Torque (magnitude)

	// Torquedir = Up x Normal (cross product)
	*/

}
/// Clean Up
void Scene1p::OnDestroy() {
	Debug::Info("Deleting assets Scene1p: ", __FILE__, __LINE__);
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
}
/// Help on the rotation , and the normals, getting the correct readings
void Scene1p::HandleEvents(const SDL_Event& sdlEvent) {
	switch (sdlEvent.type) {
	case SDL_KEYDOWN:
		switch (sdlEvent.key.keysym.scancode) {
		case SDL_SCANCODE_W:
			drawInWireMode = !drawInWireMode;
			break;

		case SDL_SCANCODE_P:
			printf("plane normal:: \n");planeNormal.print();
			break;

			/// W , A , S , D
		case SDL_SCANCODE_UP:
			printf("UP\n"); planeNormal.print();
			planeBody->orientation *= QMath::angleAxisRotation(10, Vec3(-1, 0, 0));
			planeNormal = QMath::rotate(planeNormal, planeBody->orientation);				/// Normals don't read right , clarification , help
			break;
		case SDL_SCANCODE_LEFT:
			printf("LEFT\n"); planeNormal.print();
			planeBody->orientation *= QMath::angleAxisRotation(10, Vec3(0, 0, 1));
			planeNormal = QMath::rotate(planeNormal, planeBody->orientation);
			break;
		case SDL_SCANCODE_RIGHT:
			printf("RIGHT\n"); planeNormal.print();
			planeBody->orientation *= QMath::angleAxisRotation(10, Vec3(0, 0, -1));
			planeNormal = QMath::rotate(planeNormal, planeBody->orientation);
			break;
		case SDL_SCANCODE_DOWN:
			printf("DOWN\n"); planeNormal.print(); 
			//planeBody->pos.y--; 
			planeBody->orientation *= QMath::angleAxisRotation(10, Vec3(1, 0, 0));
			planeNormal = QMath::rotate(planeNormal, planeBody->orientation);				/// Normals don't read right , clarification , help
			///break;
		/// Just to test the Deg_to_rad
		//case SDL_SCANCODE_C:
		//	//bodyAdmin.converttoRads(1.0f, 10.0f);
		//	break;
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

void Scene1p::Update(const float deltaTime) 
{
	/// THIS NEEDS FINISHING !! TODAY !
	// TODO for YOU
	// Calculate torqueMag using forceMag * distance to pivot
	// The force is the weight of the sphere
	// The distance to the pivot relies on the angle 
	// between the weight and the normal
	
	/// UpVector is set in INIT - it is a const , should it be ?
	/// we know the perpendicular distance between pivot and force
	// Set the upVector
	// Get the cos(angle) - planeNormal DOT up = cos(angle)
	// Get perpendicular distance between pivot & force
	// force = torqueMagnitude
	// pivot is the upVector || is it the planeNormal
	upVector = { 0.0f,1.0f,0.0f };														/// generate the upVector
	angle = VMath::dot(planeNormal, upVector);					/// acos(planeNormal DOT upVector)	/// not the right angle set to avoid error
	float distanceToPivot = sphereBody->radius * sin(angle);
	printf("UpVetor = { %f,%f,%f }\nangle = %f\n", upVector.x , upVector.y , upVector.z , angle);
	printf("Distance To Pivot = %f\n", distancetoPivot);

	/// Find mag of torque
	/// find axis of rotation
	/// torque = (UP CROSS planeNormal) -- is it the planeNormal ? , yes it is dummy !!
	/// torqueMagnitude is the weight of the ball - we need a direction and a Magnitude 
	// weight * distance to pivot
	torque = VMath::cross(upVector, planeNormal);
	torqueMagnitude = VMath::mag(torque);
	torqueMagnitude = torqueMagnitude * distanceToPivot;
	printf("Torque Magnitude = %f\n", torqueMagnitude);
	
	//Vec3 torqueDirection;
	//float torqueMagnitude = 1.0f; /// init to avoid error ! - CHANGE TO CORRECT VARIABLE
	//Vec3 torque = torqueMagnitude * torqueDirection;

	/// BALL MOVING
	sphereBody->ApplyTourque(torque);
	sphereBody->UpdateAngularVelocity(deltaTime);

	/// Change the orientation using quaternion. Change the orientation of the ball, based on the angular Velocity
	sphereBody->UpdateOrientation(deltaTime);
	/// velocityMag = angularVelocityMag * radius
	velocityMagnitutde = VMath::mag(sphereBody->angularVelocity * sphereBody->radius);
	/// Velocity Direction
	// velocityDirection = angularVelocityDirection CROSS planeNormal
	velocityDirection = VMath::cross(sphereBody->angularVelocity, planeNormal);
	/// set the sphereBody velocity to velocityMagnitude * velocityDirection
	sphereBody->vel = velocityMagnitutde * velocityDirection;
	sphereBody->Update(deltaTime);

}

void Scene1p::Render() const {

	/// Set the background color then clear the screen
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glEnable(GL_CULL_FACE);

	if (drawInWireMode) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	else {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

	/// have to edit the shader for the plane !!!!
	/// Fragcolor = {1.0,1.0,1.0,0.0}; -> FragShader

	/// Render Shader
	glUseProgram(shader->GetProgram());
	glUniform3fv(shader->GetUniformID("Light_Position0"), 1, Light_Position0);
	glUniformMatrix4fv(shader->GetUniformID("projectionMatrix"), 1, GL_FALSE, projectionMatrix);
	glUniformMatrix4fv(shader->GetUniformID("viewMatrix"), 1, GL_FALSE, viewMatrix);
	glUniformMatrix4fv(shader->GetUniformID("modelMatrix"), 1, GL_FALSE, modelMatrix);
	
	/// Render plane
	glUniformMatrix4fv(shader->GetUniformID("modelMatrix"), 1, GL_FALSE, planeBody->getModelMatrix());
	planeMesh->Render(GL_TRIANGLES);
	/// Render sphere 
	glUniformMatrix4fv(shader->GetUniformID("modelMatrix"), 1, GL_FALSE, sphereBody->getModelMatrix());
	sphereMesh->Render(GL_TRIANGLES);


	glUseProgram(0);
}


// END



