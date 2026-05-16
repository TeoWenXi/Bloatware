/*!
All content © 2023 DigiPen Institute of Technology Singapore, all rights reserved.
@file       Player.h
@author     TEO Wen Xi (50%)
@co-author  Jovan Zhuo Wen LOW (50%)

----------------------------------------------------------------------------

@course     CSD2401F23
@section    CSD2401F23-A
@project    CSD2401F23 Software Engineering Project 3

@brief      Player class component, used for anything related to the player
*//*______________________________________________________________________*/

#ifndef PLAYER_H
#define PLAYER_H

#include "ComponentBase.h"

#include "Vector3.h"
#include "EntityManager.h"

//Components
#include "PhysicsComponent.h"
#include "TileComponent.h"

class Player : public ComponentBase
{
public:
	float mJumpStrength{};
	float mMoveSpeed{};
    bool mIsOnGround{ false };
    bool mIsJumping{ false };
    bool mWasJumping{ false }; //For sound fix
    bool mDeathPlayed{ false };
    bool mIsDead{ false };
    bool mIsReviving{ false };

    /*!
	 * @brief 
     * Checks if wind is affecting the player
     * @return 
     * True if it is
     * False if otherwise
    */
    bool IsWindAffected();

    /*!
	 * @brief 
     * For shader player blob movement
     * @return 
     * True if timer > 0
     * False if otherwise
    */
    bool ShouldExtendBottom();

    /*!
	 * @brief 
     * Checks if the tile below the player is a ground tile
     * @return 
     * True if it is a ground
     * False if its not
    */
    bool GroundLineRaycast();
	Vector3 mInitialPosition{0.f, 0.f, 0.f};
    bool mHasKey{ false };
    EntityID mAttachedKeyID{0};
    Vector3 mDirectionalVector{ 0.f, 0.f, 0.f };

    //Cutscene timer
    float mCutsceneTimer = 0.f; //Cuts all movement controls for player
    float mDisableGravityTimer = 0.f; //Disables gravity for a while
    float mDelayRespawnTimer = 0.f; //Delays respawning
    float mMoveSpeedMultiplier = 1; //Mainly used for ending cutscene
private:
	PhysicsComponent* mPhy{};
	TileComponent* mMap{};
	Entity* mEntBodySprite{};
	Entity* mEntEyeHolder{};
	Entity* mEntEyeLeft{};
	Entity* mEntEyeRight{};
    float mMinimumJumpDurationTime{ 0.05f };
	float mMinimumJumpDurationTimer{0.0f};
    float mTotalJumpDurationTime{ 0.5f };
    float mTotalJumpDurationTimer{ 0.0f };
    float mDeathDurationTime{ 0.6f };
    float mDeathDurationTimer{ 0.0f };
    float mReviveDurationTime{ 0.4f };
    float mReviveDurationTimer{ 0.0f };
    float mExtendBottomTime{ 0.08f };
    float mExtendBottomTimer{ 0.0f };

    bool mIsXTriggered = false;

    Vector3 mInitialBodyScale{ 1.28f * 0.606f * 14.5f * 256.f / 200.f, 1.28f * 0.606f * 14.5f * 256.f / 200.f, 1 };
    Vector3 mInitialEyeLeftScale{ 1.28f * 0.606f * 7.25f, 1.28f * 0.606f * 7.25f, 1 };
    Vector3 mInitialEyeRightScale{ 1.28f * 0.606f * 7.25f, 1.28f * 0.606f * 7.25f, 1 };

    float mZoomLvlStorageSmallOther = 0.25f;
    float mZoomLvlStorageSmall = 0.5f;
    float mZoomLvlStorageNormal = 1.0f;
    float mZoomScaleTarget = 1.0f;
    float mZoomScale = 1.0f;
    float mZoomSpeed = 5.f;
    float mPreviousYVelocity = 0.f;
    
    //CUSTOM PHYSICS
    float mInternalLeftRightVelocity = 0;
    float mInternalLeftRightBuildupRate = 5.0f;
    float mInternalLeftRightPower = 2.0f;
    float mLastFallingValue = 0;
    float mCoyoteTimeMax = 0.1f;
    float mCoyoteTimer = 0.0f;
    float mInitiatedJumpForce = 1.0f;
    float mGravitationalforce = 16.0f * 10.0f * 0.75f;
    float mRisingGravity = 0.0f;
    float mGravityRiseRate = 50.0f * 10.0f * 0.75f;
    //List<AttachedWind> AttachedWindList = new List<AttachedWind>();
    float mWindAttachedProgress = 0.0f; //0-1
    float mWindTransitionTime = 0.31f;
    float mWindExitTime = 0.5f;
    Vector3 mLastSavedExistentWindForce = Vector3{};
    //float mWindForce = 12.f * 5.0f;
    float mDebugDisplayValue = 0;

    //***** CHARACTER ANIMATION START
    float mRatioAnimMultiplier = 1.28f * 0.606f * 7.25f; //coordinates different from unity, need to expand accordingly

