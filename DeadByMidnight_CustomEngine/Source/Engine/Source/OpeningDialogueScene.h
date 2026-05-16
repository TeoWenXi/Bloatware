/*!
All content © 2023 DigiPen Institute of Technology Singapore, all rights reserved.
@file       OpeningDialogueScene.h
@author		Jovan Zhuo Wen LOW (100%)
@co-author

----------------------------------------------------------------------------

@course     CSD2401F23
@section    CSD2401F23-A
@project    CSD2401F23 Software Engineering Project 3

@brief      Opening Dialogue Scene for the game
*//*______________________________________________________________________*/

#ifndef OPENING_DIALOGUE_SCENE_H
#define OPENING_DIALOGUE_SCENE_H

#include "Scene.h"
#include "SceneManager.h"
#include "EntityManager.h"
#include "ComponentsHeader.h"
#include "LevelLoader.h"

enum DIALOGUE_OPENING_DATA_TYPE
{
	TYPE_DIALOGUE,
	TYPE_WIPE,
	TYPE_COUNT
};

class DialogueOpeningData
{
public:
	DIALOGUE_OPENING_DATA_TYPE datatype = TYPE_DIALOGUE;
	std::string text = "";
	float postTextWaitingTime = 0.2f;
	float speed = 1.0f;

	DialogueOpeningData() {}

	DialogueOpeningData(DIALOGUE_OPENING_DATA_TYPE _datatype, std::string _text, float _postTextWaitingTime, float _speed = 1.0f)
		: datatype(_datatype), text(_text), postTextWaitingTime(_postTextWaitingTime), speed(_speed)
	{

	}
};

class OpeningDialogueScene : public Scene
{
public:
	//Scene functions
	/*!
	 * @brief
	 * Load function for the opening dialogue scene
	*/
	void Load();

	/*!
	 * @brief
	 * Init function for the opening dialogue scene
	*/
	void Init();

	/*!
	 * @brief
	 * Input function for the opening dialogue scene
	 * @param _dt
	 * Delta time for the program
	*/
	void Input(double _dt);

	/*!
	 * @brief 
	 * Update function for the opening dialogue scene
	 * @param _dt
	 * Delta time for the program
	*/
	void Update(double _dt);

	/*!
	 * @brief
	 * Pause function loop for the opening dialogue scene
	 * @param _dt
	 * Delta time for the program
	*/
	void Pause(double _dt);

	/*!
	 * @brief
	 * Render function for the opening dialogue scene
	 * @param _dt
	 * Delta time for the program
	*/
	void Render(double _dt);

	/*!
	 * @brief 
	 * Free function for the opening dialogue scene. Should clear all memory at this stage.
	*/
	void Free();

	/*!
	 * @brief Exit function for the opening dialogue scene.
	*/
	void Exit();

	/*!
	 * @brief Loads Game Scene
	*/
	void GoToGameScene();


	OpeningDialogueScene();

private:
	std::string mScene = "GameScene";
	std::vector<DialogueOpeningData> entireDialogueOpeningData;
	std::string currentTextDisplay = "";
	std::string currentDialogueDataIteration = "";
	float mTextWaitingTimer = 0.0f;
	bool mCurrentlyTyping = false;
	int mTextVectorIndex = -1;
	int mCurrentDialogueDataIndex = 0;
	float mTimePerChar = 0.015f;
	float mTimerPerChar = 0.00f;

	bool mTypelinePresent = false;
	float mTypelineTime = 0.3f;
	float mTypelineTimer = 0.3f;

	float mChangeSceneDelayTime = 0.4f;
	float mChangeSceneDelayTimer = 0.0f;

	DialogueOpeningData currentData;

	std::string fontName = "IosevkaNerdFont-Bold";

	SoundComponent* g_TerminalSound = nullptr;
	float mTerminalTime = 0.125f;
	float mTerminalTimer = 0.0f;
	bool mTerminalIsPlaying = false;
};

#endif