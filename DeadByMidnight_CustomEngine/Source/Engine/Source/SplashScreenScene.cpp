/*!
All content © 2023 DigiPen Institute of Technology Singapore, all rights reserved.
@file       SplashScreenScene.cpp
@author     TAN Kang Zheng (100%)
@co-author

----------------------------------------------------------------------------

@course     CSD2401F23
@section    CSD2401F23-A
@project    CSD2401F23 Software Engineering Project 3

@brief      Splash Screen Scene for the game
*//*______________________________________________________________________*/

#define UNREFERENCED_PARAMETER(P) (P)

#include "CoreEngine.h"
#include "SplashScreenScene.h"
#include "WindowsManager.h"

float mElapsedTime = 0.0f;
bool mDigipenLogo = false;
bool mGroupLogo = false;
bool mFmodLogo = false;

void SplashScreenScene::Init()
{
	//Reset camera zoom
	CoreEngine::GetInstance()->mGameState = CoreEngine::GameState::GS_PLAY;
	EventManager::GetInstance()->AddEvent(EventPlayPause(EventPlayPause::PLAY));
	GraphicsSystem::GetInstance()->mCamera.mCurrCamInfo.mZoom = 1.f;
	GraphicsSystem::GetInstance()->mCamera.UpdateCameraInfo();
}

void SplashScreenScene::Load()
{
	LevelLoader::LoadLevel(SceneManager::DemangleSceneName(typeid(SplashScreenScene).name()), GraphicsSystem::GetInstance()->mBackgroundColor);
}

void SplashScreenScene::Input(double _dt)
{
	UNREFERENCED_PARAMETER(_dt);
}

void SplashScreenScene::Update(double _dt)
{
	mElapsedTime += (float)_dt;

	//Between 0.5 and 1.5s -> Digipen Logo fade in
	if (mElapsedTime > 0.5 && mElapsedTime <= 1.5)
	{
		for (auto i : EntityManager::GetInstance()->mEntityList)
		{
			if (i.GetName() == "Digipen Logo")
			{
				Renderer* renderer = i.GetComponent<Renderer>();
				if (renderer != nullptr)
					renderer->mAlphaValue += (float)_dt;
			
				return;
			}
		}
	}

	//Between 3.5 and 4.5s -> Digipen Logo fade out
	if (mElapsedTime > 3.5 && mElapsedTime <= 4.5)
	{
		for (auto i : EntityManager::GetInstance()->mEntityList)
		{
			if (i.GetName() == "Digipen Logo")
			{
				Renderer* renderer = i.GetComponent<Renderer>();
				if (renderer != nullptr)
					renderer->mAlphaValue -= (float)_dt;
			
				return;
			}
		}
	}

	//Ensure Digipen logo is fully cleared
	if (mElapsedTime > 4.5 && !mDigipenLogo)
	{
		for (auto i : EntityManager::GetInstance()->mEntityList)
		{
			if (i.GetName() == "Digipen Logo")
			{
				Renderer* renderer = i.GetComponent<Renderer>();
				if (renderer != nullptr)
					renderer->mAlphaValue = 0;

				mDigipenLogo = true;
			}
		}
	}

	//Betweem 5.5 and 6.5s -> Group Logo fade in
	if (mElapsedTime > 5.5 && mElapsedTime <= 6.5)
	{
		for (auto i : EntityManager::GetInstance()->mEntityList)
		{

			if (i.GetName() == "Group Logo")
			{
				Renderer* renderer = i.GetComponent<Renderer>();
				if (renderer != nullptr)
					renderer->mAlphaValue += (float)_dt;
				
				return;
			}
		}
	}

	//Between 8.5 and 9.5s -> Group Logo fade out
	if (mElapsedTime > 8.5 && mElapsedTime <= 9.5)
	{
		for (auto i : EntityManager::GetInstance()->mEntityList)
		{
			if (i.GetName() == "Group Logo")
			{
				Renderer* renderer = i.GetComponent<Renderer>();
				if (renderer != nullptr)
					renderer->mAlphaValue -= (float)_dt;
			}
		}
	}

	//Ensure Grouup logo is fully cleared
	if (mElapsedTime > 9.5 && !mGroupLogo)
	{
		for (auto i : EntityManager::GetInstance()->mEntityList)
		{
			if (i.GetName() == "Group Logo")
			{
				Renderer* renderer = i.GetComponent<Renderer>();
				if (renderer != nullptr)
					renderer->mAlphaValue = 0;

				mGroupLogo = true;
			}
		}
	}


	//Betweem 10.5 and 11.5s -> FMOD Logo fade out
	if (mElapsedTime > 10.5 && mElapsedTime <= 11.5)
	{
		for (auto i : EntityManager::GetInstance()->mEntityList)
		{
			if (i.GetName() == "FMOD Logo")
			{
				Renderer* renderer = i.GetComponent<Renderer>();
				if (renderer != nullptr)
					renderer->mAlphaValue += (float)_dt;

				return;
			}
		}
	}

	//Betweem 13.5 and 14.5s -> FMOD Logo fade out
	if (mElapsedTime > 13.5 && mElapsedTime <= 14.5)
	{
		for (auto i : EntityManager::GetInstance()->mEntityList)
		{
			if (i.GetName() == "FMOD Logo")
			{
				Renderer* renderer = i.GetComponent<Renderer>();
				if (renderer != nullptr)
					renderer->mAlphaValue -= (float)_dt;

				return;
			}
		}
	}

	//Ensure FMOD logo is fully cleared
	if (mElapsedTime > 15.5 && !mFmodLogo)
	{
		for (auto i : EntityManager::GetInstance()->mEntityList)
		{
			if (i.GetName() == "FMOD Logo")
			{
				Renderer* renderer = i.GetComponent<Renderer>();
				if (renderer != nullptr)
					renderer->mAlphaValue = 0;

				mFmodLogo = true;
			}
		}
	}
	
	if (mElapsedTime > 16.5)
	{
		SceneManager::GetInstance()->SetActiveScene(SceneManager::SCENE_MAIN_MENU);
	}
}

void SplashScreenScene::Pause(double _dt)
{
	UNREFERENCED_PARAMETER(_dt);
}

void SplashScreenScene::Render(double _dt)
{
	UNREFERENCED_PARAMETER(_dt);

}

void SplashScreenScene::Free()
{

}

void SplashScreenScene::Exit()
{

}