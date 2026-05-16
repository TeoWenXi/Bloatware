/*!
All content © 2023 DigiPen Institute of Technology Singapore, all rights reserved.
@file       EntityManager.h
@author     TEO Wen Xi (100%)
@co-author

----------------------------------------------------------------------------

@course     CSD2401F23
@section    CSD2401F23-A
@project    CSD2401F23 Software Engineering Project 3

@brief      Manages all entities
*//*______________________________________________________________________*/

#ifndef ENTITY_MANAGER_H
#define ENTITY_MANAGER_H

#include <string>
#include <vector>

#include "Singleton.h"
#include "Manager.h"
#include "ECS_Types.h"
#include "Entity.h"

class EntityManager : public Singleton <EntityManager>, public Manager
{
public:
	//Manager functions

	/*!
	 * @brief 
	 * Init function
	*/
	void Init();

	/*!
	 * @brief 
	 * Load function
	*/
	void Load();

	/*!
	 * @brief 
	 * Update function
	*/
	void Update();

	/*!
	 * @brief 
	 * Unload function
	*/
	void Unload();

	//Entity

	/*!
	 * @brief 
	 * Creates an entity
	 * @return 
	 * Pointer to the newly created entity
	 * nullptr if there are no more available entities to be created
	*/
	Entity* CreateEntity();

	/*!
	 * @brief 
	 * Creates an entity with the input ID
	 * @param _entityID 
	 * ID of the newly created entity to set
	 * @return 
	 * nullptr if ID is already used
	 * Pointer to the newly created entity if successful
	*/
	Entity* CreateEntityWithID(EntityID _entityID);

	/*!
	 * @brief 
	 * Duplicates the entity tied to the input entity id
	 * @param _entityID 
	 * Entity id of the entity to be duplicated
	 * @return 
	 * Pointer to the newly created entity
	 * nullptr if there are no more available entities to be created
	 * 
	*/
	Entity* DuplicateEntity(EntityID _entityID);

	/*!
	 * @brief
	 * Destroys the entity tied to the input entity id
	 * with all children
	 * @param _entityID
	 * Entity id of the entity to be destroyed
	*/
	void DestroyEntityAll(EntityID _entityID);

	/*!
	 * @brief 
	 * Destroys the entity tied to the input entity id
	 * @param _entityID 
	 * Entity id of the entity to be destroyed
	*/
	void DestroyEntity(EntityID _entityID);

	/*!
	 * @brief 
	 * Overload function of DestroyEntity but using iterator instead
	 * @param _iter 
	 * Iterator of the entity to be destroyed
	*/
	void DestroyEntity(std::vector<Entity>::iterator& _iter);

	//Parent & Child

	/*!
	 * @brief 
	 * Creates a child entity tied to the input entityID
	 * @param _parentEntID 
	 * ID of the entity to add a child entity to
	 * @return 
	 * Pointer to the newly created entity
	 * nullptr if there are no more available entities to be created
	*/
	Entity* CreateChildEntity(EntityID _parentEntID);

	//Name

	/*!
	 * @brief 
	 * Sets the name of the entity tied to the input entity id
	 * @param _entityID 
	 * Entity id of the object to set the new name to
	 * @param _name 
	 * New name to be set to the entity
	*/
	void SetName(EntityID _entityID, std::string _name);

	/*!
	 * @brief 
	 * Gets the name of the entity tied to the input entity id
	 * @param _entityID 
	 * Entity id of the object to get the name from
	 * @return 
	 * Name of the entity
	*/
	std::string GetName(EntityID _entityID);

	//Find entity with ID

	/*!
	 * @brief 
	 * Gets an entity from the entity list tied to the input entity id
	 * @param _entityID 
	 * Entity id of the entity to be retrieved
	 * @return 
	 * Pointer to the retrieved entity
	 * nullptr if entity does not exist
	*/
	Entity* GetEntity(EntityID _entityID);

	/*!
	 * @brief 
	 * Gets an entity from the entity list with the name
	 * Will always gets the first entity in the list with the name
	 * Maybe make another function to get an array of those with the name
	 * @param _entityName 
	 * The entity name to find
	 * @return 
	 * Pointer to the retrieved entity
	 * nullptr if entity does not exist
	*/
	Entity* GetEntity(std::string _entityName);

	//Debug printing

	/*!
	 * @brief
	 * Prints out the entire entity list into the debug console, mainly for debugging
	*/
	void Print();

	//Memory resizing

	/*!
	 * @brief
	 * Realloates memory for entities
	*/
	void ReallocateMemory();

	//Scene Play & Stop

	/*!
	 * @brief 
	 * Backups the current entities and its components
	*/
	void BackupCurrScene();

	/*!
	 * @brief 
	 * Loads the backup
	*/
	void LoadSceneBackup();

	/*!
	 * @brief
	 * Clears the backups
	*/
	void ClearSceneBackup();

	//Current active entity list container
	std::vector<Entity> mEntityList;

	//Target max entities count (Used for adjusting entity count)
	EntityID mTargetMaxEntities = 0; //Will be read from config file

private:
	//For singleton
	friend Singleton<EntityManager>;

	/*!
	 * @brief 
	 * Searches the list to check if the entity exists
	 * @param _entityID 
	 * The id of the entity to search for
	 * @return 
	 * iterator of the searched entity
	*/
	std::vector<Entity>::iterator FindEntityFromList(EntityID _entityID);

	/*!
	 * @brief 
	 * Gets the smallest available entity id which is smaller than the input entity id
	 * @param _entityID 
	 * Input entity id to search smaller for
	 * @return 
	 * Smallest entity id that is smaller than the input entity id
	*/
	EntityID GetSmallestAvailableID(EntityID _entityID);

	//Container containing the list of available entities
	std::vector<EntityID> mAvailableEntities;

	//Max entities count
	EntityID mMaxEntities = 0;

	//Scene Play & Stop
	std::vector<Entity> mBackupEntityList;
	std::vector<EntityID> mBackupAvailableEntities;
};

#endif