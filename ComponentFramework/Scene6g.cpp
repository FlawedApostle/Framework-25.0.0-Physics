///									FILENAME:: (phongFrag5g / phongVert5g) working as of 2025 / 05 / 28 10:30 AM
///	Note:: the Vert and Frag shader is correct, the array, along with the formula. Do not f***** with it please !
#include <glew.h>
#include <SDL.h>
#include "Scene6g.h"			/// scene6g.h  Graphics skybox A3
#include <MMath.h>
#include "Debug.h"
#include "Texture.h"
#include "Shader.h"
#include "Mesh.h"
#include "Body.h"
#include "SkyBox.h"				/// Skybox 
#include "Camera.h"
/// INIT
Scene6g::Scene6g() :
	shader{ nullptr }
	, drawInWireMode{ false }
	, body_cube{ nullptr }
	, mesh_cube{ nullptr }
	, CN_SkyBox{ nullptr }
	, CN_Camera{ nullptr }
	, SPEC_EXPONENT_VALUE{ NULL }
{
	Debug::Info("Created Scene6g: ", __FILE__, __LINE__);
}
/// DESTRUCTOR
Scene6g::~Scene6g() {
	Debug::Info("Deleted Scene6g: ", __FILE__, __LINE__);
}
/// BUILDING
bool Scene6g::OnCreate() {
	Debug::Info("Loading assets Scene6g: ", __FILE__, __LINE__);
	/// Camera
	// gathering the camera orientation , view , and projection.
	// This is the Camera we use for the current scene in Scene5g 
	CN_Camera = new Camera();
	CN_Camera->GetOrientation();
	CN_Camera->GetProjectionMatrix();

	/// Color Unifroms
	dColor = Vec4(0.1f, 0.3f, 0.4f,1.0f);
	color_diffuse = Vec4(0.1f, 0.3f, 0.4f,1.0f);

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
	LIGHT_POSITION[0] = Vec3(5.0f, 0.0f, 0.0f);
	/// Array
/// shinyness
	SPEC_EXPONENT_VALUE[0] = 100.0f;
	/// Color 1
	SPEC[0] = Vec4(0.0f, 0.0f, 1.0f, 0.0f);
	DIFF[0] = Vec4(0.0f, 0.0f, 0.5f, 0.0f);
	AMB[0] = 0.3f * DIFF[0];

	/// BODY
	body_cube = new Body();
	body_cube->OnCreate();
	// MESH NOTE:: when I use a cube , the colors are fk'd ..... is it based on the reflection indeces off the cube ? why though ? , could it depend on light position ?
	mesh_cube = new Mesh("meshes/Sphere.obj");
	mesh_cube->OnCreate();
	/// SHADER
	// when porting the shader Scene6g doesnt work, - multiple bindings to output semantic ATTR0 - fixed (2025-07-04)
	shader = new Shader("shaders/Scene6g/fresnalVert6g.glsl", "shaders/Scene6g/fresnalFrag6g.glsl");
	if (shader->OnCreate() == false)
	{
		std::cout << "Shader failed ... we have a problem\n";
	}


	/// MATRIX
	// These defaults Mats are being used in update, the trackball.Quat is basing its translate, and orientation off these basic Matrices 
	projectionMatrix = MMath::perspective(45.0f, (16.0f / 9.0f), 0.5f, 100.0f);
	viewMatrix = MMath::lookAt(Vec3(0.0f, 0.0f, 10.0f), Vec3(0.0f, 0.0f, 0.0f), Vec3(0.0f, 1.0f, 0.0f));

	return true;
}
/// CLEAN UP !
void Scene6g::OnDestroy() {
	Debug::Info("Deleting assets Scene6g: ", __FILE__, __LINE__);
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
void Scene6g::HandleEvents(const SDL_Event& sdlEvent) {
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
void Scene6g::Update(const float deltaTime)
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
void Scene6g::Render() const {

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
	/// Render the skyBox
	// inside the Render function the cubeMap is unbind(ed)
	CN_SkyBox->Render();		 


	/// Mesh Inside SkyBox
	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_CULL_FACE);

	glUseProgram(shader->GetProgram());
	/// Bind - renders the skybox images onto the spehere
	/// getTextureID is created inside Skybox.h , There the textureId is gathered from the LoadImages function for the skybox. 
	/// the textureID is used to gather the six skybox images, and BIND it onto the sphere , remembe to unbind after rendering the model
	glBindTexture(GL_TEXTURE_CUBE_MAP, CN_SkyBox->getTextureID());

	glUniformMatrix4fv(shader->GetUniformID("projectionMatrix"), 1, GL_FALSE, projectionMatrix);
	glUniformMatrix4fv(shader->GetUniformID("viewMatrix"), 1, GL_FALSE, viewMatrix * trackball_V);
	glUniformMatrix4fv(shader->GetUniformID("modelMatrix"), 1, GL_FALSE, body_cube->getModelMatrix());
	/// Render MeshCube
	mesh_cube->Render(GL_TRIANGLES);			
	
	/// Unbind the texture cubeMap onto the spehere
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);		
	
	glDisable(GL_DEPTH_TEST);
	glUseProgram(0);
}