    float mTurnBodyTime = 0.3f;
    float mEyeLLocalLeft = -0.772f;
    float mEyeLLocalRight = -0.269f;
    float mEyeRLocalLeft = 0.269f;
    float mEyeRLocalRight = 0.772f;
    float mEyeLInternal = -0.269f;
    float mEyeLCurrent = -0.269f;
    float mEyeRInternal = 0.772f;
    float mEyeRCurrent = 0.772f;
    float mBodyFacingInternal = 1;
    float mBodyFacingCurrent = 1;

    float mEyeHeightDefault = -0.188f;
    float mEyeHeightTop = -0.08f;
    float mEyeHeightBottom = -0.148f;
    float mEyeHeightInternal = -0.188f;
    float mEyeHeightCurrent = -0.188f;
    float mTurnEyeHeightTime = 0.5f;

    float mMorphBodyProgressInternal = 0; //1 if mid-air, 0 otherwise
    float mMorphBodyProgressCurrent = 0;
    float mMorphBodyVerticalSInternal = 0; //1 if going upwards, 0 if going downwards
    float mMorphBodyVerticalSCurrent = 0;
    float mMorphBodyTime = 0.2f;

    float mMorphPlanXProgressInternal = 0;
    float mMorphPlanXProgressCurrent = 0;
    float mMorphPlanXRunningTimer = 0;
    float mMorphPlanXRate = 2;
    float mMorphPlanXTime = 0.2f;

    float mIdleBodyProgressInternal = 0;
    float mIdleBodyProgressCurrent = 0;
    float mIdleBodyRunningTimer = 0.0f;
    float mIdleBodyTime = 0.2f;

    /*!
     * @brief 
     * Smoothens out the player animation
     * @param _edge0 
     * First edge
     * @param _edge1 
     * Second edge
     * @param _x 
     * Value to smooth from
     * @return 
     * Final smoothened out value
    */
    float SmoothStep(float _edge0, float _edge1, float _x);

    /*!
     * @brief 
     * Flips the sprite
     * @param _right 
     * True if facing right
     * False if facing left
     * @param _immediate
     * True if instant snap to the facing direction
     * False if not
    */
    void SetSpriteFlip(bool _right, bool _immediate = false);
    //***** CHARACTER ANIMATION END
private:
    bool mLoaded = false;
public:

    /*!
     * @brief 
	 * Init function
    */
	void Init();

    /*!
     * @brief 
	 * Update function
    */
	void Update();

    /*!
     * @brief 
	 * Exit function
    */
	void Exit();

    /*!
     * @brief 
	 * Load function
    */
	void Load();

    /*!
     * @brief 
	 * Save function
    */
	void Save();

    /*!
     * @brief 
	 * Imgui display properties
    */
	void DisplayProperties();

	//Movement Stuff

    /*!
     * @brief 
     * Player move left
    */
	void MoveLeft();

    /*!
     * @brief 
     * Player move right
    */
	void MoveRight();

    /*!
     * @brief 
     * Normal jump function
    */
	void Jump();

    /*!
     * @brief 
     * Jump function when player is big sized
    */
    void JumpBig();

    /*!
     * @brief 
     * Resets player horizontal velocity
    */
	void ResetHorizontalVel();

    /*!
     * @brief 
     * Kills the player
    */
	void Kill();

    /*!
     * @brief 
     * Player respawn function
    */
    void Respawn();

    /*!
     * @brief 
     * Spawns the player death VFX on player's death
    */
    void SpawnDeathVFX_Normal();

    /*!
     * @brief 
     * Falling physics calculation for player movement
    */
    void FallingFunction();

	//Linking Keycode Retrieval to Functions

	/*!
	 * @brief 
     * Returns true when player's jump key is held down
     * False if otherwise
	 * @return 
     * Returns true when player's jump key is held down
     * False if otherwise
	*/
	bool GetJumpKey_Hold();

	/*!
	 * @brief 
     * Returns true when player's jump key is pressed
     * False if otherwise
	 * @return 
     * Returns true when player's jump key is pressed
     * False if otherwise
	*/
	bool GetJumpKey_OnPress();

    /*!
     * @brief 
     * Updates the player child objects, mainly for shader work
    */
    void SetChildObjects();

	/*!
	 * @brief 
	 * Cout overload function for component debug printout
	 * @param os 
	 * The output stream to write into
	 * @param _player 
	 * The input class object
	 * @return 
	 * The output stream with the written string
	*/
	friend std::ostream& operator<< (std::ostream& os, const Player& _player);

    //Player Death Text
    std::string mPlayerDeathText{ "" };
    std::string mPlayerDeathFont{ "" };
    float mPlayerDeathTextTime{ 0.0 };

    //Player Level Death Tracker
    unsigned int mCurrentLevelDeaths = 0;

    /*!
     * @brief 
     * Resets the death counter for level tracking
    */
    void ResetCurrentLevelDeaths();

    /*!
	 * @brief 
     * Gets the current level death counter
     * @return 
     * Value of the current level death counter
    */
    unsigned int GetCurrentLevelDeaths();

    /*!
     * @brief
     * Reset cutscene related timers
    */
    void ResetCutsceneTimers();

    /*!
     * @brief
     * Update player shaders
    */
    void UpdatePlayerShaders();

	static EntityID mPlayerID;
};

#endif