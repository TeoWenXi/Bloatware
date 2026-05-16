/*!
All content © 2024 DigiPen Institute of Technology Singapore, all rights reserved.
@file       UIButton_Keybind.cpp
@author     TAN Kang Zheng (100%)
@co-author

----------------------------------------------------------------------------

@course     CSD2451F24
@section    CSD2451F24-A
@project    CSD2451F24 Software Engineering Project 4

@brief      Button Component to edit keybinds
*//*______________________________________________________________________*/
#include "UIButton_Keybind.h"

void UIButtonKeybind::OnClick()
{
	InputManager::GetInstance()->ChangeActionKeybind(mKeybind);
}

void UIButtonKeybind::OnHoverEnter()
{
	GameUITextHandler* _textHandler = EntityManager::GetInstance()->GetEntity(mEntID)->GetComponent<GameUITextHandler>();
	if (_textHandler != nullptr)
	{
		_textHandler->mFontSize += 1;
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

void UIButtonKeybind::OnHoverExit()
{
	GameUITextHandler* _textHandler = EntityManager::GetInstance()->GetEntity(mEntID)->GetComponent<GameUITextHandler>();
	if (_textHandler != nullptr)
	{
		_textHandler->mFontSize -= 1;
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

void UIButtonKeybind::Update()
{
	GameUITextHandler* _textHandler = EntityManager::GetInstance()->GetEntity(mEntID)->GetComponent<GameUITextHandler>();
	if (_textHandler != nullptr)
	{
		//Update text here with the input manager function
		_textHandler->mText = InputManager::GetInstance()->KeycodeToString(InputManager::GetInstance()->GetCurrentActionKeybind(mKeybind));
	}
}

std::string& UIButtonKeybind::GetKeybind()
{
	return mKeybind;
}