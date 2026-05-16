/*!
All content © 2023 DigiPen Institute of Technology Singapore, all rights reserved.
@file       MainMenuTransition.cpp
@author		TAN Kang Zheng (100%)

----------------------------------------------------------------------------

@course     CSD2401F23
@section    CSD2401F23-A
@project    CSD2401F23 Software Engineering Project 3

@brief      MainMenu Transition Scene for the game
*//*______________________________________________________________________*/

#include "CoreEngine.h"

#include "MainMenuTransitionScene.h"

#include "WindowsManager.h"

#define TRANSITION_TIME 0.7f //Time before moving to game scene

void MainMenuTransitionScene::Init()
{
#if _BLOATWARE
	//Game start
	CoreEngine::GetInstance()->mGameState = CoreEngine::GameState::GS_PLAY;
	EventManager::GetInstance()->AddEvent(EventPlayPause(EventPlayPause::PLAY));
	GraphicsSystem::GetInstance()->mCamera.UpdateCameraInfo();
#endif
	elapsedTime = 0.f;
	std::vector<Entity> ptrEntityList = EntityManager::GetInstance()->mEntityList;
	for (auto i : ptrEntityList)
	{
		entityList.push_back(i.GetID());
	}

	WindowsManager::GetInstance()->mSceneOpacity = 0.0f;
}

void MainMenuTransitionScene::Load()
{
	LevelLoader::LoadLevel(SceneManager::DemangleSceneName(typeid(MainMenuTransitionScene).name()), GraphicsSystem::GetInstance()->mBackgroundColor);
}

void MainMenuTransitionScene::Input(double _dt)
{
	UNREFERENCED_PARAMETER(_dt);
}

void MainMenuTransitionScene::Update(double _dt)
{
	elapsedTime += (float)_dt;
	if (elapsedTime >= TRANSITION_TIME)
	{
		SceneManager::GetInstance()->SetActiveScene(SceneManager::SCENE_OPENING_DIALOGUE);
	}
	else
	{
		//for (auto i : entityList)
		//{
		//	Transform* transform = EntityManager::GetInstance()->GetEntity(i)->GetComponent<Transform>();
		//	if (transform != nullptr)
		//		transform->SetLocalPosition(transform->GetLocalPosition() + Vector3{ 0, -1.0f, 0 });
		//}
	}
}

void MainMenuTransitionScene::Pause(double _dt)
{
	UNREFERENCED_PARAMETER(_dt);
}

void MainMenuTransitionScene::Render(double _dt)
{
	UNREFERENCED_PARAMETER(_dt);

}

void MainMenuTransitionScene::Free()
{

}

void MainMenuTransitionScene::Exit()
{

}