/*!
All content © 2023 DigiPen Institute of Technology Singapore, all rights reserved.
@file       UIButton_RestartStart.h
@author     TAN Kang Zheng (100%)
@co-author

----------------------------------------------------------------------------

@course     CSD2401F23
@section    CSD2401F23-A
@project    CSD2401F23 Software Engineering Project 3

@brief      Button Component to restart the game
*//*______________________________________________________________________*/

#include "UIButton.h"

#ifndef UI_BUTTON_RESTART_START_H
#define UI_BUTTON_RESTART_START_H

class UIButtonRestartStart : public UIButton
{
public:

	/*!
	 * @brief 
	 * Constructor for Restart button
	 * @param _id 
	 * EntityID to initialise button with
	*/
	UIButtonRestartStart(EntityID _id) : UIButton(_id) {};

	/*!
	 * @brief 
	 * Destructor for Restart button
	*/
	~UIButtonRestartStart() {};

	/*!
	 * @brief 
	 * Called when the button is clicked.
	*/
	void OnClick();

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
	void OnHoverEnter();

	/*!
	 * @brief 
	 * Called when the button exiting hovered.
	*/
	void OnHoverExit();
};

#endif