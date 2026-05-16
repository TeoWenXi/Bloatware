/*!
All content © 2023 DigiPen Institute of Technology Singapore, all rights reserved.
@file       MainMenuScene.cpp
@author     TAN Kang Zheng (100%)
@co-author

----------------------------------------------------------------------------

@course     CSD2401F23
@section    CSD2401F23-A
@project    CSD2401F23 Software Engineering Project 3

@brief      Main Menu Scene for the game
*//*______________________________________________________________________*/

#define UNREFERENCED_PARAMETER(P) (P)

#include "CoreEngine.h"

#include "MainMenuScene.h"

#include "WindowsManager.h"


void MainMenuScene::Init()
{
#if _BLOATWARE
	//Game start
	CoreEngine::GetInstance()->mGameState = CoreEngine::GameState::GS_PLAY;
	EventManager::GetInstance()->AddEvent(EventPlayPause(EventPlayPause::PLAY));
	GraphicsSystem::GetInstance()->mCamera.UpdateCameraInfo();
#endif

	//Reset camera zoom
	GraphicsSystem::GetInstance()->mCamera.mCurrCamInfo.mZoom = 1.f;
	GraphicsSystem::GetInstance()->mCamera.UpdateCameraInfo();
}

void MainMenuScene::Load()
{
	LevelLoader::LoadLevel(SceneManager::DemangleSceneName(typeid(MainMenuScene).name()), GraphicsSystem::GetInstance()->mBackgroundColor);
}

void MainMenuScene::Input(double _dt)
{
	UNREFERENCED_PARAMETER(_dt);
}

void MainMenuScene::Update(double _dt)
{
	UNREFERENCED_PARAMETER(_dt);
}

void MainMenuScene::Pause(double _dt)
{
	UNREFERENCED_PARAMETER(_dt);
}

void MainMenuScene::Render(double _dt)
{
	UNREFERENCED_PARAMETER(_dt);

}

void MainMenuScene::Free()
{

}

void MainMenuScene::Exit()
{
	SoundManager::GetInstance()->StopChannel(SoundManager::SoundType_SFX);
	SoundManager::GetInstance()->StopChannel(SoundManager::SoundType_BGM);
}