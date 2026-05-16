/*!
All content © 2024 DigiPen Institute of Technology Singapore, all rights reserved.
@file       UIButton_ChangeScene.h
@author     TAN Kang Zheng (100%)
@co-author

----------------------------------------------------------------------------

@course     CSD2401F23
@section    CSD2401F23-A
@project    CSD2401F23 Software Engineering Project 3

@brief      Button Component to change scenes
*//*______________________________________________________________________*/

#include "UIButton.h"
#include "EntityManager.h"
#include "GameUITextHandler.h"
#include "SoundComponent.h"

#ifndef UI_BUTTON_KEYBIND_H
#define UI_BUTTON_KEYBIND_H

class UIButtonChangeScene : public UIButton
{
public:

	/*!
	 * @brief 
	 * Constructor for button to change scene
	 * @param _id 
	 * EntityID to initialise button with
	 * @param _scene 
	 * The name of the scene that will be changed to
	*/
	UIButtonChangeScene(EntityID _id, std::string _scene) : UIButton(_id), mScene(_scene) {};

	/*!
	 * @brief 
	 * Destructor for button to change scene
	*/
	~UIButtonChangeScene() {};

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

	/*!
	 * @brief
	 * Sets the scene that this button will change to
	 * @param _scene 
	 * The name of the scene
	*/
	void SetScene(std::string _scene);
private:
	std::string mScene;
};


#endif