/*!
All content © 2023 DigiPen Institute of Technology Singapore, all rights reserved.
@file       LevelManager.h  
@author     Jovan Zhuo Wen LOW (100%)
@co-author

----------------------------------------------------------------------------

@course     CSD2401F23
@section    CSD2401F23-A
@project    CSD2401F23 Software Engineering Project 3

@brief      Contains all level sub-sections' boundaries and checkpoints
*//*______________________________________________________________________*/

#ifndef LEVEL_MANAGER_H
#define LEVEL_MANAGER_H

#include "Matrix3x3.h"

#include "Singleton.h"
#include "Manager.h"
#include "SoundManager.h"

class LevelManager : public Singleton<LevelManager>, public Manager
{
public:
	/*!
	 * @brief 
	 * Init function for Level Manager
	*/
	void Init();

	/*!
	 * @brief 
	 * Load function for Level Manager
	*/
	void Load();

	/*!
	 * @brief 
	 * Update functinon for Level Manager
	*/
	void Update();

	/*!
	 * @brief 
	 * Unload function for Level Manager
	*/
	void Unload();

	/*!
	 * @brief 
	 * Get the Spawning location of the player
	 * @param _currPos 
	 * Current position in the map
	 * @return 
	 * The position that the player should spawn at when dies in game
	*/
	Vector3 RetrieveSpawnPosition(Vector3 _currPos);

	/*!
	 * @brief 
	 * Updates the current position of the camera
	 * @param _currPos 
	 * Vector3 of the position of the camera
	*/
	void UpdateCameraInfo(Vector3 _currPos);

	/*!
	 * @brief 
	 * Shakes the screen when the player dies
	*/
	void DoScreenshake();

	/*!
	 * @brief
	 * Gradual shake effect that lasts until the game ends, for ending transition
	*/
	void DoPermanentScreenshake();

private:
	friend Singleton<LevelManager>;
	int LastScenePresent = 0; //failsafe
};

#endif