/*!
All content © 2023 DigiPen Institute of Technology Singapore, all rights reserved.
@file       EntityManager.cpp
@author     TEO Wen Xi (100%)
@co-author

----------------------------------------------------------------------------

@course     CSD2401F23
@section    CSD2401F23-A
@project    CSD2401F23 Software Engineering Project 3

@brief      Manages all entities
*//*______________________________________________________________________*/

#include "EntityManager.h"
#include "JSONManager.h"
#include "GraphicsSystem.h"
#include "ParticleSystem.h"

#include <iostream>
#include <algorithm>

void EntityManager::Init()
{
}

void EntityManager::Load()
{
	//Load Max Entities size
	JSONManager::GetInstance()->LoadFile("config");
	mMaxEntities = mTargetMaxEntities = JSONManager::GetInstance()->GetInt("MaxEntities");

	//Init vector with all possible entity IDs
	for (EntityID entity = 1; entity < mMaxEntities; ++entity)
		mAvailableEntities.emplace_back(entity);

	//Reserve memory space for entity list
	mEntityList.reserve(mMaxEntities);
}

void EntityManager::Update()
{
	//Destroy all entities that are set to destroy
	for (std::vector<Entity>::iterator iter = mEntityList.begin(); iter != mEntityList.end();)
	{
		if (iter->GetName() == "Player")
		{
			std::vector<Entity>::iterator iter2 = iter;
			iter2++;
			//update child
			int childlistsize = (int)iter->GetChildEntList().size();
			for (int count = 0; count < childlistsize; ++iter2, ++count)
			{
				iter2->GetComponent<Transform>()->mScale = iter->GetComponent<Transform>()->mScale;
			}
		}
		if (iter->GetToDestroy())
			DestroyEntity(iter);
		else
			++iter;
	}

	//Re-adjust entity count if data is changed
	if (mTargetMaxEntities != mMaxEntities)
		ReallocateMemory();

}

void EntityManager::Unload()
{
	for (int i = (int)(mEntityList.size() - 1); i >= 0; --i)
	{
		DestroyEntity(mEntityList[i].GetID());
	}

	//Destroy instance
	Destroy();
}

Entity* EntityManager::CreateEntity()
{
	//Check for max number of entities
	if (mAvailableEntities.size() == 0)
	{
#if _ENGINE
		DebugLogger::GetInstance()->Log("Max number of entities reached\n");
#endif // _ENGINE
		return nullptr;
	}

	//Take an ID from the vector and pop it
	EntityID id = mAvailableEntities.front();
	mAvailableEntities.erase(mAvailableEntities.begin());
	Entity* newEnt = &mEntityList.emplace_back(id);

	//Return created entity id
	return newEnt;
}

Entity* EntityManager::CreateEntityWithID(EntityID _entityID)
{
	//Entity ID not found
	auto iter = std::find(mAvailableEntities.begin(), mAvailableEntities.end(), _entityID);
	if (iter == mAvailableEntities.end())
	{
#if _ENGINE
		DebugLogger::GetInstance()->Log("Entity ID %d not found\n", _entityID);
#endif // _ENGINE
		return nullptr;
	}

	//Create entity
	mAvailableEntities.erase(iter);
	Entity* newEnt = &mEntityList.emplace_back(_entityID);

	//Return created entity id
	return newEnt;
}

Entity* EntityManager::DuplicateEntity(EntityID _entityID)
{
	Entity* currEnt = GetEntity(_entityID);
	Entity* newEnt = CreateEntity();
	
	//Copy Basic Info
	newEnt->SetName(currEnt->GetName() + "Dupe");

	//Duplicate Components
	ComponentManager::GetInstance()->DuplicateEntityComponents(currEnt->GetID(), newEnt->GetID());

	//Duplicate child entities if any
	if (currEnt->GetChildEntList().empty() == false)
	{
		auto childEntList = currEnt->GetChildEntList();
		for (size_t i = 0; i < childEntList.size(); ++i)
		{
			Entity* childEnt = DuplicateEntity(childEntList[i]);
			newEnt->AddChild(childEnt->GetID());
		}
	}

	return newEnt;
}

