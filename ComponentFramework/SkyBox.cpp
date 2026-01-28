#include "glew.h"
#include "SDL.h"
#include "SDL_image.h"
#include "Texture.h"
#include "Shader.h"
#include "SkyBox.h"
#include "Debug.h"
SkyBox::SkyBox()
{
	/// Mem cleaning 
	SkyBoxShader = nullptr;
	skyBoxcubeMesh = nullptr;
	Mesh_ = nullptr;
	mode = NULL;
	textureSurface = nullptr;
	textureID = NULL;
	posX = NULL;
	posY = NULL;
	posZ = NULL;
	negX = NULL;
	negY = NULL;
	negZ = NULL;
	cubeMap[0] = nullptr;
	cubeMap[1] = nullptr;
	cubeMap[2] = nullptr;
	cubeMap[3] = nullptr;
	cubeMap[4] = nullptr;
	cubeMap[5] = nullptr;

}
// This is why I feel like im wasting my time..
// So in the header of skyBox I have a constructor from which the user can input images (files) upon creating an object of the skyBox
// Here in the cpp of skyBox I have created temp vars (underscores_). So everytime an object is created of skyBox the temp vars are built
// these temp vars are then assigned to the private vars of the skyBox imageLoad function
SkyBox::SkyBox
(
	const char* posX_,
	const char* NegX_,
	const char* posY_,
	const char* NegY_,
	const char* posZ_,
	const char* Negz_
)

{
	/// Mem cleaning 
	SkyBoxShader = nullptr;
	skyBoxcubeMesh = nullptr;
	Mesh_ = nullptr;
	mode = NULL;
	textureSurface = nullptr;
	textureID = NULL;

	/// Here we assign the new public temp vars to the private skyBox positional photo variables
	// X
	posX = posX_;
	negX = NegX_;
	// Y
	posY = posY_;
	negY = NegY_;
	// Z
	posZ = posZ_;
	negZ = Negz_;

	/// input array pos, neg
	// Positive
	cubeMap[0] = posX_;
	cubeMap[1] = posY_;
	cubeMap[2] = posZ_;
	// Negative
	cubeMap[3] = negX;
	cubeMap[3] = negY;
	cubeMap[3] = negZ;


}
/// Clean up 
SkyBox::~SkyBox()
{
	glDeleteTextures(1, &textureID);
	SDL_Surface(textureSurface);
}

/// Destroy
bool SkyBox::OnDestroy()
{
	if (skyBoxcubeMesh != nullptr) {
		skyBoxcubeMesh->OnDestroy();
		delete skyBoxcubeMesh;
	}

	if (SkyBoxShader != nullptr)
	{
		SkyBoxShader->OnDestroy();
		delete SkyBoxShader;
	}

	if (Mesh_ != nullptr)
	{
		Mesh_->OnDestroy();
		delete Mesh_;
	}

	
	return true;
}

// TextureID is a private variable inside skyBox.h , there is a get function for it, used in LoadImages function
bool SkyBox::LoadImages()
{
	printf("Loading Skybox.....\n");
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
	
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	textureSurface = IMG_Load(posX);
	if (textureSurface == nullptr) {
			return false;
	}
	// what are you loading ? - PNG = (RGBA) || JPG = (RGB) 
	// (?) if the format BytesPerPixel == 4 , (:) if true(left) then RGBA , if false(right) RGB 
	mode = (textureSurface->format->BytesPerPixel == 4) ? GL_RGBA : GL_RGB;
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, mode, textureSurface->w, textureSurface->h, 0, mode, GL_UNSIGNED_BYTE, textureSurface->pixels);
	SDL_FreeSurface(textureSurface);
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	textureSurface = IMG_Load(posY);
	if (textureSurface == nullptr) {
		return false;
	}
	mode = (textureSurface->format->BytesPerPixel == 4) ? GL_RGBA : GL_RGB;
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, mode, textureSurface->w, textureSurface->h, 0, mode, GL_UNSIGNED_BYTE, textureSurface->pixels);
	SDL_FreeSurface(textureSurface);
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	textureSurface = IMG_Load(posZ);
	if (textureSurface == nullptr) {
		return false;
	}
	mode = (textureSurface->format->BytesPerPixel == 4) ? GL_RGBA : GL_RGB;
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, mode, textureSurface->w, textureSurface->h, 0, mode, GL_UNSIGNED_BYTE, textureSurface->pixels);
	SDL_FreeSurface(textureSurface);
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	textureSurface = IMG_Load(negX);
	if (textureSurface == nullptr) {
		return false;
	}
	mode = (textureSurface->format->BytesPerPixel == 4) ? GL_RGBA : GL_RGB;
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, mode, textureSurface->w, textureSurface->h, 0, mode, GL_UNSIGNED_BYTE, textureSurface->pixels);
	SDL_FreeSurface(textureSurface);
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	textureSurface = IMG_Load(negY);
	if (textureSurface == nullptr) {
		return false;
	}
	mode = (textureSurface->format->BytesPerPixel == 4) ? GL_RGBA : GL_RGB;
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, mode, textureSurface->w, textureSurface->h, 0, mode, GL_UNSIGNED_BYTE, textureSurface->pixels);
	SDL_FreeSurface(textureSurface);
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	textureSurface = IMG_Load(negZ);
	if (textureSurface == nullptr) {
		return false;
	}
	mode = (textureSurface->format->BytesPerPixel == 4) ? GL_RGBA : GL_RGB;
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, mode, textureSurface->w, textureSurface->h, 0, mode, GL_UNSIGNED_BYTE, textureSurface->pixels);
	SDL_FreeSurface(textureSurface);
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);


	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	return true;
}


/// Create
bool SkyBox::OnCreate()
{
	Debug::Info("Loading assets SkyBox: ", __FILE__, __LINE__);
	/// MESH
	// Load the cube mesh, which is used to render the images onto, the scale of the cube is 1.0f , any change will affect the skybox
	skyBoxcubeMesh = new Mesh("meshes/Cube.obj");
	if (skyBoxcubeMesh->OnCreate() == false)
	{
		printf("error loading error SkyBox cube Mesh !\n");
		return false;
	}printf("SkyBox Cube Mesh Loaded\n");
	/// SHADER
	SkyBoxShader = new Shader("shaders/SkyBox/SkyBoxVert.glsl", "shaders/SkyBox/SkyBoxFrag.glsl");
	if (SkyBoxShader->OnCreate() == false)
	{
		std::cout << "SkyBoxShader failed ... we have a problem\n";
	}printf("Skybox Shader Loaded !\n");

	return true;
}


bool SkyBox::Render()
{
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);		// textureID
	skyBoxcubeMesh->Render();
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	
	glUseProgram(0);

	return true;
}

Shader* SkyBox::getShader_SkyBoxShader()
{

	return SkyBoxShader;
}
