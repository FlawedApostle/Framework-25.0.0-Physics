#ifndef SCENE8_H
#define SCENE8_H
#include "Scene.h"
#include "Vector.h"
#include <Matrix.h>
#include "QMath.h"
#include "Texture.h"
#include "trackball.h"			// trackball

using namespace MATH;

/// Forward declarations 
union SDL_Event;
class Body;
class Mesh;
class Shader;

class Scene8g : public Scene {
private:
	// Plane
	Body* plane_Body;
	Mesh* plane_Mesh;
	Shader* tess_shader;
	/// light
	Vec3 lightposition;
	/// Textures for the Diffuse , Height , Normal
	float tesslevelControl;
	Texture* terrainDiffuse_map;
	Texture* heightMapText;
	Texture* normal_Map;

	
	// Matrix
	Matrix4 projectionMatrix;
	Matrix4 viewMatrix;
	Matrix4 modelMatrix;
	
	bool drawInWireMode;			// wiremeshh mode
	// trackball
	Trackball* trackballMouse;
	Vec3 cameraPosition;			
	Quaternion cameraPositionOrientation; 
	Matrix4 trackball_translate;
	Matrix4 trackball_rotate;
	Matrix4 trackball_view;

public:
	explicit Scene8g();
	virtual ~Scene8g();

	virtual bool OnCreate() override;
	virtual void OnDestroy() override;
	virtual void Update(const float deltaTime) override;
	virtual void Render() const override;
	virtual void HandleEvents(const SDL_Event& sdlEvent) override;
};


#endif // SCENE8_H