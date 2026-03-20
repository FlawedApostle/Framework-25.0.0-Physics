///									FILENAME:: (phongFrag3g / phongVert3g) working as of 2025 / 05 / 28 10:30 AM
///	Note:: the Vert and Frag shader is correct, the array, along with the formula. Do not f***** with it please !
#include <glew.h>
#include <SDL.h>
#include "Scene5g.h"			/// scene5g.h  Graphics skybox A3
#include <MMath.h>
#include "Debug.h"
#include "Texture.h"
#include "Shader.h"
#include "Mesh.h"
#include "Body.h"
#include "SkyBox.h"				/// Skybox 
#include "Camera.h"
Camera cameraRun;
/// INIT
Scene5g::Scene5g() :
	shader{ nullptr }
	, drawInWireMode{ false }
	, body_cube		{ nullptr }
	, mesh_cube		{ nullptr }	
	, CN_SkyBox		{nullptr}
	, CN_Camera		{nullptr}
	, SPEC_EXPONENT_VALUE{ NULL }
{
	Debug::Info("Created Scene5g: ", __FILE__, __LINE__);
}
/// DESTRUCTOR
Scene5g::~Scene5g() {
	Debug::Info("Deleted Scene5g: ", __FILE__, __LINE__);
}
/// BUILDING
bool Scene5g::OnCreate() {
	Debug::Info("Loading assets Scene5g: ", __FILE__, __LINE__);
	/// THERE IS A MEMORY LEAK, FIX IT !!!
	/// Camera
	// gathering the camera orientation , view , and projection.
	// This is the Camera we use for the current scene in Scene5g 
	CN_Camera = new Camera();
	CN_Camera->GetOrientation();
	CN_Camera->GetProjectionMatrix();

	/// SkyBox
	// load the SkyBox inside the scene, using the constructor
	bool status = NULL;
	 CN_SkyBox = new SkyBox
	(
		"textures/CN_Tower/cn_posx.jpg",
		"textures/CN_Tower/cn_negx.jpg",
		"textures/CN_Tower/cn_posy.jpg",
		"textures/CN_Tower/cn_negy.jpg",
		"textures/CN_Tower/cn_posz.jpg",
		"textures/CN_Tower/cn_negz.jpg"
	);

	 /// SkyBox onCreate
	 // load up the SkyBox load the mesh , load the shader
	 status = CN_SkyBox->OnCreate();
	 if (status = false)
	 {
		 delete CN_SkyBox;
		 CN_SkyBox = nullptr;
	 }

	 /// SkyBox load images function
	 // to be loaded after onCreate
	 // loads the images set by the SkyBox object which are set in the constructor
	 status = CN_SkyBox->LoadImages();
	 if (status = false)
	 {
		 delete CN_SkyBox;
		 CN_SkyBox = nullptr;
	 }

	////////////////////////////////////////////////ARRAY/////////////////////////////////////////////////////////////////////////////////////////////////////
	/// LIGHTS _ POSITIONS
	LIGHT_POSITION[0] = Vec3( -10.0f,  0.0f,  0.0f);
	LIGHT_POSITION[1] = Vec3(  10.0f,  0.0f,  0.0f);
	/// Array
/// shinyness
	SPEC_EXPONENT_VALUE[0] = 100.0f;
	SPEC_EXPONENT_VALUE[1] = 100.0f;
	 /// Color 1
	SPEC[0] = Vec4(0.0f, 1.0f, 0.0f, 0.0f);
	DIFF[0] = Vec4(0.0f, 0.5f, 0.0f, 0.0f);
	AMB[0] = 0.3f * DIFF[0];
 
	/// Color 2
	SPEC[1] = Vec4(1.0f, 0.0f, 0.0f, 0.0f);
	DIFF[1] = Vec4(0.5f, 0.0f, 0.0f, 0.0f);
	AMB[1] = 0.3f * DIFF[1];
 
	/// BODY
	body_cube = new Body();
	body_cube->OnCreate();
	// MESH NOTE:: when I use a cube , the colors are fk'd ..... is it based on the reflection indeces off the cube ? why though ? , could it depend on light position ?
	mesh_cube = new Mesh("meshes/Sphere.obj");
	mesh_cube->OnCreate();
	/// SHADER
	shader = new Shader("shaders/Scene5g/phongVert5g.glsl", "shaders/Scene5g/phongFrag5g.glsl");
	if (shader->OnCreate() == false) 
	{ std::cout << "Shader failed ... we have a problem\n"; }
	else 
	printf("shader loaded:: PhongVert3g && phongFrag3g\n");


	/// MATRIX
	// These defaults Mats are being used in update, the trackball.Quat is basing its translate, and orientation off these basic Matrices 
	projectionMatrix = MMath::perspective(45.0f, (16.0f / 9.0f), 0.5f, 100.0f);
	viewMatrix		 = MMath::lookAt(Vec3(0.0f, 0.0f, 10.0f), Vec3(0.0f, 0.0f, 0.0f), Vec3(0.0f, 1.0f, 0.0f));

	return true;
}
/// CLEAN UP !
void Scene5g::OnDestroy() {
	Debug::Info("Deleting assets Scene5g: ", __FILE__, __LINE__);
	/// SPHERE
	body_cube->OnDestroy();
	delete body_cube;
	mesh_cube->OnDestroy();
	delete mesh_cube;
	/// SHADER
	shader->OnDestroy();
	delete shader;
	/// Camera
	CN_Camera->OnDestroy();
	delete CN_Camera;
	/// Skybox
	CN_SkyBox->OnDestroy();
	delete CN_SkyBox;

}
/// JOYSTICK
void Scene5g::HandleEvents(const SDL_Event& sdlEvent) {
	/// To move the mesh
	trackball.HandleEvents(sdlEvent);
	/// using the Camera Handle events
	/// using a pointer created inside Scene5g
	/// then calling the Camera view , projection
	/// using inside the handle events to call the orientation of the camera movement (mouse movement)
	CN_Camera->HandelEvents(sdlEvent); /// camera movement 
	switch (sdlEvent.type) {
	case SDL_KEYDOWN:

		switch (sdlEvent.key.keysym.scancode) {
		case SDL_SCANCODE_W:
			drawInWireMode = !drawInWireMode;
			break;

		case SDL_SCANCODE_S:
			printf("scancode :: S\n");
			//scancode_specular_ += 1.0f,
			//k_specular0 = Vec4(scancode_specular_, scancode_specular_, scancode_specular_, 0.0);
			break;
		case SDL_SCANCODE_D:
			printf("scancode :: D\n");
			//k_diffuse0 = Vec4(scancode_diffuse_ += 1.0f, scancode_diffuse_ += 1.0f, scancode_diffuse_ += 1.0f, 0.0);
			break;
		case SDL_SCANCODE_A:
			printf("scancode :: A\n");
			break;
		case SDL_SCANCODE_KP_PLUS:
			printf("scancode :: +\n");
			break;
		case SDL_SCANCODE_KP_MINUS:
			printf("scancode :: -\n");
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
/// SHOW TIME
void Scene5g::Update(const float deltaTime)
{
	/*
	//float totalTime = 0.0f;
	//totalTime += deltaTime;
	//MMath::rotate(90, Vec3(0.0f, 1.0f, 0.0f));
	//MMath::translate(Vec3(0.0f, 1.0f, 0.0f));
	*/
	/// Getting the trackball quaternion from the current scene handle sdlevents
	/// seperating - by taking the vec3 translation of the quat fro trackball
	/// seperating - by taking the rotation matrix from the quaternion 
	/// Final view == inverse of translate * inverse of rotate 
	//		my question is why inverse ?
	cameraOrientationWorldSpace = trackball.getQuat();
	trackball_T = MMath::translate(cameraWorldSpace);
	trackball_R = MMath::toMatrix4(cameraOrientationWorldSpace);
	trackball_V = MMath::inverse(trackball_T) * MMath::inverse(trackball_R);
}
/// JAZZ HANDS
void Scene5g::Render() const {
	
	glDisable(GL_DEPTH_TEST);
	
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/// skyBox
	// Checking the skyBox pointer
	// I get the shader from skybox pointer
	// I grab the shader from the pointer and then as an exstention from that object I get the uniform from that shader
	// from that uniform I am using my Camera ppointer to call the camera projection, and view matrix. Both are created inside the Camera cpp file.
	if (CN_SkyBox == nullptr)
	{
		printf("Error CN - SkyBox Render error");
	}
	glUseProgram(CN_SkyBox->getShader().GetProgram());
	glUniformMatrix4fv(CN_SkyBox->getShader().GetUniformID("projectionMatrix"), 1, GL_FALSE, CN_Camera->GetProjectionMatrix());
	glUniformMatrix4fv(CN_SkyBox->getShader().GetUniformID("viewMatrix"), 1, GL_FALSE, MMath::toMatrix4(CN_Camera->GetOrientation()));

	CN_SkyBox->Render();		 /// load the skyBox in skyBox render

	
	/// Mesh Inside SkyBox
	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_CULL_FACE);

	glUseProgram(shader->GetProgram());

	glUniformMatrix4fv(shader->GetUniformID("projectionMatrix"), 1, GL_FALSE, projectionMatrix);
	glUniformMatrix4fv(shader->GetUniformID("viewMatrix"), 1, GL_FALSE, viewMatrix * trackball_V);
	glUniformMatrix4fv(shader->GetUniformID("modelMatrix"), 1, GL_FALSE, body_cube->getModelMatrix());
	/// UNIFORM ACCESS	
	// Arrays were a pain in the a** the uniform must match , the size of the array must match , and the index must match .....
	/// Color 1 red
	glUniform3fv(glGetUniformLocation(shader->GetProgram(), "LIGHT_POSITION[0]"), 2, LIGHT_POSITION[0]);
	glUniform4fv(glGetUniformLocation(shader->GetProgram(), "SPEC[0]"), 4, SPEC[0]);
	glUniform4fv(glGetUniformLocation(shader->GetProgram(), "DIFF[0]"), 4, DIFF[0]);
	glUniform4fv(glGetUniformLocation(shader->GetProgram(), "AMB[0]"), 4, AMB[0]);
	glUniform1f(glGetUniformLocation(shader->GetProgram(), "SPEC_EXPONENT_VALUE[0]"), SPEC_EXPONENT_VALUE[0]);

	/// Color 2 yell
	glUniform3fv(glGetUniformLocation(shader->GetProgram(), "LIGHT_POSITION[1]"), 2, LIGHT_POSITION[1]);
	glUniform4fv(glGetUniformLocation(shader->GetProgram(), "SPEC[1]"), 4, SPEC[1]);
	glUniform4fv(glGetUniformLocation(shader->GetProgram(), "DIFF[1]"), 4, DIFF[1]);
	glUniform4fv(glGetUniformLocation(shader->GetProgram(), "AMB[1]"), 4, AMB[1]);
	glUniform1f(glGetUniformLocation(shader->GetProgram(), "SPEC_EXPONENT_VALUE[1]"), SPEC_EXPONENT_VALUE[1]);

	mesh_cube->Render(GL_TRIANGLES);			/// Render MeshCube
	glDisable(GL_DEPTH_TEST);
	
	glUseProgram(0);
}




