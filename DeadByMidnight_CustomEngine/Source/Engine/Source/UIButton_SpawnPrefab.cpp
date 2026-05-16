/*!
All content © 2024 DigiPen Institute of Technology Singapore, all rights reserved.
@file       UIButton_SpawnPrefab.cpp
@author     TAN Kang Zheng (100%)
@co-author

----------------------------------------------------------------------------

@course     CSD2451F24
@section    CSD2451F24-A
@project    CSD2451F24 Software Engineering Project 4

@brief      Button Component to spawn a given prefab
*//*______________________________________________________________________*/

#include "UIButton_SpawnPrefab.h"
#include "LevelLoader.h"

void UIButtonSpawnPrefab::OnClick()
{
	EntityID _parentID = EntityManager::GetInstance()->GetEntity(mEntID)->GetParentID();
	if (_parentID)
	{
		EntityManager::GetInstance()->GetEntity(_parentID)->SetToDestroy(true);
		std::vector<EntityID> _childEnt = EntityManager::GetInstance()->GetEntity(_parentID)->GetChildEntList();
		for (auto i : _childEnt)
		{
			EntityManager::GetInstance()->GetEntity(i)->SetToDestroy(true);
		}
	}
	else
	{
		EntityManager::GetInstance()->GetEntity(mEntID)->SetToDestroy(true);
	}

	LevelLoader::LoadPrefab(mPrefab);
}

void UIButtonSpawnPrefab::OnHoverEnter()
{
	GameUITextHandler* _textHandler = EntityManager::GetInstance()->GetEntity(mEntID)->GetComponent<GameUITextHandler>();
	if (_textHandler != nullptr)
	{
		_textHandler->mFontSize += 1;
		_textHandler->mColor += Vec3{ 0.8f, 0.8f, 0.8f };
	}

	SoundComponent* _soundComp = EntityManager::GetInstance()->GetEntity(mEntID)->GetComponent<SoundComponent>();
	if (_soundComp != nullptr)
	{
		_soundComp->PlayOverride();
	}
}

void UIButtonSpawnPrefab::OnHoverExit()
{
	GameUITextHandler* _textHandler = EntityManager::GetInstance()->GetEntity(mEntID)->GetComponent<GameUITextHandler>();
	if (_textHandler != nullptr)
	{
		_textHandler->mFontSize -= 1;
		_textHandler->mColor -= Vec3{ 0.8f, 0.8f, 0.8f };
	}
}

void UIButtonSpawnPrefab::SetPrefab(std::string _prefab)
{
	mPrefab = _prefab;
}