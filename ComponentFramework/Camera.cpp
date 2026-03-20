#include "Camera.h"
#include <SDL.h>
#include <MMath.h>
#include "QMath.h"
#include "SkyBox.h"

// NOTE:: I would like to use this header file for my capstone, perhaps putting constraints on quaternion.
	
// So world space is the 'world' cliche yes to say. 
// The world contains the objects and the space they occupy
/// the view is the 'camera' so to speak. As if to say that the view translates the world to the view of the 'camera' the POV so to speak
Camera::Camera() 
	
{
	skyboxRun = nullptr;
	/// Inside skyBox shader the texCoords are related to the loaded models vertex(s)
	/// by inverting the texCoords axes in the vert shader corrects the images loaded on the skyBox 
	projection = MMath::perspective(45.0f, (16.0f / 9.0f), 0.5f, 100.0f);				// view fustrum
	
	orientation = Quaternion(1.0f, Vec3(0.0f, 0.0f, 0.0f));								// quat orientation
	position = Vec3(0.0f, 0.0f, -5.0f);	// z = -15.0f									// position from origin
	view = MMath::toMatrix4(orientation) * MMath::translate(position);					// world space to camera spac

	translation = MMath::translate(0.0f, 0.0f, -5.0f);									// moving, only on the z axis

	view = view * translation;															// applying the translations to the view 'camera'

	orientation = Quaternion(90.0f, Vec3(0.0f, 0.0f, 1.0f));							
	
	orientation = QMath::angleAxisRotation(0.0f, (Vec3(0.0f, 1.0f, 0.0f)));
	orientationMat4 = MMath::inverse(MMath::toMatrix4(orientation));
}

void Camera::SetView(const Quaternion& orientation_, const Vec3& position_) {
	orientation = orientation_;
	position = position_;
}

void Camera::OnDestroy() 
{
	if (skyboxRun != NULL) {
		skyboxRun->OnDestroy();
		delete skyboxRun;
	}
	
}

Camera::~Camera() {}

bool Camera::OnCreate() {
	/*
	skybox = new SkyBox("textures/Underwater Box_Posx.png",
						"textures/Underwater Box_Negx.png",
						"textures/Underwater Box_Posy.png",
						"textures/Underwater Box_Negy.png",
						"textures/Underwater Box_Posz.png",
						"textures/Underwater Box_Negz.png");
	
	if (skybox->OnCreate() == false) {
		return false;
	}
	*/

	skyboxRun = new SkyBox();
	if (skyboxRun == NULL)
	{
		printf("Camera Pointer to Skybox error !\n");
		return false;
	}

	skyboxRun->LoadShader();

	return true;
}

void Camera::HandelEvents(const SDL_Event& sdlEvent) {
	trackball.HandleEvents(sdlEvent);
	orientation = trackball.getQuat();
}

/// Perhaps we can add the skybox into here ?
// Im a dumbass and I just seperated the skybox and the camera to be used seperatately
void Camera::RenderSkyBox() const{
	//if (skybox == nullptr) return;
	//glDisable(GL_DEPTH_TEST);
	//glDisable(GL_CULL_FACE);
	//glUseProgram(skybox->GetShader()->GetProgram());
	//glUniformMatrix4fv(skybox->GetShader()->GetUniformID("projectionMatrix"), 1, GL_FALSE, projection);
	//glUniformMatrix4fv(skybox->GetShader()->GetUniformID("viewMatrix"), 1, GL_FALSE, MMath::toMatrix4(orientation));
	/// Here I has turned on the shader and set the matricies. The shader will remain in this state
	/// until I turn off the shader. In Skybox::Render, I will bind the textures, because that is where
	/// they are, then draw the cube.  
	//skybox->Render();
	//glUseProgram(0);
}
