/*!
All content © 2023 DigiPen Institute of Technology Singapore, all rights reserved.
@file       KeyCollisionHandler.h
@author     TEO Wen Xi (100%)
@co-author

----------------------------------------------------------------------------

@course     CSD2401F23
@section    CSD2401F23-A
@project    CSD2401F23 Software Engineering Project 3

@brief		Collision handler observer for the key object.
*//*______________________________________________________________________*/

#ifndef KEY_HANDLER_H
#define KEY_HANDLER_H

#include "ComponentBase.h"
#include "EventsHeader.h"

#include "EntityManager.h"
#include "ComponentsHeader.h"
#include "Key.h"

class KeyCollisionHandler : public ComponentBase, public Observer
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
		JSONManager::GetInstance()->Save("Component", "KeyCollisionHandler");
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
			Entity* currEnt = EntityManager::GetInstance()->GetEntity(mEntID);

			//Player already has key
			if (EntityManager::GetInstance()->GetEntity(colEvent->mEntB)->GetComponent<Player>()->mHasKey == true)
				return;

			//Update key state
			currEnt->GetComponent<Key>()->mKeyState = Key::KeyState::COLLECTEDHOVER;
			if (currEnt->GetComponent<Key>()->mSpawnLocation == Vector3(0, 0, 0))
				currEnt->GetComponent<Key>()->mSpawnLocation = currEnt->GetComponent<Transform>()->GetPosition();

			//Set parent item to player
			EntityManager::GetInstance()->GetEntity(colEvent->mEntB)->AddChild(mEntID);
			//Update player to having the key
			EntityManager::GetInstance()->GetEntity(colEvent->mEntB)->GetComponent<Player>()->mHasKey = true;
			EntityManager::GetInstance()->GetEntity(colEvent->mEntB)->GetComponent<Player>()->mAttachedKeyID = mEntID;
			//Update position
			currEnt->GetComponent<Transform>()->SetLocalPosition(currEnt->GetComponent<Key>()->mOffsetFromPlayer);
			//Play SFX for collecting a key
			if(currEnt->GetComponent<Key>()->mHideImage == false)
				SoundManager::GetInstance()->PlaySound("SFX_CollectItem", SoundManager::GetInstance()->GetSFXVolume(), SoundManager::GetInstance()->GetChannelPitch());
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
	friend std::ostream& operator<< (std::ostream& os, const KeyCollisionHandler& _handler)
	{
		os << "Key Collision Handler tied to Ent ID " << _handler.mEntID;
		return os;
	}
};

#endif