/*!
All content © 2023 DigiPen Institute of Technology Singapore, all rights reserved.
@file       AIManager.cpp
@author     TAN Kang Zheng (100%)
@co-author

----------------------------------------------------------------------------

@course     CSD2401F23
@section    CSD2401F23-A
@project    CSD2401F23 Software Engineering Project 3

@brief      Manages the AI Components
*//*______________________________________________________________________*/

#include "AIManager.h"
#include "AIComponent.h"
#include "CoreEngine.h"

void AIManager::Update()
{
	if (CoreEngine::GetInstance()->mGameState != CoreEngine::GameState::GS_PLAY)
		return;

	//Run the update functino for each AIComponent
	for (auto i : mAIComponent_vec)
	{
		EntityManager::GetInstance()->GetEntity(i)->GetComponent<AIComponent>()->Update();
	}
}

void AIManager::Unload()
{
	//Remove instance
	Destroy();
}

void AIManager::AddAIComponent(EntityID _AIComponent)
{
	mAIComponent_vec.push_back(_AIComponent);
}

void AIManager::RemoveAIComponent(EntityID _AIComponent)
{
	for (int i = 0; i < mAIComponent_vec.size(); i++)
	{
		if (mAIComponent_vec.at(i) == _AIComponent)
		{
			mAIComponent_vec.erase(mAIComponent_vec.begin() + i);
		}
	}
}

std::vector<EntityID> AIManager::GetAIComponentVector()
{
	return mAIComponent_vec;
}

void AIManager::CreateBackup()
{
	mAIComponent_vecBackup = mAIComponent_vec;
}

void AIManager::LoadBackup()
{
	mAIComponent_vec = mAIComponent_vecBackup;
}

void AIManager::InitiateTalking(std::string _Dialogue, std::string _Audio)
{
	if (mAIComponent_vec.size())
	{
		AIComponent* aiComponent = EntityManager::GetInstance()->GetEntity(mAIComponent_vec.at(0))->GetComponent<AIComponent>();
		if (aiComponent != nullptr)
		{
			aiComponent->InitiateTalking(_Dialogue, _Audio);
		}
	}
}

bool AIManager::IsHostile()
{
	if (mAIComponent_vec.size())
	{
		AIComponent* aiComponent = EntityManager::GetInstance()->GetEntity(mAIComponent_vec.at(0))->GetComponent<AIComponent>();
		if (aiComponent != nullptr && aiComponent->mIsHostile)
		{
			return true;
		}
	}
	return false;
}

void AIManager::EnableAIBossMode()
{
	if (mAIComponent_vec.size())
	{
		AIComponent* aiComponent = EntityManager::GetInstance()->GetEntity(mAIComponent_vec.at(0))->GetComponent<AIComponent>();
		if (aiComponent != nullptr)
			aiComponent->EnableHostileAI();
	}
}

void AIManager::DisableAIBossMode()
{
	if (mAIComponent_vec.size())
	{
		AIComponent* aiComponent = EntityManager::GetInstance()->GetEntity(mAIComponent_vec.at(0))->GetComponent<AIComponent>();
		if (aiComponent != nullptr)
			aiComponent->DisableHostileAI();
	}
}

void AIManager::SetAudioDucking(float _val)
{
	if (mAIComponent_vec.size())
	{
		AIComponent* aiComponent = EntityManager::GetInstance()->GetEntity(mAIComponent_vec.at(0))->GetComponent<AIComponent>();
		if (aiComponent != nullptr)
			aiComponent->SetAudioDucking(_val);
	}
}
