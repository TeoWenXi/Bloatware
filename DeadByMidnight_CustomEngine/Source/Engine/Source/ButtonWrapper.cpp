/*!
All content © 2023 DigiPen Institute of Technology Singapore, all rights reserved.
@file       ButtonWrapper.cpp
@author     TAN Kang Zheng (100%)
@co-author

----------------------------------------------------------------------------

@course     CSD2401F23
@section    CSD2401F23-A
@project    CSD2401F23 Software Engineering Project 3

@brief      Base Wrapper Component for buttons in the UI
*//*______________________________________________________________________*/

#include "ButtonWrapper.h"
#include "GameUIManager.h"
#include "GameUIObjectHandler.h"
#include "InputManager.h"
#include "ButtonSelectManager.h"

#if _ENGINE
#include "MyImGui.h"
#include <imgui.h>
#include <imgui_internal.h>
#endif // _ENGINE

void ButtonWrapper::Init()
{
	GameUIManager::GetInstance()->AddButton(mEntID);
	ButtonSelectManager::GetInstance()->AddButton(mEntID);
}

void ButtonWrapper::Load()
{
	mButtonType = (BUTTON_TYPE)JSONManager::GetInstance()->GetInt("ButtonType");
	mButtonOrderPosition = JSONManager::GetInstance()->GetInt("ButtonOrderPosition");
	mInputField = JSONManager::GetInstance()->GetString("InputField");
	//mChannelType = (UIButtonVolumeChanger::ChannelType)(JSONManager::GetInstance()->GetInt("ChannelType"));
	if (JSONManager::GetInstance()->GetInt("ChannelType") < 0)
		mChannelType = (UIButtonVolumeChanger::ChannelType)(0);
	else
		mChannelType = (UIButtonVolumeChanger::ChannelType)(JSONManager::GetInstance()->GetInt("ChannelType"));
}

void ButtonWrapper::LoadButtonType()
{
	switch (mButtonType)
	{
	case ButtonWrapper::TEST:
		mButtonObj = new UIButtonTest(mEntID);
		break;
	case ButtonWrapper::EXIT_GAME:
		mButtonObj = new UIButtonExitGame(mEntID);
		break;
	case ButtonWrapper::UNPAUSE:
		mButtonObj = new UIButtonUnpause(mEntID);
		break;
	case ButtonWrapper::CLOSE_MENU:
		mButtonObj = new UIButtonCloseMenu(mEntID);
		break;
	case ButtonWrapper::EXIT_APP:
		mButtonObj = new UIButtonExitApp(mEntID);
		break;
	case ButtonWrapper::START_GAME:
		mButtonObj = new UIButtonStartGame(mEntID);
		break;
	case ButtonWrapper::PAUSE_MENU:
		mButtonObj = new UIButtonPauseMenu(mEntID);
		break;
	case ButtonWrapper::HOW_TO_PLAY:
		mButtonObj = new UIButtonHowToPlay(mEntID);
		break;
	case ButtonWrapper::EXIT_GAME_MENU:
		mButtonObj = new UIButtonExitMenu(mEntID);
		break;
	case ButtonWrapper::RESTART_START:
		mButtonObj = new UIButtonRestartStart(mEntID);
		break;
	case ButtonWrapper::SPAWN_PREFAB:
		mButtonObj = new UIButtonSpawnPrefab(mEntID, mInputField);
		break;
	case ButtonWrapper::CHANGE_SCENE:
		mButtonObj = new UIButtonChangeScene(mEntID, mInputField);
		break;
	case ButtonWrapper::VOLUME_CHANGER:
		mButtonObj = new UIButtonVolumeChanger(mEntID, mChannelType);
		break;
	case ButtonWrapper::KEYBIND:
		mButtonObj = new UIButtonKeybind(mEntID, mInputField);
		break;
	default:
		mButtonObj = nullptr;
		break;
	}
}

void ButtonWrapper::Save()
{
	JSONManager::GetInstance()->Save("Component", "ButtonWrapper");
	JSONManager::GetInstance()->Save("ButtonType", (int)mButtonType);
	JSONManager::GetInstance()->Save("ButtonOrderPosition", mButtonOrderPosition);
	JSONManager::GetInstance()->Save("InputField", mInputField);
	JSONManager::GetInstance()->Save("ChannelType", (int)mChannelType);
}

