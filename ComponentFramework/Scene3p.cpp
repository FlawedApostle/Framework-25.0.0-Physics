#include <glew.h>
#include <iostream>
#include <SDL.h>
#include "Scene3p.h"			/// scene3p.h
#include <MMath.h>
#include "Debug.h"
#include "Mesh.h"
#include "Shader.h"
#include "Body.h"
#include <QMath.h>
#include "Quaternion.h"

#include "Collision.h"

/// Notes are Written in Repo Jargon - check NOTES_PHYSICS FOR FORMULA BREAKDOWNS


Scene3p::Scene3p() :

	  drawInWireMode			{ false }
	, drawInNormalsFace			{ false }
	, drawInNormalsLine			{ false }
	, shader					{ nullptr }
	, shader_normals_face		{ nullptr }
	, shader_normals_line		{ nullptr }
	, sphereMesh				{ nullptr }
	, sphereBody				{ nullptr }
	, planeBody					{ nullptr }
	, planeMesh					{ nullptr }
	, sphereCollision0_Body		{ nullptr }
	, sphereCollision0_Mesh		{ nullptr }
	, sphereCollision0_Normal	{ 0,0,0 }
	, trackball					{ nullptr }
	, distancetoPivot			{ NULL  }
	, torqueMagnitude			{ NULL	}
	, angle						{ NULL }
	, velocityMagnitutde		{ NULL }
	, normalScale				{ NULL }
	, lightPosLoc				{ NULL }
	, torqueDirection			{ 0,0,0 }
	, torque					{ 0,0,0 }
	, velocityDirection			{ 0,0,0 }
	, linearVelocity			{ 0,0,0 }
	, planeNormal				{ 0,1,0 }
	, upVector					{ 0.0f,1.0f,0.0f }
	
{
	Debug::Info("Created Scene3p: ", __FILE__, __LINE__);
}
Scene3p::~Scene3p()
{
	Debug::Info("Deleted Scene3p: ", __FILE__, __LINE__);
}

