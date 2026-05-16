/*!
All content � 2023 DigiPen Institute of Technology Singapore, all rights reserved.
@file       AIComponent.h
@author     TAN Kang Zheng (50%)
@co-author	Jovan Zhuo Wen LOW (50%)

----------------------------------------------------------------------------

@course     CSD2401F23
@section    CSD2401F23-A
@project    CSD2401F23 Software Engineering Project 3

@brief      Handles the AI logic
*//*______________________________________________________________________*/

#ifndef AI_COMPONENT_H
#define AI_COMPONENT_H

#include "ComponentBase.h"
#include "Vector3.h"        //Manipulating vectors
#include "Transform.h"      //Manupulating transforms
#include "EntityManager.h"  //Manipulating entities
#include "EventsHeader.h"   //Event Handling
#include "GraphicsSystem.h" //World Dimensions
#include "WindowsManager.h" //DeltaTime
#include "AIManager.h"

class AIComponent : public ComponentBase
{
public:
	bool mIsVisible = false;
	bool mIsHostile = false;
	bool mIsHostileLockedOn = false;

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
	*/
	void Save();

	/*!
	 * @brief
	 * Update virtual function for components
	*/
	void Update();

	/*!
	 * @brief
	 * Update virtual function for components
	*/
	void Exit();

	//void OnNotify(EventObject* _eventObj);

	/*!
	 * @brief
	 * DisplayProperties virtual function for components
	*/
	void DisplayProperties();

	/*!
	 * @brief
	 * Updates Elapsed Time from initialization value for Laser Obstacle Material
	*/
	void UpdateLasers();

	/*!
	 * @brief
	 * Updates Elapsed Time from initialization value for Wind Tunnel Material
	*/
	void UpdateWinds();

	/*!
	 * @brief
	 * Searches for the player object in the scene.
	 * @return
	 * True if the player object is found, false otherwise.
	*/
	bool SearchPlayer();

	/*!
	 * @brief
	 * Begins the display of dialogue on screen and playing of audio file
	 * @param _dialogueText 
	 * The dialogue to display
	 * @param _audioFileName 
	 * The name of the audio file to play
	*/
	void InitiateTalking(std::string _dialogueText, std::string _audioFileName);

	/*!
	 * @brief
	 * Ends current laser and restarts its cooldown
	*/
	void ResetLaser();

	/*!
	 * @brief
	 * If it is hostile, delay the movement for going to next screen
	*/
	void DelayMovement();

	/*!
	 * @brief
	 * If it is hostile, delay the next laser upon the players death.
	*/
	void DelayNextLaser();

	/*!
	 * @brief
	 * Checks if player is within Y-Axis Range of AI.
	 * @return
	 * True if the player object is within it, false otherwise.
	*/
	bool IsPlayerInKillRange();

	/*!
	 * @brief
	 * Enable AI boss mode
	*/
	void EnableHostileAI();

	/*!
	 * @brief
	 * Disable AI boss mode
	*/
	void DisableHostileAI();

	/*!
	 * @brief
	 * Sets the Audio Ducking value
	 * @param _val
	 * The value to set the Audio Ducking to
	*/
	void SetAudioDucking(float _val);

	enum QUADRANT
	{
		UP_RIGHT,
		UP_LEFT,
		DOWN_RIGHT,
		DOWN_LEFT
	};

	enum AI_STATE
	{
		AI_IDLE,
		AI_MOVE
	};

	/*!
	 * @brief
	 *	Operator << overload to print to logs on creation
	 * @param os
	 *	Output stream to print to
	 * @param _handler
	 *	Reference to the AIComponent
	 * @return
	 *	The output stream to continue printing to
	*/
	friend std::ostream& operator<< (std::ostream& _os, const AIComponent& _handler);

private:
	QUADRANT mCurrentQuadrant = UP_RIGHT;
	AI_STATE mCurrentAIState = AI_MOVE;
	float mMoveSpeed{};

	float mLaserHeightMagnification = 0.8f;

	bool mHostileDelayedMovement = false;
	double mHostileDelayedMovementTime{0.65f};
	double mHostileDelayedMovementTimer{0.0f};

	bool mHostileDelayedLaser = false;
	double mHostileDelayedLaserTime{ 3.0f };
	double mHostileDelayedLaserTimer{ 0.0f };

