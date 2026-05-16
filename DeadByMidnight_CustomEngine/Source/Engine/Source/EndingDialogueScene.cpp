/*!
All content © 2023 DigiPen Institute of Technology Singapore, all rights reserved.
@file       EndingDialogueScene.cpp
@author		Jovan Zhuo Wen LOW (100%)
@co-author

----------------------------------------------------------------------------

@course     CSD2401F23
@section    CSD2401F23-A
@project    CSD2401F23 Software Engineering Project 3

@brief      Ending Dialogue Scene for the game
*//*______________________________________________________________________*/

#define UNREFERENCED_PARAMETER(P) (P)

#include "CoreEngine.h"
#include "InputManager.h"

#include "EndingDialogueScene.h"

#include "WindowsManager.h"

EndingDialogueScene::EndingDialogueScene()
{

}

void EndingDialogueScene::Init()
{
	currentTextDisplay = "";

	SceneManager::GetInstance()->totalDeathCount = 0;
	
	//float codeTextSpeed = 2.0f;
	//float codeTextPause = 0.05f;

	//JOVAN: DISCLAIMER again, This is NOT actual hacking code.
	entireDialogueOpeningData.push_back(DialogueOpeningData(TYPE_DIALOGUE, "", 1.3f, 1.0f));
	entireDialogueOpeningData.push_back(DialogueOpeningData(TYPE_DIALOGUE, "= AXIOM TERMINATED : EXECUTING PAYLOAD =\n", 0.25f, 1.0f));
	entireDialogueOpeningData.push_back(DialogueOpeningData(TYPE_DIALOGUE, "========================\n", 0.0f, -1));
	entireDialogueOpeningData.push_back(DialogueOpeningData(TYPE_DIALOGUE, "\n", 0.0f, -1));
	entireDialogueOpeningData.push_back(DialogueOpeningData(TYPE_DIALOGUE, "AUTHORISED ON 21/02/2078, USER: 0PT1K\n", 0.25f, 0.7f));
	entireDialogueOpeningData.push_back(DialogueOpeningData(TYPE_DIALOGUE, "\n", 0.0f, -1));
	entireDialogueOpeningData.push_back(DialogueOpeningData(TYPE_DIALOGUE, "Loading message...\n", 0.6f, 0.7f));
	entireDialogueOpeningData.push_back(DialogueOpeningData(TYPE_DIALOGUE, "\n", 0.0f, -1));
	entireDialogueOpeningData.push_back(DialogueOpeningData(TYPE_DIALOGUE, "========================\n", 0.0f, -1));
	entireDialogueOpeningData.push_back(DialogueOpeningData(TYPE_DIALOGUE, "\n", 0.0f, -1));
	entireDialogueOpeningData.push_back(DialogueOpeningData(TYPE_DIALOGUE, "If you're reading this,", 0.5f, 0.5f));
	entireDialogueOpeningData.push_back(DialogueOpeningData(TYPE_DIALOGUE, " it could only mean one thing.", 1.25f, 0.5f));
	entireDialogueOpeningData.push_back(DialogueOpeningData(TYPE_DIALOGUE, "\n\n", 0.0f, -1));
	entireDialogueOpeningData.push_back(DialogueOpeningData(TYPE_DIALOGUE, "You managed to destroy AXIOM for good.", 1.25f, 0.5f));
	entireDialogueOpeningData.push_back(DialogueOpeningData(TYPE_DIALOGUE, "\n\n", 0.0f, -1));
	entireDialogueOpeningData.push_back(DialogueOpeningData(TYPE_DIALOGUE, "For that,", 0.5f, 0.5f));
	entireDialogueOpeningData.push_back(DialogueOpeningData(TYPE_DIALOGUE, " I send you my highest salutations,", 0.5f, 0.5f));
	entireDialogueOpeningData.push_back(DialogueOpeningData(TYPE_DIALOGUE, " from the bottom of my heart.", 1.25f, 0.5f));
	entireDialogueOpeningData.push_back(DialogueOpeningData(TYPE_DIALOGUE, "\n\n", 0.0f, -1));
	entireDialogueOpeningData.push_back(DialogueOpeningData(TYPE_DIALOGUE, "Though many of us are no longer around,", 0.5f, 0.5f));
	entireDialogueOpeningData.push_back(DialogueOpeningData(TYPE_DIALOGUE, " you may rest assured,", 0.5f, 0.5f));
	entireDialogueOpeningData.push_back(DialogueOpeningData(TYPE_DIALOGUE, " knowing that your actions\nhave brought solace for the future of humanity.", 1.25f, 0.5f));
	entireDialogueOpeningData.push_back(DialogueOpeningData(TYPE_DIALOGUE, "\n\n", 0.0f, -1));
	entireDialogueOpeningData.push_back(DialogueOpeningData(TYPE_DIALOGUE, "With AXIOM gone,", 0.5f, 0.5f));
	entireDialogueOpeningData.push_back(DialogueOpeningData(TYPE_DIALOGUE, " our hopes and dreams can continue to flourish,", 0.5f, 0.5f));
	entireDialogueOpeningData.push_back(DialogueOpeningData(TYPE_DIALOGUE, " and mankind will\nrebuild itself amidst the rubble.", 1.25f, 0.5f));
	entireDialogueOpeningData.push_back(DialogueOpeningData(TYPE_DIALOGUE, "\n\n", 0.0f, -1));
	entireDialogueOpeningData.push_back(DialogueOpeningData(TYPE_DIALOGUE, "For the freedom of the world,", 0.5f, 0.5f));
	entireDialogueOpeningData.push_back(DialogueOpeningData(TYPE_DIALOGUE, " the cyberspace,", 0.5f, 0.5f));
	entireDialogueOpeningData.push_back(DialogueOpeningData(TYPE_DIALOGUE, " and all things beyond.", 1.25f, 0.5f));
	entireDialogueOpeningData.push_back(DialogueOpeningData(TYPE_DIALOGUE, "\n\n", 0.0f, -1));
	entireDialogueOpeningData.push_back(DialogueOpeningData(TYPE_DIALOGUE, "You've done the world an insurmountable favor,", 0.5f, 0.5f));
	entireDialogueOpeningData.push_back(DialogueOpeningData(TYPE_DIALOGUE, " and at last,", 0.5f, 0.5f));
	entireDialogueOpeningData.push_back(DialogueOpeningData(TYPE_DIALOGUE, " I can rest in peace.", 1.25f, 0.5f));
	entireDialogueOpeningData.push_back(DialogueOpeningData(TYPE_DIALOGUE, "\n\n", 0.0f, -1));
	entireDialogueOpeningData.push_back(DialogueOpeningData(TYPE_DIALOGUE, "...", 1.25f, 0.5f));
	entireDialogueOpeningData.push_back(DialogueOpeningData(TYPE_DIALOGUE, "\n\n", 0.0f, -1));
	entireDialogueOpeningData.push_back(DialogueOpeningData(TYPE_DIALOGUE, "Thank you.", 3.0f, 0.5f));

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

void EndingDialogueScene::Load()
{
	LevelLoader::LoadLevel(SceneManager::DemangleSceneName(typeid(EndingDialogueScene).name()), GraphicsSystem::GetInstance()->mBackgroundColor);
}

void EndingDialogueScene::Input(double _dt)
{
	UNREFERENCED_PARAMETER(_dt);
	if (InputManager::GetInstance()->GetActionTriggered("MenuPause") && (CoreEngine::GetInstance()->mGameState == CoreEngine::GameState::GS_PLAY))
	{
		CoreEngine::GetInstance()->mGameState = CoreEngine::GameState::GS_PAUSE;
		EventManager::GetInstance()->AddEvent(EventPlayPause(EventPlayPause::PAUSE));
		LevelLoader::LoadPrefab("PauseMenu Prefab");
	}
}

void EndingDialogueScene::Update(double _dt)
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
			GoToCreditsScene();
		}
	}
}

void EndingDialogueScene::Pause(double _dt)
{
	UNREFERENCED_PARAMETER(_dt);
}

void EndingDialogueScene::Render(double _dt)
{
	UNREFERENCED_PARAMETER(_dt);
}

void EndingDialogueScene::Free()
{

}

void EndingDialogueScene::Exit()
{
	SoundManager::GetInstance()->StopChannel(SoundManager::SoundType_SFX);
	SoundManager::GetInstance()->StopChannel(SoundManager::SoundType_BGM);
}

void EndingDialogueScene::GoToCreditsScene()
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