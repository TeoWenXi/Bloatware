/*!
All content © 2024 DigiPen Institute of Technology Singapore, all rights reserved.
@file       UIButton_VolumeChanger.cpp
@author     TAN Kang Zheng (100%)
@co-author

----------------------------------------------------------------------------

@course     CSD2401F23
@section    CSD2401F23-A
@project    CSD2401F23 Software Engineering Project 3

@brief      Button Component to change the volume level of different channels
*//*______________________________________________________________________*/

#include "UIButton_VolumeChanger.h"
#include "SoundComponent.h"
#include "InputManager.h"
#include "SoundManager.h"

void UIButtonVolumeChanger::OnHoverEnter()
{
	mActive = true;
	GameUITextHandler* _textHandler = EntityManager::GetInstance()->GetEntity(mEntID)->GetComponent<GameUITextHandler>();
	if (_textHandler != nullptr)
	{
		_textHandler->mColor += Vec3{ 0.8f, 0.8f, 0.8f };
	}

	std::vector<EntityID> _childEnt = EntityManager::GetInstance()->GetEntity(mEntID)->GetChildEntList();
	for (auto i : _childEnt)
	{
		GameUITextHandler* _childTextHandler = EntityManager::GetInstance()->GetEntity(i)->GetComponent<GameUITextHandler>();
		if (_childTextHandler != nullptr)
		{
			_childTextHandler->mColor += Vec3{ 0.8f, 0.8f, 0.8f };
		}
	}

	SoundComponent* _soundComp = EntityManager::GetInstance()->GetEntity(mEntID)->GetComponent<SoundComponent>();
	if (_soundComp != nullptr)
	{
		_soundComp->PlayOverride();
	}
}

void UIButtonVolumeChanger::OnHoverExit()
{
	mActive = false;

	GameUITextHandler* _textHandler = EntityManager::GetInstance()->GetEntity(mEntID)->GetComponent<GameUITextHandler>();
	if (_textHandler != nullptr)
	{
		_textHandler->mColor -= Vec3{ 0.8f, 0.8f, 0.8f };
	}

	std::vector<EntityID> _childEnt = EntityManager::GetInstance()->GetEntity(mEntID)->GetChildEntList();
	for (auto i : _childEnt)
	{
		GameUITextHandler* _childTextHandler = EntityManager::GetInstance()->GetEntity(i)->GetComponent<GameUITextHandler>();
		if (_childTextHandler != nullptr)
		{
			_childTextHandler->mColor -= Vec3{ 0.8f, 0.8f, 0.8f };
		}
	}
}

void UIButtonVolumeChanger::Update()
{
	GameUITextHandler* text = EntityManager::GetInstance()->GetEntity(mEntID)->GetComponent<GameUITextHandler>();
	if (text != nullptr)
	{
		int volumeVal = 0;
		switch (mChannelType)
		{
		case SFX:
			volumeVal = (int)(SoundManager::GetInstance()->GetSFXVolume() * 10);
			break;
		case BGM:
			volumeVal = (int)(SoundManager::GetInstance()->GetBGMVolume() * 10);
			break;
		case MASTER:
			volumeVal = (int)(SoundManager::GetInstance()->GetMasterVolume() * 10);
			break;
		default:
			break;
		}

		if (volumeVal < 10)
			text->mText = "0" + std::to_string(volumeVal);
		else
			text->mText = std::to_string(volumeVal);
	}


	if (mActive == false)
		return;

	if (InputManager::GetInstance()->GetActionTriggered("MenuRight"))
	{
		switch (mChannelType)
		{
		case SFX:
			SoundManager::GetInstance()->SetSFXVolume(SoundManager::GetInstance()->GetSFXVolume() + 0.1f);
			break;
		case BGM:
			SoundManager::GetInstance()->SetBGMVolume(SoundManager::GetInstance()->GetBGMVolume() + 0.1f);
			break;
		case MASTER:
			SoundManager::GetInstance()->SetMasterVolume(SoundManager::GetInstance()->GetMasterVolume() + 0.1f);
			break;
		default:
			break;
		}
	}

	if (InputManager::GetInstance()->GetActionTriggered("MenuLeft"))
	{
		switch (mChannelType)
		{
		case SFX:
			SoundManager::GetInstance()->SetSFXVolume(SoundManager::GetInstance()->GetSFXVolume() - 0.1f);
			break;
		case BGM:
			SoundManager::GetInstance()->SetBGMVolume(SoundManager::GetInstance()->GetBGMVolume() - 0.1f);
			break;
		case MASTER:
			SoundManager::GetInstance()->SetMasterVolume(SoundManager::GetInstance()->GetMasterVolume() - 0.1f);
			break;
		default:
			break;
		}
	}
}

void UIButtonVolumeChanger::ChangeChannelType(ChannelType _type)
{
	mChannelType = _type;
}