	Vector3 mScreenPos{};
	Vector3 mFinalPos{};
	float mHeightOffset{}, mHeightOffsetRange{};
	double mIdleElapsedTime{};
	Vector3 mTargetCoordinate{};
	Vector3 mCurrentCoordinate{};
	float mClampRadius{ 0.5f };
	float mClampPlayerDistance{133.3f};
	int hostileLockedOn_Phase = 1;
	float hostileLockedOn_Timer = 0.0f;
	float hostileLockedOn_Phase1Time = 3.0f;
	float hostileLockedOn_Phase2Time = 0.25f;
	float hostileLockedOn_Phase3Time = 1.2f;
	float hostileLockedOn_Phase4Time = 2.5f;
	Entity* ent_HB1_FocusRing;
	Entity* ent_HB1_Sparks;
	Entity* ent_HB1_Orb;
	Entity* ent_HB2_Sparks;
	Entity* ent_HB3_Sparks;
	Entity* ent_HB2_RingA;
	Entity* ent_HB2_RingB;
	Entity* ent_HB3_FrontBeamSparksA;
	Entity* ent_HB3_FrontBeamSparksB;
	Entity* ent_HB3_BackBeamSparksA;
	Entity* ent_HB3_BackBeamSparksB;
	Entity* ent_HB3_BackBeamSparksC;
	Entity* ent_HB2_HyperBeam;
	Entity* ent_HB2_HyperGlow;
	Entity* ent_HB4_SparksAftermathA;
	Entity* ent_HB4_SparksAftermathB;
	Entity* ent_HB4_SparksAftermathC;
	Entity* ent_HB1_Sparkle;
	Entity* ent_HB1_SparkleLineVertical;
	Entity* ent_HB1_SparkleLineHorizontal;
	float mEyeRadiusAdditional = 0.0f;
	float mHyperBeamProgress = 0.0f;
	bool mSwitchToSmallSparks = false;
	bool mCanDamagePlayer = false;
	float mBeamLastSavedCamY = 0.0f;
	float mBeamLastSavedY = 0.0f;
	float mKillRange = 35.0f;
	float mDetectPlayerCooldown_Max = 4.0f;
	float mDetectPlayerCooldown_Timer = 0.0f;

	float mTalkSpeed = 2.8f;
	float mBaseTalkingTime = 1.0f;
	float mTimePerChar = 0.05f;
	float mPostTalkingTime = 3.0f;
	float mPostTalkingTimer = 0.0f;
	float mDialogueTimeMultiplier = 0.4f;
	bool mIsTalking = false;
	std::string mTalkingFullText = "";
	float mCurrentTalkingTime = 0.f;
	float mCurrentTalkingTimer = 0.f;
	int mStateOfTyping = 0; //0 = gibberish, 1 = not gibberish, 2 = done
	int mStringCharPointer = 0;
	float mTimerToNextChar = 0.0f;
	std::string mCurrentText = "";
	std::string mCurrentText2 = "";
	std::string mBlankSpaces = "";
	float mTimePerCharExtra = 0.f;
	float mTalkingProgress = 0.f;
	float mTargetAlpha = 0.4f;
	float mTrueAlpha = 0.4f;
	float mOverallAlpha = 1.0f;

	float mTimerToMoveEye = 0.0f;
	float mTimeToMoveEye = 2.0f;
	Vector3 mTargetMoveEye = Vector3();
	Vector3 mTrueMoveEye = Vector3();
	Vector3 mMovePosExtents = Vector3(90.0f, 35.f, 0.f);
	float mMovePosSpeed = 1.0f;
	float mMovePosPercentThresholdX = 0.75f, MovePosPercentThresholdY = 0.7f;
	Vector3 mTargetMovePos = Vector3(2.5f, 0.5f, 0.0f);
	Vector3 mTrueMovePos = Vector3(2.5f, 0.5f, 0.0f);
	Vector3 mActualLocalPositionNoHover = Vector3();
	bool mAnchoredFloatIsRight = true;
	bool mAnchoredFloatIsTop = true;
	bool mPlayerInLeftDelay = false;
	float mRightAnchorSlow = 1.0f;
	float mDistMagTestX = 10.f * 3.f, mDistMagTestY = 10.f * 2.f;
	float mTalkingDistMag = 0.5f;

	bool mBGMRestored = true;
	float mBGMReductionVal = 0.3f; //How much to reduce the volume of the BGM when AI is speaking (value between 0 and 1, multiplies volume by this value)
	float mDialogueVoiceDuration = 0.0f, mDialogueVoiceElapsed = 0.0f;

	float mTileIntensity= 0.0f;
	float mTileTrueIntensity = 0.0f;

	float mHostileInitialNoContactTime = 3.0f;
	float mHostileInitialNoContactTimer = 0.0f;

	std::vector<std::pair<FMOD::Channel*, bool>> mAIAudioSourceHandler; //bool represents enabled or disabled, remove if is not playing
};

#endif