/*!
All content © 2023 DigiPen Institute of Technology Singapore, all rights reserved.
@file       ComponentManager.h
@author     TEO Wen Xi (100%)
@co-author

----------------------------------------------------------------------------

@course     CSD2401F23
@section    CSD2401F23-A
@project    CSD2401F23 Software Engineering Project 3

@brief      Manages all the components the entity component system
*//*______________________________________________________________________*/

#ifndef COMPONENT_MANAGER_H
#define COMPONENT_MANAGER_H
#define UNREFERENCED_PARAMETER(P) (P)

#include <unordered_map>
#include <string>
#include <memory>

#include "Singleton.h"
#include "Manager.h"
#include "ComponentArray.h"

#if _ENGINE
#include "DebugLogger.h"
#endif // _ENGINE

class ComponentManager : public Singleton<ComponentManager>, public Manager
{
public:
	//Manager functions, mainly for unload only

	/*!
	 * @brief
	 * Init function of the component manager
	*/
	void Init() {}

	/*!
	 * @brief
	 * Load function of the component manager
	*/
	void Load() {}

	/*!
	 * @brief
	 * Update function of the component manager
	*/
	void Update() {}

	/*!
	 * @brief 
	 * Unload function of the component manager
	*/
	void Unload()
	{
		//Destroy instance
		Destroy();
	}

	//Management Functions

	/*!
	 * @brief 
	 * Adds a component into the corresponding component array
	 * @tparam T 
	 * Type of the component to be added
	 * @param _entityID 
	 * Entity id of the component to be added
	 * @param component 
	 * The compomponent to be added
	 * @return 
	 * Returns a pointer to the newly added component in the component array if
	 * component has been successfully added
	 * Return nullptr if otherwise
	*/
	template <typename T>
	T* AddComponent(const EntityID& _entityID, const T& component)
	{
		//Get component array
		auto compArr = GetComponentArray<T>();

		//Create component array if it doesn't exist yet
		if (nullptr == compArr)
			compArr = CreateComponentArray<T>();

		//Successfully added
		if (compArr->AddData(_entityID, component))
			return compArr->GetData(_entityID);

		//Component already exists
		return nullptr;
	}

	//Overload that creates a default of the input component and returns it

	/*!
	 * @brief 
	 * Overload function of AddComponent that creates a default of the input component and returns it
	 * @tparam T 
	 * Type of the component to be added
	 * @param _entityID 
	 * Entity id of the component to be added
	 * @return 
	 * Returns a pointer to the newly added component in the component array if
	 * component has been successfully added
	 * Return nullptr if otherwise
	*/
	template <typename T>
	T* AddComponent(const EntityID& _entityID)
	{
		const T component{};

		//Get component array
		auto compArr = GetComponentArray<T>();

		//Create component array if it doesn't exist yet
		if (nullptr == compArr)
			compArr = CreateComponentArray<T>();

		//Successfully added
		if (compArr->AddData(_entityID, component))
			return compArr->GetData(_entityID);

		//Component already exists
		return nullptr;
	}

	//Returns false if entity's component doesnt exist in map
	//Return true if entity's component is successfully removed from the array

	/*!
	 * @brief 
	 * Removes the input component tied to the input entityID from the component array list
	 * @tparam T 
	 * Type of the component to be removed
	 * @param _entityID 
	 * Entity id of the component to be removed
	 * @return 
	 * Returns false if entity's component doesnt exist in map
	 * Return true if entity's component is successfully removed from the array
	*/
	template <typename T>
	bool RemoveComponent(const EntityID& _entityID)
	{
		//Get component array
		auto compArr = GetComponentArray<T>();

		//Successfully removed
		if (compArr->RemoveData(_entityID))
		{
			//Remove component array if empty
			if (compArr->GetSize() == 0)
				DestroyComponentArray<T>();

			return true;
		}

		//Component not found
		return false;
	}

	/*!
	 * @brief
	 * Retrives a component from the component list
	 * @tparam T 
	 * Type of the component to be retrieved
	 * @param _entityID 
	 * Entity id of the component to be retrieved
	 * @return 
	 * Returns a pointer to the component to be retrieved if successful
	 * Return nullptr if otherwise
	*/
	template <typename T>
	T* GetComponent(const EntityID& _entityID)
	{
		auto compArr = GetComponentArray<T>();

		//Check if array exists
		if (nullptr == compArr)
			return nullptr;

		return compArr->GetData(_entityID);
	}

	/*!
	 * @brief
	 * Deletes all components tied to the input entityID in the component array list
	 * @param _entityID
	 * The component's entity ID to check for
	*/
	void EntityDestroyed(const EntityID& _entityID)
	{
		//Notify all component arrays that an entity has been destroyed
		for (auto it = mComponentArrays.begin(); it != mComponentArrays.end();)
		{
			it->second->EntityDestroyed(_entityID);

			//Clear empty component arrays
			if (it->second->GetSize() == 0)
				mComponentArrays.erase(it++);
			else
				++it;
		}

	}

