/*!
All content © 2023 DigiPen Institute of Technology Singapore, all rights reserved.
@file       ButtonWrapper.h
@author     TAN Kang Zheng (100%)
@co-author

----------------------------------------------------------------------------

@course     CSD2401F23
@section    CSD2401F23-A
@project    CSD2401F23 Software Engineering Project 3

@brief      Base Wrapper Component for buttons in the UI
*//*______________________________________________________________________*/

#ifndef BUTTON_WRAPPER_COMPONENT_H
#define BUTTON_WRAPPER_COMPONENT_H

#include "ComponentBase.h"

#include "UIButton_Test.h"
#include "UIButton_Unpause.h"
#include "UIButton_CloseMenu.h"
#include "UIButton_ExitGame.h"
#include "UIButton_StartGame.h"
#include "UIButton_ExitApp.h"
#include "UIButton_PauseMenu.h"
#include "UIButton_HowToPlay.h"
#include "UIButton_ExitMenu.h"
#include "UIButton_RestartStart.h"
#include "UIButton_SpawnPrefab.h"
#include "UIButton_ChangeScene.h"
#include "UIButton_VolumeChanger.h"
#include "UIButton_Keybind.h"

class ButtonWrapper : public ComponentBase
{
public:

	//ButtonWrapper() : mButtonObj(UIButtonTest(mEntID)), mButtonType(BUTTON_TYPE::TEST) {};

	/*!
	 * @brief
	 * Init virtual function for components
	*/
	void Init();

	/*!
	 * @brief
	 * Load virtual function for components
	*/
	void Load();

	/*!
	 * @brief
	 * Save virtual function for components
	 * Each individual button type must save its name
	*/
	void Save();

	/*!
	 * @brief
	 * Update virtual function for components
	*/
	void Update();

	/*!
	 * @brief
	 * Exit virtual function for components
	*/
	void Exit();

	/*!
	 * @brief
	 * DisplayProperties virtual function for components
	*/
	void DisplayProperties();

	/*!
	 * @brief
	 * Automatic logging for the creation of a ButtonWrapper
	 * @param os
	 * The output stream to write to
	 * @param _handler
	 * Reference to the ButtonWrapper
	 * @return
	 * The output stream to write to
	*/
	friend std::ostream& operator<< (std::ostream& os, const ButtonWrapper& _handler)
	{
		os << "ButtonWrapper tied to Ent ID " << _handler.mEntID;
		return os;
	}

	/*!
	 * @brief
	 * Initializes the mButtonObj based on the current button type
	*/
	void LoadButtonType();

	UIButton* mButtonObj = nullptr;

	enum BUTTON_TYPE : int
	{
		NONE = 0,
		TEST,
		EXIT_GAME, //Game -> Main Menu
		UNPAUSE, //Unpause game
		RESTART_CHECKPOINT, //Button to restart from checkpoint
		RESTART_START, //Button to restart from beginning
		EXIT_APP, //Closes the whole application
		CLOSE_MENU, //Close the current menu (aka delete entity + all child)
		START_GAME, //Main Menu -> Game
		EXIT_GAME_MENU, //Pause Menu -> Exit
		HOW_TO_PLAY, //Pause Menu -> How To Play
		PAUSE_MENU, //?? -> Pause Menu
		SPAWN_PREFAB, //Spawns a given prefab
		CHANGE_SCENE,  //Sets the next scene
		VOLUME_CHANGER, //Adjusts the channels volumes
		KEYBIND        //Changes a keybind
	};
	BUTTON_TYPE mButtonType = BUTTON_TYPE::NONE;
	int mButtonOrderPosition = 0;

	//For spawnPrefab
	std::string mInputField = "";

	//For Volume Changer
	UIButtonVolumeChanger::ChannelType mChannelType;
};

#endif
