/*!
All content © 2023 DigiPen Institute of Technology Singapore, all rights reserved.
@file       UIButton.h
@author     TAN Kang Zheng (100%)
@co-author

----------------------------------------------------------------------------

@course     CSD2401F23
@section    CSD2401F23-A
@project    CSD2401F23 Software Engineering Project 3

@brief      Base Component for buttons in the UI
*//*______________________________________________________________________*/

#ifndef UI_BUTTON_H
#define UI_BUTTON_H

#include "ECS_Types.h"

#if _ENGINE
#include "DebugLogger.h"
#endif // _ENGINE

class UIButton
{
public:

	/*!
	 * @brief 
	 * Constructor for buttons
	 * @param _id 
	 * EntityID to initialise button with
	*/
	UIButton(EntityID _id) : mEntID(_id) {
#if _ENGINE
		DebugLogger::GetInstance()->Log("Button Initiated with mEntID %d\n", mEntID);
#endif // _ENGINE
	};

	/*!
	 * @brief
	 * Virtual destructor for buttons
	*/
	virtual ~UIButton();

	/*!
	 * @brief
	 * Update function run every loop
	*/
	void Update();

	/*!
	 * @brief
	 * Virtual function for all inherited classes. Called when the button is clicked.
	*/
	virtual void OnClick() = 0;

	/*!
	 * @brief
	 * Virtual function for all inherited classes. Called when the button is held.
	*/
	virtual void OnHold() = 0;

	/*!
	 * @brief
	 * Virtual function for all inherited classes. Called when the button being hovered.
	*/
	virtual void OnHoverEnter() = 0;

	/*!
	 * @brief
	 * Virtual function for all inherited classes. Called when the button exiting hovered.
	*/
	virtual void OnHoverExit() = 0;

private:
	bool lastFrameHover = false;
	bool lastFramePress = false;
public:
	EntityID mEntID;
};

#endif