	/*!
	 * @brief
	 * Prints out the component array in the debug console, mainly used for debugging
	 * @param _backupVer 
	 * Bool to decide if to print the backup copy as well
	*/
	void Print(bool _backupVer)
	{
		UNREFERENCED_PARAMETER(_backupVer);
#if _ENGINE
		DebugLogger::GetInstance()->Log("\n\n");
		if (_backupVer)
			DebugLogger::GetInstance()->Log("BACKUP\n");
		DebugLogger::GetInstance()->Log("COMPONENT MANAGER PRINTING\n");

		if (_backupVer)
		{
			for (auto const& pair : mBackupComponentArrays)
			{
				mBackupComponentArrays[pair.first]->Print();
				DebugLogger::GetInstance()->Log("\n");
			}
		}
		else
		{
			for (auto const& pair : mComponentArrays)
			{
				mComponentArrays[pair.first]->Print();
				DebugLogger::GetInstance()->Log("\n");
			}
		}
#endif
	}

	//Get a component array

	/*!
	 * @brief
	 * Get a component array
	 * @tparam T 
	 * Type of the component array to be retrieved
	 * @return 
	 * Shared pointer pointing to the component array to be retrieved if succesful
	 * nullptr if otherwise
	*/
	template <typename T>
	std::shared_ptr<ComponentArray<T>> GetComponentArray()
	{
		//Convert typename to string
		std::string typeNameStr = typeid(T).name();

		//Check if component exists in current map
		if (mComponentArrays.find(typeNameStr) != mComponentArrays.end())
			return std::static_pointer_cast<ComponentArray<T>>(mComponentArrays[typeNameStr]);
#if _ENGINE
		//DebugLogger::GetInstance()->Log("Component %s does not exist\n", typeNameStr.c_str());
#endif // _ENGINE
		return nullptr;
	}

	/*!
	 * @brief 
	 * Duplicates all components tied to the input entity id and sets it to the input new entity id
	 * @param _entityIDToCopy 
	 * The entity id of the component to be duplicated
	 * @param _newEntityID 
	 * The entity id of the newly duplicated component
	*/
	void DuplicateEntityComponents(const EntityID& _entityIDToCopy, const EntityID& _newEntityID)
	{
		//Cycle through the component arrays to check which components this entity has
		for (auto const& pair : mComponentArrays)
			mComponentArrays[pair.first]->DuplicateData(_entityIDToCopy, _newEntityID);
	}

	//Scene Play & Stop

	/*!
	 * @brief 
	 * Duplicates the component array list into a backup array
	*/
	void BackupCurrScene()
	{
		//Create new container to store all component info
		for (auto const& pair : mComponentArrays)
			mBackupComponentArrays.insert({ pair.first, mComponentArrays[pair.first]->CloneUnique() });
	}

	/*!
	 * @brief 
	 * Loads the backup array
	*/
	void LoadSceneBackup()
	{
		mComponentArrays.clear();

		for (auto const& pair : mBackupComponentArrays)
			mComponentArrays.insert({pair.first, mBackupComponentArrays[pair.first]->CloneShared() });

		mBackupComponentArrays.clear();
	}

	/*!
	 * @brief 
	 * Clears the backup component array list
	*/
	void ClearSceneBackup()
	{
		mBackupComponentArrays.clear();
	}
private:
	//For singleton
	friend Singleton<ComponentManager>;

	//Container
	std::unordered_map<std::string, std::shared_ptr<IComponentArray>> mComponentArrays;

	/*!
	 * @brief Creating & Destroying a component array
	 * @tparam T 
	 * Type of the component array to be created
	 * @return 
	 * Static pointer pointing to the newly created component array
	*/
	template <typename T>
	std::shared_ptr<ComponentArray<T>> CreateComponentArray()
	{
		//Convert typename to string
		std::string typeNameStr = typeid(T).name();

		//Create new component array
		mComponentArrays.insert({ typeNameStr, std::make_shared<ComponentArray<T>>() });
#if _ENGINE
		DebugLogger::GetInstance()->Log("Component %s container has been created\n", typeNameStr.c_str());
#endif // _ENGINE

		return std::static_pointer_cast<ComponentArray<T>>(mComponentArrays[typeNameStr]);
	}

	/*!
	 * @brief 
	 * Destroys a component array
	 * @tparam T 
	 * The type of the component array to be destroyed
	*/
	template <typename T>
	void DestroyComponentArray()
	{
		//Destroy component array if it is empty
		mComponentArrays.erase(typeid(T).name());
	}

	//Scene Play & Stop
	std::unordered_map<std::string, std::unique_ptr<IComponentArray>> mBackupComponentArrays;
};

#endif