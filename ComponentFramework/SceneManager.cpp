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
#include "Scene1p.h"			// physics Part 1 ball rolling
#include "Scene2p.h"			// physics Part 2 ball rolling
#include "Scene3p.h"			// physics Part 3 ball rolling

/// <HANDLE EVENTS  CHANGED>
///		NOTE:: CHANGED THE SCENES FROM F5 TO NUMERICAL 5
/// 
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
/// Change which scene to init
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
	BuildNewScene(SCENE_NUMBER::SCENE1p);
	//BuildNewScene(SCENE_NUMBER::SCENE2p);
	/********************************************************************************/
	return true;
}

/// This is the whole game
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
					"List Of Scenes::\n"
					"PHYSICS\n"																			
					" Scene 0p (1) - Physics Baseline\n"
					" Scene 1p (2) - Ball Rolling\n"
					" Scene 2p (3) - Ball & Plane \n"
					//" Scene 3p (4) - Skull Texture Model Matrix PT0\n"
					//" Scene 5g (5) - Graphics - Skybox TEST\n"
					//" Scene 6g (6) - SkyBox Fresnal - Assignemnt (finished)\n"
					//" Scene 7g (7) - SkyBox water pt 0\n"
				);
				break;
			case SDL_SCANCODE_0:
				printf("Scene 0p - Graphics Phong Lighting Baseline\n");
				BuildNewScene(SCENE_NUMBER::SCENE0p);
				break;
			case SDL_SCANCODE_1:
				printf("Scene 1p - Ball Rolling\n");
				BuildNewScene(SCENE_NUMBER::SCENE1p);
				break;
			case SDL_SCANCODE_2:	// phong lighting refined pt 0
				printf("Scene 2p - Ball & Plane\n");
				BuildNewScene(SCENE_NUMBER::SCENE2p);
				break;
		//	case SDL_SCANCODE_4:	// phong lighting refined pt 1
		//		printf("Scene 4g - Skull Texture Model Matrix PT0\n");
		//		BuildNewScene(SCENE_NUMBER::SCENE4g);
		//		break;
		//	case SDL_SCANCODE_5:	// Skybox fresnal pt 0
		//		printf("Scene 5g - Graphics - Skybox TEST\n");
		//		BuildNewScene(SCENE_NUMBER::SCENE5g);
		//		break;
		//	case SDL_SCANCODE_6:	// Skybox Fresnal pt 1 - Assignment (finished)
		//		printf("Scene 6g - SkyBox Fresnal - Assignemnt (finished)\n");
		//		BuildNewScene(SCENE_NUMBER::SCENE6g);
		//		break;
		//case SDL_SCANCODE_7:		// skybox Water Pt 0
		//		printf("Scene 7g - SkyBox water pt 0\n");
		//		BuildNewScene(SCENE_NUMBER::SCENE7g);
		//		break;	
		//	case SDL_SCANCODE_8:	// empty
		//		printf("Scene 8g - Tesselation\n");
		//		BuildNewScene(SCENE_NUMBER::SCENE8g);
		//		break;	
		//	case SDL_SCANCODE_9:
		//		printf("scancode 9\n");
		//		break;

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
	case SCENE_NUMBER::SCENE1p:						// Phong Lighting
		currentScene = new Scene1p();
		status = currentScene->OnCreate();
		break;
	case SCENE_NUMBER::SCENE2p:						// Parenting / Matrix
		currentScene = new Scene2p();
		status = currentScene->OnCreate();
		break;
	//case SCENE_NUMBER::SCENE4g:						// Parenting / Matrix Model
	//	currentScene = new Scene4g();
	//	status = currentScene->OnCreate();
	//	break;	
	//case SCENE_NUMBER::SCENE5g:						// Parenting / Matrix Model
	//	currentScene = new Scene5g();
	//	status = currentScene->OnCreate();
	//	break;
	//case SCENE_NUMBER::SCENE6g:
	//	currentScene = new Scene6g();
	//	status = currentScene->OnCreate();
	//	break;
	//case SCENE_NUMBER::SCENE7g:
	//	currentScene = new Scene7g();
	//	status = currentScene->OnCreate();
	//	break;
	//case SCENE_NUMBER::SCENE8g:
	//	currentScene = new Scene8g();
	//	status = currentScene->OnCreate();
	//	break;

	default:
		Debug::Error("Incorrect scene number assigned in the manager", __FILE__, __LINE__);
		currentScene = nullptr;
		return false;
	}
	return true;
}


