/*!
All content © 2023 DigiPen Institute of Technology Singapore, all rights reserved.
@file       AIManager.h
@author     TAN Kang Zheng (100%)
@co-author

----------------------------------------------------------------------------

@course     CSD2401F23
@section    CSD2401F23-A
@project    CSD2401F23 Software Engineering Project 3

@brief      Manages the AI Components
*//*______________________________________________________________________*/
#ifndef AI_MANAGER_H
#define AI_MANAGER_H

#include "Singleton.h"
#include "Manager.h"
#include "EntityManager.h"

#if _ENGINE
#include "DebugLogger.h"
#endif // _ENGINE

#include <string>
#include <vector>

class AIComponent;
class AIManager : public Singleton<AIManager>, public Manager
{
public:

	/*!
	 * @brief
	 * Init virtual function for manager classes
	*/
	void Init() {};

	/*!
	 * @brief
	 * Load virtual function for manager classes
	*/
	void Load() {};

	/*!
	 * @brief
	 * Update virtual function for manager classes
	*/
	void Update();

	/*!
	 * @brief
	 * Unload virtual function for manager classes
	*/
	void Unload();

	/*!
	 * @brief 
	 *	Adds a given AI Component to the AI Manager's lists
	 * @param _AIComponent 
	 *	Pointer to the AI Component
	*/
	void AddAIComponent(EntityID _AIComponent);

	/*!
	 * @brief 
	 *	Removes a given AI Component from the AI Manager's lists
	 * @param _AIComponent 
	 *	Pointer to the AI Component
	*/
	void RemoveAIComponent(EntityID _AIComponent);

	/*!
	 * @brief
	 * Saves all currently tracked entities to a backup
	*/
	void CreateBackup();

	/*!
	 * @brief
	 * Loads all saved entities from backup
	*/
	void LoadBackup();
	
	/*!
	 * @brief
	 * Find an AI Component and calls it's InitiateDiaogue function
	 * @param _Dialogue 
	 * The dialogue to be showns
	 * @param _Audio 
	 * The audio file to be played
	*/
	void InitiateTalking(std::string _Dialogue, std::string _Audio);

	/*!
	 * @brief
	 * Find an AI Component and check if it is currently in a Hostile state
	 * @return
	 * True if any are in Hostile state, False otherwise
	*/
	bool IsHostile();

	/*!
	 * @brief
	 * Sets AI Component to boss mode
	*/
	void EnableAIBossMode();

	/*!
	 * @brief
	 * Disables AI Component boss mode
	*/
	void DisableAIBossMode();

	/*!
	 * @brief
	 * Sets the audio ducking for the AI Dialogue
	 * @param _val
	 * The value to set the Audio Ducking to
	*/
	void SetAudioDucking(float _val);

	/*!
	 * @brief
	 * Return mAIComponent_vec
	 * @return
	 * Return mAIComponent_vec
	*/
	std::vector<EntityID> GetAIComponentVector();
private:
	//Create vectors to store entities with AIComponent
	std::vector<EntityID> mAIComponent_vec, mAIComponent_vecBackup;
};

#endif