bool Scene3p::OnCreate() {
	Debug::Info("Loading assets Scene3p: ", __FILE__, __LINE__);
	//lightPosLoc = glGetUniformLocation(shader->GetProgram(), "lightPos");						// Cache the uniform location for the light position in the shader
	//upVector = { 0.0f,1.0f,0.0f };															/// generate the upVector - Currently in Update

	/// Trackball
	trackball = new Trackball();

	/// UNIFORMS
	lightPos = Vec3(10.0f, 10.0f, 0.0f);														// light position for shader
	normalScale = 2.0f;																			// normal scale for shader

	/// Plane
	planeBody = new Body();
	planeBody->OnCreate();
	planeBody->orientation = QMath::angleAxisRotation(90, Vec3(-1, 0, 0));
	planeNormal = Vec3(0.0f, 0.0f, 1.0f);
	planeNormal = QMath::rotate(Vec3(0.0f, 0.0f, 1.0f), planeBody->orientation);					// No Drift - Fixed Base to start off
	planeBody->scale = Vec3(5.0f, 1.0f, 5.0f);
	

	/// Sphere 1.
	// V = W X N (velocity = angular velocity cross normal -> (assume each letter is a vector)
	sphereBody = new Body();
	sphereBody->OnCreate();
	sphereBody->pos = Vec3(0.0f, 1.0, 0.0f);
	sphereBody->angularVelocity = Vec3(0.0f, 0.0f, 1.0f);										// starts at 0 for rest
	//sphereBody->radius = 1;
	sphereBody->scale = Vec3(2.0f, 2.0f, 2.0f);
	sphereBody->angularAcceleration = Vec3(1.0f, 0.0f, 0.0f);									// SPEED  - starts at 0 for rest
	
	/// sphere .2 - COLLISIONS
	sphereCollision0_Body = new Body();
	sphereCollision0_Body->OnCreate();
	sphereCollision0_Body->pos = Vec3(0.0f, 1.0, 0.0f);


	// ----- 3D
	// PLANE
	planeMesh = new Mesh("meshes/Plane.obj");
	planeMesh->OnCreate();
	// SPHERE 1.
	sphereMesh = new Mesh("meshes/Sphere.obj");
	sphereMesh->OnCreate();
	// SPHERE 2.
	sphereCollision0_Mesh = new Mesh("meshes/Sphere.obj");
	sphereCollision0_Mesh->OnCreate();

	/// ----- SHADER 1. ---- DEFAULT.
	shader = new Shader("shaders/defaultPhong/phongVert.glsl", "shaders/defaultPhong/phongFrag.glsl");
	shader->CheckShader(shader);										// added shader CHECK function to Shader.h
	
	// SHADER - NORMALS 
	shader_normals_face = new Shader("shaders/Normals/normalsVert.glsl", "shaders/Normals/normalsFrag.glsl");
	shader_normals_face->CheckShader(shader_normals_face);
	
	// SHADER - NORMAL LINES
	shader_normals_line = new Shader(
		"shaders/NormalsDraw/drawNormals.vert",
		"shaders/NormalsDraw/drawNormals.frag",
		nullptr, nullptr,
		"shaders/NormalsDraw/drawNormals.geom"
	);
	shader_normals_line->CheckShader(shader_normals_line);				// ----- DEBUG SHADER NORMALS LINES

	
	/// ----- CAMERA STARTING POSITIONING
	cameraPosition = sphereBody->pos + Vec3(10.0f, 0.0f, 25.0f);
	//cameraPosition = planeBody->pos + Vec3(0.0f, 0.0f, 10.0f); 
	
	// ----- CAMERA
	projectionMatrix = MMath::perspective(45.0f, (16.0f / 9.0f), 0.5f, 100.0f);
	//viewMatrix = MMath::lookAt(Vec3(0.0f, 0.0f, 20.0f), Vec3(0.0f, 0.0f, 0.0f), Vec3(0.0f, 1.0f, 0.0f));
	cameraOrientation = QMath::angleAxisRotation(0, Vec3(1.0f, 0.0f, 0.0f));
	Matrix4 T = MMath::translate(cameraPosition);
	Matrix4 R = MMath::toMatrix4(cameraOrientation);
	viewMatrix = MMath::inverse(R) * MMath::inverse(T);

	// ----- FIND POINT BETWEEN TWO POSITIONS
	//std::cout << "test Body* " << Collision::SphereSphereCollisionDetected_test(sphereBody, sphereCollision0_Body) << "\n";

	return true;
}

