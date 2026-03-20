#pragma once
#define SkyBox_H

#include "glew.h"
#include "SDL.h"
#include "SDL_image.h"
#include "Texture.h"
#include "Shader.h"
#include "Mesh.h"
#include "Body.h"
/// Fowarad Declerations
class Scene;
class Mesh;
class Texture;
class Shader;
class Camera;

class SkyBox {
protected:
private:
	int mode;
	SDL_Surface* textureSurface;
	GLuint textureID;
	
	// array this dude .... 
	const char* cubeMap[6];
	/// SkyBox images
	const char* posX;
	const char* negX;
	const char* posY;
	const char* negY;
	const char* posZ;
	const char* negZ;

	Shader* SkyBoxShader;
	Mesh* skyBoxcubeMesh;
	Mesh* Mesh_;
public:
	// default construct , this is neccessary. 
	// why ? because I have to call another pointer .... inside Camera to load the skyBox shader
	// The Camera we are legit only using it as a CAMERA. so I am only using orientation , view , projection and taking the variables and applying to to the scene
	// this default constructor is vital to call the camera variables
	SkyBox();
	// array this sh*** plz || struct
	SkyBox
	(
		const char* posX_ ,
		const char* negX_ ,
		const char* posY_ ,
		const char* negY_ ,
		const char* posZ_ ,
		const char* negZ_
	);

	~SkyBox();

	/// Loading && getting the SkyBox Cube Mesh , this mesh is what we are rending the photos onto for the skybox.
	bool getSkyBoxcubeMesh() 
	{
			skyBoxcubeMesh = new Mesh("meshes/Cube.obj");
			if (skyBoxcubeMesh->OnCreate() == false)
			{
				return false;
			}
	}
	/// Load the skyBox imgs , this is a global functions (or the ideal behind it)
	// So any scene can use this function, providing they include the header , they can then load any SIX images for a SkyBox
	bool LoadImages();						
	//bool LoadImages_Array();				/// eventually I want to load the images into an array
	bool OnCreate();						/// OnCreate is used for loading the SkyBox 
	bool OnDestroy();

	bool Render();

	bool getTextureID()
	{
		return textureID;
	}

	/// return the skybox shader
	// this was a fkn pain in the ass to solve
	Shader* getShader_SkyBoxShader();
	// I put the shader into a function. this is called in the Camera cpp file in OnCreate()
	// I used an inline function.
	/// From what I've read its to reduce overhead, so at the time of call the functions 'guts' code so to speak is being called
	/// rather than the compiler going though the cpp file to the function
	/// its also noted that its pointless to call inline, because a function defined inside a class is already considered an 'implicite inline function'
	//	im leaving it inline for now
	inline Shader& LoadShader()
	{
		SkyBoxShader = new Shader("shaders/SkyBox/SkyBoxVert.glsl", "shaders/SkyBox/SkyBoxFrag.glsl");
		if (SkyBoxShader->OnCreate() == false)
		{
			std::cout << "SkyBoxShader failed ... we have a problem\n";
		}
		return  *SkyBoxShader;

	};
	/// This funcions will get the memoray address of the pointer SkyBox shader
	// note the dereference to get the value of the pointer
	inline Shader& getShader() 
	{ 
		return *SkyBoxShader; 
	};



};

#ifndef SkyBox_H
#endif