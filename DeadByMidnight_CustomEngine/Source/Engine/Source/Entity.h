/*!
All content © 2023 DigiPen Institute of Technology Singapore, all rights reserved.
@file       Entity.h
@author     TEO Wen Xi (100%)
@co-author

----------------------------------------------------------------------------

@course     CSD2401F23
@section    CSD2401F23-A
@project    CSD2401F23 Software Engineering Project 3

@brief      Entity class for the entity component system
*//*______________________________________________________________________*/

#ifndef ENTITY_H
#define ENTITY_H

#include <vector>
#include <string>

#include "ECS_Types.h"
#include "ComponentManager.h"

class Entity
{
public:
	//Constructors

	/*!
	 * @brief 
	 * Ctor
	*/
	Entity();

	/*!
	 * @brief 
	 * Overload Ctor with a input entity id
	 * @param _id 
	 * The newly created entity's id
	*/
	Entity(const EntityID& _id);

	/*!
	 * @brief 
	 * Destroys all components tied to this entity
	*/
	void Destroy();

	//Get Set functions
	//ID

	/*!
	 * @brief 
	 * Gets the EntityID of this entity
	 * @return 
	 * Entity ID of this entity
	*/
	EntityID GetID();

	/*!
	 * @brief 
	 * Sets the entity id of this entity
	 * @param _id 
	 * The new id of this entity
	*/
	void SetID(const EntityID& _id);

	//Name
	
	/*!
	 * @brief 
	 * Gets the name of this entity
	 * @return 
	 * Returns the name of this entity
	*/
	std::string GetName();

	/*!
	 * @brief 
	 * Sets the name of this entity
	 * @param _name 
	 * The new name of this entity
	*/
	void SetName(const std::string& _name);

	//Active
	
	/*!
	 * @brief 
	 * Returns true if entity is active, false if otherwise
	 * @return 
	 * Returns true if entity is active, false if otherwise
	*/
	bool GetActive();

	/*!
	 * @brief 
	 * Sets this entity's active to the input bool
	 * @param _active 
	 * The input bool to set the entity's active to
	*/
	void SetActive(bool _active);

	//ToDestroy

	/*!
	 * @brief 
	 * Returns true if this entity is set to be destroyed in the next game loop, false if otherwise 
	 * @return 
	 * Returns true if this entity is to be destroyed, false if otherwise
	 * 
	*/
	bool GetToDestroy();

	/*!
	 * @brief 
	 * Sets this entity to be destroyed in the next game loop
	 * @param _toDestroy 
	 * The input bool to set the entity's to be destroyed bool to
	*/
	void SetToDestroy(bool _toDestroy);

	//mSaveInScene

	/*!
	 * @brief
	 * Gets the mSaveInScene variable of this entity
	 * @return
	 * mSaveInScene of this entity
	*/
	bool GetSaveInScene();

	/*!
	 * @brief
	 * Sets whether this entity can be saved in the editor or not
	 * @param _id
	 * Boolean for whether entity can be saved
	*/
	void SetSaveInScene(bool _saveInScene);

	//Component linking

	/*!
	 * @brief 
	 * Adds a component to this entity
	 * @tparam T 
	 * Type of the component to be added
	 * @param component 
	 * Component data of the component to be added
	 * @return 
	 * Returns a pointer to the newly added component in the component array if
	 * component has been successfully added
	 * Return nullptr if otherwise
	*/
	template <typename T>
	T* AddComponent(const T& component)
	{
		return ComponentManager::GetInstance()->AddComponent(mID, component);
	}

	/*!
	 * @brief 
	 * Overload function of AddComponent that creates a default of the input component and returns it
	 * @tparam T 
	 * Type of the component to be added
	 * @return 
	 * Returns a pointer to the newly added component in the component array if
	 * component has been successfully added
	 * Return nullptr if otherwise
	*/
	template <typename T>
	T* AddComponent()
	{
		return ComponentManager::GetInstance()->AddComponent<T>(mID);
	}

	/*!
	 * @brief 
	 * Removes the input component tied to the input entityID from the component array list
	 * @tparam T 
	 * Type of the component to be removed
	*/
	template <typename T>
	void RemoveComponent()
	{
		ComponentManager::GetInstance()->RemoveComponent<T>(mID);
	}

	/*!
	 * @brief 
	 * Retrives the entity component of the input type
	 * @tparam T 
	 * Type of the component to be retrieved
	 * @return 
	 * Returns a pointer to the component to be retrieved if successful
	 * Return nullptr if otherwise
	*/
	template <typename T>
	T* GetComponent()
	{
		return ComponentManager::GetInstance()->GetComponent<T>(mID);
	}

	//Parenting & Child

	/*!
	 * @brief 
	 * Retrives the child entity id list of this entity
	 * @return 
	 * Child entity id list of this entity
	*/
	const std::vector<EntityID> GetChildEntList();

	/*!
	 * @brief
	 * Gets the parent id of this entity if any
	 * @return 
	 * -1 if parent doesnt exist
	 * ID of the parent entity object if otherwise.
	*/
	EntityID GetParentID();

	/*!
	 * @brief
	 * Adds a child to this entity
	 * @param _id 
	 * Entity id of the child entity to be added
	*/
	void AddChild(EntityID _id);

	/*!
	 * @brief 
	 * Removes a child entity from this entity object
	 * @param _id 
	 * Id of the child entity to be removed
	*/
	void RemoveChild(EntityID _id);

	/*!
	 * @brief 
	 * Sets the parent entity id
	 * @param _id 
	 * The id to set the parent to
	*/
	void SetParentID(EntityID _id);

	/*!
	 * @brief
	 * Removes parenting from this entity
	*/
	void RemoveParenting();

private:
	EntityID mID;
	std::string mName;
	bool mActive;
	bool mToDestroy;
	bool mSaveInScene; //true by default, disable to prevent certain entities from being saved in the editor

	//Parenting
	EntityID mParentID;
	std::vector<EntityID> mChildEntList;
};

#endif