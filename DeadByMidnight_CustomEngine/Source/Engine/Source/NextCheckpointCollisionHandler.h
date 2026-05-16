/*!
All content © 2023 DigiPen Institute of Technology Singapore, all rights reserved.
@file       NextCheckpointCollisionHandler.h
@author     TEO Wen Xi (100%)
@co-author

----------------------------------------------------------------------------

@course     CSD2401F23
@section    CSD2401F23-A
@project    CSD2401F23 Software Engineering Project 3

@brief		Collision handler observer for the next checkpoint collider.
*//*______________________________________________________________________*/

#ifndef NEXT_CP_HANDLER_H
#define NEXT_CP_HANDLER_H

#include "ComponentBase.h"
#include "EventsHeader.h"

#include "EntityManager.h"
#include "ComponentsHeader.h"

class NextCPCollisionHandler : public ComponentBase, public Observer
{
public:

	/*!
	 * @brief 
	 * Init function
	*/
	void Init()
	{
		EventManager::GetInstance()->AddObserver(EventCollision{}, mEntID);
	}

	/*!
	 * @brief 
	 * Update function
	*/
	void Update() {};

	/*!
	 * @brief 
	 * Exit function
	*/
	void Exit()
	{
		EventManager::GetInstance()->RemoveObserver(EventCollision{}, mEntID);
	}

	/*!
	 * @brief 
	 * Load function
	*/
	void Load() 
	{
		mHasCollided = JSONManager::GetInstance()->GetBool("HasCollided");
		mNextStageCollider = JSONManager::GetInstance()->GetBool("NextStageCollider");
		mNextSceneName = JSONManager::GetInstance()->GetString("NextSceneName");
	}

	/*!
	 * @brief 
	 * Save function
	*/
	void Save()
	{
		JSONManager::GetInstance()->Save("Component", "NextCPCollisionHandler");
		JSONManager::GetInstance()->Save("HasCollided", mHasCollided);
		JSONManager::GetInstance()->Save("NextStageCollider", mNextStageCollider);
		JSONManager::GetInstance()->Save("NextSceneName", mNextSceneName);
	}

	/*!
	 * @brief 
	 * Imgui display properties
	*/
	void DisplayProperties();
	
	/*!
	 * @brief 
	 * On notify function for the event handler
	 * @param _eventObj 
	 * The event object to read from
	*/
	void OnNotify(EventObject* _eventObj);

	/*!
	 * @brief 
	 * Cout overload function for component debug printout
	 * @param os 
	 * The output stream to write into
	 * @param _handler 
	 * The input class object
	 * @return 
	 * The output stream with the written string
	*/
	friend std::ostream& operator<< (std::ostream& os, const NextCPCollisionHandler& _handler)
	{
		os << "Next CP Collision Handler tied to Ent ID " << _handler.mEntID;
		return os;
	}

public:
	bool mHasCollided = false;
	bool mNextStageCollider = false;
	std::string mNextSceneName = "";
};

#endif