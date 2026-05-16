/*!
All content © 2023 DigiPen Institute of Technology Singapore, all rights reserved.
@file       EndScene.cpp
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

#include "EndScene.h"

#include "WindowsManager.h"


void EndScene::Init()
{
	//Reset camera zoom
	GraphicsSystem::GetInstance()->mCamera.mCurrCamInfo.mZoom = 1.f;
	GraphicsSystem::GetInstance()->mCamera.UpdateCameraInfo();
}

void EndScene::Load()
{
	LevelLoader::LoadLevel(SceneManager::DemangleSceneName(typeid(EndScene).name()), GraphicsSystem::GetInstance()->mBackgroundColor);
}

void EndScene::Input(double _dt)
{
	UNREFERENCED_PARAMETER(_dt);
}

void EndScene::Update(double _dt)
{
	UNREFERENCED_PARAMETER(_dt);
}

void EndScene::Pause(double _dt)
{
	UNREFERENCED_PARAMETER(_dt);
}

void EndScene::Render(double _dt)
{
	UNREFERENCED_PARAMETER(_dt);

}

void EndScene::Free()
{

}

void EndScene::Exit()
{
	SoundManager::GetInstance()->StopChannel(SoundManager::SoundType_SFX);
	SoundManager::GetInstance()->StopChannel(SoundManager::SoundType_BGM);
}