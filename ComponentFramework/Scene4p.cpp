#include <glew.h>
#include <iostream>
#include <SDL.h>
#include "Scene4p.h"			/// scene3p.h
#include <MMath.h>
#include "Debug.h"
#include "Mesh.h"
#include "Shader.h"
#include "Body.h"
#include <QMath.h>
#include "Quaternion.h"

#include "Collision.h"
#include "Plane.h"


/// Notes are Written in Repo Jargon - check NOTES_PHYSICS FOR FORMULA BREAKDOWNS
//Local X is still Left / Right.
//Local Z is now pointing straight UP(Height).
//Local Y is now pointing FORWARD(Depth).

Scene4p::Scene4p() :

	drawInWireMode					{ false }
	, drawInNormalsFace				{ false }
	, drawInNormalsLine				{ false }
	, bWantsToJump					{ NULL }
	, shader						{ nullptr }
	, shader_normals_face			{ nullptr }
	, shader_normals_line			{ nullptr }
	, sphereMesh					{ nullptr }
	, sphereBody					{ nullptr }
	, planeBody						{ nullptr }
	, planeMesh						{ nullptr }
	, sphereCollision0_Body			{ nullptr }
	, sphereCollision0_Mesh			{ nullptr }
	, sphereCollision0_Normal		{ 0,0,0 }
	, trackball						{ nullptr }
	, distancetoPivot				{ NULL }
	, torqueMagnitude				{ NULL }
	, angle							{ NULL }
	, velocityMagnitutde			{ NULL }
	, normalScale					{ NULL }
	, lightPosLoc					{ NULL }
	, baseHalfSize					{ NULL }
	, color_ambient_exponent		{ 0.1f }
	, torqueDirection				{ 0,0,0 }
	, torque						{ 0,0,0 }
	, velocityDirection				{ 0,0,0 }
	, linearVelocity				{ 0,0,0 }
	, planeNormal					{ 0,1,0 }
	, upVector						{ 0.0f,1.0f,0.0f }
	, plane_plane1					{nullptr}

{
	Debug::Info("Created Scene4p: ", __FILE__, __LINE__);
}
Scene4p::~Scene4p()
{
	Debug::Info("Deleted Scene4p: ", __FILE__, __LINE__);
}

