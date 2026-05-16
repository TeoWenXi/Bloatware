/*!
All content © 2023 DigiPen Institute of Technology Singapore, all rights reserved.
@file       MonoManager.h
@author		HEAR Li Heng (100%)
@co-author	

----------------------------------------------------------------------------

@course     CSD2401F23
@section    CSD2401F23-A
@project    CSD2401F23 Software Engineering Project 3

@brief      Creates and runs the C# runtime to be able to modify and run
			C# scripts during runtime.
*//*______________________________________________________________________*/
#pragma once
#include "Singleton.h"
#include "Manager.h"
#include <string>
#include "Observer.h"
#include "CSScriptComponent.h"

class CSScriptComponent;
class MonoManager : public Singleton <MonoManager>, public Manager, public Observer
{
public:
	/*!
	 * @brief 
	 * Inits the Mono runtime and sets up the internal calls.
	*/
	void Init();

	/*!
	 * @brief 
	 * Loads the MonoManager before Update loop.
	*/
	void Load();

	/*!
	 * @brief 
	 * Updates the MonoManager and calls all registered scripts.
	*/
	void Update();

	/*!
	 * @brief 
	 * Unloads the Mono domain and destroys this instance.
	*/
	void Unload();

	/*!
	 * @brief 
	 * Calls Compile() to compile the C# script at that location.
	 * @param fullPath
	 * The fullpath to the C# script file.
	 * @param dataPtr 
	 * The pointer to the data but NOT used in this case.
	*/
	void CreateScriptAsset(std::string _fullPath, void*& _dataPtr);

	/*!
	 * @brief 
	 * Removes the script info from the scriptList
	 * @param dataPtr 
	 * The name of the script that was removed
	*/
	static void DeleteScriptAsset(void*& _dataPtr);

	/*!
	 * @brief 
	 * Registers the script assembly into the Mono domain and gets
	 * the relevant methods for us to update and call the script
	 * @param entID 
	 * The entity id that contains the script
	 * @param scriptName 
	 * The name of the script to be added
	*/
	void RegisterScript(uint64_t _entID, std::string _scriptName, CSScriptComponent* _component);

	/*!
	 * @brief 
	 * Removes the script assembly from the Mono domain and
	 * so we will not update it anymore
	 * @param entID 
	 * The entity id that contains the script
	 * @param scriptName 
	 * The name of the script to be removed
	*/
	void DeregisterScript(uint64_t _entID, std::string _scriptName);

	/*!
	 * @brief
	 * Handles whatever happens when an event has notified this handler.
	 * @param _eventObj
	 * Pointer to the event object that notified this handler.
	*/
	void OnNotify(EventObject* _eventObj);


	/*!
	 * @brief
	 * Reloads the domain by removing all previous loaded assemblies
	 * and registering them again
	*/
	void Reload();

	/*!
	 * @brief 
	 * Updates the fields in the mono environment with the values from the engine
	 * @param info 
	 * The list of serialized infos to update to
	 * @param component 
	 * The component which the scriptobject is linked to
	*/
	void UpdateSerializedValues(std::vector<CSScriptComponent::SerializedInfo>& info, CSScriptComponent* component);

	/*!
	 * @brief 
	 * Sets to be first run so Start would be called
	*/
	void SetFirstRun();

	/*!
	 * @brief 
	 * Gets called by PhysicsSystem when 2 entities collided for the first time
	 * @param thisID 
	 * The first entity that collided
	 * @param otherID 
	 * The second entity that collided
	*/
	void OnTriggerEnter(EntityID thisID, EntityID otherID);

	/*!
	 * @brief
	 * Gets called by PhysicsSystem when 2 entities collided every update
	 * @param thisID
	 * The first entity that collided
	 * @param otherID
	 * The second entity that collided
	*/
	void OnTriggerStay(EntityID thisID, EntityID otherID);

	/*!
	 * @brief
	 * Gets called by PhysicsSystem when 2 entities stop collided for the first time
	 * @param thisID
	 * The first entity that stop colliding
	 * @param otherID
	 * The second entity that stop colliding
	*/
	void OnTriggerExit(EntityID thisID, EntityID otherID);

	/*!
	 * @brief 
	 * Gets called by EngineDLL when a script want to run another function in another script
	 * This is only temp cause i havent figure out how to link 2 different script together
	 * @param entityID 
	 * The other entity who contains the function we want to run
	 * @param funcName 
	 * The name of the function to run
	*/
	void RunFunction(EntityID entityID, std::string funcName);

private:

	/*!
	 * @brief 
	 * Uses the roslyn compiler to compile a C# file and stores
	 * the relevant info into mScriptList
	 * @param scriptPath 
	 * The path to the C# file
	*/
	void Compile(std::string _scriptPath);

	/*!
	 * @brief 
	 *	Registers all the internal calls that the scripts might use
	*/
	void RegisterInternalCalls();

	//Keeps track of when triggers happen so the correct functions gets called
	enum TRIGGER_FUNC
	{
		ENTER,
		STAY,
		EXIT
	};
	std::unordered_map<EntityID, std::list< std::pair<EntityID, TRIGGER_FUNC>>> mTriggerFunctions;
};

