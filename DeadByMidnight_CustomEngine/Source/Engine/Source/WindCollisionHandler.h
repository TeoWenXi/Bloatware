/*!
All content © 2023 DigiPen Institute of Technology Singapore, all rights reserved.
@file       WindCollisionHandler.h
@author     TEO Wen Xi (100%)
@co-author

----------------------------------------------------------------------------

@course     CSD2401F23
@section    CSD2401F23-A
@project    CSD2401F23 Software Engineering Project 3

@brief		Collision handler observer for the wind object.
*//*______________________________________________________________________*/

#ifndef WIND_HANDLER_H
#define WIND_HANDLER_H

#include "ComponentBase.h"
#include "EventsHeader.h"

#include "EntityManager.h"
#include "ComponentsHeader.h"
#include "SoundManager.h"

class WindCollisionHandler : public ComponentBase, public Observer
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
	void Load() {};

	/*!
	 * @brief 
	 * Save function
	*/
	void Save()
	{
		JSONManager::GetInstance()->Save("Component", "WindCollisionHandler");
	};

	/*!
	 * @brief 
	 * Imgui display properties
	*/
	void DisplayProperties() {};

	/*!
	 * @brief 
	 * On notify function for the event handler
	 * @param _eventObj 
	 * The event object to read from
	*/
	void OnNotify(EventObject* _eventObj)
	{
		EventCollision* colEvent = dynamic_cast<EventCollision*>(_eventObj);
		if (colEvent)
		{
			//Ensures that this object is in mEntA
			if (colEvent->SameEntCheck())
				return;
			if (colEvent->mEntB == mEntID)
				std::swap(colEvent->mEntA, colEvent->mEntB);

			//Check if this entity is the item that collided
			if (colEvent->mEntA != mEntID)
				return;

			//Check if other collided object is player
			if (EntityManager::GetInstance()->GetEntity(colEvent->mEntB)->GetComponent<Player>() == nullptr)
				return;

			//Collided object is player

			//Get wind's orientation
			Entity* currEnt = EntityManager::GetInstance()->GetEntity(mEntID);
			Transform* xform = currEnt->GetComponent<Transform>();
			Wind::mPlayWindSound = true;

			//Get player's physics component
			PhysicsComponent* pPhy = EntityManager::GetInstance()->GetEntity(colEvent->mEntB)->GetComponent<PhysicsComponent>();

			//Upwards wind
			if (xform->mRotation == 0)
				pPhy->mIsCharacterWindTunnelUp = true;
			//Sideways
			else if ((int)xform->mRotation % 180 != 0 && (int)xform->mRotation % 90 == 0)
			{
				//Right
				if ((int)xform->mRotation % 270 == 0)
					pPhy->mIsCharacterWindTunnelRight = true;
				//Left
				else
					pPhy->mIsCharacterWindTunnelLeft = true;
			}

			//if character is big
			if (!pPhy->mIsCharacterSmall)
			{
				pPhy->mIsCharacterWindTunnelUp = false;
				pPhy->mIsCharacterWindTunnelRight = false;
				pPhy->mIsCharacterWindTunnelLeft = false;
				Wind::mPlayWindSound = false;
			}
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
	friend std::ostream& operator<< (std::ostream& os, const WindCollisionHandler& _handler)
	{
		os << "Wind Collision Handler tied to Ent ID " << _handler.mEntID;
		return os;
	}
};

#endif