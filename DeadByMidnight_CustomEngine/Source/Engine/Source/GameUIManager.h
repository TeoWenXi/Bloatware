/*!
All content © 2023 DigiPen Institute of Technology Singapore, all rights reserved.
@file       GameUIManager.h
@author     TAN Kang Zheng (100%)
@co-author

----------------------------------------------------------------------------

@course     CSD2401F23
@section    CSD2401F23-A
@project    CSD2401F23 Software Engineering Project 3

@brief      Manages the in-game UI for the engine
*//*______________________________________________________________________*/
#ifndef GAMEUI_MANAGER_H
#define GAMEUI_MANAGER_H

#include "Singleton.h"
#include "Manager.h"
#include "TextManager.h"
//#include "EntityManager.h"
#include "ButtonWrapper.h"
#include "ECS_Types.h"

#if _ENGINE
#include "DebugLogger.h"
#endif // _ENGINE

#include <string>
#include <vector>

class GameUITextHandler;
class GameUIObjectHandler;
class GameUIManager : public Singleton<GameUIManager>, public Manager
{
public:
	/*!
	 * @brief 
	 * Init virtual function for manager classes
	*/
	void Init();

	/*!
	 * @brief 
	 * Load virtual function for manager classes
	*/
	void Load();

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
	 *	Calls rendering for all text objects the manager is watching
	*/
	void Render();

	/*!
	 * @brief 
	 *	Adds an entity with an attached parent with a given entity ID to the UI Manager's list
	 * @param _entityID 
	 *	The EntityID to add to the list
	*/
	void AddParent(EntityID _entityID);

	/*!
	 * @brief
	 *	Adds an entity with an attached text object with a given entity ID to the UI Manager's list
	 * @param _entityID
	 *	The EntityID to add to the list
	*/
	void AddText(EntityID _entityID);

	/*!
	 * @brief
	 *	Adds an entity with an attached button with a given entity ID to the UI Manager's list
	 * @param _entityID
	 *	The EntityID to add to the list
	*/
	void AddButton(EntityID _entityID);

	/*!
	 * @brief
	 *	Adds an entity with an attached endgoal with a given entity ID to the UI Manager's list
	 * @param _entityID
	 *	The EntityID to add to the list
	*/
	void AddEndGoal(EntityID _entityID);

	/*!
	 * @brief
	 *	Adds an entity with an attached dialoguetrigger with a given entity ID to the UI Manager's list
	 * @param _entityID
	 *	The EntityID to add to the list
	*/
	void AddDialogueTrigger(EntityID _entityID);

	/*!
	 * @brief 
	 *	Removes an entity with an attached parent with a given entity ID from the UI Manager's list
	 * @param _entityID
	 *	The EntityID to remove from the list
	*/
	void RemoveParent(EntityID _entityID);

	/*!
	 * @brief
	 *	Removes an entity with an attached text object with a given entity ID from the UI Manager's list
	 * @param _entityID
	 *	The EntityID to remove from the list
	*/
	void RemoveText(EntityID _entityID);

	/*!
	 * @brief
	 *	Removes an entity with an attached button object with a given entity ID from the UI Manager's list
	 * @param _entityID
	 *	The EntityID to remove from the list
	*/
	void RemoveButton(EntityID _entityID);

	/*!
	 * @brief
	 *	Removes an entity with an attached endgoal object with a given entity ID from the UI Manager's list
	 * @param _entityID
	 *	The EntityID to remove from the list
	*/
	void RemoveEndGoal(EntityID _entityID);

	/*!
	 * @brief
	 *	Removes an entity with an attached dialoguetrigger with a given entity ID to the UI Manager's list
	 * @param _entityID
	 *	The EntityID to remove to the list
	*/
	void RemoveDialogueTrigger(EntityID _entityID);

	/*!
	 * @brief 
	 *	Searches and accesses an entity with an attached text component within the UI Manager's lists
	 * @param _entityName 
	 *	The name of the entity
	 * @return 
	 *	A pointer to the entity
	*/
	Entity* GetTextEntity(std::string _entityName);

	/*!
	 * @brief
	 *	Searches and accesses an entity with an attached renderer component within the UI Manager's lists
	 * @param _entityName
	 *	The name of the entity
	 * @return
	 *	A pointer to the entity
	*/
	Entity* GetParentEntity(std::string _entityName);

	/*!
	 * @brief
	 *	Searches and accesses an entity with an attached button component within the UI Manager's lists
	 * @param _entityName
	 *	The name of the entity
	 * @return
	 *	A pointer to the entity
	*/
	Entity* GetButtonEntity(std::string _entityName);

	/*!
	 * @brief
	 * Clears the manager of all components it is tracking
	*/
	void ClearAllUI();

	/*!
	 * @brief
	 * Creates a backup of all the entities the manager is currently tracking
	*/
	void CreateBackup();

	/*!
	 * @brief
	 * Loads all the entities from the backup
	*/
	void LoadBackup();

private:
	//Create vectors to store entities that function as component headers
	std::vector<EntityID> mParentComponents, mParentComponentsBackup;
	//Create vector to store entities with text
	std::vector<EntityID> mTextToRender, mTextToRenderBackup;
	//Create vectors to store entities with button
	std::vector<EntityID> mButtons, mButtonsBackup;
	//Create vectors to store entities with EndGoal
	std::vector<EntityID> mEndGoal, mEndGoalBackup;
	//Create vectors to store entities with DialogueTrigger
	std::vector<EntityID> mDialogueTriggers, mDialogueTriggersBackup;
};

#endif