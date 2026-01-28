#include <glew.h>
#include <iostream>
#include <SDL.h>
#include "Scene2g.h"			/// scene2g.h
#include <MMath.h>
#include "Debug.h"
#include "Mesh.h"
#include "Shader.h"
#include "Body.h"
/// INIT
Scene2g::Scene2g() :
	shader{ nullptr },
	drawInWireMode{ false }, 
	body_sphere{ nullptr }, 
	mesh_sphere{ nullptr },
	l1Pos_{ },
	lightPos{ }
	//texture_moon{ NULL },
{	Debug::Info("Created Scene2g: ", __FILE__, __LINE__);	}
/// DESTRUCTOR
Scene2g::~Scene2g() {
	Debug::Info("Deleted Scene2g: ", __FILE__, __LINE__);
}
/// BUILDING
bool Scene2g::OnCreate() {
	Debug::Info("Loading assets Scene2g: ", __FILE__, __LINE__);
	/// UNIFORMS - light 0 original light position
	lightPos_0 = Vec3(7.0f,0.0f,0.0f);
	/// light 1 position
	l1Pos_ = Vec3(-7.0f, -7.0f, 0.0f);


	/// UNIFORMS - spec, diff, ambient lighting
	// passing in the spec , diff , ambient through uniforms
	// ambient is also multiplyed by the diffuse uniform
	/// Light 0 - original 
	// spec is the light source itself
	// diffuse is how much it covers the object
	// ambient is how well it blends
	
	/// I built and array inside the fragshader 
	/// a Vec4 of size 6 for a total of (6 * 16) = 96 bytes
	/// the order or the array is ks , kd , ka ..... ks , kd , ka 
	/// I choose this pattern to just keep track , 
	/// in essence the order doesn't matter as long as you know which array location holds what
	/// I prefer a vector array but you cant do that in openGL..... So here you have to manually add it.....
	/// depending on how many lights you want you'd have to statically set the array , 
	/// could i set a uniform to set the array size the user can say how many lights they want first, 
	/// then statically create the array ?
	/// light 1
	/// Passing in the specular expont through uniform into the fragshder pFrag.glsl
	specExponent0_ = 100.0;				/// makes it look gross wet !
	specExponent1_ = 100;				
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// LIGHTS _ POSITIONS
	lightPos = Vec3(0.0f,0.0f,0.0f);
	LIGHT_POSITION[0] = Vec3(0.0f, 0.0f, 0.0f);
	// LIGHT ONE
	kspecular[0] = Vec4(0.0f, 0.0f, 1.0f, 0.0f);
	kdiffuse[0] = Vec4(0.0f, 0.0f, 0.5f, 0.0f);
	kambient[0] = 0.04f * kdiffuse[0];
	specular_uniform_pow[0] = 14.0f;

	kspecular[1] = Vec4(1.0f, 0.0f, 0.0f, 0.0f);
	kdiffuse[1] = Vec4(0.3f, 0.0f, 0.0f, 0.0f);
	kambient[1] = 0.9f * kdiffuse[1];
	specular_uniform_pow[1] = 50.0f;
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// PhongFrag - copy - copy
	/// Lights
	//LIGHT_POSITION[0] = Vec3(0.0f, 0.0f, 0.0f);
	//LIGHT_POSITION[1] = Vec3(10.0f, 5.0f, 0.0f);
	/// Colors 2
	light_position_one = Vec3(7.0f, 7.0f,0.0f);
	spec_exponent_value_1 = 100.0f;
	k_specular0 = Vec4(1.0, 0.0, 0.0, 0.0);
	k_diffuse0 = Vec4(0.3, 0.0, 0.0, 0.0);
	k_ambient0 = 0.10f * k_diffuse0;
	/// color 3
	light_position_two = Vec3(-7.0f, 7.0f, 0.0f);
	spec_exponent_value_2 = 100.0f;
	k_specular1 = Vec4(0.0, 0.0, 1.0, 0.0);
	k_diffuse1 = Vec4(0.0, 0.0, 0.3, 0.0);
	k_ambient1 = 0.10f * k_diffuse1;
	/// color 4
	light_position_three = Vec3(-7.0f,-7.0f, 0.0f);
	spec_exponent_value_3 = 100.0f;
	k_specular2 = Vec4(0.0, 1.0, 0.0, 0.0);
	k_diffuse2 = Vec4(0.0, 0.3, 0.0, 0.0);
	k_ambient2 = 0.10f * k_diffuse2;
	/// color 5
	light_position_four = Vec3(7.0f, -7.0f, 0.0f);
	spec_exponent_value_4 = 100.0f;
	k_specular3 = Vec4(0.0, 1.0, 1.0, 0.0);
	k_diffuse3 = Vec4(0.0, 0.3, 0.3, 0.0);
	k_ambient3 = 0.10f * k_diffuse3;

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// BODY
	body_sphere = new Body();
	body_sphere->OnCreate();
	/// MESH NOTE:: changed to skull
	mesh_sphere = new Mesh("meshes/Skull.obj");
	mesh_sphere->OnCreate();
	/// SHADER
	shader = new Shader("shaders/phongVert - Copy - Copy.glsl", "shaders/phongFrag - Copy - Copy.glsl");
	if (shader->OnCreate() == false) {
		std::cout << "Shader failed ... we have a problem\n";
	} else printf("shader loaded:: pVert && pFrag\n");
	/// MATRIX
	projectionMatrix = MMath::perspective(45.0f, (16.0f / 9.0f), 0.5f, 100.0f);
	viewMatrix = MMath::lookAt(Vec3(0.0f, 0.0f, 5.5f), Vec3(0.0f, 0.0f, 0.0f), Vec3(0.0f, 1.0f, 0.0f));
	//modelMatrix.loadIdentity();
	
	/// Sphere Matrix /// to Move the ball
	//projectionMatrix_sphere = MMath::perspective(45.0f, (16.0f / 9.0f), 0.5f, 100.0f);;
	//viewMatrix_sphere = MMath::lookAt(Vec3(0.0f, 0.0f, 5.5f), Vec3(0.0f, 0.0f, 0.0f), Vec3(0.0f, 1.0f, 0.0f));
	//rotate_sphere = MMath::rotate(0.0f, Vec3(0.0f, 0.0f, 0.0f));
	//translate_sphere = MMath::translate(Vec3(0.0f,0.0f,0.0f));
	//modelMatrix_sphere = translate_sphere * rotate_sphere * scale_sphere;


	return true;
}
/// CLEAN UP !
void Scene2g::OnDestroy() {
	Debug::Info("Deleting assets Scene2g: ", __FILE__, __LINE__);
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
void Scene2g::HandleEvents(const SDL_Event& sdlEvent) {
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
void Scene2g::Update(const float deltaTime) 
{
	//float totalTime = 0.0f;
	//totalTime += deltaTime;
	//MMath::rotate(90, Vec3(0.0f, 1.0f, 0.0f));
	//MMath::translate(Vec3(0.0f, 1.0f, 0.0f));
}
/// JAZZ HANDS
void Scene2g::Render() const {

	/// Set the background color then clear the screen
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_CULL_FACE);
	//glCullFace(GL_BACK);			/// WHERE LOOKING AT THE BACK OF THE SPEHERE...

	glEnable(GL_DEPTH_TEST);

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
	// UNIFORMS control the following in pFrag.glsl file spec, diff , ambient , light 1 , light 2
	//glUniform3fv(shader->GetUniformID("lightOne_Pos"), 1, lightOne_Pos);
	/// UNIFORMS - POSITION Light 0 - orignial light position
	//glUniform3fv(shader->GetUniformID("lightPos_0"), 1, lightPos);
	/// UNIFORMS - POSITION Light 2 phongFrag - Copy - Copy
	glUniform3fv(shader->GetUniformID("light_position_one"), 1, light_position_one);
	glUniform3fv(shader->GetUniformID("light_position_two"), 1, light_position_two);
	glUniform3fv(shader->GetUniformID("light_position_three"), 1, light_position_three);
	glUniform3fv(shader->GetUniformID("light_position_four"), 1, light_position_four);
	//glUniform3fv(shader->GetUniformID("LIGHT_POSITION[0]"), 1, LIGHT_POSITION[0]);
	//glUniform3fv(shader->GetUniformID("LIGHT_POSITION[1]"), 1, LIGHT_POSITION[1]);
	/// UNIFORMS - POSITION Light 1 - light 1 position
	//glUniform3fv(shader->GetUniformID("l1Pos"), 1, l1Pos_);
	/// UNIFORMS Phong - Light 0 orinigal light - spec , diff , ambient
	//glUniform4fv(shader->GetUniformID("specular"), 1, Light_[0]);
	//glUniform4fv(shader->GetUniformID("diffuse"), 1, Light_[1]);
	//glUniform4fv(shader->GetUniformID("ambient"), 1, Light_[2]);
	/// Uniforms Phong - light 1 - spec , dif , ambient
	//glUniform4fv(shader->GetUniformID("l1specular"), 1, l1specular_);
	//glUniform4fv(shader->GetUniformID("l1diffuse"), 1, l1diffuse_);
	//glUniform4fv(shader->GetUniformID("l1ambient"), 1, l1ambient_);
	/// UNIFORMS specular exponent
	//glUniform1f(shader->GetUniformID("specExponent0"), specExponent0_);
	//glUniform1f(shader->GetUniformID("specExponent1"), specExponent1_);
	/// Uniform PhongFrag - copy
	//glUniform4fv(shader->GetUniformID("specular"), 1, spec_);
	//glUniform4fv(shader->GetUniformID("diffuse"), 1, diff_);
	//glUniform4fv(shader->GetUniformID("ambient"), 1, amb_);

	/// Light 1 phongFrag - Copy - Copy
	glUniform4fv(shader->GetUniformID("k_specular0"), 1, k_specular0);
	glUniform4fv(shader->GetUniformID("k_diffuse0"), 1, k_diffuse0);
	glUniform4fv(shader->GetUniformID("k_ambient0"), 1, k_ambient0);
	glUniform1f(shader->GetUniformID("spec_exponent_value_1"), spec_exponent_value_1);

	glUniform4fv(shader->GetUniformID("k_specular1"), 1, k_specular1);
	glUniform4fv(shader->GetUniformID("k_diffuse1"), 1, k_diffuse1);
	glUniform4fv(shader->GetUniformID("k_ambient1"), 1, k_ambient1);
	glUniform1f(shader->GetUniformID("spec_exponent_value_2"), spec_exponent_value_2);

	glUniform4fv(shader->GetUniformID("k_specular2"), 1, k_specular2);
	glUniform4fv(shader->GetUniformID("k_diffuse2"), 1, k_diffuse2);
	glUniform4fv(shader->GetUniformID("k_ambient2"), 1, k_ambient2);
	glUniform1f(shader->GetUniformID("spec_exponent_value_3"), spec_exponent_value_3);

	glUniform4fv(shader->GetUniformID("k_specular3"), 1, k_specular3);
	glUniform4fv(shader->GetUniformID("k_diffuse3"), 1, k_diffuse3);
	glUniform4fv(shader->GetUniformID("k_ambient3"), 1, k_ambient3);
	glUniform1f(shader->GetUniformID("spec_exponent_value_3"), spec_exponent_value_4);


	// RENDER
	mesh_sphere->Render(GL_TRIANGLES);


	glUseProgram(0);
}




