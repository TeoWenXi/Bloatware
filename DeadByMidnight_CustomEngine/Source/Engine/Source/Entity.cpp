/*!
All content © 2023 DigiPen Institute of Technology Singapore, all rights reserved.
@file       Entity.cpp
@author     TEO Wen Xi (100%)
@co-author

----------------------------------------------------------------------------

@course     CSD2401F23
@section    CSD2401F23-A
@project    CSD2401F23 Software Engineering Project 3

@brief		Function definitions for the entity class
*//*______________________________________________________________________*/

#include "Entity.h"
#include "ComponentManager.h"
#include "EntityManager.h"

#include <algorithm>

Entity::Entity()
	: mID(std::numeric_limits<EntityID>::max()),
	mName(""),
	mActive(true),
	mToDestroy(false),
	mSaveInScene(true),
	mParentID(0)
{
}

Entity::Entity(const EntityID& _id)
	: mID(_id),
	mName(""),
	mActive(true),
	mToDestroy(false),
	mSaveInScene(true),
	mParentID(0)
{
}

void Entity::Destroy()
{
	ComponentManager::GetInstance()->EntityDestroyed(mID);
}

//Get Set functions
EntityID Entity::GetID()
{
	return mID;
}

void Entity::SetID(const EntityID& _id)
{
	mID = _id;
}

std::string Entity::GetName()
{
	return mName;
}

void Entity::SetName(const std::string& _name)
{
	mName = _name;
}

bool Entity::GetActive()
{
	return mActive;
}

void Entity::SetActive(bool _active)
{
	mActive = _active;
}

bool Entity::GetToDestroy()
{
	return mToDestroy;
}

void Entity::SetToDestroy(bool _toDestroy)
{
	mToDestroy = _toDestroy;
}

bool Entity::GetSaveInScene()
{
	return mSaveInScene;
}

void Entity::SetSaveInScene(bool _saveInScene)
{
	mSaveInScene = _saveInScene;
}

EntityID Entity::GetParentID()
{
	return mParentID;
}

void Entity::AddChild(EntityID _id)
{
	if (std::find(mChildEntList.begin(), mChildEntList.end(), _id) == mChildEntList.end())
		mChildEntList.push_back(_id);

	EntityManager::GetInstance()->GetEntity(_id)->SetParentID(mID);
}

void Entity::RemoveChild(EntityID _id)
{
	for(size_t i = 0; i < mChildEntList.size(); ++i)
		if (mChildEntList[i] == _id)
		{
			mChildEntList.erase(mChildEntList.begin() + i);
			EntityManager::GetInstance()->GetEntity(_id)->RemoveParenting();
			break;
		}
}

void Entity::SetParentID(EntityID _id)
{
	mParentID = _id;
}

void Entity::RemoveParenting()
{
	mParentID = 0;
}

const std::vector<EntityID> Entity::GetChildEntList()
{
	return mChildEntList;
}