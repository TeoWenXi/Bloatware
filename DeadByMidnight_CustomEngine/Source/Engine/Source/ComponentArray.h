/*!
All content © 2023 DigiPen Institute of Technology Singapore, all rights reserved.
@file       ComponentArray.h
@author     TEO Wen Xi (100%)
@co-author

----------------------------------------------------------------------------

@course     CSD2401F23
@section    CSD2401F23-A
@project    CSD2401F23 Software Engineering Project 3

@brief      Component Array class for ECS use
*//*______________________________________________________________________*/

#ifndef COMPONENT_ARRAY_H
#define COMPONENT_ARRAY_H

#include <unordered_map>
#include <iostream>

#include "ECS_Types.h"
#include "ComponentBase.h"

class IComponentArray
{
public:
	/*!
	 * @brief
	 * default destructor
	*/
	virtual ~IComponentArray() = default;

	/*!
	 * @brief 
	 * Deletes the corresponding component in the component array tied to this entityID if any
	 * @param _entityID 
	 * The component's entity ID to check for
	*/
	virtual void EntityDestroyed(const EntityID& _entityID) = 0;

	/*!
	 * @brief 
	 * Duplicates the component data and assigns it to the new input entity id
	 * @param _entityIDToCopy 
	 * The entity id of the component to be duplicated
	 * @param _newEntityID 
	 * The entity id of the newly duplicated component
	*/
	virtual void DuplicateData(const EntityID& _entityIDToCopy, const EntityID& _newEntityID) = 0;

	/*!
	 * @brief 
	 * Gets the size of the component array
	 * @return 
	 * The size of the component array
	*/
	virtual size_t GetSize() = 0;

	/*!
	 * @brief 
	 * Prints out the component array in the debug console, mainly used for debugging
	*/
	virtual void Print() = 0;

	/*!
	 * @brief 
	 * Clones the component array and assigns it to a shared ptr
	 * @return 
	 * Shared pointer pointing to the newly duplicated component array
	*/
	virtual std::shared_ptr<IComponentArray> CloneShared() const = 0;

	/*!
	 * @brief 
	 * Clones the component array and assigns it to a shared ptr
	 * @return 
	 * Unique pointer pointing to the newly duplicated component array
	*/
	virtual std::unique_ptr<IComponentArray> CloneUnique() const = 0;
};


//Wrapper template class to wrap around the IComponentArray class
template <typename T>
class ComponentArray : public IComponentArray
{
public:
	
	/*!
	 * @brief 
	 * Adds a new component data into the container
	 * @param _entityID 
	 * Entity ID of the component data to be added
	 * @param _component 
	 * The component data to be added
	 * @return 
	 * Returns false if entity's component already exist in map
	 * Return true if entity's component is successfully added to the array
	*/
	bool AddData(const EntityID& _entityID, const T& _component)
	{
		//Entity already exists in map
		if (true == CheckIfExistInMap(_entityID))
			return false;

		//Entity doesn't exist in map yet
		mComponentArray[_entityID] = _component;

		//Set entity ID and init component
		static_cast<ComponentBase&>(mComponentArray[_entityID]).mEntID = _entityID;
		mComponentArray[_entityID].Init();

		return true;
	}

	/*!
	 * @brief 
	 * Removes data from the component array
	 * @param _entityID 
	 * Entity ID of the component data to be removed
	 * @return 
	 * Returns false if entity's component doesnt exist in map
	 * Return true if entity's component is successfully removed from the array
	*/
	bool RemoveData(const EntityID& _entityID)
	{
		//Entity doesn't exist in map
		if (false == CheckIfExistInMap(_entityID))
			return false;

		//Entity exists in map
		mComponentArray[_entityID].Exit();
		mComponentArray.erase(_entityID);
		return true;
	}

	/*!
	 * @brief 
	 * Returns a pointer to the component data tied to the input EntityID
	 * @param _entityID 
	 * EntityID of the component to be returned
	 * @return 
	 * Pointer to the component retrieved if the component exists
	 * nullptr if the component doesnt exist
	*/
	T* GetData(const EntityID& _entityID)
	{
		//Entity doesn't exist in map
		if (false == CheckIfExistInMap(_entityID))
			return nullptr;

		//Entity exists in map
		return &mComponentArray[_entityID];
	}

	/*!
	 * @brief
	 * Duplicates the component data and assigns it to the new input entity id
	 * @param _entityIDToCopy
	 * The entity id of the component to be duplicated
	 * @param _newEntityID
	 * The entity id of the newly duplicated component
	*/
	void DuplicateData(const EntityID& _entityIDToCopy, const EntityID& _newEntityID)
	{
		//Entity does not have this component
		if (false == CheckIfExistInMap(_entityIDToCopy))
			return;

		//Duplicate component data
		mComponentArray[_newEntityID] = mComponentArray[_entityIDToCopy];

		//Set entity ID and init component
		static_cast<ComponentBase&>(mComponentArray[_newEntityID]).mEntID = _newEntityID;
	}

	/*!
	 * @brief
	 * Deletes the corresponding component in the component array tied to this entityID if any
	 * @param _entityID
	 * The component's entity ID to check for
	*/
	void EntityDestroyed(const EntityID& _entityID)
	{
		if (true == CheckIfExistInMap(_entityID))
			RemoveData(_entityID);
	}

	/*!
	 * @brief 
	 * Get size of mComponentArray
	 * @return 
	 * Size of mComponentArray
	*/
	size_t GetSize()
	{
		return mComponentArray.size();
	}

	/*!
	 * @brief
	 * Prints out the component array in the debug console, mainly used for debugging
	*/
	void Print()
	{
#if _ENGINE
		DebugLogger::GetInstance()->Log("Component Array: %s\n", typeid(T).name());
		for (auto const& pair : mComponentArray)
		{
			DebugLogger::GetInstance()->Log("[%d: %s][%d]\n", pair.first, typeid(T).name(), typeid(pair.second).name());
		}
#endif // _ENGINE
	}

	/*!
	 * @brief
	 * Clones the component array and assigns it to a shared ptr
	 * @return
	 * Shared pointer pointing to the newly duplicated component array
	*/
	std::shared_ptr<IComponentArray> CloneShared() const
	{
		return std::make_shared<ComponentArray<T>>(*this);
	}

	/*!
	 * @brief
	 * Clones the component array and assigns it to a shared ptr
	 * @return
	 * Unique pointer pointing to the newly duplicated component array
	*/
	std::unique_ptr<IComponentArray> CloneUnique() const
	{
		return std::make_unique<ComponentArray<T>>(*this);
	}
public:
	
	/*!
	 * @brief 
	 * Checks if there is any component in the component array tied to the input entity id
	 * @param _entityID 
	 * Entity id to check for
	 * @return 
	 * true if found, false if not found
	*/
	bool CheckIfExistInMap(const EntityID& _entityID)
	{
		if (mComponentArray.find(_entityID) == mComponentArray.end())
			return false;

		return true;
	}

	//Container
	std::unordered_map<EntityID, T> mComponentArray{};
};

#endif