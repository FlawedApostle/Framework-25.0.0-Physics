///									FILENAME:: (phongFrag3g / phongVert3g) working as of 2025 / 05 / 28 10:30 AM
#include <glew.h>
#include <iostream>
#include <SDL.h>
#include "Scene3g.h"			/// scene3g.h  this is to play with the shader , getting arrays to work .. 
#include <MMath.h>
#include "Debug.h"
#include "Mesh.h"
#include "Shader.h"
#include "Body.h"
/// INIT
Scene3g::Scene3g() :
	 shader{ nullptr }
   , drawInWireMode{ false }
   , body_sphere{ nullptr }
   , mesh_sphere{ nullptr }
   , l1Pos_{ }
   , lightPos{ }
	//texture_moon{ NULL },
{
	Debug::Info("Created Scene3g: ", __FILE__, __LINE__);
}
/// DESTRUCTOR
Scene3g::~Scene3g() {
	Debug::Info("Deleted Scene3g: ", __FILE__, __LINE__);
}
/// BUILDING
bool Scene3g::OnCreate() {
	Debug::Info("Loading assets Scene3g: ", __FILE__, __LINE__);
	/// UNIFORMS - light 0 original light position
	lightPos_0 = Vec3(7.0f, 0.0f, 0.0f);
	/// light 1 position
	//l1Pos_ = Vec3(-7.0f, -7.0f, 0.0f);


	/// UNIFORMS - spec, diff, ambient lighting
	// passing in the spec , diff , ambient through uniforms
	// ambient is also multiplyed by the diffuse uniform
	/// Light 0 - original 
	// spec is the light source itself
	// diffuse is how much it covers the object
	// ambient is how well it blends
	/// light 1
	/// Passing in the specular expont through uniform into the fragshder pFrag.glsl

	////////////////////////////////////////////////ARRAY/////////////////////////////////////////////////////////////////////////////////////////////////////
	/// LIGHTS _ POSITIONS
	LIGHT_POSITION[0] = Vec3(10.0f, 0.0f, 0.0f);
	LIGHT_POSITION[1] = Vec3(-10.0f,0.0f, 0.0f);
						/// Array
	// Wetnesss , shinyness
	SPEC_EXPONENT_VALUE[0] = 25.0f;
	SPEC_EXPONENT_VALUE[1] = 2.0f;
	// Color 1
	SPEC[0] = Vec4(1.0f, 0.0f, 0.0f, 0.0f);
	DIFF[0] = Vec4(0.5f, 0.0f, 0.0f, 0.0f);
	AMB[0]  = 0.03f * DIFF[0];
	// Color 2
	SPEC[1] = Vec4(0.0f, 0.0f, 1.0f, 0.0f);
	DIFF[1] = Vec4(0.0f, 0.0f, 0.5f, 0.0f);
	AMB[1] = 0.03f * DIFF[1];

	/// BODY
	body_sphere = new Body();
	body_sphere->OnCreate();
	/// MESH NOTE:: changed to skull
	mesh_sphere = new Mesh("meshes/Skull.obj");
	mesh_sphere->OnCreate();
	/// SHADER
	shader = new Shader("shaders/Scene3g/phongVert3g.glsl", "shaders/Scene3g/phongFrag3g.glsl");
	if (shader->OnCreate() == false) {
		std::cout << "Shader failed ... we have a problem\n";
	}
	else printf("shader loaded:: PhongVert3g && phongFrag3g\n");
	/// MATRIX
	projectionMatrix = MMath::perspective(45.0f, (16.0f / 9.0f), 0.5f, 100.0f);
	viewMatrix = MMath::lookAt(Vec3(0.0f, 0.0f, 5.5f), Vec3(0.0f, 0.0f, 0.0f), Vec3(0.0f, 1.0f, 0.0f));
	//modelMatrix.loadIdentity();		// the reason for this line ? 
	return true;
}
/// CLEAN UP !
void Scene3g::OnDestroy() {
	Debug::Info("Deleting assets Scene3g: ", __FILE__, __LINE__);
	/// SPHERE
	body_sphere->OnDestroy();
	delete body_sphere;
	mesh_sphere->OnDestroy();
	delete mesh_sphere;
	/// SHADER
	shader->OnDestroy();
	delete shader;
}
/// JOYSTICK
void Scene3g::HandleEvents(const SDL_Event& sdlEvent) {
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
void Scene3g::Update(const float deltaTime)
{
	//float totalTime = 0.0f;
	//totalTime += deltaTime;
	//MMath::rotate(90, Vec3(0.0f, 1.0f, 0.0f));
	//MMath::translate(Vec3(0.0f, 1.0f, 0.0f));
}
/// JAZZ HANDS
void Scene3g::Render() const {
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	//glCullFace(GL_BACK);

	/// Set the background color then clear the screen
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (drawInWireMode) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	else {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
	glUseProgram(shader->GetProgram());
	
	glUniformMatrix4fv(shader->GetUniformID("projectionMatrix"), 1, GL_FALSE, projectionMatrix);
	glUniformMatrix4fv(shader->GetUniformID("viewMatrix"), 1, GL_FALSE, viewMatrix);
	glUniformMatrix4fv(shader->GetUniformID("modelMatrix"), 1, GL_FALSE, modelMatrix);
	/// UNIFORM ACCESS	
	// Arrays were a pain in the a** the uniform must match , the size of the array must match , and the index must match .....
	/// Color 1 red
	glUniform3fv(glGetUniformLocation(shader->GetProgram(), "LIGHT_POSITION[0]"), 2, LIGHT_POSITION	[0]);
	glUniform4fv(glGetUniformLocation(shader->GetProgram(),"SPEC[0]"), 2, SPEC[0]);
	glUniform4fv(glGetUniformLocation(shader->GetProgram(),"DIFF[0]"), 2, DIFF[0]);
	glUniform4fv(glGetUniformLocation(shader->GetProgram(),"AMB[0]"), 2, AMB[0]);

	/// Color 2 yell
	glUniform3fv(glGetUniformLocation(shader->GetProgram(), "LIGHT_POSITION[1]"), 2 ,LIGHT_POSITION[1]);
	glUniform4fv(glGetUniformLocation(shader->GetProgram(), "SPEC[1]"), 2, SPEC[1]);
	glUniform4fv(glGetUniformLocation(shader->GetProgram(), "DIFF[1]"), 2, DIFF[1]);
	glUniform4fv(glGetUniformLocation(shader->GetProgram(), "AMB[1]"), 2,  AMB[1]);


	// RENDER
	mesh_sphere->Render(GL_TRIANGLES);


	glUseProgram(0);
}