void ButtonWrapper::Update()
{
	if(mButtonObj != nullptr)
	{
		switch (mButtonType)
		{
		case UNPAUSE:
		{
			if (InputManager::GetInstance()->GetActionTriggered("MenuPause"))
			{
				mButtonObj->OnClick();
			}
		}
			break;
		case VOLUME_CHANGER:
		{
			UIButtonVolumeChanger* _btnPtr = (UIButtonVolumeChanger*)mButtonObj;
			if (_btnPtr != nullptr)
			{
				_btnPtr->Update();
			}
		}
			break;
		case KEYBIND:
		{
			UIButtonKeybind* _btnPtr = (UIButtonKeybind*)mButtonObj;
			if (_btnPtr != nullptr)
			{
				_btnPtr->Update();
			}
		}
			break;
		default:
			break;
		}
	}
	else
	{
		LoadButtonType();
	}
}

void ButtonWrapper::Exit()
{
	GameUIManager::GetInstance()->RemoveButton(mEntID);
	ButtonSelectManager::GetInstance()->RemoveButton(mEntID);
	if (mButtonObj != nullptr)
		delete mButtonObj;
}

void ButtonWrapper::DisplayProperties()
{
#if _ENGINE
	const char* items[] = { "None",
							"Test",
							"Exit_Game",
							"Unpause",
							"Restart_Checkpoint",
							"Restart_Start",
							"Exit_App",
							"Close_Menu",
							"Start_Game",
							"Exit_Game_Menu",
							"How_To_Play",
							"Pause_Menu",
							"SpawnPrefab",
							"ChangeScene",
							"VolumeChanger",
							"Keybind"
						  };
	const char* current_item = items[(int)mButtonType];

	ImGui::Text("Button Type:");
	if (ImGui::BeginCombo("##Button Type", current_item)) // The second parameter is the label previewed before opening the combo.
	{
		for (int n = 0; n < IM_ARRAYSIZE(items); n++)
		{
			bool is_selected = (current_item == items[n]); // You can store your selection however you want, outside or inside your objects
			if (ImGui::Selectable(items[n], is_selected))
			{
				current_item = items[n];
				if (n != (int)mButtonType) //If choosen button type is different
				{
					delete mButtonObj;
					mButtonType = BUTTON_TYPE(n);
					LoadButtonType();
				}
			}
				if (is_selected)
					ImGui::SetItemDefaultFocus();   // You may set the initial focus when opening the combo (scrolling + for keyboard navigation support)
		}
		ImGui::EndCombo();
	}
	if (mButtonType == SPAWN_PREFAB)
	{
		MyImGui::MyImGuiStringInput(mInputField, "Prefab Name");
		UIButtonSpawnPrefab* _btnPtr = (UIButtonSpawnPrefab*)mButtonObj;
		if(_btnPtr != nullptr)
		{
			_btnPtr->SetPrefab(mInputField);
		}
	}
	if (mButtonType == CHANGE_SCENE)
	{
		MyImGui::MyImGuiStringInput(mInputField, "Scene Name");
		UIButtonChangeScene* _btnPtr = (UIButtonChangeScene*)mButtonObj;
		if (_btnPtr != nullptr)
		{
			_btnPtr->SetScene(mInputField);
		}
	}
	if (mButtonType == VOLUME_CHANGER)
	{
		const char* volumeitems[] = {	"SFX",
										"BGM",
										"MASTER"
		};
		const char* volumecurrent_item = volumeitems[(int)mChannelType];

		ImGui::Text("Channel Type:");
		if (ImGui::BeginCombo("##Channel Type", volumecurrent_item)) // The second parameter is the label previewed before opening the combo.
		{
			for (int n = 0; n < IM_ARRAYSIZE(volumeitems); n++)
			{
				bool is_selected = (volumecurrent_item == volumeitems[n]); // You can store your selection however you want, outside or inside your objects
				if (ImGui::Selectable(volumeitems[n], is_selected))
				{
					volumecurrent_item = volumeitems[n];
					if (n != (int)mChannelType) //If choosen button type is different
					{
						mChannelType = (UIButtonVolumeChanger::ChannelType)(n);
						UIButtonVolumeChanger* _btnPtr = (UIButtonVolumeChanger*)mButtonObj;
						if (_btnPtr != nullptr)
						{
							_btnPtr->ChangeChannelType((UIButtonVolumeChanger::ChannelType)(n));
						}
					}
				}
				if (is_selected)
					ImGui::SetItemDefaultFocus();   // You may set the initial focus when opening the combo (scrolling + for keyboard navigation support)
			}
			ImGui::EndCombo();
		}
	}
	if (mButtonType == KEYBIND)
	{
		if (mButtonObj != nullptr)
		{
			
			MyImGui::MyImGuiStringInput(mInputField, "Keybind");

			UIButtonKeybind* _btnPtr = (UIButtonKeybind*)mButtonObj;
			_btnPtr->GetKeybind() = mInputField;
		}
	}
	MyImGui::MyImGuiIntInput(mButtonOrderPosition, "ButtonOrderPosition");
#endif // _ENGINE
}