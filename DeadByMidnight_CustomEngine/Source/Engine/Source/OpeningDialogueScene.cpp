/*!
All content © 2023 DigiPen Institute of Technology Singapore, all rights reserved.
@file       OpeningDialogueScene.cpp
@author		Jovan Zhuo Wen LOW (100%)
@co-author

----------------------------------------------------------------------------

@course     CSD2401F23
@section    CSD2401F23-A
@project    CSD2401F23 Software Engineering Project 3

@brief      Opening Dialogue Scene for the game
*//*______________________________________________________________________*/

#define UNREFERENCED_PARAMETER(P) (P)

#include "CoreEngine.h"
#include "InputManager.h"

#include "OpeningDialogueScene.h"

#include "WindowsManager.h"

OpeningDialogueScene::OpeningDialogueScene()
{

}

void OpeningDialogueScene::Init()
{
	currentTextDisplay = "";
	
	SceneManager::GetInstance()->totalDeathCount = 0;
	
	float codeTextSpeed = 2.0f;
	float codeTextPause = 0.05f;

	//JOVAN: DISCLAIMER, This is NOT actual hacking code. This is part of the bootup sequence's opening dialogue at the start of the game.
	entireDialogueOpeningData.push_back(DialogueOpeningData(TYPE_DIALOGUE, "", 1.3f, 1.0f));
	entireDialogueOpeningData.push_back(DialogueOpeningData(TYPE_DIALOGUE, "DarkNetV2 Kernel Version 1.0.0: Wed March 6 20:56:35 EST 2078; root:xnu-1699.22.73`1/RE\nLEASE_X86_64\n", codeTextPause, codeTextSpeed));
	entireDialogueOpeningData.push_back(DialogueOpeningData(TYPE_DIALOGUE, "\n", 0.2f, -1));
	entireDialogueOpeningData.push_back(DialogueOpeningData(TYPE_DIALOGUE, "kernel_page_bootstrap:987323 free pages and 53061 wired pages\n", codeTextPause, codeTextSpeed));
	entireDialogueOpeningData.push_back(DialogueOpeningData(TYPE_DIALOGUE, "kext submap [0xffffff7f8072e000 - 0xffffff8000000], kernel text [0xffffff7f8072e000 - 0\nxffffff8000000]\n", codeTextPause, codeTextSpeed));
	entireDialogueOpeningData.push_back(DialogueOpeningData(TYPE_DIALOGUE, "\n", 0.6f, -1));
	entireDialogueOpeningData.push_back(DialogueOpeningData(TYPE_DIALOGUE, "The Regents of the University of Panthea. All rights reserved.\n", codeTextPause, codeTextSpeed));
	entireDialogueOpeningData.push_back(DialogueOpeningData(TYPE_DIALOGUE, "MAC Framework successfully initialized\n", codeTextPause, codeTextSpeed));
	entireDialogueOpeningData.push_back(DialogueOpeningData(TYPE_DIALOGUE, "com.DarkNetV2.DarkNetV2/loaded: Safety net for TMSafety let for Rollback (THSafetyllet)\n", codeTextPause, codeTextSpeed));
	entireDialogueOpeningData.push_back(DialogueOpeningData(TYPE_DIALOGUE, "\n", 0.8f, -1));
	entireDialogueOpeningData.push_back(DialogueOpeningData(TYPE_DIALOGUE, "Successfully initialized headers and 10240 cluster 10 buffer headers\n", codeTextPause, codeTextSpeed));
	entireDialogueOpeningData.push_back(DialogueOpeningData(TYPE_DIALOGUE, "\n", 0.6f, -1));
	entireDialogueOpeningData.push_back(DialogueOpeningData(TYPE_DIALOGUE, "DarkNetV2 Intel(PUPower ManagementClient: ready BTCOEXIST\n", codeTextPause, codeTextSpeed));
	entireDialogueOpeningData.push_back(DialogueOpeningData(TYPE_DIALOGUE, "10: Broadcom BCM4331 802.11 Wireless Controller\n", codeTextPause, codeTextSpeed));
	entireDialogueOpeningData.push_back(DialogueOpeningData(TYPE_DIALOGUE, "\n", 0.3f, -1));
	entireDialogueOpeningData.push_back(DialogueOpeningData(TYPE_DIALOGUE, "5.100.98.75\n", codeTextPause, codeTextSpeed));
	entireDialogueOpeningData.push_back(DialogueOpeningData(TYPE_DIALOGUE, "LOGIN ATTEMPT BY USER: Byte \n", codeTextPause, codeTextSpeed));
	entireDialogueOpeningData.push_back(DialogueOpeningData(TYPE_DIALOGUE, "\n", 0.8f, -1));
	entireDialogueOpeningData.push_back(DialogueOpeningData(TYPE_DIALOGUE, "LOGIN AUTHORISED. \n", codeTextPause, codeTextSpeed));
	entireDialogueOpeningData.push_back(DialogueOpeningData(TYPE_DIALOGUE, "\n", 0.2f, -1));
	entireDialogueOpeningData.push_back(DialogueOpeningData(TYPE_DIALOGUE, "en1: 802.11d country code set to 'Panthea'.\n", codeTextPause, codeTextSpeed));
	entireDialogueOpeningData.push_back(DialogueOpeningData(TYPE_DIALOGUE, "\n", 0.0f, -1));
	entireDialogueOpeningData.push_back(DialogueOpeningData(TYPE_DIALOGUE, "Loaded os-config.sys: System Config Initialized Loaded xeserver.sys UX Graphics Rodule Loaded\n", codeTextPause, codeTextSpeed));
	entireDialogueOpeningData.push_back(DialogueOpeningData(TYPE_DIALOGUE, "\n", 0.8f, -1));
	entireDialogueOpeningData.push_back(DialogueOpeningData(TYPE_DIALOGUE, "Boot Complete\n", 0.75f, codeTextSpeed));
	entireDialogueOpeningData.push_back(DialogueOpeningData(TYPE_DIALOGUE, "\n", 0.2f, -1));
	entireDialogueOpeningData.push_back(DialogueOpeningData(TYPE_DIALOGUE, "Signal Detected: 1 incoming message detected\n", 0.75f, codeTextSpeed));
	entireDialogueOpeningData.push_back(DialogueOpeningData(TYPE_DIALOGUE, "\n", 0.2f, -1));
	entireDialogueOpeningData.push_back(DialogueOpeningData(TYPE_DIALOGUE, "Opening...\n", 0.75f, codeTextSpeed));
	entireDialogueOpeningData.push_back(DialogueOpeningData(TYPE_DIALOGUE, "\n", 0.8f, -1));
	entireDialogueOpeningData.push_back(DialogueOpeningData(TYPE_WIPE, "", 0.4f, -1));
	entireDialogueOpeningData.push_back(DialogueOpeningData(TYPE_DIALOGUE, "= 14 DAY TIMER EXPIRED : EXECUTING FAILSAFE =\n", 0.25f, 1.0f));
	entireDialogueOpeningData.push_back(DialogueOpeningData(TYPE_DIALOGUE, "========================\n", 0.0f, -1));
	entireDialogueOpeningData.push_back(DialogueOpeningData(TYPE_DIALOGUE, "\n", 0.0f, -1));
	entireDialogueOpeningData.push_back(DialogueOpeningData(TYPE_DIALOGUE, "AUTHORISED ON 21/02/2078, USER: 0PT1K\n", 0.25f, 0.7f));
	entireDialogueOpeningData.push_back(DialogueOpeningData(TYPE_DIALOGUE, "\n", 0.0f, -1));
	entireDialogueOpeningData.push_back(DialogueOpeningData(TYPE_DIALOGUE, "Loading message...\n", 0.6f, 0.7f));
	entireDialogueOpeningData.push_back(DialogueOpeningData(TYPE_DIALOGUE, "\n", 0.0f, -1));
	entireDialogueOpeningData.push_back(DialogueOpeningData(TYPE_DIALOGUE, "========================\n", 0.0f, -1));
	entireDialogueOpeningData.push_back(DialogueOpeningData(TYPE_DIALOGUE, "\n", 0.0f, -1));
	entireDialogueOpeningData.push_back(DialogueOpeningData(TYPE_DIALOGUE, "If you're reading this,", 0.5f, 0.5f));
	entireDialogueOpeningData.push_back(DialogueOpeningData(TYPE_DIALOGUE, " I've been missing for the past 14 days and things have gotten \nreally bad.", 1.25f, 0.5f));
	entireDialogueOpeningData.push_back(DialogueOpeningData(TYPE_DIALOGUE, "\n\n", 0.0f, -1));
	entireDialogueOpeningData.push_back(DialogueOpeningData(TYPE_DIALOGUE, "For months,", 0.5f, 0.5f));
	entireDialogueOpeningData.push_back(DialogueOpeningData(TYPE_DIALOGUE, " I've been running and covering my online activity,", 0.5f, 0.5f));
	entireDialogueOpeningData.push_back(DialogueOpeningData(TYPE_DIALOGUE, " but I always knew they \nwould get to me.", 1.25f, 0.5f));
	entireDialogueOpeningData.push_back(DialogueOpeningData(TYPE_DIALOGUE, "\n\n", 0.0f, -1));
	entireDialogueOpeningData.push_back(DialogueOpeningData(TYPE_DIALOGUE, "I know I promised to keep you and your family away from this,", 0.5f, 0.5f));
	entireDialogueOpeningData.push_back(DialogueOpeningData(TYPE_DIALOGUE, " but this has gotten too \nbig we don't have a choice anymore.", 1.25f, 0.5f));
	entireDialogueOpeningData.push_back(DialogueOpeningData(TYPE_DIALOGUE, "\n\n", 0.0f, -1));
	entireDialogueOpeningData.push_back(DialogueOpeningData(TYPE_DIALOGUE, "The things they plan to do with AXIOM...", 0.5f, 0.5f));
	entireDialogueOpeningData.push_back(DialogueOpeningData(TYPE_DIALOGUE, "\n\n", 0.0f, -1));
	entireDialogueOpeningData.push_back(DialogueOpeningData(TYPE_DIALOGUE, "It affects you,", 0.5f, 0.5f));
	entireDialogueOpeningData.push_back(DialogueOpeningData(TYPE_DIALOGUE, " me,", 0.5f, 0.5f));
	entireDialogueOpeningData.push_back(DialogueOpeningData(TYPE_DIALOGUE, " all of us.", 0.5f, 0.5f));
	entireDialogueOpeningData.push_back(DialogueOpeningData(TYPE_DIALOGUE, " Find and activate AXIOM's fail-safe.", 1.25f, 0.5f));
	entireDialogueOpeningData.push_back(DialogueOpeningData(TYPE_DIALOGUE, "\n\n", 0.0f, -1));
	entireDialogueOpeningData.push_back(DialogueOpeningData(TYPE_DIALOGUE, "We're counting on you.", 3.0f, 0.5f));

	//Reset camera zoom
	GraphicsSystem::GetInstance()->mCamera.mCurrCamInfo.mZoom = 1.f;
	GraphicsSystem::GetInstance()->mCamera.UpdateCameraInfo();

	for (size_t i{}; i < EntityManager::GetInstance()->mEntityList.size(); ++i)
	{
		if ("TerminalSound" == EntityManager::GetInstance()->mEntityList[i].GetName())
		{
			g_TerminalSound = EntityManager::GetInstance()->mEntityList[i].GetComponent<SoundComponent>();
			break;
		}
	}
}

