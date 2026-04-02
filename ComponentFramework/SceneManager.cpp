#include <SDL.h>
#include "SceneManager.h"
#include "Timer.h"
#include "Window.h"
#include "Scene0g.h"			/// default Graphics
#include "Scene0p.h"			/// default Physics
#include "Scene1g.h"			// graphics - phong lighting baseline working as of 5/10/25
#include "Scene2g.h"			// graphics - phong lighting A1
#include "Scene3g.h"			// Graphics - Scene3 Model Manipulation
#include "Scene4g.h"			// Graphics - Texture
#include "Scene5g.h"			// Graphics - Texture
#include "Scene6g.h"			// Graphics - skyBox
#include "Scene7g.h"			// Graphics - Fresnal PT 2
#include "Scene8g.h"			// Graphics - Tesselation
/// Physics
#include "Scene1p.h"			// Rolling Ball at slant
#include "Scene2p.h"			// Rotation Ball on plane
#include "Scene3p.h"			// Collisions Ball-to-Ball - rotation on testing different functions pt 1.
#include "Scene4p.h"			// Collisions Ball-to-Ball - rotation on testing different functions pt 2.


SceneManager::SceneManager(): 
	currentScene{nullptr}, window{nullptr}, timer{nullptr},
	fps(60), isRunning{false}, fullScreen{false} {
	Debug::Info("Starting the SceneManager", __FILE__, __LINE__);
}

SceneManager::~SceneManager() {
	Debug::Info("Deleting the SceneManager", __FILE__, __LINE__);

	if (currentScene) {
		currentScene->OnDestroy();
		delete currentScene;
		currentScene = nullptr;
	}
	
	if (timer) {
		delete timer;
		timer = nullptr;
	}

	if (window) {
		delete window;
		window = nullptr;
	}
	
}
/// ------ CHANGE SCENE
bool SceneManager::Initialize(std::string name_, int width_, int height_) {

	window = new Window();
	if (!window->OnCreate(name_, width_, height_)) {
		Debug::FatalError("Failed to initialize Window object", __FILE__, __LINE__);
		return false;
	}

	timer = new Timer();
	if (timer == nullptr) {
		Debug::FatalError("Failed to initialize Timer object", __FILE__, __LINE__);
		return false;
	}

	/********************************   Default first scene   ***********************/
	//BuildNewScene(SCENE_NUMBER::SCENE0p);				
	//BuildNewScene(SCENE_NUMBER::SCENE1p);
	//BuildNewScene(SCENE_NUMBER::SCENE2p);
	//BuildNewScene(SCENE_NUMBER::SCENE3p);
	BuildNewScene(SCENE_NUMBER::SCENE4p);
	/********************************************************************************/
	return true;
}
///------ RUNS GAME
void SceneManager::Run() {
	timer->Start();
	isRunning = true;
	while (isRunning) {
		HandleEvents();
		timer->UpdateFrameTicks();
		currentScene->Update(timer->GetDeltaTime());
		currentScene->Render();
		
		SDL_GL_SwapWindow(window->getWindow());
		SDL_Delay(timer->GetSleepTime(fps));
	}
}

void SceneManager::HandleEvents() {
	SDL_Event sdlEvent;
	while (SDL_PollEvent(&sdlEvent)) { /// Loop over all events in the SDL queue
		if (sdlEvent.type == SDL_EventType::SDL_QUIT) {
			isRunning = false;
			return;
		}
		else if (sdlEvent.type == SDL_KEYDOWN) {
			switch (sdlEvent.key.keysym.scancode) {
			[[fallthrough]]; /// C17 Prevents switch/case fallthrough warnings
			case SDL_SCANCODE_ESCAPE:
			
			case SDL_SCANCODE_Q:
				isRunning = false;
				return;
				
			case SDL_SCANCODE_F12:
				printf
				(
					"PHYSICS Scenes::\n"																			
					"Scene 0p (1) - Physics Baseline\n"
					"Scene 1p (2) - Ball Rolling\n"
					"Scene 2p (3) - Ball & Plane \n"
					"Scene 3p (4) - Collision - rotation on testing different functions pt 1. \n"
					"Scene 4p (5) - Collision - rotation on testing different functions pt 2.\n"
				);
				break;
			case SDL_SCANCODE_1:
				printf("Scene 0p - BASELINE\n");
				BuildNewScene(SCENE_NUMBER::SCENE0p);
				break;
			case SDL_SCANCODE_2:
				printf("Scene 1p - ROTATIONS - BALL ON SLANT\n");
				BuildNewScene(SCENE_NUMBER::SCENE1p);
				break;
			case SDL_SCANCODE_3:
				printf("Scene 2p - ROTATIONS - BALL ON FLAT PLANE\n");
				BuildNewScene(SCENE_NUMBER::SCENE2p);
				break;
			case SDL_SCANCODE_4:
				printf("Scene 3p - COLLISIONS - BALL-TO-BALL - rotation on testing different functions pt 1.");
				BuildNewScene(SCENE_NUMBER::SCENE3p);
				break;
			case SDL_SCANCODE_5:
				printf("Scene 4p - COLLISIONS - BALL-TO-BALL - rotation on testing different functions pt 2.");
				BuildNewScene(SCENE_NUMBER::SCENE4p);
				break;

			default:
				break;
			}
		}
		if (currentScene == nullptr) { /// Just to be careful
			Debug::FatalError("No currentScene", __FILE__, __LINE__);
			isRunning = false;
			return;
		}
		currentScene->HandleEvents(sdlEvent);
	}
}

bool SceneManager::BuildNewScene(SCENE_NUMBER scene) {
	bool status; 

	if (currentScene != nullptr) {
		currentScene->OnDestroy();
		delete currentScene;
		currentScene = nullptr;	
	}

	switch (scene) {
	case SCENE_NUMBER::SCENE0p:
		currentScene = new Scene0p();
		status = currentScene->OnCreate();
		break;
	case SCENE_NUMBER::SCENE1p:
		currentScene = new Scene1p();
		status = currentScene->OnCreate();
		break;
	case SCENE_NUMBER::SCENE2p:
		currentScene = new Scene2p();
		status = currentScene->OnCreate();
		break;
	case SCENE_NUMBER::SCENE3p:
		currentScene = new Scene3p();
		status = currentScene->OnCreate();
		break;
	case SCENE_NUMBER::SCENE4p:
		currentScene = new Scene4p();
		status = currentScene->OnCreate();
		break;

	default:
		Debug::Error("Incorrect scene number assigned in the manager", __FILE__, __LINE__);
		currentScene = nullptr;
		return false;
	}
	return true;
}


