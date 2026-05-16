/*!
All content © 2023 DigiPen Institute of Technology Singapore, all rights reserved.
@file       main.cpp
@author     TEO Wen Xi (100%)
@co-author

----------------------------------------------------------------------------

@course     CSD2401F23
@section    CSD2401F23-A
@project    CSD2401F23 Software Engineering Project 3

@brief      File containing the 'main' functon for running the program
*//*______________________________________________________________________*/

//Core Engine
#include "CoreEngine.h"

//All managers
#include "AssetManager.h"
#include "JSONManager.h"
#include "WindowsManager.h"
#include "LibraryManager.h"
#include "ComponentManager.h"
#include "EntityManager.h"
#include "InputManager.h" 
#include "TextManager.h"
#include "SceneManager.h"
#include "GraphicsSystem.h"
#include "SoundManager.h"
#include "PhysicsSystem.h"
#include "ParticleSystem.h"
#include "EventManager.h"
#include "GameUIManager.h"
#include "MonoManager.h"
#include "AIManager.h"
#include "ButtonSelectManager.h"
#include "LevelManager.h"

#define _CRTDBG_MAP_ALLOC
	#include <stdlib.h>
	#include <crtdbg.h>
#ifdef _DEBUG
	#define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
	#define new DEBUG_NEW
#endif

#include <iostream>

void RegisterSystems();
void SetInitSequence();

#include <windows.h>
int WINAPI WinMain(HINSTANCE thisInst, HINSTANCE prevInst, LPSTR cmdLine, int cmdShow)
{
	UNREFERENCED_PARAMETER(cmdShow);
	UNREFERENCED_PARAMETER(cmdLine);
	UNREFERENCED_PARAMETER(prevInst);
	UNREFERENCED_PARAMETER(thisInst);
	// Enable run-time memory check for debug builds.
#if defined(DEBUG) | defined(_DEBUG)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	_CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_DEBUG);
	//_CrtSetBreakAlloc(18555);
#endif

	//Set system init sequencing
	SetInitSequence();

	//Register Systems
	RegisterSystems();

#if _ENGINE
	//Set First scene (Main Menu for editor)
	SceneManager::GetInstance()->InitFirstScene(SceneManager::SCENE_MAIN_MENU);
#endif // _ENGINE
#if _BLOATWARE
	//Set First scene (Splash Screen for game)
	SceneManager::GetInstance()->InitFirstScene(SceneManager::SCENE_SPLASH_SCREEN);
#endif // _BLOATWARE

	//Init all systems
	CoreEngine::GetInstance()->Init();
	CoreEngine::GetInstance()->Load();

	//Main game loop
	CoreEngine::GetInstance()->MainGameLoop();

	//Cleanup & Unload all systems
	CoreEngine::GetInstance()->Unload();
#if _ENGINE
	DebugLogger::GetInstance()->Destroy();//Not part of systems so we destroy manually
#endif // _ENGINE
	return 0;
}

//Set Init sequence for Systems
void SetInitSequence()
{
	//Loading of assets
	CoreEngine::GetInstance()->AddToInitSequence(SoundManager::GetInstance());
	CoreEngine::GetInstance()->AddToInitSequence(AssetManager::GetInstance());
	CoreEngine::GetInstance()->AddToInitSequence(JSONManager::GetInstance());
	//Load mono env & compile c# scripts
	CoreEngine::GetInstance()->AddToInitSequence(MonoManager::GetInstance());

	//Core Engine systems
	CoreEngine::GetInstance()->AddToInitSequence(WindowsManager::GetInstance());
	CoreEngine::GetInstance()->AddToInitSequence(LibraryManager::GetInstance());
	CoreEngine::GetInstance()->AddToInitSequence(EntityManager::GetInstance());
	CoreEngine::GetInstance()->AddToInitSequence(ComponentManager::GetInstance());
	CoreEngine::GetInstance()->AddToInitSequence(InputManager::GetInstance());
	CoreEngine::GetInstance()->AddToInitSequence(SceneManager::GetInstance());
	CoreEngine::GetInstance()->AddToInitSequence(AIManager::GetInstance());
	CoreEngine::GetInstance()->AddToInitSequence(ButtonSelectManager::GetInstance());
	CoreEngine::GetInstance()->AddToInitSequence(LevelManager::GetInstance());

	//Physics + events
	CoreEngine::GetInstance()->AddToInitSequence(PhysicsSystem::GetInstance());
	CoreEngine::GetInstance()->AddToInitSequence(EventManager::GetInstance());

	//Rendering
	CoreEngine::GetInstance()->AddToInitSequence(GraphicsSystem::GetInstance());
	CoreEngine::GetInstance()->AddToInitSequence(TextManager::GetInstance());
	CoreEngine::GetInstance()->AddToInitSequence(GameUIManager::GetInstance());
}

//Register Systems
//Load, Update & Exit will follow this sequence
void RegisterSystems()
{
	//Register all Systems, the sequence matters
	//Loading of assets
	CoreEngine::GetInstance()->RegisterSystem(SoundManager::GetInstance());
	CoreEngine::GetInstance()->RegisterSystem(AssetManager::GetInstance());
	CoreEngine::GetInstance()->RegisterSystem(JSONManager::GetInstance());

	//Core Engine systems
	CoreEngine::GetInstance()->RegisterSystem(WindowsManager::GetInstance());
	CoreEngine::GetInstance()->RegisterSystem(LibraryManager::GetInstance());
	CoreEngine::GetInstance()->RegisterSystem(EntityManager::GetInstance());
	CoreEngine::GetInstance()->RegisterSystem(ComponentManager::GetInstance());
	CoreEngine::GetInstance()->RegisterSystem(InputManager::GetInstance());
	CoreEngine::GetInstance()->RegisterSystem(SceneManager::GetInstance());
	CoreEngine::GetInstance()->RegisterSystem(MonoManager::GetInstance());
	CoreEngine::GetInstance()->RegisterSystem(AIManager::GetInstance());
	CoreEngine::GetInstance()->RegisterSystem(ButtonSelectManager::GetInstance());
	CoreEngine::GetInstance()->RegisterSystem(LevelManager::GetInstance());

	//Physics + events
	CoreEngine::GetInstance()->RegisterSystem(PhysicsSystem::GetInstance());
	CoreEngine::GetInstance()->RegisterSystem(EventManager::GetInstance());

	//Rendering
	CoreEngine::GetInstance()->RegisterSystem(GraphicsSystem::GetInstance());
	CoreEngine::GetInstance()->RegisterSystem(ParticleSystem::GetInstance());
	CoreEngine::GetInstance()->RegisterSystem(TextManager::GetInstance());
	CoreEngine::GetInstance()->RegisterSystem(GameUIManager::GetInstance());
}

int main() {
	return WinMain(GetModuleHandle(NULL), NULL, GetCommandLineA(), SW_SHOWNORMAL);
}