void EntityManager::DestroyEntityAll(EntityID _entityID)
{
	//Entity out of range
	if (_entityID < 0 || _entityID >= mMaxEntities)
	{
#if _ENGINE
		DebugLogger::GetInstance()->Log("Entity out of range\n");
#endif // _ENGINE
		return;
	}

	//Look for entity in current entity list
	auto iter = FindEntityFromList(_entityID);
	if (iter == mEntityList.end())
		return;

	//Remove parenting from child entities if any
	if (iter->GetChildEntList().empty() == false)
	{
		auto childEntList = iter->GetChildEntList();
		for (size_t i = 0; i < childEntList.size(); ++i)
			if (GetEntity(childEntList[i]) != nullptr)
				DestroyEntityAll(childEntList[i]);
	}

	//Remove from parent entity if any
	if (iter->GetParentID() != 0)
		GetEntity(iter->GetParentID())->RemoveChild(_entityID);

	//Remove entity from current entity list
	iter->Destroy();
	mEntityList.erase(iter);
	mAvailableEntities.emplace_back(_entityID);
}

void EntityManager::DestroyEntity(EntityID _entityID)
{
	//Entity out of range
	if (_entityID < 0 || _entityID >= mMaxEntities)
	{
#if _ENGINE
		DebugLogger::GetInstance()->Log("Entity out of range\n");
#endif // _ENGINE
		return;
	}

	//Look for entity in current entity list
	auto iter = FindEntityFromList(_entityID);
	if (iter == mEntityList.end())
		return;

	//Remove parenting from child entities if any
	if (iter->GetChildEntList().empty() == false)
	{
		auto childEntList = iter->GetChildEntList();
		for (size_t i = 0; i < childEntList.size(); ++i)
			if(GetEntity(childEntList[i]) != nullptr)
				GetEntity(childEntList[i])->RemoveParenting();
	}

	//Remove from parent entity if any
	if (iter->GetParentID() != 0)
		GetEntity(iter->GetParentID())->RemoveChild(_entityID);

	//Remove entity from current entity list
	iter->Destroy();
	mEntityList.erase(iter);
	mAvailableEntities.emplace_back(_entityID);
}

void EntityManager::DestroyEntity(std::vector<Entity>::iterator& _iter)
{
	//Remove entity from current entity list
	mAvailableEntities.emplace_back(_iter->GetID());

	//Remove parenting from child entities if any
	if (_iter->GetChildEntList().empty() == false)
	{
		auto childEntList = _iter->GetChildEntList();
		for (size_t i = 0; i < childEntList.size(); ++i)
			if (GetEntity(childEntList[i]) != nullptr)
				GetEntity(childEntList[i])->RemoveParenting();
	}

	//Remove from parent entity if any
	if (_iter->GetParentID() != 0)
		GetEntity(_iter->GetParentID())->RemoveChild(_iter->GetID());

	_iter->Destroy();
	_iter = mEntityList.erase(_iter);
}

Entity* EntityManager::CreateChildEntity(EntityID _parentEntID)
{
	//Create new entity
	Entity* newEnt = CreateEntity();

	//Set parent child
	GetEntity(_parentEntID)->AddChild(newEnt->GetID());

	return newEnt;
}

void EntityManager::SetName(EntityID _entityID, std::string _name)
{
	//Look for entity in current entity list
	auto iter = FindEntityFromList(_entityID);
	if (iter == mEntityList.end())
		return;

	//Set new name
	iter->SetName(_name);
}

std::string EntityManager::GetName(EntityID _entityID)
{
	//Look for entity in current entity list
	auto iter = FindEntityFromList(_entityID);
	if (iter == mEntityList.end())
		return "";

	//Get name
	return iter->GetName();
}

Entity* EntityManager::GetEntity(EntityID _entityID)
{
	auto iter = FindEntityFromList(_entityID);

	//Entity cannot be found
	if (iter == mEntityList.end())
		return nullptr;

	//Entity found
	return &*iter;
}

Entity* EntityManager::GetEntity(std::string _entityName)
{
	auto iter = std::find_if(mEntityList.begin(), mEntityList.end(),
		[&](Entity& ent) {return ent.GetName() == _entityName; });
	//Entity cannot be found
	if (iter == mEntityList.end())
		return nullptr;

	//Entity found
	return &*iter;
}

std::vector<Entity>::iterator EntityManager::FindEntityFromList(EntityID _entityID)
{
	auto iter = std::find_if(mEntityList.begin(), mEntityList.end(),
		[&](Entity& ent) {return ent.GetID() == _entityID; });

	return iter;
}

