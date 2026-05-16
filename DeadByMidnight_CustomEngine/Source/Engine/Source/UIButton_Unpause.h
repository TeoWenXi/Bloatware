/*!
All content © 2023 DigiPen Institute of Technology Singapore, all rights reserved.
@file       UIButton_Unpause.h
@author     TAN Kang Zheng (100%)
@co-author

----------------------------------------------------------------------------

@course     CSD2401F23
@section    CSD2401F23-A
@project    CSD2401F23 Software Engineering Project 3

@brief      Button Component to unpause the game
*//*______________________________________________________________________*/

#include "UIButton.h"
#include "EntityManager.h"
#include "Transform.h"
#include "GameUITextHandler.h"
#include "CoreEngine.h"
#include "EventManager.h"
#include "SoundComponent.h"

#ifndef UI_BUTTON_UNPAUSE_H
#define UI_BUTTON_UNPAUSE_H

class UIButtonUnpause : public UIButton
{
public:

	/*!
	 * @brief 
	 * Constructor for Unpause Button
	 * @param _id
	 * EntityID to initialise button with
	*/
	UIButtonUnpause(EntityID _id) : UIButton(_id) {};

	/*!
	 * @brief
	 * Destructor for Unpause Button
	*/
	~UIButtonUnpause() {};

	/*!
	 * @brief 
	 * Called when the button is clicked.
	*/
	void OnClick()
	{
		CoreEngine::GetInstance()->mGameState = CoreEngine::GameState::GS_PLAY;
		EventManager::GetInstance()->AddEvent(EventPlayPause(EventPlayPause::PLAY));
		//TODO:: Get parent of this object
		//Then set it to delete
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