/*!
All content © 2023 DigiPen Institute of Technology Singapore, all rights reserved.
@file       UIButton_PauseMenu.h
@author     TAN Kang Zheng (100%)
@co-author

----------------------------------------------------------------------------

@course     CSD2401F23
@section    CSD2401F23-A
@project    CSD2401F23 Software Engineering Project 3

@brief      Button Component to restore the pause menu
*//*______________________________________________________________________*/

#include "UIButton.h"
#include "EntityManager.h"
#include "GameUITextHandler.h"
#include "SoundComponent.h"

#ifndef UI_BUTTON_PAUSE_MENU_H
#define UI_BUTTON_PAUSE_MENU_H

class UIButtonPauseMenu : public UIButton
{
public:

	/*!
	 * @brief 
	 * Constructor for Pause Menu
	 * @param _id 
	 * EntityID to initialise button with
	*/
	UIButtonPauseMenu(EntityID _id) : UIButton(_id) {};

	/*!
	 * @brief 
	 * Destructor for Pause Menu
	*/
	~UIButtonPauseMenu() {};

	/*!
	 * @brief 
	 * Called when the button is clicked.
	*/
	void OnClick();

	/*!
	 * @brief 
	 * Called when the button is held.
	*/
	void OnHold() {};

	/*!
	 * @brief 
	 * Called when the button being hovered.
	*/
	void OnHoverEnter();

	/*!
	 * @brief 
	 * Called when the button exiting hovered.
	*/
	void OnHoverExit();
};

#endif