// DESTROY
void Scene3p::OnDestroy() {
	Debug::Info("Deleting assets Scene3p: ", __FILE__, __LINE__);
	/// sphere 1.
	sphereBody->OnDestroy();
	delete sphereBody;
	sphereMesh->OnDestroy();
	delete sphereMesh;
	/// Sphere 2.
	sphereCollision0_Body->OnDestroy();
	delete sphereCollision0_Body;
	sphereCollision0_Mesh->OnDestroy();
	delete sphereCollision0_Mesh;
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
void Scene3p::HandleEvents(const SDL_Event& sdlEvent) {
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
			planeNormal.print("planeNormal");  upVector.print("upVector");
			sphereBody->vel.print("sphereBody->vel");

			printf("torqueMagnitude %f\n", torqueMagnitude);
			printf("velocityMagnitutde %f\n", velocityMagnitutde);
			printf("velocityDirection (%f,%f,%f)\n", velocityDirection.x, velocityDirection.y, velocityDirection.z);
			break;

			/// W , A , S , D
		case SDL_SCANCODE_W:
			printf("UP\n");
			rotation = QMath::angleAxisRotation(2.0f, axis_PitchUp);
			planeBody->orientation = rotation * planeBody->orientation;
			//planeNormal = QMath::rotate(planeNormal, rotation);
			//planeNormal = QMath::rotate(Vec3(0.0f, 1.0f, 0.0f), planeBody->orientation);

			break;
		case SDL_SCANCODE_A:
			printf("LEFT\n");
			rotation = QMath::angleAxisRotation(2.0f, axis_Left);
			planeBody->orientation = rotation * planeBody->orientation;
			//planeNormal = QMath::rotate(planeNormal, rotation);
			//planeNormal = QMath::rotate(Vec3(0.0f, 1.0f, 0.0f), planeBody->orientation);

			break;
		case SDL_SCANCODE_D:
			printf("RIGHT\n");
			rotation = QMath::angleAxisRotation(2.0f, axis_Right);
			planeBody->orientation = rotation * planeBody->orientation;
			//planeNormal = QMath::rotate(planeNormal, rotation);
			//planeNormal = QMath::rotate(Vec3(0.0f, 1.0f, 0.0f), planeBody->orientation);

			break;
		case SDL_SCANCODE_S:
			printf("DOWN\n"); planeNormal.print();
			rotation = QMath::angleAxisRotation(2.0f, axis_PitchDown);
			planeBody->orientation = rotation * planeBody->orientation;
			//planeNormal = QMath::rotate(planeNormal, rotation);
			//planeNormal = QMath::rotate(Vec3(0.0f, 1.0f, 0.0f), planeBody->orientation);

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

void Scene3p::Update(const float deltaTime)
{
	// ----- FIND POINT BETWEEN TWO POSITIONS
	//std::cout << "test Body* " << Collision::SphereSphereCollisionDetected(sphereBody, sphereCollision0_Body) << "\n";
	
	/// 1. Recompute plane normal from orientation
	planeNormal = QMath::rotate(Vec3(0.0f, 0.0f, 1.0f), planeBody->orientation);
	planeNormal = VMath::normalize(planeNormal);
	/// 2.
	upVector = { 0.0f,1.0f,0.0f };																// GENERATE UPVECTOR ----- MORE RESPOSNIVE ??

	/// 3. TORQUE - ROTATION , BEGIN ROTATION
	/*  UpVector is set in INIT - it is a const , should it be ?
	 we know the perpendicular distance between pivot and force
	 ----- ANGULAR ACCELERATION 
	 ------ FIND the angle between PLANE normal and UPVECTOR to distinguish angular acceleration
	 Set the upVector
	 Get the cos(angle) - planeNormal DOT up = cos(angle)
	 Get perpendicular distance between pivot & force
	 force = torqueMagnitude
	 pivot is the upVector || is it the planeNormal

	 Calculate torqueMag using forceMag * distance to pivot
	 The force is the weight of the sphere
	 The distance to the pivot relies on the angle
	 between the weight and the normal
	*/
	float cosTheta = VMath::dot(planeNormal, upVector);
	//cosTheta = MMath::clamp(cosTheta, -1.0f, 1.0f);
	float theta = acos(cosTheta);
	float distanceToPivot = sphereBody->radius * sin(theta);

	/// AXIS OF ROTATION : UP x NORMAL
	/* Find mag of torque
	 find axis of rotation
	 torque = (UP CROSS planeNormal)
	 torqueMagnitude is the weight of the ball - we need a direction and a Magnitude
	 weight * distance to pivot 
	 */
	torque = VMath::cross(upVector, planeNormal);
	if(VMath::mag(torque) > VERY_SMALL) {
	Vec3 torqueDir = VMath::normalize(torque);
	torqueMagnitude = sphereBody->mass * 9.8f * distanceToPivot;
	//torqueMagnitude = distanceToPivot * sphereBody->mass;						// simple proportional model
	//torqueMagnitude = torqueMagnitude * distanceToPivot;						// simple proportional model
	Vec3 torqueFinal = torqueDir * torqueMagnitude;

	sphereBody->ApplyTourque(torqueFinal);
	}

	/// 4. UPDATE ANGULAR MOTION
	sphereBody->UpdateAngularVelocity(deltaTime);
	sphereBody->UpdateOrientation(deltaTime);													// Change the orientation using quaternion.
	
	
	// ----- GLOBAL ----- EXPLICITLY ZERO OUT THE LINEAR VELOCITY BEFORE CALCULATING NEW ONE BASED ON DOWNHILL DIRECTION
	linearVelocity = Vec3(0.0f, 0.0f, 0.0f);		

	/// ----- GLOBAL ----- SPHERE ON PLANE
	float planeDist = VMath::dot(sphereBody->pos - planeBody->pos, planeNormal);
	const float CONTACT_EPS = 0.01f;
	bool onPlane = planeDist <= sphereBody->radius + CONTACT_EPS;
	//bool onPlane = fabs(planeDist - sphereBody->radius) < CONTACT_EPS;										// ----- DEPRECAED
	
	// ------ GLOBAL ----- GRAVITY
	Vec3 gravity(0.0f, -9.8f, 0.0f);
	Vec3 gravityNormalComp = VMath::dot(gravity, planeNormal) * planeNormal;									// Remove component along plane normal → get tangent component
	Vec3 downhill = gravity - gravityNormalComp;
	float angSpeed = VMath::mag(sphereBody->angularVelocity);
	float speed = angSpeed * sphereBody->radius;
	
	/// 5. ROTATION - ANGULAR ACCELERATION / VELOCITY
	/* 
	Remove component along plane normal → get tangent component
	Project gravity onto plane - remove perpendicular compeoent to leave parallel direction
	*/
	if (onPlane)
	{
		/// ------------- LOOP FUNCTIONS 2 , 3
		linearVelocity = ComputeRollingVelocity_Cross(planeNormal);
		//sphereBody->vel = ComputeRollingVelocity_Cross(planeNormal);
		//linearVelocity = ComputeLinearVelocity(onPlane, sphereBody, gravity, downhill, deltaTime);
	}
	else
	{
		sphereBody->vel += gravity * deltaTime;
		linearVelocity = sphereBody->vel;
		//linearVelocity = ComputeFreeFallVelocity(sphereBody, gravity, deltaTime);								// GRAVITY FUNCTION TEST
	}


	
	
	/// ------------- LOOP FUNCTIONS 1
	//IfOnPlane(onPlane, sphereBody, gravity, downhill, linearVelocity, angSpeed, speed, deltaTime);

	// DEPRECATED LOOP 2. 
	/*
	if (onPlane) {
		// --- rolling on plane ---
		//Vec3 gravityNormalComp = VMath::dot(gravity, planeNormal) * planeNormal;
		//Vec3 downhill = gravity - gravityNormalComp;

		if (VMath::mag(downhill) > VERY_SMALL) {
			Vec3 downhillDir = VMath::normalize(downhill);
			float angSpeed = VMath::mag(sphereBody->angularVelocity);
			float speed = angSpeed * sphereBody->radius;
			linearVelocity = downhillDir * speed;
		}
	}
	else {
		// --- free fall ---
		sphereBody->vel += gravity * deltaTime;
		linearVelocity = sphereBody->vel;
	}
	*/





	// DEPRECATED LOOP 1. 
	//ComputeRollingVelocity(downhill);
	/*
	if (VMath::mag(downhill) > VERY_SMALL)
	{
		Vec3 downhillDir = VMath::normalize(downhill);
		
		// Rolling without slipping: v = ω × R (approx using |ω| * R along downhill)
		float angSpeed = VMath::mag(sphereBody->angularVelocity);
		float speed = angSpeed * sphereBody->radius;

		linearVelocity = downhillDir * speed;
	}
	*/



	/// 6. ASSIGN LINEAR VELOCITY AND INTEGRATE POSITION
	sphereBody->vel = linearVelocity;
	sphereBody->UpdatePos(deltaTime);
	
	/// 7. PLANE CONTACT CONSTRAINT (KEEP SPHERE RESTING ON PLANE)
	planeDist = VMath::dot(sphereBody->pos - planeBody->pos, planeNormal);
	if (planeDist < sphereBody->radius)
	{
		float penetration = sphereBody->radius - planeDist;
		sphereBody->pos += planeNormal * penetration;
	}

	// ------ Remove any velocity that pushes away from the plane (one‑way valve)
	float vdot = VMath::dot(sphereBody->vel, planeNormal);
	if (vdot > 0.0f) {
		sphereBody->vel -= planeNormal * vdot;
	}


	// ------ TRACKBALL - SYNTHETIC CAMERA - [Starting camera position]
	//cameraPosition = cameraPosition - sphereBody->pos;
	cameraPosition = cameraPosition - planeBody->pos;

	/* WHY INVERSE !...... looking down the neg z axis !
	// initial is getQuat() in handle events, gets the inital position of the orientation of the quat
	// trackball.HandleEvents(sdlEvent) is sandwiched in between to gather controler input - final is is getQuat() is the orientation after movement */
	Quaternion changeInTrackballOrientation = finalTrackballOrientation * QMath::inverse(initialTrackballOrientation);
	/* cam orientation will equal the finalOrientaion *= inverseOrientaion(initial)
	// - then correct the rotate of the cam position in relation to the change in trackball orientaion */
	cameraOrientation *= changeInTrackballOrientation;
	cameraPosition = QMath::rotate(cameraPosition, changeInTrackballOrientation);

	/// ----- MATRIX
	Matrix4 T = MMath::translate(cameraPosition);
	Matrix4 R = MMath::toMatrix4(cameraOrientation);
	viewMatrix = MMath::inverse(R) * MMath::inverse(T);


}

void Scene3p::Render() const {
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
	if (drawInWireMode) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	else {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

	// ============================
	// PASS 1 — BASE GEOMETRY
	// ============================

	//if (drawInNormalsFace) {
	//	glUseProgram(shader_normals_face->GetProgram());
	//	// 2. Send matrices to Face Shader
	//	glUniformMatrix4fv(shader_normals_face->GetUniformID("projectionMatrix"), 1, GL_FALSE, projectionMatrix);
	//	glUniformMatrix4fv(shader_normals_face->GetUniformID("viewMatrix"), 1, GL_FALSE, viewMatrix);
	//	// 3. Render Plane with Face Shader
	//	glUniformMatrix4fv(shader_normals_face->GetUniformID("modelMatrix"), 1, GL_FALSE, planeBody->getModelMatrix());
	//	planeMesh->Render(GL_TRIANGLES);

	//}
	//else
	//{
		// Default Shader
		glUseProgram(shader->GetProgram());
		glUniform3fv(glGetUniformLocation(shader->GetProgram(), "lightPos"), 1, &lightPos.x);

		glUniformMatrix4fv(shader->GetUniformID("projectionMatrix"), 1, GL_FALSE, projectionMatrix);
		glUniformMatrix4fv(shader->GetUniformID("viewMatrix"), 1, GL_FALSE, viewMatrix);

		// PLANE
		glUniformMatrix4fv(shader->GetUniformID("modelMatrix"), 1, GL_FALSE, planeBody->getModelMatrix());
		planeMesh->Render(GL_TRIANGLES);

		// SPHERE 1. ---- DEFAULT
		glUniformMatrix4fv(shader->GetUniformID("modelMatrix"), 1, GL_FALSE, sphereBody->getModelMatrix());
		sphereMesh->Render(GL_TRIANGLES);

		// SPEHERE 2. - COLLISION
		glUniformMatrix4fv(shader->GetUniformID("modelMatrix"), 1, GL_FALSE, sphereCollision0_Body->getModelMatrix());
		sphereCollision0_Mesh->Render(GL_TRIANGLES);

		glUseProgram(0);

	}
	// ============================
	// PASS 2 — NORMAL LINES
	// ============================

	//if (drawInNormalsLine) {
	//	glUseProgram(shader_normals_line->GetProgram());

	//	glUniformMatrix4fv(shader_normals_line->GetUniformID("projectionMatrix"), 1, GL_FALSE, projectionMatrix);
	//	glUniformMatrix4fv(shader_normals_line->GetUniformID("viewMatrix"), 1, GL_FALSE, viewMatrix);

	//	// PLANE NORMALS
	//	glUniformMatrix4fv(shader_normals_line->GetUniformID("modelMatrix"), 1, GL_FALSE, planeBody->getModelMatrix());
	//	planeMesh->Render(GL_TRIANGLES);

	//	// SPHERE NORMALS
	//	glUniformMatrix4fv(shader_normals_line->GetUniformID("modelMatrix"), 1, GL_FALSE, sphereBody->getModelMatrix());
	//	sphereMesh->Render(GL_TRIANGLES);
	//}

	//glUseProgram(0);
//}






/// ----- Linear Velocity

// 1.
Vec3 Scene3p::ComputeRollingVelocity_Cross(const Vec3& planeNormal)
{
	// Contact vector (center → contact point)
	// THIS ASSUMES THAT THE POINT IS DIRECTLY ALONG THE NORMAL - THIS IS OK ONLY FOR SPHERES...
	Vec3 r = planeNormal * sphereBody->radius;

	// True rolling velocity
	Vec3 v = VMath::cross(sphereBody->angularVelocity, r);

	return v;
}

// 2.
Vec3 Scene3p::ComputeFreeFallVelocity(Body* body, const Vec3& gravity, float dt)
{
	body->vel += gravity * dt;
	return body->vel;
}
 
// 3. MUTEABLE
Vec3 Scene3p::ComputeLinearVelocity(
    bool onPlane,
    Body* body,
    const Vec3& gravity,
    const Vec3& downhill,
    float deltaTime)
{
    if (onPlane)
    {
        if (VMath::mag(downhill) > VERY_SMALL)
        {
            Vec3 downhillDir = VMath::normalize(downhill);
            float angSpeed = VMath::mag(body->angularVelocity);
            float speed = angSpeed * body->radius;

            return downhillDir * speed;
        }

        return Vec3(0.0f, 0.0f, 0.0f);
    }
    else
    {
        body->vel += gravity * deltaTime;
        return body->vel;
    }
}

// 4. MUTEABLE
void Scene3p::UpdateLinearVelocity(
	bool onPlane,
	Body* body,
	const Vec3& gravity,
	const Vec3& downhill,
	float deltaTime,
	Vec3& outLinearVelocity)
{
	if (onPlane)
	{
		if (VMath::mag(downhill) > VERY_SMALL)
		{
			Vec3 downhillDir = VMath::normalize(downhill);
			float angSpeed = VMath::mag(body->angularVelocity);
			float speed = angSpeed * body->radius;

			outLinearVelocity = downhillDir * speed;
		}
		else
		{
			outLinearVelocity = Vec3(0.0f, 0.0f, 0.0f);
		}
	}
	else
	{
		body->vel += gravity * deltaTime;
		outLinearVelocity = body->vel;
	}
}

/// <summary>
/// ARCADE ANGULAR DAMPNING MOVMENT
/// </summary>
/// <param name="deltaTime"></param>
void Scene3p::ApplyAngularDamping(float deltaTime)
{
	const float damping = 0.95f; // 0.0 = instant stop, 1.0 = no damping
	sphereBody->angularVelocity *= damping;
}



/// -------------- DEPRECATED

// 5. REDUNDANT
Vec3 Scene3p::IfOnPlane(bool onPlane, Body* _body1, Vec3 _gravity, Vec3 _downHill, Vec3 _linearVelocity, float _angSpeed, float _speed , const float _time)
{
	if (onPlane) {
		// --- rolling on plane ---
		//Vec3 gravityNormalComp = VMath::dot(gravity, planeNormal) * planeNormal;
		//Vec3 downhill = gravity - gravityNormalComp;

		if (VMath::mag(_downHill) > VERY_SMALL) {
			Vec3 downhillDir = VMath::normalize(_downHill);
			_angSpeed = VMath::mag(_body1->angularVelocity);
			_speed = _angSpeed * _body1->radius;
			return linearVelocity = downhillDir * _speed;
		}
	}
	else {
		// --- free fall ---
		_body1->vel += _gravity * _time;
		return linearVelocity = _body1->vel;
	}

	//return linearVelocity = Vec3(0.0f, 0.0f, 0.0f);
}

// 2. REDUNDANT
Vec3 Scene3p::ComputeRollingVelocity(const Vec3& downhill)
{

	if (VMath::mag(downhill) > VERY_SMALL)
	{
		Vec3 downhillDir = VMath::normalize(downhill);
		float angSpeed = VMath::mag(sphereBody->angularVelocity);
		float speed = angSpeed * sphereBody->radius;

		return linearVelocity =  downhillDir * speed;
	}

	return linearVelocity = Vec3(0.0f, 0.0f, 0.0f);
}




