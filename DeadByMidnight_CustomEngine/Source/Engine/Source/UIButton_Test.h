/*!
All content © 2023 DigiPen Institute of Technology Singapore, all rights reserved.
@file       UIButton_Test.h
@author     TAN Kang Zheng (100%)
@co-author

----------------------------------------------------------------------------

@course     CSD2401F23
@section    CSD2401F23-A
@project    CSD2401F23 Software Engineering Project 3

@brief      Test Component for buttons in the UI
*//*______________________________________________________________________*/

#include "UIButton.h"
#include "EntityManager.h"
#include "Transform.h"
#include "CoreEngine.h"

#ifndef UI_BUTTON_TEST_H
#define UI_BUTTON_TEST_H

class UIButtonTest : public UIButton
{
public:

	/*!
	 * @brief
	 * Constructor for Test Button
	 * @param _id
	 * EntityID to initialise button with
	*/
	UIButtonTest(EntityID _id) : UIButton(_id) {};

	/*!
	 * @brief 
	 * Destructor for Test Button
	*/
	~UIButtonTest() {};

	/*!
	 * @brief 
	 * Called when the button is clicked.
	*/
	void OnClick()
	{
		if (CoreEngine::GetInstance()->mGameState == CoreEngine::GameState::GS_PAUSE)
		{
			CoreEngine::GetInstance()->mGameState = CoreEngine::GameState::GS_PLAY;
		}
		else if (CoreEngine::GetInstance()->mGameState == CoreEngine::GameState::GS_PLAY)
		{
			CoreEngine::GetInstance()->mGameState = CoreEngine::GameState::GS_PAUSE;
		}
	}

	/*!
	 * @brief 
	 * Called when the button is held.
	*/
	void OnHold()
	{
		EntityManager::GetInstance()->GetEntity(mEntID)->GetComponent<Transform>()->mRotation -= 1;
	}

	/*!
	 * @brief 
	 * Called when the button being hovered.
	*/
	void OnHoverEnter()
	{
#if _ENGINE
		DebugLogger::GetInstance()->Log("OnHoverEnter() | UIButton_Test\n");
#endif // _ENGINE
		EntityManager::GetInstance()->GetEntity(mEntID)->GetComponent<Transform>()->mScale.mX += 5;
		EntityManager::GetInstance()->GetEntity(mEntID)->GetComponent<Transform>()->mScale.mY += 5;
	}

	/*!
	 * @brief 
	 * Called when the button exiting hovered.
	*/
	void OnHoverExit()
	{
		EntityManager::GetInstance()->GetEntity(mEntID)->GetComponent<Transform>()->mScale.mX -= 5;
		EntityManager::GetInstance()->GetEntity(mEntID)->GetComponent<Transform>()->mScale.mY -= 5;
	}
};

#endif