void EntityManager::Print()
{
#if _ENGINE
	DebugLogger::GetInstance()->Log("\n\n");
	//Available Entities
	DebugLogger::GetInstance()->Log("=============== AVAILABLE ENTITY IDS ==================\n");
	for (size_t i = 0; i < mAvailableEntities.size(); ++i)
		DebugLogger::GetInstance()->Log("%d \n\n", mAvailableEntities[i]);


	//Entity list
	DebugLogger::GetInstance()->Log("=============== ENTITY LIST =================\n");
	for (size_t i = 0; i < mEntityList.size(); ++i)
	{
		Entity* currEntity = &mEntityList[i];
		DebugLogger::GetInstance()->Log("[%d,%d] %s\n", i, currEntity->GetID(), currEntity->GetName().c_str());
	}
#endif // _ENGINE
}

void EntityManager::ReallocateMemory()
{
	//Increase in size
	if (mMaxEntities < mTargetMaxEntities)
	{
		//Add new IDs to available entities container
		for (EntityID i = mMaxEntities; i < mTargetMaxEntities; ++i)
			mAvailableEntities.emplace_back(i);

		//Reserve memory space for entity list
		mEntityList.reserve(mTargetMaxEntities);
	}
	//decrease in size
	else
	{
		//Try to replace current entityList's entities with a lower ID
		for (EntityID i = 0; i < mEntityList.size(); ++i)
		{
			if (mEntityList[i].GetID() >= mTargetMaxEntities)
			{
				//Look for the current available smallest entity ID
				EntityID availableSmallestID = GetSmallestAvailableID(mEntityList[i].GetID());

				//There is no available smaller entity IDs
				if (availableSmallestID >= mEntityList[i].GetID())
					break;

				//If there is a smaller entity ID available
				//Replace current entity's ID with the smaller ID
				auto iter = std::find(mAvailableEntities.begin(), mAvailableEntities.end(), availableSmallestID);
				mAvailableEntities.erase(iter);
				mEntityList[i].SetID(availableSmallestID);
			}
		}

		//Remove entities with IDs above the max entitity size
		for (EntityID i = 0; i < mEntityList.size();)
		{
			if (mEntityList[i].GetID() >= mTargetMaxEntities)
				mEntityList.erase(mEntityList.begin() + i);
			else
				++i;
		}

		//Search for IDs above the target and remove them
		for (EntityID i = 0; i < mAvailableEntities.size();)
		{
			if (mAvailableEntities[i] >= mTargetMaxEntities)
				mAvailableEntities.erase(mAvailableEntities.begin() + i);
			else
				++i;
		}
	}

	//Assign max entities to target max entities
	mMaxEntities = mTargetMaxEntities;
}

EntityID EntityManager::GetSmallestAvailableID(EntityID _entityID)
{
	EntityID smallestID = _entityID;
	for (EntityID i = 0; i < mAvailableEntities.size(); ++i)
	{
		if (mAvailableEntities[i] < smallestID)
			smallestID = mAvailableEntities[i];
	}

	return smallestID;
}

void EntityManager::BackupCurrScene()
{
	ClearSceneBackup();
	mBackupEntityList = mEntityList;
	mBackupAvailableEntities = mAvailableEntities;
	GraphicsSystem::GetInstance()->mCamera.mPrevPlayerID = GraphicsSystem::GetInstance()->mCamera.mPlayerID;
	ComponentManager::GetInstance()->BackupCurrScene();
	AIManager::GetInstance()->CreateBackup();
	GameUIManager::GetInstance()->CreateBackup();
	SoundManager::GetInstance()->CreateBackup();
	ParticleSystem::GetInstance()->ClearParticles();
}

void EntityManager::LoadSceneBackup()
{
	mEntityList = mBackupEntityList;
	mAvailableEntities = mBackupAvailableEntities;
	GraphicsSystem::GetInstance()->mCamera.mPlayerID = GraphicsSystem::GetInstance()->mCamera.mPrevPlayerID;
	ComponentManager::GetInstance()->LoadSceneBackup();
	AIManager::GetInstance()->LoadBackup();
	GameUIManager::GetInstance()->LoadBackup();
	SoundManager::GetInstance()->LoadBackup();
	ParticleSystem::GetInstance()->ClearParticles();
}

void EntityManager::ClearSceneBackup()
{
	mBackupEntityList.clear();
	mBackupAvailableEntities.clear();
	ComponentManager::GetInstance()->ClearSceneBackup();
	ParticleSystem::GetInstance()->ClearParticles();
}
