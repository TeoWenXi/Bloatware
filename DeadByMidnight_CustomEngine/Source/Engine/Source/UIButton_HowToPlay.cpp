/*!
All content © 2023 DigiPen Institute of Technology Singapore, all rights reserved.
@file       UIButton_HowToPlay.cpp
@author     TAN Kang Zheng (100%)
@co-author

----------------------------------------------------------------------------

@course     CSD2401F23
@section    CSD2401F23-A
@project    CSD2401F23 Software Engineering Project 3

@brief      Button Component to create the HowToPlay menu
*//*______________________________________________________________________*/
#include "UIButton_HowToPlay.h"

#include "LevelLoader.h"

void UIButtonHowToPlay::OnClick()
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

	LevelLoader::LoadPrefab("HowToPlay Prefab");
}

void UIButtonHowToPlay::OnHoverEnter()
{
	GameUITextHandler* _textHandler = EntityManager::GetInstance()->GetEntity(mEntID)->GetComponent<GameUITextHandler>();
	if (_textHandler != nullptr)
	{
		_textHandler->mFontSize += 1;
		_textHandler->mColor -= Vec3{ 0.1f, 0.f, 0.f };
	}

	SoundComponent* _soundComp = EntityManager::GetInstance()->GetEntity(mEntID)->GetComponent<SoundComponent>();
	if (_soundComp != nullptr)
	{
		_soundComp->PlayOverride();
	}
}

void UIButtonHowToPlay::OnHoverExit()
{
	GameUITextHandler* _textHandler = EntityManager::GetInstance()->GetEntity(mEntID)->GetComponent<GameUITextHandler>();
	if (_textHandler != nullptr)
	{
		_textHandler->mFontSize -= 1;
		_textHandler->mColor += Vec3{ 0.1f, 0.f, 0.f };
	}
}