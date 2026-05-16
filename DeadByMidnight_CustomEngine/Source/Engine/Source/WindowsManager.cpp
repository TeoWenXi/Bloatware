/*!
All content © 2023 DigiPen Institute of Technology Singapore, all rights reserved.
@file       WindowsManager.cpp
@author     TEO Wen Xi (100%)
@co-author

----------------------------------------------------------------------------

@course     CSD2401F23
@section    CSD2401F23-A
@project    CSD2401F23 Software Engineering Project 3

@brief      Manages all windows in the program
*//*______________________________________________________________________*/

#define UNREFERENCED_PARAMETER(P) (P)

#include "WindowsManager.h"
#include "GraphicsSystem.h"
#include "JSONManager.h"
#include "InputManager.h"
#include "LevelLoader.h"
#include "Scene.h"
#include "SceneManager.h"

//Scenes
#include "MainMenuScene.h"
#include "EditorMainScene.h"
#include "GameScene.h"
#include "BossScene.h"

#include <iostream>
GLint WindowsManager::mWidth;
GLint WindowsManager::mHeight;
WindowsManager::WindowsManager()
	: mPtrWindow(nullptr)
	, mTitle("")
	, mDeltaTime(0)
	, mFPSOfUpdate(0)
	, mElapsedTime(0)
	, mPrevTime(0)
	, mGameLoopCount(0)
	, mStartTime(0)
	, mFPSUpdateInterval(0)
	, mDTThreshold(0.1)
{
}

void WindowsManager::Init()
{
	//GLFW Init
	if (!glfwInit()) {
#if _ENGINE
		DebugLogger::GetInstance()->Log("GLFW init has failed - abort program!!!\n");
#endif // _ENGINE
		return;
	}

	//Specify OpenGL version 4.5
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);

	//GLFW Callbacks
	glfwSetErrorCallback(ErrorCallback);
	//Variable inits
	mGameLoopCount = 0;
	mPrevTime = mStartTime = glfwGetTime();

	JSONManager::GetInstance()->LoadFile("config");

	//Create the window
	GLFWmonitor* monitor = glfwGetPrimaryMonitor();
	const GLFWvidmode* mode = glfwGetVideoMode(monitor);
	CreateWindow(mode->width, mode->height, "Game Engine", 1.0);

#if _BLOATWARE
	glfwSetWindowTitle(mPtrWindow, "Bloatware");
#endif // _BLOATWARE

	//Callback for focus -- needs to be called after window is created
	glfwSetWindowFocusCallback(mPtrWindow, FocusCallback);
}

void WindowsManager::Load()
{

}

void WindowsManager::Update()
{
	//Update time on all windows
	UpdateTime();

	//Poll events
	glfwPollEvents();

	//Switch of window modes
	if ((InputManager::GetInstance()->GetKey(KeyCode::LEFT_ALT) || InputManager::GetInstance()->GetKey(KeyCode::RIGHT_ALT)) &&
		InputManager::GetInstance()->GetKey(KeyCode::ENTER))
	{
		if (mWindowSwapped == false)
		{
			int nextMode = (int)mWindowMode + 1;
			if (nextMode >= WINDOW_MODE::WM_COUNT)
				nextMode = 0;

			mWindowMode = WINDOW_MODE(nextMode);
			SwitchWindowMode(mWindowMode);
			mWindowSwapped = true;
		}
	}
	else
		mWindowSwapped = false;

	if (fabsf(mSceneTrueOpacity - mSceneOpacity) > (1.0f / 0.6f) * (float)mDeltaTime)
	{
		mSceneTrueOpacity += ((mSceneOpacity - mSceneTrueOpacity) / fabsf(mSceneOpacity - mSceneTrueOpacity)) * (1.0f / 0.6f) * (float)mDeltaTime;
	}
	else
	{
		mSceneTrueOpacity = mSceneOpacity;
	}
	mDeathVignette = std::clamp(mDeathVignette - (1.0f / 0.2f) * (float)mDeltaTime, 0.0f, 1.0f);
}

void WindowsManager::Unload()
{
	//Destroy window
	glfwDestroyWindow(mPtrWindow);

	//Destroy instance
	Destroy();
}

