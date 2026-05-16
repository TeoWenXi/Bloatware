/*!
All content © 2023 DigiPen Institute of Technology Singapore, all rights reserved.
@file       BossScene.cpp
@author		TAN Kang Zheng (100%)

----------------------------------------------------------------------------

@course     CSD2401F23
@section    CSD2401F23-A
@project    CSD2401F23 Software Engineering Project 3

@brief      Boss Scene for the game
*//*______________________________________________________________________*/

#include "CoreEngine.h"

#include "BossScene.h"

#include "WindowsManager.h"
#include "TextManager.h"

#include "InputManager.h"
#include "SoundManager.h"
#include "EntityManager.h"
#include "GraphicsSystem.h"
#include "ComponentManager.h"
#include "ParticleSystem.h"

void BossScene::Init()
{
#if _BLOATWARE
	//Game start
	CoreEngine::GetInstance()->mGameState = CoreEngine::GameState::GS_PLAY;
	EventManager::GetInstance()->AddEvent(EventPlayPause(EventPlayPause::PLAY));
	GraphicsSystem::GetInstance()->mCamera.UpdateCameraInfo();
#endif

	//Reset cutscene timer if any
	Entity* player = EntityManager::GetInstance()->GetEntity(Player::mPlayerID);
	if (player != nullptr)
		player->GetComponent<Player>()->ResetCutsceneTimers();
}

void BossScene::Load()
{
	LevelLoader::LoadLevel(SceneManager::DemangleSceneName(typeid(BossScene).name()), GraphicsSystem::GetInstance()->mBackgroundColor);
	TileComponent::LoadTileData();

	//Reset current cam waypoint
	mCamWaypointIndex = -1;

	//Set camera waypoint to starting waypoint
	MoveNextCamWaypoint();

	//Set player to starting point
	Player* player = EntityManager::GetInstance()->GetEntity(Player::mPlayerID)->GetComponent<Player>();
	if (player != nullptr)
		EntityManager::GetInstance()->GetEntity(Player::mPlayerID)->GetComponent<Transform>()->SetPosition(mCamWaypoints[mCamWaypointIndex].mPlayerSpawnPos);

	//Update camera zoom
	GraphicsSystem::GetInstance()->mCamera.UpdateCameraInfo();

	//Move parallax backgrounds
	if (ComponentManager::GetInstance()->GetComponentArray<CSScriptComponent>() == nullptr)
		return;

	auto& csScriptArr = ComponentManager::GetInstance()->GetComponentArray<CSScriptComponent>()->mComponentArray;
	if (csScriptArr.size() < 1)
		return;

	for (auto& obj : csScriptArr)
	{
		std::string entName = EntityManager::GetInstance()->GetName(obj.first);

		if (entName.find("Parallax") != std::string::npos)
			EntityManager::GetInstance()->GetEntity(obj.first)->GetComponent<Transform>()->SetPosition(mCamWaypoints[mCamWaypointIndex].mPosition);
	}
}

void BossScene::Input(double _dt)
{
	UNREFERENCED_PARAMETER(_dt);
	if (InputManager::GetInstance()->GetActionTriggered("MenuPause") && (CoreEngine::GetInstance()->mGameState == CoreEngine::GameState::GS_PLAY))
	{
		CoreEngine::GetInstance()->mGameState = CoreEngine::GameState::GS_PAUSE;
		EventManager::GetInstance()->AddEvent(EventPlayPause(EventPlayPause::PAUSE));
		LevelLoader::LoadPrefab("PauseMenu Prefab");
	}

	//Cheat code to skip to last 2nd stage
	if (InputManager::GetInstance()->GetActionTriggered("CheatSkip")
		&& mCamWaypointIndex < ((int)mCamWaypoints.size() - 1)
		&& EntityManager::GetInstance()->GetEntity(Player::mPlayerID)->GetComponent<Player>()->mCutsceneTimer <= 0)
	{
		//Move camera
		GraphicsSystem::GetInstance()->mCamera.mCurrCamInfo = mCamWaypoints[mCamWaypoints.size() - 1];
		mCamWaypointIndex = (int)mCamWaypoints.size() - 1;

		//Move player
		EntityManager::GetInstance()->GetEntity(Player::mPlayerID)->GetComponent<Player>()->Kill();

		//Move parallax backgrounds
		if (ComponentManager::GetInstance()->GetComponentArray<CSScriptComponent>() == nullptr)
			return;

		auto& csScriptArr = ComponentManager::GetInstance()->GetComponentArray<CSScriptComponent>()->mComponentArray;
		if (csScriptArr.size() < 1)
			return;

		for (auto& obj : csScriptArr)
		{
			std::string entName = EntityManager::GetInstance()->GetName(obj.first);

			if (entName.find("Parallax") != std::string::npos)
				EntityManager::GetInstance()->GetEntity(obj.first)->GetComponent<Transform>()->SetPosition(mCamWaypoints[mCamWaypoints.size() - 1].mPosition);
		}
	}
}

