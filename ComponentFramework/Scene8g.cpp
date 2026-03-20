#include <glew.h>
#include <iostream>
#include <SDL.h>
#include "Scene8g.h"			/// scene1g.h
#include <MMath.h>
#include "Debug.h"
#include "Mesh.h"
#include "Shader.h"
#include "Body.h"
#include "SDL_events.h"			// mouse scroll 



Scene8g::Scene8g() :
	
	 plane_Body				{ nullptr } 
	,tess_shader			{ nullptr } 
	,plane_Mesh				{ nullptr }
	,drawInWireMode			{ false }
	,heightMapText			{ nullptr }
	,normal_Map				{ nullptr }
	,terrainDiffuse_map		{ nullptr }
	,tesslevelControl		{ NULL }
	,trackballMouse			{ NULL }

{
	Debug::Info("Created Scene8: ", __FILE__, __LINE__);
}

Scene8g::~Scene8g() {
	Debug::Info("Deleted Scene8: ", __FILE__, __LINE__);
}

bool Scene8g::OnCreate() {
	Debug::Info("Loading assets Scene8: ", __FILE__, __LINE__);
 
	/// Query the max amount of Patches on GPU
	GLint maxPatchVertices;
	glGetIntegerv(GL_MAX_PATCH_VERTICES, &maxPatchVertices);
	printf("MAX PATCHES %i\n", maxPatchVertices);

	/// Trackball - Camera
	trackballMouse = new Trackball();
	cameraPosition = Vec3(0.0f, 0.0f, 15.0f);				// pushes the cam outside the z axis // read in the camera !
	viewMatrix = MMath::translate(-cameraPosition);			// then updates the position to the viewmatrix , it is negative becuz were on the z axis

	/// Uniform(s)
	// Map Textures	
	tesslevelControl = 25.0f;		/// tesselation level  - max amount
	// Lights
	lightposition = Vec3(0.0f, 0.0f, -10.0f);
	
	/// Plane Body , Mesh
	plane_Body = new Body();
	plane_Body->OnCreate();
	plane_Body->orientation = plane_Body->orientation *= QMath::angleAxisRotation(55.0f, Vec3(-1.0f, 0.0f, 0.0f));

	// Mesh - Plane
	plane_Mesh = new Mesh("meshes/Plane.obj");
	plane_Mesh->OnCreate();

	/// Textures(s)
	// Map - Height
	heightMapText = new Texture;
	heightMapText->LoadImage("textures/terrainHeight.png");
	if (heightMapText == NULL) { printf("height Map error\n"); }			// debug
	// Map - Normal
	normal_Map = new Texture;
	normal_Map->LoadImage("textures/terrainNormal.png");
	if (normal_Map == NULL) { printf("normal Map error\n"); }				// debug
	// Map - Diffuse
	terrainDiffuse_map = new Texture;
	terrainDiffuse_map->LoadImage("textures/terrainDiffuse.png");
	if (terrainDiffuse_map == NULL) { printf("diffuse Map error\n"); }		// debug
	
	/// Shader(s)
	// Vert , Control , Eval , Frag /// add the files into the function
	tess_shader = new Shader
	(
		"shaders/tess/tessVert.glsl",
		"shaders/tess/tessFrag.glsl",
		"shaders/tess/tessControl.glsl",
		"shaders/tess/tessEval.glsl"
	);
	if (tess_shader->OnCreate() == false) { std::cout << "Shader failed ... we have a problem\n"; }		// debug
	
	/// Matrix - Default
	projectionMatrix = MMath::perspective(45.0f, (16.0f / 9.0f), 0.5f, 100.0f);
	//viewMatrix = MMath::lookAt(Vec3(0.0f, 0.0f, 50.0f), Vec3(0.0f, 0.0f, 0.0f), Vec3(0.0f, 1.0f, 0.0f));
	
	return true;
}

