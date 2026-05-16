/*!
All content © 2023 DigiPen Institute of Technology Singapore, all rights reserved.
@file       SceneManager.cpp
@author     TAN Kang Zheng (100%)
@co-author

----------------------------------------------------------------------------

@course     CSD2401F23
@section    CSD2401F23-A
@project    CSD2401F23 Software Engineering Project 3

@brief      Functions for the SceneManager, for handling the changing of scenes
*//*______________________________________________________________________*/
#include "SceneManager.h"
#include "Scene.h"
#include "WindowsManager.h"
#include "CoreEngine.h"
#include "InputManager.h"
#include "MonoManager.h"

//Include for all scenes
#if _ENGINE
#include "EditorMainScene.h"
#endif // _ENGINE

#include "GameScene.h"
#include "MainMenuScene.h"
#include "OpeningDialogueScene.h"
#include "BossScene.h"
#include "MainMenuTransitionScene.h"
#include "EndScene.h"
#include "EndingDialogueScene.h"
#include "CreditsScene.h"
#include "SplashScreenScene.h"

SceneManager::SceneManager() : mActiveScene(nullptr), mNextScene(nullptr), mEditorScene(nullptr), mQuit(false), mRestartScene(false)
{
}

SceneManager::~SceneManager()
{
}

void SceneManager::InitFirstScene(SCENES firstScene)
{
	mActiveScene = mNextScene = SetActiveScene(firstScene);
}

void SceneManager::Init()
{
#if _ENGINE
	mEditorScene = new EditorMainScene();
	mEditorScene->Load();
	mEditorScene->Init();
#endif // _ENGINE

}

void SceneManager::Load()
{
	mActiveScene->Load();
	mActiveScene->Init();
}

void SceneManager::Update()
{
	//Update editor scene
	double dt = WindowsManager::GetInstance()->mDeltaTime;

#if _ENGINE
	mEditorScene->Input(dt);
	mEditorScene->Update(dt);
#endif // _ENGINE

	//SCENE TRANSITION CODE
	if (isChangingScene)
	{
		mChangeSceneDelayTimer -= (float)dt;
		if (mChangeSceneDelayTimer < FLT_EPSILON)
		{
			isChangingScene = false;
			SetActiveScene(SceneManager::GetInstance()->GetSceneFromName(mChangeSceneName));
		}
	}

	// Check for change of scene
	if (mNextScene != mActiveScene)
	{
		if (mActiveScene)
		{
			// Scene is valid, need to call appropriate function to exit
			mActiveScene->Free();
			mActiveScene->Exit();
			delete mActiveScene;
		}

		mActiveScene = mNextScene;
		mActiveScene->Load();
		mActiveScene->Init();
		EntityManager::GetInstance()->BackupCurrScene();
	}

	//Check for scene restart
	if (mRestartScene)
	{
		SoundManager::GetInstance()->ResetPlayOnceComponent();
		GameUIManager::GetInstance()->ClearAllUI();
		mActiveScene->Free();
		mActiveScene->Load();
		mActiveScene->Init();
		EntityManager::GetInstance()->BackupCurrScene();
		mRestartScene = false;
	}

	if (mActiveScene && CoreEngine::GetInstance()->mGameState == CoreEngine::GameState::GS_PLAY)
	{
		mActiveScene->Input(dt);
		mActiveScene->Update(dt);
	}
	else if (mActiveScene && CoreEngine::GetInstance()->mGameState == CoreEngine::GameState::GS_PAUSE)
	{
		mActiveScene->Input(dt);
		mActiveScene->Pause(dt);
	}
}

void SceneManager::Render()
{
#if _ENGINE
	mEditorScene->Render(WindowsManager::GetInstance()->mDeltaTime);
#endif // _ENGINE
	if (mActiveScene)
		mActiveScene->Render(WindowsManager::GetInstance()->mDeltaTime);
}

void SceneManager::Unload()
{
	// Delete all scenes stored and empty the entire map
	mActiveScene->Free();
	mActiveScene->Exit();
	delete mActiveScene;
#if _ENGINE
	mEditorScene->Free();
	mEditorScene->Exit();
	delete mEditorScene;
#endif // _ENGINE

	if (SoundManager::GetInstance())
		SoundManager::GetInstance()->Unload();

	//Destroy instance
	Destroy();
}

void SceneManager::Quit()
{
	mQuit = true;
}