//Window creation
bool WindowsManager::CreateWindow(GLint _width, GLint _height, std::string _winTitle, GLdouble _fpsUpdateInterval)
{
	//Set variables
	mWidth = _width;
	mHeight = _height;
	mTitle = _winTitle;
	mFPSUpdateInterval = _fpsUpdateInterval;

	// Change second last parameter to NULL to not start in full screen or glfwGetPrimaryMonitor() to start in full screen
	mPtrWindow = glfwCreateWindow(_width, _height, _winTitle.c_str(), glfwGetPrimaryMonitor(), NULL);
	if (!mPtrWindow)
	{
		// Window or OpenGL context creation failed
#if _ENGINE
		DebugLogger::GetInstance()->Log("Failed to create GLFW window\n");
#endif // _ENGINE
		glfwTerminate();
		return false;
	}

	// For the icon in the top left of the window
	GLFWimage icon[1];
	icon[0].pixels = stbi_load(".\\Assets\\Textures\\Character_PixelSlimeFull.png", &icon[0].width, &icon[0].height, 0, 4); //rgba channels 
	glfwSetWindowIcon(mPtrWindow, 1, icon);
	stbi_image_free(icon[0].pixels);

	glfwMakeContextCurrent(mPtrWindow);

	//Set window callbacks
	glfwSetFramebufferSizeCallback(mPtrWindow, FrameBufferSizeCallback);

	glfwSwapInterval(1); //Enable vsync

	return true;
}

//Updating Delta time
void WindowsManager::UpdateTime()
{
	//Elapsed time (Time between prev frame and current frame)
	double currTime = glfwGetTime();
	mDeltaTime = currTime - mPrevTime;
	if (mDeltaTime > mDTThreshold)
		mDeltaTime = mDTThreshold;

	mPrevTime = currTime;

	//FPS
	mGameLoopCount++;
	double elapsedTime = currTime - mStartTime;

	//Update FPS based on update interval
	if (elapsedTime >= mFPSUpdateInterval)
	{
		mFPSOfUpdate = mGameLoopCount / elapsedTime;
		mStartTime = currTime;
		mGameLoopCount = 0;
	}
}

//Callback functions
void WindowsManager::ErrorCallback(int _error, const char* _description)
{
	UNREFERENCED_PARAMETER(_error);

#ifdef _ENGINE
	DebugLogger::GetInstance()->Log("GLFW Error: %s\n", _description);
#else
	UNREFERENCED_PARAMETER(_description);
#endif

}

void WindowsManager::FrameBufferSizeCallback(GLFWwindow* _winPtr, GLint _width, GLint _height)
{
	UNREFERENCED_PARAMETER(_winPtr);

	//Set viewport as entire new window size
	glViewport(0, 0, _width, _height);

	mWidth = _width;
	mHeight = _height;

	GraphicsSystem::GetInstance()->RescaleFrameBuffer(_width, _height);
	InputManager::GetInstance()->UpdateRegionMax(static_cast<float>(mWidth), static_cast<float>(mHeight));
}

void WindowsManager::FocusCallback(GLFWwindow* _window, int _focused)
{
	UNREFERENCED_PARAMETER(_window);

	if (_focused)
	{
#if _BLOATWARE
		SoundManager::GetInstance()->SetBGMPaused(false);
		SoundManager::GetInstance()->SetSFXPaused(false);

		bool sceneChecker = false;
		//Check if game scene is active
		GameScene* gameSceneCheck = dynamic_cast<GameScene*>(SceneManager::GetInstance()->GetActiveScene());
		if (gameSceneCheck != nullptr)
			sceneChecker = true;

		BossScene* bossSceneCheck = dynamic_cast<BossScene*>(SceneManager::GetInstance()->GetActiveScene());
		if (bossSceneCheck != nullptr)
			sceneChecker = true;

		//One of the two player scenes is active
		if (sceneChecker != true)
		{
			CoreEngine::GetInstance()->mGameState = CoreEngine::GameState::GS_PLAY;
		}
#endif // _BLOATWARE

	}
	else
	{
		//Pause the engine
		if (CoreEngine::GetInstance()->mGameState == CoreEngine::GameState::GS_PLAY)
		{
#if _ENGINE
			CoreEngine::GetInstance()->mGameState = CoreEngine::GameState::GS_PAUSE;
			EventManager::GetInstance()->AddEvent(EventPlayPause(EventPlayPause::PAUSE));
			dynamic_cast<EditorMainScene*>(SceneManager::GetInstance()->mEditorScene)->mIsEditorPlaying = false;
#elif _BLOATWARE
				bool sceneChecker = false;
				//Check if game scene is active
				GameScene* gameSceneCheck = dynamic_cast<GameScene*>(SceneManager::GetInstance()->GetActiveScene());
				if (gameSceneCheck != nullptr)
					sceneChecker = true;

				BossScene* bossSceneCheck = dynamic_cast<BossScene*>(SceneManager::GetInstance()->GetActiveScene());
				if (bossSceneCheck != nullptr)
					sceneChecker = true;

				//One of the two player scenes is active
				if (sceneChecker == true)
				{
					LevelLoader::LoadPrefab("PauseMenu Prefab");
				}

				SoundManager::GetInstance()->SetBGMPaused(true);
				SoundManager::GetInstance()->SetSFXPaused(true);
				EventManager::GetInstance()->AddEvent(EventPlayPause(EventPlayPause::PAUSE));
				CoreEngine::GetInstance()->mGameState = CoreEngine::GameState::GS_PAUSE;
#endif
		}
#ifdef _BLOATWARE
		//Minimize -- uncomment when needed if not very annoying
		glfwIconifyWindow(_window);
#endif // BLOATWARE
	}
}

