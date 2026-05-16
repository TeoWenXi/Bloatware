/*!
All content © 2023 DigiPen Institute of Technology Singapore, all rights reserved.
@file       PlayerCollisionHandler.cpp
@author     TEO Wen Xi (100%)
@co-author

----------------------------------------------------------------------------

@course     CSD2401F23
@section    CSD2401F23-A
@project    CSD2401F23 Software Engineering Project 3

@brief		Player collision handler (observer). Handles all events for the player.
*//*______________________________________________________________________*/

#include "PlayerCollisionHandler.h"

#include "ComponentsHeader.h"
#include "EntityManager.h"

#include <algorithm>

void PlayerCollisionHandler::Init()
{
	EventManager::GetInstance()->AddObserver(EventPlayer{}, mEntID);
	EventManager::GetInstance()->AddObserver(EventCollision{}, mEntID);
}

void PlayerCollisionHandler::Update() 
{
}

void PlayerCollisionHandler::Exit()
{
	EventManager::GetInstance()->RemoveObserver(EventPlayer{}, mEntID);
	EventManager::GetInstance()->RemoveObserver(EventCollision{}, mEntID);
}


void PlayerCollisionHandler::Save()
{
	JSONManager::GetInstance()->Save("Component", "PlayerCollisionHandler");
}

void PlayerCollisionHandler::Load() 
{
}

void PlayerCollisionHandler::DisplayProperties()
{
#if _ENGINE
#endif // _ENGINE
}

void PlayerCollisionHandler::OnNotify(EventObject* _eventObj)
{
	EventPlayer* playerEvent = dynamic_cast<EventPlayer*>(_eventObj);
	if (playerEvent)
	{
		switch (playerEvent->mEventType)
		{
		case EventPlayer::PLAYER_COLLIDE_ENEMY:
			//std::cout << "PLAYER: COLLIDED" << std::endl;
			break;
		}
	}

	EventCollision* colEvent = dynamic_cast<EventCollision*>(_eventObj);
	if (colEvent)
	{
		if (colEvent->SameEntCheck())
			return;
		if (colEvent->mEntB == mEntID)
			std::swap(colEvent->mEntA, colEvent->mEntB);
		if (colEvent->mEntA == mEntID)
		{
			//Check if other collided object is enemy
			if (EntityManager::GetInstance()->GetEntity(colEvent->mEntB)->GetComponent<EnemyCollisionHandler>())
			{
				Player* g_Player = EntityManager::GetInstance()->GetEntity(colEvent->mEntA)->GetComponent<Player>();
				if (g_Player != nullptr)
				{
					g_Player->Kill();
				}

				//Destroy Prefab
				//EntityManager::GetInstance()->DestroyEntity(colEvent->mEntB);
			}
		}
	}
}

std::ostream& operator<< (std::ostream& os, const PlayerCollisionHandler& _handler)
{
	os << "Player Collision Handler tied to Ent ID " << _handler.mEntID;
	return os;
}