/*!
All content © 2023 DigiPen Institute of Technology Singapore, all rights reserved.
@file       KeyWallCollisionHandler.h
@author     TEO Wen Xi (100%)
@co-author

----------------------------------------------------------------------------

@course     CSD2401F23
@section    CSD2401F23-A
@project    CSD2401F23 Software Engineering Project 3

@brief		Collision handler observer for the key wall object
*//*______________________________________________________________________*/

#ifndef KEY_WALL_HANDLER_H
#define KEY_WALL_HANDLER_H

#include "ComponentBase.h"
#include "EventsHeader.h"

#include "EntityManager.h"
#include "ComponentsHeader.h"
#include "WindowsManager.h"

class KeyWallCollisionHandler : public ComponentBase, public Observer
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
		JSONManager::GetInstance()->Save("Component", "KeyWallCollisionHandler");
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
		Renderer* rdr;
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

			//Wall already unlocked
			if (currEnt->GetComponent<KeyWall>()->mLocked == false)
				return;

			//Player doesnt have key
			if (EntityManager::GetInstance()->GetEntity(colEvent->mEntB)->GetComponent<Player>()->mHasKey == false)
			{
				//Player components
				Entity* playerEnt = EntityManager::GetInstance()->GetEntity(colEvent->mEntB);
				Transform* playerXform = playerEnt->GetComponent<Transform>();
				PhysicsComponent* playerPhy = playerEnt->GetComponent<PhysicsComponent>();
				
				//Key wall's transform
				Transform* wallXform = currEnt->GetComponent<Transform>();

				//Push player back
				//Player on top of Wall
				if (colEvent->mColDir == Collision::CD_BOTTOM || colEvent->mColDir == Collision::CD_BOTTOM_LEFT || colEvent->mColDir == Collision::CD_BOTTOM_RIGHT)
				{
					if (playerPhy->mCurrentVelocity.mY < 0)
					{
						Vector3 newPos = Vector3{ playerXform->GetPosition().mX,
												  wallXform->GetPosition().mY + abs(wallXform->mScale.mY) / 2 + abs(playerXform->mScale.mY) / 2 + 0.01f,
												  playerXform->GetPosition().mZ };
						playerXform->SetPosition(newPos);
						playerPhy->SetOnGround();
						playerEnt->GetComponent<Player>()->mIsOnGround = true;
					}
				}
				//Player below of Wall
				else if (colEvent->mColDir == Collision::CD_TOP || colEvent->mColDir == Collision::CD_TOP_LEFT || colEvent->mColDir == Collision::CD_TOP_RIGHT)
				{
					if (playerPhy->mCurrentVelocity.mY > 0)
					{
						Vector3 newPos = Vector3{ playerXform->GetPosition().mX,
												  wallXform->GetPosition().mY - abs(wallXform->mScale.mY) / 2 - abs(playerXform->mScale.mY) / 2 - 0.01f,
												  playerXform->GetPosition().mZ };
						playerXform->SetPosition(newPos);
						playerPhy->mCurrentVelocity.mY = 0;
					}
				}
				//Player right of Wall
				else if (colEvent->mColDir == Collision::CD_LEFT)
				{
					Vector3 newPos = Vector3{ wallXform->GetPosition().mX + (abs(wallXform->mScale.mX) + abs(playerXform->mScale.mX)) / 2,
												  playerXform->GetPosition().mY,
												  playerXform->GetPosition().mZ };
					playerXform->SetPosition(newPos);

					//Reset Vel X as player has collided with something
					playerPhy->mCurrentVelocity.mX = 0;
				}
				//Player left of Wall
				else if (colEvent->mColDir == Collision::CD_RIGHT)
				{
					Vector3 newPos = Vector3{ wallXform->GetPosition().mX - (abs(wallXform->mScale.mX) + abs(playerXform->mScale.mX)) / 2,
												  playerXform->GetPosition().mY,
												  playerXform->GetPosition().mZ };
					playerXform->SetPosition(newPos);

					playerPhy->mCurrentVelocity.mX = 0;
				}
			}
			//Player has key
			else
			{
				//Player reference
				Player* playerRef = EntityManager::GetInstance()->GetEntity(colEvent->mEntB)->GetComponent<Player>();
				//Key Object
				Entity* keyObj = EntityManager::GetInstance()->GetEntity(playerRef->mAttachedKeyID);
				if (keyObj == nullptr || keyObj->GetComponent<Key>() == nullptr)
					return;

				//Set key to be used
				keyObj->GetComponent<Key>()->mKeyState = Key::KeyState::USED;
				rdr = keyObj->GetComponent<Renderer>();
				rdr->mEnabled = false;
				keyObj->SetActive(false);
				playerRef->mHasKey = false;

				//Set wall to be open
				rdr = currEnt->GetComponent<Renderer>();
				rdr->mEnabled = false;
				currEnt->SetActive(false);

				//Play SFX for unlocking a keywall
				SoundManager::GetInstance()->PlaySound("SFX_CollectItemAndUnlock", SoundManager::GetInstance()->GetSFXVolume(), SoundManager::GetInstance()->GetChannelPitch());
			}

			//Reset collision
			EntityManager::GetInstance()->GetEntity(colEvent->mEntB)->GetComponent<PhysicsComponent>()->mCollisionDir = Collision::CD_NONE;
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
	friend std::ostream& operator<< (std::ostream& os, const KeyWallCollisionHandler& _handler)
	{
		os << "Key Collision Handler tied to Ent ID " << _handler.mEntID;
		return os;
	}
};

#endif