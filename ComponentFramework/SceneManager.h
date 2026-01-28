#ifndef SCENEMANAGER_H
#define SCENEMANAGER_H

#include <string>
class SceneManager  {
public:
	
	SceneManager();
	~SceneManager();
	void Run();
	bool Initialize(std::string name_, int width_, int height_);
	void HandleEvents();
	
	
private:
	enum class SCENE_NUMBER {
		SCENE0g = 0,
		SCENE1g = 1,			// Graphics engine - phong lighnting -> default baseline working as of 5/10/25
		SCENE7g = 2,			// Graphics Fresnal PT 2
		SCENE2g = 3,			// Graphics game scene 2 - phong lighting
		SCENE3g = 4,			// Graphics game scene 2 - phong lighting
		SCENE2p = 5,			// Physics Toruqe Ball Part 2
		SCENE4g = 6,			// Graphics game scene 2 - phong lighting
		SCENE5g = 7,			// Graphics - SkyBox PT 1
		SCENE3p = 8,			// Physics Toruqe Ball Part 3
		SCENE6g = 9,			// Graphics - SkyBox PT 2
		SCENE8g = 10,			// Graphics - Tesselation
		SCENE0p = 11,
		SCENE1p = 12,
		//SCENE3p = 14,
		SCENE4p,
		SCENE5,
		SCENE6,
	};

	class Scene* currentScene;
	class Timer* timer;
	class Window* window;

	unsigned int fps;
	bool isRunning;
	bool fullScreen;
	bool BuildNewScene(SCENE_NUMBER scene_);
};


#endif // SCENEMANAGER_H