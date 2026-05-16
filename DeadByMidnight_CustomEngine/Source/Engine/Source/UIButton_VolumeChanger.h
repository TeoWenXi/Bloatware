/*!
All content © 2024 DigiPen Institute of Technology Singapore, all rights reserved.
@file       UIButton_VolumeChanger.h
@author     TAN Kang Zheng (100%)
@co-author

----------------------------------------------------------------------------

@course     CSD2401F23
@section    CSD2401F23-A
@project    CSD2401F23 Software Engineering Project 3

@brief      Button Component to change the volume level of different channels
*//*______________________________________________________________________*/

#include "UIButton.h"
#include "EntityManager.h"
#include "GameUITextHandler.h"

#ifndef UI_BUTTON_VOLUME_CHANGER_H
#define UI_BUTTON_VOLUME_CHANGER_H

class UIButtonVolumeChanger : public UIButton
{
public:

	enum ChannelType {
		SFX,
		BGM,
		MASTER
	};

	/*!
	 * @brief
	 * Constructor for button to adjust volumes
	*/
	UIButtonVolumeChanger(EntityID _id, ChannelType _type) 
		: UIButton(_id)
		, mChannelType(_type)
		, mActive(false)
	{};

	/*!
	 * @brief
	 * Destructor for button to change scene
	*/
	~UIButtonVolumeChanger() {};

	/*!
	 * @brief
	 * Called when the button is clicked.
	*/
	void OnClick() {};

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

	/*!
	 * @brief
	 * Update function for this button
	*/
	void Update();

	/*!
	 * @brief
	 * Change the type of channel this button adjusts
	*/
	void ChangeChannelType(ChannelType _type);
private:
	ChannelType mChannelType;
	bool mActive;
};

#endif