Scene* SceneManager::SetActiveScene(SCENES scene_mNextScene)
{
	switch (scene_mNextScene)
	{
#if _ENGINE
	case SCENE_EDITOR_MAIN:
		this->mNextScene = new EditorMainScene();
		mActiveSceneName = DemangleSceneName(typeid(EditorMainScene).name());
		break;
#endif // _ENGINE
	case SCENE_MAIN_MENU:
		this->mNextScene = new MainMenuScene();
		mActiveSceneName = DemangleSceneName(typeid(MainMenuScene).name());
		break;
	case SCENE_GAME:
		this->mNextScene = new GameScene();
		mActiveSceneName = DemangleSceneName(typeid(GameScene).name());
		break;
	case SCENE_OPENING_DIALOGUE:
		this->mNextScene = new OpeningDialogueScene();
		mActiveSceneName = DemangleSceneName(typeid(OpeningDialogueScene).name());
		break;
	case SCENE_BOSS:
		this->mNextScene = new BossScene();
		mActiveSceneName = DemangleSceneName(typeid(BossScene).name());
		break;
	case SCENE_MAIN_MENU_TRANSITION:
		this->mNextScene = new MainMenuTransitionScene();
		mActiveSceneName = DemangleSceneName(typeid(MainMenuTransitionScene).name());
		break;
	case SCENE_END:
		this->mNextScene = new EndScene();
		mActiveSceneName = DemangleSceneName(typeid(EndScene).name());
		break;
	case SCENE_ENDING_DIALOGUE:
		this->mNextScene = new EndingDialogueScene();
		mActiveSceneName = DemangleSceneName(typeid(EndingDialogueScene).name());
		break;
	case SCENE_CREDITS:
		this->mNextScene = new CreditsScene();
		mActiveSceneName = DemangleSceneName(typeid(CreditsScene).name());
		break;
	case SCENE_SPLASH_SCREEN:
		this->mNextScene = new SplashScreenScene();
		mActiveSceneName = DemangleSceneName(typeid(SplashScreenScene).name());
	default:
		break;
	}

	WindowsManager::GetInstance()->mSceneOpacity = 1.0f;
	TextManager::GetInstance()->SetOpeningDialogueText("", "IosevkaNerdFont-Bold");

	return this->mNextScene;
}

SceneManager::operator bool() const
{
	return mQuit;
}

SceneManager::SCENES SceneManager::GetSceneFromName(const std::string& sceneName)
{
#if _ENGINE
	if (sceneName == DemangleSceneName(typeid(EditorMainScene).name()))
		return SCENE_EDITOR_MAIN;
#endif // _ENGINE
	if (sceneName == DemangleSceneName(typeid(MainMenuScene).name()))
		return SCENE_MAIN_MENU;
	else if (sceneName == DemangleSceneName(typeid(GameScene).name()))
		return SCENE_GAME;
	else if (sceneName == DemangleSceneName(typeid(OpeningDialogueScene).name()))
		return SCENE_OPENING_DIALOGUE;
	else if (sceneName == DemangleSceneName(typeid(BossScene).name()))
		return SCENE_BOSS;
	else if (sceneName == DemangleSceneName(typeid(MainMenuTransitionScene).name()))
		return SCENE_MAIN_MENU_TRANSITION;
	else if (sceneName == DemangleSceneName(typeid(EndScene).name()))
		return SCENE_END;
	else if (sceneName == DemangleSceneName(typeid(EndingDialogueScene).name()))
		return SCENE_ENDING_DIALOGUE;
	else if (sceneName == DemangleSceneName(typeid(CreditsScene).name()))
		return SCENE_CREDITS;
	else if (sceneName == DemangleSceneName(typeid(SplashScreenScene).name()))
		return SCENE_SPLASH_SCREEN;

	return SCENES::SCENE_COUNT;
}

std::string SceneManager::DemangleSceneName(const std::string& inputStr)
{
	size_t lastSpaceIndex = inputStr.find_last_of(" ");
	return inputStr.substr(lastSpaceIndex + 1, inputStr.length());
}

void SceneManager::SetSceneToRestart(bool _restart)
{
	mRestartScene = _restart;
}

void SceneManager::SetSceneToTransitionTo(std::string _sceneName)
{
	if (isChangingScene) return;

	isChangingScene = true;
	mChangeSceneDelayTimer = mChangeSceneDelayTime * 1.4f;
	mChangeSceneName = _sceneName;
	WindowsManager::GetInstance()->mSceneOpacity = 0.0f;
}

Scene* SceneManager::GetActiveScene()
{
	return mActiveScene;
}