void BossScene::Update(double _dt)
{
	UNREFERENCED_PARAMETER(_dt);
	//Update player if it exists
	Entity* player = EntityManager::GetInstance()->GetEntity(Player::mPlayerID);
	if (player != nullptr)
	{
		Player* playerComponent = player->GetComponent<Player>();
		if (playerComponent != nullptr)
			playerComponent->Update();
	}

	//Update keys
	for (size_t i = 0; i < Key::mKeyIDs.size(); ++i)
	{
		Entity* key = EntityManager::GetInstance()->GetEntity(Key::mKeyIDs[i]);
		if (key != nullptr)
		{
			Key* keyComponent = key->GetComponent<Key>();
			if (keyComponent != nullptr)
				keyComponent->Update();
		}
	}

	//Update walls
	for (size_t i = 0; i < KeyWall::mKeyWallIDs.size(); ++i)
	{
		Entity* keyWall = EntityManager::GetInstance()->GetEntity(KeyWall::mKeyWallIDs[i]);
		if (keyWall != nullptr)
		{
			KeyWall* keyWallComponent = keyWall->GetComponent<KeyWall>();
			if (keyWallComponent != nullptr)
				keyWallComponent->Update();
		}
	}

	//Update all VFXs
	for (size_t i = 0; i < VFXEntity::mVFXIDs.size(); ++i)
	{
		Entity* vfxEnt = EntityManager::GetInstance()->GetEntity(VFXEntity::mVFXIDs[i]);
		if (vfxEnt != nullptr)
		{
			VFXEntity* vfxEntityComponent = vfxEnt->GetComponent<VFXEntity>();
			if (vfxEntityComponent != nullptr)
				vfxEntityComponent->Update();
		}
	}

	//Update wind
	Wind::UpdateWindSound();
	Wind::mPlayWindSound = false;

	//Update Cinematic Bars
	{
		if (ComponentManager::GetInstance()->GetComponentArray<CinematicBars>() == nullptr)
			return;

		auto& cinematicBarsArr = ComponentManager::GetInstance()->GetComponentArray<CinematicBars>()->mComponentArray;
		if (cinematicBarsArr.size() < 1)
			return;

		for (auto& obj : cinematicBarsArr)
			obj.second.Update();
	}
}

void BossScene::Pause(double _dt)
{
	UNREFERENCED_PARAMETER(_dt);
	Entity* player = EntityManager::GetInstance()->GetEntity(Player::mPlayerID);
	if (player != nullptr)
	{
		Player* playerComponent = player->GetComponent<Player>();
		if (playerComponent != nullptr)
			playerComponent->UpdatePlayerShaders();
	}
}

void BossScene::Render(double _dt)
{
	UNREFERENCED_PARAMETER(_dt);

}

void BossScene::Free()
{

}

void BossScene::Exit()
{
	SoundManager::GetInstance()->StopChannel(SoundManager::SoundType_SFX);
	SoundManager::GetInstance()->StopChannel(SoundManager::SoundType_BGM);

	Key::mKeyIDs.clear();
	KeyWall::mKeyWallIDs.clear();
	Player::mPlayerID = 0;

	TextManager::GetInstance()->ClearAllText();
	ParticleSystem::GetInstance()->ClearParticles();
}