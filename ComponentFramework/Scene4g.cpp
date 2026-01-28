#include <glew.h>
#include <iostream>
#include <SDL.h>
#include "Scene4g.h"			/// scene4g.h  Textures , Parenting
#include <MMath.h>
#include "Debug.h"
#include "Mesh.h"
#include "Shader.h"
#include "Body.h"
#include "Texture.h"			/// Textures
/// INIT
Scene4g::Scene4g() :
	shader{ nullptr }
	, drawInWireMode{ false }
	, body_Skull	{ nullptr }
	, mesh_Skull	{ nullptr }
	, texture_Skull { nullptr }
	, texture_Eye	{ nullptr }
	, body_EyeL		{ nullptr }
	, mesh_EyeL		{ nullptr }

{
	Debug::Info("Created Scene4g: ", __FILE__, __LINE__);
}
// DESTRUCTOR
Scene4g::~Scene4g() {
	Debug::Info("Deleted Scene4g: ", __FILE__, __LINE__);
}
/// BUILDING
bool Scene4g::OnCreate() {
	Debug::Info("Loading assets Scene4g: ", __FILE__, __LINE__);
	/// Trackball
	trackball = new Trackball();
	cameraPosition = Vec3(0.0f, 0.0f, 5.5f);				// pushes the cam outside the z axis 
	viewMatrix = MMath::translate(-cameraPosition);			// then updates the position to the viewmatrix , it is negative becuz were on the z axis

	/// UNIFORMS
	/// Light 1
	// add in the exponent value ... for shinyness the techie term...
	LIGHT_POSITION[0] = Vec3(10.0f, 0.0f, 0.0f);
	SPEC[0] = Vec4(1.0f,1.0f,0.0f,0.0f);
	DIFF[0] = Vec4(0.5f,0.15f,0.0f,0.0f);
	AMB[0] = 0.2 * DIFF[0];
	SPECULAR_EXPONENT[0] = 1.0f;
	/// Light 2
	LIGHT_POSITION[1] = Vec3(-10.0f, 0.0f, 0.0f);
	SPEC[1] = Vec4(0.0f, 1.0f, 1.0f, 0.0f);
	DIFF[1] = Vec4(0.0f, 0.15f, 0.5f, 0.0f);
	AMB[1] = 0.2 * DIFF[1];
	/// Body 
	// SKULL
	body_Skull = new Body();
	body_Skull->OnCreate();
	// Eye Right
	body_EyeR = new Body();
	body_EyeR->OnCreate();
	// Eye Left
	body_EyeL = new Body();
	body_EyeL->OnCreate();
	/// MESH 
	// SKULL
	mesh_Skull = new Mesh("meshes/Skull.obj");
	mesh_Skull->OnCreate();
	// EYE RIGHT
	mesh_EyeR = new Mesh("meshes/Sphere.obj");
	mesh_EyeR->OnCreate();
	// EYE LEFT
	mesh_EyeL = new Mesh("meshes/Sphere.obj");
	mesh_EyeL->OnCreate();
	/// TEXTURE
	// SKULL
	texture_Skull = new Texture;
	texture_Skull->LoadImage("textures/skull_texture.jpg");
	if (!texture_Skull) { printf("LoadImage error ! texture_skull\n"); }			// debug
	else { printf("Shader Loaded:: texture_skull\n"); }
	// EYE  .... Using the same tex for both spheres !
	texture_Eye = new Texture;
	texture_Eye->LoadImage("textures/evilEye.jpg");
	if (!texture_Eye) { printf("LoadImage error ! texture_Eye\n"); }				// debug
	else { printf("Shader Loaded:: texture_Eye\n"); }			
	
	/// SHADER
	shader = new Shader("shaders/tex/texturePhongVert.glsl", "shaders/tex/texturePhongFrag.glsl");
	if (shader->OnCreate() == false) {
		std::cout << "Shader failed ... we have a problem\n";
	}
	else printf("shader loaded:: PhongVert4g && phongFrag4g\n");
	
	/// MATRIX
	projectionMatrix = MMath::perspective(45.0f, (16.0f / 9.0f), 0.5f, 100.0f);
	viewMatrix = MMath::lookAt(Vec3(0.0f, 0.0f, 5.5f), Vec3(0.0f, 0.0f, 0.0f), Vec3(0.0f, 1.0f, 0.0f));
	
	/// MATRIX EYE GLOBAL VARIABLES 
	eye_modelRotate = MMath::rotate(-90.0f, Vec3(0.0f, 1.0f, 0.0f));
	eye_modelScale = MMath::scale(Vec3(0.32f, 0.32f, 0.32f));
	/// MATRIX EYE RIGHT
	eyeRight_modelTranslate = MMath::translate(Vec3(0.56f, 0.25f, 0.8f));
	eyeRight_modelMat = eyeRight_modelTranslate * eye_modelRotate * eye_modelScale;
	/// MATRIX EYE LEFT
	eyeLeft_modelTranslate = MMath::translate(Vec3(-0.56f, 0.25f, 0.8f));
	eyeLeft_modelMat = eyeLeft_modelTranslate * eye_modelRotate * eye_modelScale;

	return true;
}
/// CLEAN UP !
void Scene4g::OnDestroy() {
	Debug::Info("Deleting assets Scene4g: ", __FILE__, __LINE__);
	/// SKULL
	body_Skull->OnDestroy();
	delete body_Skull;
	mesh_Skull->OnDestroy();
	delete mesh_Skull;
	/// EYE RIGHT
	body_EyeR->OnDestroy();
	delete body_EyeR;
	mesh_EyeR->OnDestroy();
	delete mesh_EyeR;
	/// EYE LEFT
	body_EyeL->OnDestroy();
	delete body_EyeL;
	mesh_EyeL->OnDestroy();
	delete mesh_EyeL;
	/// SHADER
	shader->OnDestroy();
	delete shader;
	/// TEXTURE
	//texture_Skull->~Texture();
	//texture_Eye->~Texture();
	delete texture_Skull;
	delete texture_Eye;
	/// Trackball
	delete trackball;
}
/// JOYSTICK
void Scene4g::HandleEvents(const SDL_Event& sdlEvent) {
	/// Trackball
	trackball->HandleEvents(sdlEvent);

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
		printf("press down\n");
		break;

	case SDL_MOUSEBUTTONUP:
		printf("press up\n");
		break;

	default:
		break;
	}
}
/// SHOW TIME
void Scene4g::Update(const float deltaTime)
{
	/*
	float totalTime = 0.0f;
	totalTime += deltaTime;
	MMath::rotate(90, Vec3(0.0f, 1.0f, 0.0f));
	MMath::translate(Vec3(0.0f, 1.0f, 0.0f));
	*/

	// pushes the cam outside the z axis 
	// then updates the position to the viewmatrix , it is negative becuz were on the z axis
	//cameraPosition = Vec3(0.0f, 0.0f, 10.0f);
	//viewMatrix = MMath::translate(-cameraPosition);

	/// Trackball
	// the view is set in OnCreate() z axis = 5.5f
	cameraPositionOrientation = trackball->getQuat();
	trackball_translate = MMath::translate(cameraPosition);
	trackball_rotate = MMath::toMatrix4(cameraPositionOrientation);
	trackball_view = MMath::inverse(trackball_translate) * MMath::inverse(trackball_rotate);

}
/// JAZZ HANDS
void Scene4g::Render() const {
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	//glCullFace(GL_BACK);			/// WHERE LOOKING AT THE BACK OF THE SPEHERE...
	
	/// Set the background color then clear the screen
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// wiremesh
	if (drawInWireMode) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	else {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
	

	glUseProgram(shader->GetProgram());

	/// trackball_view used as viewMatrix ... 
	// NOTE :: make a cpy file play around with this later

	glUniformMatrix4fv(shader->GetUniformID("projectionMatrix"), 1, GL_FALSE, projectionMatrix);
	glUniformMatrix4fv(shader->GetUniformID("viewMatrix"), 1, GL_FALSE, trackball_view);

	// btw i got this answer of how to access, and use the array from stackoverflow.... yeah thats right....
	glUniform3fv(glGetUniformLocation(shader->GetProgram(), "LIGHT_POSITION[0]"), 2 , LIGHT_POSITION[0]);			// light 1
	glUniform3fv(glGetUniformLocation(shader->GetProgram(), "LIGHT_POSITION[1]"), 2 , LIGHT_POSITION[1]);			// light 2

	// LIGHT 1
	glUniform4fv(glGetUniformLocation(shader->GetProgram(), "SPEC[0]"), 4, SPEC[0]);
	glUniform4fv(glGetUniformLocation(shader->GetProgram(), "DIFF[0]"), 4, DIFF[0]);
	glUniform4fv(glGetUniformLocation(shader->GetProgram(), "AMB[0]"), 4, AMB[0]);
	glUniform1i(glGetUniformLocation(shader->GetProgram(), "SPECULAR_EXPONENT[0]"), SPECULAR_EXPONENT[0]);
	// LIGHT 2
	glUniform4fv(glGetUniformLocation(shader->GetProgram(), "SPEC[1]"), 4, SPEC[1]);
	glUniform4fv(glGetUniformLocation(shader->GetProgram(), "DIFF[1]"), 4, DIFF[1]);
	glUniform4fv(glGetUniformLocation(shader->GetProgram(), "AMB[1]"), 4, AMB[1]);
	glUniform1i(glGetUniformLocation(shader->GetProgram(), "SPECULAR_EXPONENT[0]"), SPECULAR_EXPONENT[0]);

	/// TEXTURE - SKULL - bind
	glBindTexture(GL_TEXTURE_2D, texture_Skull->getTextureID());
	glUniformMatrix4fv(shader->GetUniformID("modelMatrix"), 1, GL_FALSE, skull_modelMat);					/// Model with T * R * S		// shinyness
	mesh_Skull->Render(GL_TRIANGLES);
	/// TEXTURE - SKULL - unbind
	glBindTexture(GL_TEXTURE_2D, 0);


	/// TEXTURE - EYE RIGHT	- bind
	glBindTexture(GL_TEXTURE_2D, texture_Eye->getTextureID());
	
	glUniformMatrix4fv(shader->GetUniformID("modelMatrix"), 1, GL_FALSE, eyeRight_modelMat * skull_modelMat);				/// Model with T * R * S
	mesh_EyeR->Render(GL_TRIANGLES);

	glUniformMatrix4fv(shader->GetUniformID("modelMatrix"), 1, GL_FALSE, eyeLeft_modelMat * skull_modelMat);				/// works if a i concatonate , works if I dont ... why !?
	mesh_EyeL->Render(GL_TRIANGLES);
	/// TEXTURE - EYE RIGHT - unbind
	glBindTexture(GL_TEXTURE_2D, 0);


	glUseProgram(0);

	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
}