void WindowsManager::SwitchWindowMode(WINDOW_MODE _nextMode)
{
	switch (_nextMode)
	{
	case WM_FULLSCREEN:
		{
			GLFWmonitor* monitor = glfwGetPrimaryMonitor();
			const GLFWvidmode* mode = glfwGetVideoMode(monitor);
			glfwSetWindowAttrib(mPtrWindow, GLFW_RESIZABLE, GLFW_TRUE);
			glfwSetWindowAttrib(mPtrWindow, GLFW_DECORATED, GLFW_TRUE);
			glfwSetWindowMonitor(mPtrWindow, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
		}
		break;
	//case WM_BORDERLESS:
	//	{
	//		GLFWmonitor* monitor = glfwGetPrimaryMonitor();
	//		const GLFWvidmode* mode = glfwGetVideoMode(monitor);
	//		glfwMaximizeWindow(mPtrWindow);
	//		glfwSetWindowAttrib(mPtrWindow, GLFW_RESIZABLE, GLFW_FALSE);
	//		glfwSetWindowAttrib(mPtrWindow, GLFW_DECORATED, GLFW_FALSE);
	//		glfwSetWindowMonitor(mPtrWindow, NULL, 0, 0, mode->width, mode->height, mode->refreshRate);
	//	}
	//	break;
	case WM_WINDOWED:
		{
			GLFWmonitor* monitor = glfwGetPrimaryMonitor();
			const GLFWvidmode* mode = glfwGetVideoMode(monitor);
#if !_BLOATWARE
			glfwSetWindowAttrib(mPtrWindow, GLFW_RESIZABLE, GLFW_TRUE);
#else
			glfwSetWindowAttrib(mPtrWindow, GLFW_RESIZABLE, GLFW_FALSE);
#endif // _BLOATWARE

			glfwSetWindowAttrib(mPtrWindow, GLFW_DECORATED, GLFW_TRUE);
			JSONManager::GetInstance()->LoadFile("config");

			//Center window on monitor
			int windowWidth = JSONManager::GetInstance()->GetInt("Width");
			int windowHeight = JSONManager::GetInstance()->GetInt("Height");
			int xPos = (int)(mode->width / 2.f - windowWidth / 2.f);
			int yPos = (int)(mode->height / 2.f - windowHeight / 2.f);

			glfwSetWindowMonitor(mPtrWindow, NULL, xPos, yPos, windowWidth, windowHeight, mode->refreshRate);
		}
		break;
	}
}

void WindowsManager::ChangeWindowSize(GLint _width, GLint _height)
{
	if (mPtrWindow == nullptr)
	{
#if _ENGINE
		DebugLogger::GetInstance()->Log("WindowsManager Error: ChangeWindowSize called with no current window.\n");
#endif // _ENGINE

		return;
	}
	glfwSetWindowSize(mPtrWindow, _width, _height);
	GraphicsSystem::GetInstance()->RescaleFrameBuffer(_width, _height);
}
