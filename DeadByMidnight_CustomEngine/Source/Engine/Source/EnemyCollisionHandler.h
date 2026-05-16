/*!
All content © 2023 DigiPen Institute of Technology Singapore, all rights reserved.
@file       EnemyCollisionHandler.h
@author     TEO Wen Xi (100%)
@co-author

----------------------------------------------------------------------------

@course     CSD2401F23
@section    CSD2401F23-A
@project    CSD2401F23 Software Engineering Project 3

@brief		Collision handler observer for enemies. Mainly used to test the
			event handling system.
*//*______________________________________________________________________*/

#ifndef ENEMY_COLLISION_HANDLER_H
#define ENEMY_COLLISION_HANDLER_H

#include "ComponentBase.h"
#include "EventsHeader.h"

class EnemyCollisionHandler : public ComponentBase, public Observer
{
public:

	/*!
	 * @brief 
	 * Init function
	*/
	void Init()
	{
		EventManager::GetInstance()->AddObserver(EventPlayer{}, mEntID);
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
		EventManager::GetInstance()->RemoveObserver(EventPlayer{}, mEntID);
	}

	/*!
	 * @brief 
	 * Load function
	*/
	void Load() {};

	/*!
	 * @brief 
	 * Save function
	*/
	void Save() { JSONManager::GetInstance()->Save("Component", "EnemyCollisionHandler"); };

	/*!
	 * @brief 
	 * Imgui display function
	*/
	void DisplayProperties() {};

	/*!
	 * @brief 
	 * On notify function for the event handler
	 * @param _eventObj 
	 * The input EventObject
	*/
	void OnNotify(EventObject* _eventObj)
	{
		EventPlayer* playerEvent = dynamic_cast<EventPlayer*>(_eventObj);
		switch (playerEvent->mEventType)
		{
		case EventPlayer::PLAYER_COLLIDE_ENEMY:
			//std::cout << "ENEMY: COLLIDED" << std::endl;
			break;
		case EventPlayer::PLAYER_MOVE:
			// if gfx debug mode enabled, draw line to represent the velocity
			break;
		}
	}

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
	friend std::ostream& operator<< (std::ostream& os, const EnemyCollisionHandler& _handler)
	{
		os << "Enemy Collision Handler tied to Ent ID " << _handler.mEntID;
		return os; 
	}
};

#endif