void OpeningDialogueScene::Load()
{
	LevelLoader::LoadLevel(SceneManager::DemangleSceneName(typeid(OpeningDialogueScene).name()), GraphicsSystem::GetInstance()->mBackgroundColor);
}

void OpeningDialogueScene::Input(double _dt)
{
	UNREFERENCED_PARAMETER(_dt);
	if (InputManager::GetInstance()->GetActionTriggered("MenuPause") && (CoreEngine::GetInstance()->mGameState == CoreEngine::GameState::GS_PLAY))
	{
		CoreEngine::GetInstance()->mGameState = CoreEngine::GameState::GS_PAUSE;
		EventManager::GetInstance()->AddEvent(EventPlayPause(EventPlayPause::PAUSE));
		LevelLoader::LoadPrefab("PauseMenu Prefab");
	}
}

void OpeningDialogueScene::Update(double _dt)
{
	mTerminalTimer = std::clamp(mTerminalTimer - (float)_dt, 0.0f, mTerminalTime);
	if (mTerminalTimer <= FLT_EPSILON)
	{
		mTerminalIsPlaying = false;
	}

	if (mCurrentlyTyping)
	{
		if (g_TerminalSound != nullptr && !mTerminalIsPlaying)
		{
			g_TerminalSound->Play();
			mTerminalIsPlaying = true;
			mTerminalTimer = mTerminalTime;
		}

		mTimerPerChar -= (float)_dt * currentData.speed;
		if (InputManager::GetInstance()->GetAction("CheatFastforward"))
		{
			currentTextDisplay += currentData.text;
			currentDialogueDataIteration = "";
			mCurrentlyTyping = false;
		}
		else if (mTimerPerChar <= FLT_EPSILON)
		{
			mTimerPerChar += mTimePerChar;
			if (mCurrentDialogueDataIndex < currentData.text.size())
			{
				currentDialogueDataIteration += currentData.text.at(mCurrentDialogueDataIndex);
			}
			mCurrentDialogueDataIndex += 1;
			if (mCurrentDialogueDataIndex >= currentData.text.size())
			{
				currentTextDisplay += currentDialogueDataIteration;
				//currentTextDisplay += '\n';
				currentDialogueDataIteration = "";
				mCurrentlyTyping = false;
			}
		}
	}
	else
	{
		if (InputManager::GetInstance()->GetAction("CheatFastforward"))
			mTextWaitingTimer = std::clamp(mTextWaitingTimer - 10.0f * (float)_dt, 0.0f, 999.f);
		else
			mTextWaitingTimer = std::clamp(mTextWaitingTimer - (float)_dt, 0.0f, 999.f);
		if (mTextWaitingTimer <= FLT_EPSILON)
		{
			mTextVectorIndex++;
			if (mTextVectorIndex >= entireDialogueOpeningData.size())
			{
				WindowsManager::GetInstance()->mSceneOpacity = 0.0f;
				if (mChangeSceneDelayTimer < FLT_EPSILON)
				{
					mChangeSceneDelayTimer = mChangeSceneDelayTime;
				}
			}
			else if (mTextVectorIndex >= 0)
			{
				currentData = entireDialogueOpeningData.at(mTextVectorIndex);
				if (currentData.datatype == TYPE_DIALOGUE)
				{
					if (currentData.speed >= 0.0f)
					{
						mCurrentlyTyping = true;
						mTimerPerChar = mTimePerChar;
						mCurrentDialogueDataIndex = 0;

						int excessstuff = (int)(std::count(currentTextDisplay.begin(), currentTextDisplay.end(), '\n') + std::count(currentData.text.begin(), currentData.text.end(), '\n'));
						if (excessstuff > 18)
						{
							for (int i = 0; i < excessstuff - 18; ++i)
							{
								size_t lenToDestroy = currentTextDisplay.find_first_of('\n');
								currentTextDisplay = currentTextDisplay.substr(lenToDestroy + 1, currentTextDisplay.length() - lenToDestroy);
							}
						}
					}
					else
					{
						currentTextDisplay += currentData.text;
					}
				}
				else if (currentData.datatype == TYPE_WIPE)
				{
					currentTextDisplay = "";
				}
				mTextWaitingTimer = currentData.postTextWaitingTime;
			}
		}
	}

	mTypelineTimer = std::clamp(mTypelineTimer - float(_dt), 0.0f, mTypelineTime);
	if (mTypelineTimer <= FLT_EPSILON)
	{
		mTypelineTimer = mTypelineTime;
		mTypelinePresent = !mTypelinePresent;
	}

	std::string textToDisplay;
	if (mTypelinePresent)
		textToDisplay = currentTextDisplay + currentDialogueDataIteration + "_";
	else
		textToDisplay = currentTextDisplay + currentDialogueDataIteration;

	TextManager::GetInstance()->SetOpeningDialogueText(textToDisplay, fontName);

	if (mTextVectorIndex >= entireDialogueOpeningData.size() && WindowsManager::GetInstance()->mSceneTrueOpacity <= FLT_EPSILON)
	{
		mChangeSceneDelayTimer -= (float)_dt;
		if (mChangeSceneDelayTimer <= FLT_EPSILON)
		{
			GoToGameScene();
		}
	}
}

void OpeningDialogueScene::Pause(double _dt)
{
	UNREFERENCED_PARAMETER(_dt);
}

void OpeningDialogueScene::Render(double _dt)
{
	UNREFERENCED_PARAMETER(_dt);
}

void OpeningDialogueScene::Free()
{

}

void OpeningDialogueScene::Exit()
{
	SoundManager::GetInstance()->StopChannel(SoundManager::SoundType_SFX);
	SoundManager::GetInstance()->StopChannel(SoundManager::SoundType_BGM);
}

void OpeningDialogueScene::GoToGameScene()
{
#if _ENGINE
	DebugLogger::GetInstance()->Log("Attempting to Load Scene: %s\n", mScene.c_str());
#endif // _ENGINE

	currentTextDisplay = "";
	currentDialogueDataIteration = "";
	mTypelineTimer = mTypelineTime;
	mTypelinePresent = false;
	TextManager::GetInstance()->SetOpeningDialogueText("", fontName);
	SceneManager::GetInstance()->SetActiveScene(SceneManager::GetSceneFromName(mScene));
}