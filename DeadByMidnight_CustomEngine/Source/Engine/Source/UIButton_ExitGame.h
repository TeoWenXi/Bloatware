/*!
All content © 2023 DigiPen Institute of Technology Singapore, all rights reserved.
@file       UIButton_ExitGame.h
@author     TAN Kang Zheng (100%)
@co-author

----------------------------------------------------------------------------

@course     CSD2401F23
@section    CSD2401F23-A
@project    CSD2401F23 Software Engineering Project 3

@brief      Button Component to Exit Game menu
*//*______________________________________________________________________*/

#include "UIButton.h"
#include "GameUITextHandler.h"
#include "EntityManager.h"
#include "CoreEngine.h"
#include "SoundComponent.h"

#ifndef UI_BUTTON_EXIT_GAME_H
#define UI_BUTTON_EXIT_GAME_H

class UIButtonExitGame : public UIButton
{
public:

	/*!
	 * @brief 
	 * Constructor for exit game menu
	 * @param _id 
	 * EntityID to initialise button with
	*/
	UIButtonExitGame(EntityID _id) : UIButton(_id) {};

	/*!
	 * @brief 
	 * Destructor for exit game menu
	*/
	~UIButtonExitGame() {};

	/*!
	 * @brief 
	 * Called when the button is clicked.
	*/
	void OnClick()
	{
		CoreEngine::GetInstance()->Exit();
	}

	/*!
	 * @brief 
	 * Called when the button is held.
	*/
	void OnHold()
	{

	}

	/*!
	 * @brief 
	 * Called when the button being hovered.
	*/
	void OnHoverEnter()
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

	/*!
	 * @brief 
	 * Called when the button exiting hovered.
	*/
	void OnHoverExit()
	{
		GameUITextHandler* _textHandler = EntityManager::GetInstance()->GetEntity(mEntID)->GetComponent<GameUITextHandler>();
		if (_textHandler != nullptr)
		{
			_textHandler->mFontSize -= 1;
			_textHandler->mColor -= Vec3{ 0.8f, 0.8f, 0.8f };
		}
	}
};

#endif