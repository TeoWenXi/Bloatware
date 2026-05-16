/*!
All content © 2023 DigiPen Institute of Technology Singapore, all rights reserved.
@file       GfxHandler.cpp
@author     CAI Wenjin (100%)
@co-author

----------------------------------------------------------------------------

@course     CSD2401F23
@section    CSD2401F23-A
@project    CSD2401F23 Software Engineering Project 3

@brief      Handles the graphical side of the events that occur.
*//*______________________________________________________________________*/

#include "GfxHandler.h"

void GfxHandler::Init()
{
	EventManager::GetInstance()->AddObserver(EventGfx{}, mEntID);
}

void GfxHandler::Update()
{
}

void GfxHandler::Exit()
{
	EventManager::GetInstance()->RemoveObserver(EventGfx{}, mEntID);
}

void GfxHandler::Save()
{
}

void GfxHandler::Load()
{
}

void GfxHandler::DisplayProperties()
{
#if _ENGINE
#endif // _ENGINE
}

void GfxHandler::OnNotify(EventObject* _eventObj)
{
	EventGfx* gfxEvent = dynamic_cast<EventGfx*>(_eventObj);
	if (gfxEvent)
	{
		Collider_AABB* col1{nullptr};
		Collider_AABB* col2{nullptr};
		switch (gfxEvent->mEventType)
		{
			case EventGfx::GFX_COLLIDED:
			{
				col1 = EntityManager::GetInstance()->GetEntity(gfxEvent->mEntA)->GetComponent<Collider_AABB>();

				if (gfxEvent->mEntB != 0)
					col2 = EntityManager::GetInstance()->GetEntity(gfxEvent->mEntB)->GetComponent<Collider_AABB>();

				if (col1 == nullptr && col2 == nullptr)
					return;

				col1->SetIsCollided(true);

				if (col2)
					col2->SetIsCollided(true);
			}
			break;
			default:
				break;
		}
	}
}

std::ostream& operator<<(std::ostream& _os, const GfxHandler& _handler)
{
	return _os << "Gfx handler tied to Ent ID" << _handler.mEntID;
}