/// Clean up
void Scene8g::OnDestroy() {
	Debug::Info("Deleting assets Scene8: ", __FILE__, __LINE__);
	// tracball 
	delete trackballMouse; 
	// plane
	plane_Body->OnDestroy();
	delete plane_Body;
	plane_Mesh->OnDestroy();
	delete plane_Mesh;
	// shader(s)
	tess_shader->OnDestroy();
	delete tess_shader;
	// textures(s)
	delete heightMapText;
	delete terrainDiffuse_map;
	delete normal_Map;
}
/// SDL_MouseScroll add !
void Scene8g::HandleEvents(const SDL_Event& sdlEvent) {
	trackballMouse->HandleEvents(sdlEvent);		/// trackball
	float zoomSpeed = 0.2f;						/// mouse scroll
	switch (sdlEvent.type) {
	case SDL_KEYDOWN:
		switch (sdlEvent.key.keysym.scancode) {
		case SDL_SCANCODE_W:
			drawInWireMode = !drawInWireMode;
			break;
		case SDL_SCANCODE_T:
			printf("tesselation\n");
			break;
		}
		break;
	case SDL_MOUSEMOTION:
		break;
	case SDL_MOUSEBUTTONDOWN:
		break;
	case SDL_MOUSEBUTTONUP:
		break;
	case SDL_MOUSEWHEEL:
		cameraPosition.z -= sdlEvent.wheel.preciseY * zoomSpeed;
		printf("Zoom level: %f\n",cameraPosition.y);
		// Clamp zoom to prevent flipping or going through the terrain

		break;

	default:
		break;
	}
}

/// Camera Is UPDATING HERE !!!
void Scene8g::Update(const float deltaTime) 
{
	/// Trackball (T * R * S) read like Hebrew le'chiam ! So the formula is reversed == S * R * T
	/// move, and call this in cam file, function 
	cameraPositionOrientation = trackballMouse->getQuat();
	trackball_translate = MMath::translate(cameraPosition);
	// below turning a quaternion into a matrix , nifty !
	trackball_rotate = MMath::toMatrix4(cameraPositionOrientation);
	trackball_view = MMath::inverse(trackball_translate) * MMath::inverse(trackball_rotate);
}

void Scene8g::Render() const {
	
	/// So this is an error , I should onnly have to use GL_CULL ... its an error, but I am uploading anyways, I will fix this !
	glEnable(GL_DEPTH_TEST); 
	glDepthFunc(GL_LESS); // Default, but important
	
	//glEnable(GL_CULL_FACE);			// no need to render the back  https://registry.khronos.org/OpenGL-Refpages/es2.0/xhtml/glCullFace.xml
	//glEnable(GL_BACK);
	//glEnable(GL_CCW);

	
	glClearColor(0.53, 0.81, 0.92, 1.0);	// 0.53, 0.81, 0.92, 1.0
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	if (drawInWireMode) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	else {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
	/// load the program
	glUseProgram(tess_shader->GetProgram());
	/// Height Map
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, heightMapText->getTextureID());
	/// Diffuse
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, terrainDiffuse_map->getTextureID());
	/// Normal Map
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, normal_Map->getTextureID());

	

	glUniformMatrix4fv(tess_shader->GetUniformID("projectionMatrix"), 1, GL_FALSE, projectionMatrix);
	glUniformMatrix4fv(tess_shader->GetUniformID("viewMatrix"), 1, GL_FALSE, trackball_view);
	glUniformMatrix4fv(tess_shader->GetUniformID("modelMatrix"), 1, GL_FALSE, plane_Body->getModelMatrix());
	glUniform1f(tess_shader->GetUniformID("tesslevelControl"), tesslevelControl);
	glUniform3fv(tess_shader->GetUniformID("lightposition"), 1, lightposition);
	 //add light - phong model

	plane_Mesh->Render(GL_PATCHES);			/// GL_Patches check notes

	/// Unbind/Clear Textures
	// clear texture 2
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, 0);
	// clear texture 1
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, 0);
	// clear texture 0
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, 0);


	glUseProgram(0);						/// unbind shader _ tess
}