// DESTROY
void Scene4p::OnDestroy() {
	Debug::Info("Deleting assets Scene4p: ", __FILE__, __LINE__);
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

	/// NEW PLANE CLASS 
	// - Scene OWNS plane_plane1 pointer 
	// - Plane.h OWNS plane_plane1 Body*
	// Once object is deleted - the destructor in Plane.h [ ~Plane() ] will handle the Body deletion
	if(plane_plane1)
	delete plane_plane1;


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
void Scene4p::HandleEvents(const SDL_Event& sdlEvent) {
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
			break;
		case SDL_SCANCODE_A:
			printf("LEFT\n");
			rotation = QMath::angleAxisRotation(2.0f, axis_Left);
			planeBody->orientation = rotation * planeBody->orientation;
			break;
		case SDL_SCANCODE_D:
			printf("RIGHT\n");
			rotation = QMath::angleAxisRotation(2.0f, axis_Right);
			planeBody->orientation = rotation * planeBody->orientation;
			break;
		case SDL_SCANCODE_S:
			printf("DOWN\n"); planeNormal.print();
			rotation = QMath::angleAxisRotation(2.0f, axis_PitchDown);
			planeBody->orientation = rotation * planeBody->orientation;
			break;
		case SDL_SCANCODE_SPACE:
			printf("SPACE\n"); 
			bWantsToJump = true;

			printf("upVector = %f,%f,%f\n" , sphereBody->upVector.x, sphereBody->upVector.y, sphereBody->upVector.z);
			printf("Plane Body orientation = %f,%f,%f\n" , planeBody->orientation.ijk.x, planeBody->orientation.ijk.y, planeBody->orientation.ijk.z);
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

bool Scene4p::OnCreate() {
	Debug::Info("Loading assets Scene4p: ", __FILE__, __LINE__);
	//lightPosLoc = glGetUniformLocation(shader->GetProgram(), "lightPos");						// Cache the uniform location for the light position in the shader
	//upVector = { 0.0f,1.0f,0.0f };															/// generate the upVector - Currently in Update

	// MESH SCALE , MESH NORMAL
	baseHalfSize = 1.0f;	// Define the Boundary old# = [ 5 ]
	upVector = { 0.0f,1.0f,0.0f };

	/// Trackball
	trackball = new Trackball();

	/// UNIFORMS --- phongVert , phongFrag [shaders/defaultPhong/phongVert.glsl", "shaders/defaultPhong/phongFrag.glsl] 
	lightPos = Vec3(0.0f, 10.0f, 0.0f);															// lightpos shader PhoneVert
	color_specular = Vec4(1.0, 0.0, 0.0, 0.0);
	color_diffuse = Vec4(0.5, 0.0, 0.0, 0.0);
	color_ambient_exponent = 0.5f;

	/// NEW :: Plane.h CLASS
	//plane_plane1 = new Plane();	
	//plane_plane1->SetPlane_Body(new Body); 
	//if (plane_plane1)
	//	printf("Success :: Plane_Mesh\n");
	//plane_plane1->GetPlane_Body()->OnCreate();
	//plane_plane1->GetPlane_Body()->scale = Vec3(10.0f, 10.0f, 10.0f);

	

	planeBody = new Body();
	planeBody->OnCreate();
	planeNormal = Vec3(0.0f, 1.0f, 0.0f);															// Plane_Blender , changed normal from z = 1.0f TO y = 1.0f
	planeNormal.print("planeNormal");
	planeBody->scale = Vec3(10.0f, 10.0f, 10.0f);														// CHECK:: baseHalfScale defines the true space - leave at 1.0f - this scale is to defin the plane size
	/*
	//planeBody->orientation = QMath::angleAxisRotation(90, Vec3(-1, 0, 0));						// Plane_Blender --- new rotated planed that requires no rotation - fks up the gravity tho
	//planeNormal = QMath::rotate(Vec3(0.0f, 0.0f, 1.0f), planeBody->orientation);					// No Drift - Fixed Base to start off
	*/

	/// Sphere 1.
	// V = W X N (velocity = angular velocity cross normal -> (assume each letter is a vector)
	sphereBody = new Body();
	sphereBody->OnCreate();
	sphereBody->pos = Vec3(0.0f, 0.0, 0.0f);
	sphereBody->angularVelocity = Vec3(0.0f, 0.0f, 0.0f);										// starts at 0 for rest
	sphereBody->scale = Vec3(1.0f, 1.0f, 1.0f);
	sphereBody->angularAcceleration = Vec3(0.0f, 0.0f, 0.0f);									// SPEED  - starts at 0 for rest
	sphereBody->radius = 1.0f;

	/// sphere .2 - COLLISIONS
	sphereCollision0_Body = new Body();
	sphereCollision0_Body->OnCreate();
	sphereCollision0_Body->pos = Vec3(0.0f, 1.0, 0.0f);
	sphereCollision0_Body->radius = 1.0f;


	// ----- 3D
	// NEW PLANE
	//plane_plane1->SetPlane_Mesh(new Mesh("meshes/Plane_3.obj"));
	//if (plane_plane1)
	//	printf("Success :: Plane_Mesh\n");
	//plane_plane1->GetPlane_Mesh()->OnCreate();

	// PLANE
	planeMesh = new Mesh("meshes/Plane_3.obj");			// Plane_Blender , new rotated planed that requires no rotation - fks up the gravity tho
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
	cameraPosition = sphereBody->pos + Vec3(0.0f, 0.0f, 35.0f);
	//cameraPosition = planeBody->pos + Vec3(0.0f, 0.0f, 10.0f); 

	// ----- CAMERA
	projectionMatrix = MMath::perspective(45.0f, (16.0f / 9.0f), 0.5f, 100.0f);
	//viewMatrix = MMath::lookAt(Vec3(0.0f, 0.0f, 20.0f), Vec3(0.0f, 0.0f, 0.0f), Vec3(0.0f, 1.0f, 0.0f));
	cameraOrientation = QMath::angleAxisRotation(0, Vec3(1.0f, 0.0f, 0.0f));
	Matrix4 T = MMath::translate(cameraPosition);
	Matrix4 R = MMath::toMatrix4(cameraOrientation);
	viewMatrix = MMath::inverse(R) * MMath::inverse(T);


	return true;
}


void Scene4p::Update(const float deltaTime)
{
	// ----- FIND POINT BETWEEN TWO POSITIONS
	//std::cout << "test Body* " << Collision::SphereSphereCollisionDetected(sphereBody, sphereCollision0_Body) << "\n";


	//										--------- GLOBALs ----- NORMAL ( PLANE ) & LOCAL POSITION
	planeNormal = QMath::rotate(Vec3(0.0f, 1.0f, 0.0f), planeBody->orientation);								//  Plane_Blender , changed normal from z = 1.0f TO y = 1.0f
	planeNormal = VMath::normalize(planeNormal);

	// LOCAL POSITION
	Vec3 localPos = sphereBody->pos - planeBody->pos;					
	localPos = QMath::inverse(planeBody->orientation) * localPos;

	///										--------- GLOBALs ----- GRAVITY & BOUNDARIES
	/*
	CONTACT_EPS is to generate a value that is ever-so-slightly above the plane
	values are not absolute so the sphere might have a subtle float value where the plane is absolute

	“How big is the plane in mesh space?”
	or 1.0f depending on Plane.obj
	WORLD_HALF_SIZE = BASE_HALF_ZIE * SCALE (MESH BODY)

	signed distance
	the position between the sphere and the plane then finding the dot product using the planes normal
	height is okay if the distance is equal to the radius of the sphere
	*/
	Vec3 gravity(0.0f, -9.8f, 0.0f);
	Vec3 gravityNormalComp = VMath::dot(gravity, planeNormal) * planeNormal;									// Remove component along plane normal → get tangent component
	Vec3 downhill = gravity - gravityNormalComp;
	float angSpeed = VMath::mag(sphereBody->angularVelocity);
	float speed = angSpeed * sphereBody->radius;
	float planeDist = VMath::dot(sphereBody->pos - planeBody->pos, planeNormal);
	bool heightOK = planeDist <= sphereBody->radius + CONTACT_EPS;

	/// 3.A									---------  TORQUE PT I ( ROTATION , BEGIN ROTATION )
	// NOTES
	/*  UpVector is set in INIT - it is a const , should it be ?
	 we know the perpendicular distance between pivot and force
	 ----- ANGULAR ACCELERATION
	 ------ FIND the angle between PLANE normal and UPVECTOR to distinguish angular acceleration
	 Set the upVector
	 Get the cos(angle) - planeNormal DOT up = cos(angle)
	 Get perpendicular distance between pivot & force
	 force = torqueMagnitude
	 pivot is the upVector || is it the planeNormal


	 The force is the weight of the sphere
	 The distance to the pivot relies on the angle
	 between the weight and the normal
	 Remove component along plane normal → get tangent component
	 Project gravity onto plane - remove perpendicular compeoent to leave parallel direction
	

	//float cosTheta = VMath::dot(planeNormal, upVector);
	////cosTheta = MMath::clamp(cosTheta, -1.0f, 1.0f);
	//float theta = acos(cosTheta);
	//float distanceToPivot = sphereBody->radius * sin(theta);


	// BLOCK BELOW - REMOVES TORQUE IF STATMENT BELOW
	torque = VMath::cross(upVector, planeNormal);				// torqueDirection
	if (torqueDirectionMag > VERY_SMALL) {
		Vec3 torqueDir = VMath::normalize(torqueDirection);
		torqueMagnitude = sphereBody->mass * 9.8f * distanceToPivot;
		Vec3 torqueFinal = torqueDir * torqueMagnitude;

		// CALLING NEW PHYSICS FUNCTION
		sphereBody->ApplyTourque(torqueFinal);
	}
	*/

	//	NEW PHYSICS FUNCTION [ TORQUE - DISTANCE TO PIVOT :: DOT ]
	float distanceToPivot = sphereBody->radius * physics.Angle_DistanceToPivot(planeNormal, upVector);
	//	NEW PHYSICS FUNCTION [ TORQUE DIRECTION :: CROSS]
	torqueDirection = physics.Torque_Direction(upVector, planeNormal);
	//	NEW PHYSICS FUNCTION [ TORQUE ]
	torque = physics.Torque(torqueDirection, sphereBody->mass, distanceToPivot);
	sphereBody->ApplyTourque(torque);
	

	/// UPDATE									--------- TORQUE PT II ( UPDATE ANGULAR MOTION )
	sphereBody->UpdateAngularVelocity(deltaTime);
	sphereBody->UpdateOrientation(deltaTime); // Change the orientation using quaternion.


	// 4.A										--------- BOUNDRIES GO PT I [ Boundary test (local space) ]
	/*
 											Local X is still Left/Right.
											Local Y is now pointing straight UP (Height).
											Local Z is now pointing FORWARD (Depth).
	 localPos yields a vector distance
	 invers-ing localPos rotates the vector onto the planes LOCAL axis
	 - Where is the sphere relative to the plane, expressed in the plane’s coordinate system -

	 converts PLANE scale to WORLD SPACE
	 - Rather than doing world space to mesh space we just translate the mesh to world -
	 physics needs state-based transitions rather than frame by frame stateless transitions

	//  ----- DEPRECATED ----- OPTION A - ( MESH SPACE )

	Divide localPos by planeBody->scale.
	Use baseHalfSize directly (no scale).
	Divide sphere radius by scale too. 
	
	convert localPos WORLD SPACE to MESH SPACE - Apply inverse scale - 
	MESHRADIUS = WORLDRADIUS / SCALE

	world - scaled space → original mesh space (un-scaled)
	Matching the boundary scale with the plane scale
	*/
	
	//  ----- DEPRECATED ----- OPTION A - ( MESH SPACE ) [leave for now]
	/*
	localPos.x /= planeBody->scale.x;
	localPos.y /= planeBody->scale.y;
	localPos.z /= planeBody->scale.z;

	bool insideBounds =
		fabs(localPos.x) <= (baseHalfSize - sphereBody->radius / planeBody->scale.x) &&
		fabs(localPos.z) <= (baseHalfSize - sphereBody->radius / planeBody->scale.z);
	*/
	
	// OPTION B ( WORLD SPACE ) - Scale to world Space
	/*
	Keep localPos rotated into plane axes, but do not divide by scale.
	Multiply baseHalfSize by plane scale for halfX/halfZ.
	Use sphere radius as-is in world units.
	*/
	//Vec3 planeHalfExtents = Vec3(10.0f, 0.0f, 10.0f);		// test 
	float halfX = baseHalfSize * planeBody->scale.x;		// float halfX = baseHalfSize;
	float halfZ = baseHalfSize * planeBody->scale.z;
	
	bool insideBounds =
	fabs(localPos.x) <= (halfX - sphereBody->radius) &&
	fabs(localPos.z) <= (halfZ - sphereBody->radius);

	//Vec3 rel = sphereBody->pos - planeBody->pos;

	//bool insideBounds =
	//	fabs(rel.x) <= halfX &&
	//	fabs(rel.z) <= halfZ;


	//std::cout << "BOUNDARIES CHECK :: " << "\n";
	//printf("localPos: %f %f %f\n", localPos.x, localPos.y, localPos.z);
	//printf("halfX: %f halfZ: %f\n", halfX, halfZ);
	//printf("localPos: %f %f\n", localPos.x, localPos.z);
	
		
	/// ------------------------------------- DEBUG
	/*
		printf("localPos FIXED: (%f, %f)\n", localPos.x, localPos.z);
		printf("plane pos: (%f,%f,%f)\n", planeBody->pos.x, planeBody->pos.y, planeBody->pos.z);
		printf("sphere pos: (%f,%f,%f)\n", sphereBody->pos.x, sphereBody->pos.y, sphereBody->pos.z);
		printf("localPos: (%f, %f) | bounds: (%f, %f)\n",
		localPos.x, localPos.z, halfX, halfZ);
		printf("halfX: %f halfZ: %f\n", halfX, halfZ);
		//------------------------------------------------------------------------------------------------
	*/

	// 4.B										--------- BOUNDRIES PT III [ (Boundary on - PLANE - LOCAL space) ]
	//bool onPlane = heightOK && insideBounds;	// OLD
		// NEW 
	bool touchingPlane = heightOK;
	bool withinBounds = insideBounds;
	bool onPlane = touchingPlane && withinBounds;
	bool offEdge = touchingPlane && !withinBounds;
		// OLD
	static bool wasOnPlane = true;
	bool justLeftPlane = wasOnPlane && !onPlane;
	wasOnPlane = onPlane;

	static bool grounded = true;
	
	// ---- DEPRECATED [not sure to remove just tyet]
	/*
	const float contactVelocityThreshold = 0.5f;
	float vNormal = VMath::dot(sphereBody->vel, planeNormal);
	bool nearPlane = planeDist <= sphereBody->radius + CONTACT_EPS;
	bool stableContact = nearPlane && (vNormal <= contactVelocityThreshold);
	if (!grounded && onPlane && stableContact) grounded = true;


	if (justLeftPlane) {
		linearVelocity = ComputeRollingVelocity_Cross(planeNormal);
		sphereBody->vel = linearVelocity;
	}
	wasOnPlane = onPlane;
	*/

	
	/// 4.C										--------- BOUNDARIES & TORQUE CONSTRAINT PT IV ( KEEP SPHERE RESTING ON PLANE )  logic needs cleaning
	// --- UPDATED GROUNDED LOGIC ---
	if (!grounded && onPlane)
	{
		// We were in the air, but now we are touching the plane and within bounds. We landed!
		printf("We were in the air, but now we are touching the plane and within bounds grounded\n");
		grounded = true;
	}
	else if (grounded && !withinBounds)
	{
		// We are on the ground but rolled off the edge
		printf(" We are on the ground but rolled off the edge !grounded\n");
		grounded = false;
	}

	// FIX ! if in the air cannot attempt to log another jump. rather jump is null untill touching ground
	// FIX ! ball will stay above plane with physics applied - jumping resets signed distance making the ball fall back onto the plane
	/// 4.D
	if (grounded && onPlane)	//onPlane
	{
		linearVelocity = Body::ComputeRollingVelocity_Cross(planeNormal, sphereBody);
	}
	else
	{
		/*
		if (justLeftPlane)
		{
			// Convert angular motion → linear velocity ONCE at detachment
			linearVelocity = Body::ComputeRollingVelocity_Cross(planeNormal, sphereBody);
			sphereBody->vel = linearVelocity;
		}
		*/
		
		// Apply gravity normally
		sphereBody->vel += gravity * deltaTime;
		linearVelocity = sphereBody->vel;
	}

	float jumpSpeed = 10.0f; // My chosen impulse scalar
	// --- NEW: JUMP IMPULSE LOGIC ---
	if (bWantsToJump && grounded) {
		// Add instantaneous upward velocity along the plane's normal
		//2linearVelocity += planeNormal * jumpSpeed;
		sphereBody->vel += planeNormal * jumpSpeed; // immediate change
		linearVelocity = sphereBody->vel;
		bWantsToJump = false;
		grounded = false;
	}




	/// ----------------------------------------	ASSIGN LINEAR VELOCITY AND INTEGRATE POSITION
	sphereBody->vel = linearVelocity;
	sphereBody->UpdatePos(deltaTime);



	/// 4.E ---------------------------------------- BOUNDARIES ( CONTACT CONSTRAINT - PLANE )[ KEEP SPHERE RESTING ON PLANE ]
	
	if (onPlane)
	{
		// Recompute planeDist after movement to keep it precise
		float planeDistAfter = VMath::dot(sphereBody->pos - planeBody->pos, planeNormal);

		if (planeDistAfter < sphereBody->radius)
		{
			float penetration = sphereBody->radius - planeDistAfter;
			sphereBody->pos += planeNormal * penetration;
		}

		/// JUMPING - CHANGED > to <
		float vdot = VMath::dot(sphereBody->vel, planeNormal);
		if (vdot < 0.0f) {
			sphereBody->vel -= planeNormal * vdot;
		}
	}





	// ------ TRACKBALL ------ SYNTHETIC CAMERA - [Starting camera position]
	//cameraPosition = cameraPosition - sphereBody->pos;
	cameraPosition = cameraPosition - planeBody->pos;

	/* 
	WHY INVERSE !...... looking down the neg z axis !
	- initial is getQuat() in handle events, gets the inital position of the orientation of the quat
	- trackball.HandleEvents(sdlEvent) is sandwiched in between to gather controler input - final is is getQuat() is the orientation after movement 

	cam orientation will equal the finalOrientaion *= inverseOrientaion(initial) -
	then correct the rotate of the cam position in relation to the change in trackball orientaion 
	*/
	Quaternion changeInTrackballOrientation = finalTrackballOrientation * QMath::inverse(initialTrackballOrientation);
	cameraOrientation *= changeInTrackballOrientation;
	cameraPosition = QMath::rotate(cameraPosition, changeInTrackballOrientation);

	/// ----- MATRIX
	Matrix4 T = MMath::translate(cameraPosition);
	Matrix4 R = MMath::toMatrix4(cameraOrientation);
	viewMatrix = MMath::inverse(R) * MMath::inverse(T);


}

void Scene4p::Render() const {
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	// -- FACE WINDING
	glFrontFace(GL_CCW);
	 //glFrontFace(GL_CW);

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
	/// ---- FIX THIS 
	if (drawInNormalsFace) {
		glUseProgram(shader_normals_face->GetProgram());
		// 2. Send matrices to Face Shader
		glUniformMatrix4fv(shader_normals_face->GetUniformID("projectionMatrix"), 1, GL_FALSE, projectionMatrix);
		glUniformMatrix4fv(shader_normals_face->GetUniformID("viewMatrix"), 1, GL_FALSE, viewMatrix);
		// 3. Render Plane with Face Shader
		glUniformMatrix4fv(shader_normals_face->GetUniformID("modelMatrix"), 1, GL_FALSE, planeBody->getModelMatrix());
		
		planeMesh->Render(GL_TRIANGLES); 
		
		sphereMesh->Render(GL_TRIANGLES);

	}
	/// ---- FIX THIS
	else
	{
		// Default Shader
		glUseProgram(shader->GetProgram());
		glUniform3fv(glGetUniformLocation(shader->GetProgram(), "lightPos"), 1, &lightPos.x);
		glUniform4fv(glGetUniformLocation(shader->GetProgram(), "color_specular"), 1, color_specular);
		glUniform4fv(glGetUniformLocation(shader->GetProgram(), "color_diffuse"), 1, color_diffuse);
		glUniform1f(glGetUniformLocation(shader->GetProgram(), "color_ambient_exponent"),color_ambient_exponent);

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
		//sphereMesh->Render(GL_TRIANGLES);
	}

	glUseProgram(0);
}






/// ----- Linear Velocity

// 1.
Vec3 Scene4p::ComputeRollingVelocity_Cross(const Vec3& planeNormal)
{
	// Contact vector (center → contact point)
	// THIS ASSUMES THAT THE POINT IS DIRECTLY ALONG THE NORMAL - THIS IS OK ONLY FOR SPHERES...
	Vec3 r = planeNormal * sphereBody->radius;

	// True rolling velocity
	Vec3 v = VMath::cross(sphereBody->angularVelocity, r);

	return v;
}

// 2.
Vec3 Scene4p::ComputeFreeFallVelocity(Body* body, const Vec3& gravity, float dt)
{
	body->vel += gravity * dt;
	return body->vel;
}


/// ARCADE ANGULAR DAMPNING MOVMENT
void Scene4p::ApplyAngularDamping(float deltaTime)
{
	const float damping = 0.95f; // 0.0 = instant stop, 1.0 = no damping
	sphereBody->angularVelocity *= damping;
}



/// -------------- DEPRECATED ( MUTEABLE(S) )


// ----- DEPRECATED -----
Vec3 Scene4p::IfOnPlane(bool onPlane, Body* _body1, Vec3 _gravity, Vec3 _downHill, Vec3 _linearVelocity, float _angSpeed, float _speed, const float _time)
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

// ----- DEPRECATED -----
Vec3 Scene4p::ComputeRollingVelocity(const Vec3& downhill)
{

	if (VMath::mag(downhill) > VERY_SMALL)
	{
		Vec3 downhillDir = VMath::normalize(downhill);
		float angSpeed = VMath::mag(sphereBody->angularVelocity);
		float speed = angSpeed * sphereBody->radius;

		return linearVelocity = downhillDir * speed;
	}

	return linearVelocity = Vec3(0.0f, 0.0f, 0.0f);
}




