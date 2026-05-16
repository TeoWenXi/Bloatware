/*!
All content © 2023 DigiPen Institute of Technology Singapore, all rights reserved.
@file       UIButton_ExitApp.h
@author     TAN Kang Zheng (100%)
@co-author

----------------------------------------------------------------------------

@course     CSD2401F23
@section    CSD2401F23-A
@project    CSD2401F23 Software Engineering Project 3

@brief      Button Component to close the application
*//*______________________________________________________________________*/

#include "UIButton.h"
#include "EntityManager.h"
#include "Transform.h"
#include "GameUITextHandler.h"
#include "CoreEngine.h"

#ifndef UI_BUTTON_EXIT_APP_H
#define UI_BUTTON_EXIT_APP_H

class UIButtonExitApp : public UIButton
{
public:
	UIButtonExitApp(EntityID _id) : UIButton(_id) {};

	~UIButtonExitApp() {};

	void OnClick()
	{
		CoreEngine::GetInstance()->Exit();
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
		}
	}

	void OnHoverExit()
	{
		GameUITextHandler* _textHandler = EntityManager::GetInstance()->GetEntity(mEntID)->GetComponent<GameUITextHandler>();
		if (_textHandler != nullptr)
		{
			_textHandler->mFontSize -= 3;
			_textHandler->mColor += Vec3{ 0.1f, 0.f, 0.f };
		}
	}
};

#endif