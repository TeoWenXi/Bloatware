/*!
All content © 2024 DigiPen Institute of Technology Singapore, all rights reserved.
@file       UIButton_ChangeScene.h
@author     TAN Kang Zheng (100%)
@co-author

----------------------------------------------------------------------------

@course     CSD2451F24
@section    CSD2451F24-A
@project    CSD2451F24 Software Engineering Project 4

@brief      Button Component to edit keybinds
*//*______________________________________________________________________*/

#include "UIButton.h"
#include "EntityManager.h"
#include "GameUITextHandler.h"
#include "SoundComponent.h"
#include "InputManager.h"

#ifndef UI_BUTTON_CHANGESCENE_H
#define UI_BUTTON_CHANGESCENE_H

class UIButtonKeybind : public UIButton
{
public:

	/*!
	 * @brief 
	 * Constructor for Editing Keybinds
	 * @param _id 
	 * EntityID to initialise button with
	 * @param _keybind 
	 * The keybind that this button will modify
	*/
	UIButtonKeybind(EntityID _id, std::string _keybind) : UIButton(_id), mKeybind(_keybind) {};

	/*!
	 * @brief 
	 * Destructor for Editing Keybinds
	*/
	~UIButtonKeybind() {};

	/*!
	 * @brief Called when the button is clicked.
	*/
	void OnClick();

	/*!
	 * @brief Called when the button is held.
	*/
	void OnHold() {};

	/*!
	 * @brief Called when the button being hovered.
	*/
	void OnHoverEnter();

	/*!
	 * @brief Called when the button exiting hovered.
	*/
	void OnHoverExit();

	/*!
	 * @brief 
	 * Update function for the button
	*/
	void Update();

	/*!
	 * @brief
	 * Returns a reference to the mKeybind string
	 * return
	 * A reference to the mKeybind string
	*/
	std::string& GetKeybind();

private:
	std::string mKeybind;
};


#endif