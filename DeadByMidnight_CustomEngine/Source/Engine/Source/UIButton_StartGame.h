/*!
All content © 2023 DigiPen Institute of Technology Singapore, all rights reserved.
@file       UIButton_StartGame.h
@author     TAN Kang Zheng (100%)
@co-author

----------------------------------------------------------------------------

@course     CSD2401F23
@section    CSD2401F23-A
@project    CSD2401F23 Software Engineering Project 3

@brief      Button Component to transition to game scene
*//*______________________________________________________________________*/

#include "UIButton.h"
#include "GameUITextHandler.h"
#include "EntityManager.h"
#include "SceneManager.h"

#ifndef UI_BUTTON_START_GAME_H
#define UI_BUTTON_START_GAME_H

class UIButtonStartGame : public UIButton
{
public:
	UIButtonStartGame(EntityID _id) : UIButton(_id) {};

	~UIButtonStartGame() {};

	void OnClick()
	{
		SceneManager::GetInstance()->SetActiveScene(SceneManager::SCENE_GAME);
	}

	void OnHold()
	{

	}

	void OnHoverEnter()
	{
		GameUITextHandler* _textHandler = EntityManager::GetInstance()->GetEntity(mEntID)->GetComponent<GameUITextHandler>();
		if (_textHandler != nullptr)
		{
			_textHandler->mFontSize += 3;
			_textHandler->mColor -= Vec3{ 0.1f, 0.f, 0.f };
			_textHandler->mText += ">";
		}
	}

	void OnHoverExit()
	{
		GameUITextHandler* _textHandler = EntityManager::GetInstance()->GetEntity(mEntID)->GetComponent<GameUITextHandler>();
		if (_textHandler != nullptr)
		{
			_textHandler->mFontSize -= 3;
			_textHandler->mColor += Vec3{ 0.1f, 0.f, 0.f };
			_textHandler->mText.resize(_textHandler->mText.length() - 1);
		}
	}
};

#endif