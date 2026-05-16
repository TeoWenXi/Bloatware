/*!
All content © 2023 DigiPen Institute of Technology Singapore, all rights reserved.
@file       DialogueTrigger.h
@author     TAN Kang Zheng (100%)
@co-author

----------------------------------------------------------------------------

@course     CSD2401F23
@section    CSD2401F23-A
@project    CSD2401F23 Software Engineering Project 3

@brief      Component to play dialogue if the player reaches a certain area
*//*______________________________________________________________________*/

#ifndef DIALOGUE_TRIGGER_COMPONENT_H
#define DIALOGUE_TRIGGER_COMPONENT_H

#include "ComponentBase.h"

class DialogueTrigger : public ComponentBase
{
public:

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
	 * Plays the appropriate logic for the current DialogueTrigger
	*/
	void PlayTrigger();

	/*!
	 * @brief
	 * Parses a concatonated save pair into data for the DialogueTrigger
	 * @param _input
	 * The concatonated data
	 * @return 
	 * A pair containing the parsed data for the DialogueTrigger
	*/
	std::pair<std::string, std::string> ParsePair(std::string _input);

	/*!
	 * @brief 
	 * Automatic logging for the creation of a DialogueTrigger
	 * @param os 
	 * The output stream to write to
	 * @param _handler 
	 * Reference to the DialogueTrigger
	 * @return 
	 * The output stream to write to
	*/
	friend std::ostream& operator<< (std::ostream& os, const DialogueTrigger& _handler)
	{
		os << "Dialogue Trigger Component tied to Ent ID " << _handler.mEntID;
		return os;
	}

	//First = Dialogue, Second = Audio
	std::vector<std::pair<std::string, std::string>> mTriggers;
	bool mTriggered = false;
	int mDeathThreshold = 0;
	float mFadeTimer = 0.f;

	//For cutscenes
	float mCutsceneDuration = 0;
	float mCinematicBarExpandingDuration = 0;
	bool mVerticalBarsOnly = false;
	float mDisablePlayerGravity = 0;
	int mCutSceneNum = 0;
	float mCutsceneTextTimer = 0;
	float mDelayPlayerRespawnTimer = 0;
	bool mCutscenePart2Triggered = false;
	bool mCutscenePart3Triggered = false;
	bool mCutscenePart4Triggered = false;

	enum TriggerType : int
	{
		PROXIMITY = 0,
		RANDOM,
		PROXIMITY_DEATH,
		CUTSCENE,
		PROXIMITY_IMAGE
	};
	TriggerType mTriggerType = TriggerType::RANDOM;

private:
	float mPlayerMsStep;
	float mEndShaderAlpha = 0.f;
	float mEndShaderSpeed = 0.f;
	float mEndShaderBloom = 0.f;
	float mAudioDucking = 0.3f;
};

#endif