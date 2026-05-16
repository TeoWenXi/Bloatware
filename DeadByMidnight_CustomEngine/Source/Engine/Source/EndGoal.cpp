/*!
All content © 2023 DigiPen Institute of Technology Singapore, all rights reserved.
@file       EndGoal.cpp
@author     TAN Kang Zheng (100%)
@co-author

----------------------------------------------------------------------------

@course     CSD2401F23
@section    CSD2401F23-A
@project    CSD2401F23 Software Engineering Project 3

@brief      Component to check if player has reached a target location
*//*______________________________________________________________________*/

#include "EndGoal.h"

#include "Collision.h"
#include "Transform.h"
#include "EntityManager.h"
#include "Player.h"
#include "LevelLoader.h"
#include "GameUIManager.h"

void EndGoal::Init()
{
	GameUIManager::GetInstance()->AddEndGoal(mEntID);
}

void EndGoal::Load()
{
	mPrefab = JSONManager::GetInstance()->GetString("prefab");
}

void EndGoal::Update()
{
	Transform* transform = EntityManager::GetInstance()->GetEntity(mEntID)->GetComponent<Transform>();
	Vec3 playerPos = EntityManager::GetInstance()->GetEntity(Player::mPlayerID)->GetComponent<Transform>()->GetPosition();

	if (transform == nullptr)
		return;

	if (Collision::CollisionStaticAABB(playerPos, *transform))
	{
		EntityManager::GetInstance()->GetEntity(mEntID)->SetActive(false);
		LevelLoader::LoadPrefab(mPrefab);
	}
}

void EndGoal::Save()
{
	JSONManager::GetInstance()->Save("Component", "EndGoal");
	JSONManager::GetInstance()->Save("prefab", mPrefab);
}

void EndGoal::Exit()
{
	GameUIManager::GetInstance()->RemoveEndGoal(mEntID);
}

void EndGoal::DisplayProperties()
{
#if _ENGINE
	MyImGui::MyImGuiStringInput(mPrefab, "Prefab");
#endif // _ENGINE
};