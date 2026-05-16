/*!
All content © 2023 DigiPen Institute of Technology Singapore, all rights reserved.
@file       EventList.h
@author     TEO Wen Xi (100%)
@co-author

----------------------------------------------------------------------------

@course     CSD2401F23
@section    CSD2401F23-A
@project    CSD2401F23 Software Engineering Project 3

@brief		List of all possible events in the system
*//*______________________________________________________________________*/

#ifndef EVENT_LIST_H
#define EVENT_LIST_H

#include "ECS_Types.h"
#include "Collision.h"

//Base object for polymorphism of events
class EventObject
{
public:

	/*!
	 * @brief 
	 * Empty function so that this class can be used as a abstract class
	*/
	virtual void doNothing() {}
};

class EventPlayer : public EventObject
{
public:
	enum PlayerEvent
	{
		NO_ACTION,

		//Player actions
		PLAYER_JUMP,
		PLAYER_MOVE,

		PLAYER_COLLIDE_ENEMY,
	} mEventType;

	/*!
	 * @brief 
	 * Default Ctor
	*/
	EventPlayer() : mEventType(NO_ACTION) {}

	/*!
	 * @brief 
	 * Overloaded ctor
	 * @param _pEvent 
	 * Player event enum type
	*/
	EventPlayer(PlayerEvent _pEvent) : mEventType(_pEvent) {}
};

class EventCollision : public EventObject
{
public:

	/*!
	 * @brief 
	 * Default Ctor
	*/
	EventCollision() : mEntA(0), mEntB(0), mColDir(Collision::CollisionDir::CD_NONE) {}

	/*!
	 * @brief 
	 * Overloaded Ctor
	 * @param _entA 
	 * ID of collided entity A
	 * @param _entB 
	 * ID of collided entity B
	 * @param _colDir 
	 * Direction of the collision
	*/
	EventCollision(EntityID _entA, EntityID _entB, Collision::CollisionDir _colDir) 
		: mEntA(_entA)
		, mEntB(_entB) 
		, mColDir(_colDir)
	{}

	/*!
	 * @brief 
	 * Same entity cheeck
	 * @return
	 * True if entA is same as entB
	*/
	bool SameEntCheck() { return mEntA == mEntB; }
	EntityID mEntA, mEntB;
	Collision::CollisionDir mColDir;
};

class EventGfx : public EventObject
{
public:
	enum GfxEvent
	{
		NO_ACTION,
		GFX_COLLIDED
	} mEventType;

	/*!
	 * @brief
	 * Constructor for EventGfx.
	*/
	EventGfx() : mEventType(NO_ACTION), mEntA(0), mEntB(0) {}
	
	/*!
	 * @brief
	 * Overloaded constructor for EventGfx.
	 * @param _entA 
	 * The ID of the entity to set.
	 * @param _gfxEvent 
	 * The event to set.
	*/
	EventGfx(EntityID _entA, GfxEvent _gfxEvent) : mEventType(_gfxEvent), mEntA(_entA), mEntB(0) {}

	/*!
	 * @brief
	 * Overloaded constructor for EventGfx.
	 * @param _entA 
	 * The ID of the first entity to set.
	 * @param _entB 
	 * The ID of the second entity to set.
	 * @param _gfxEvent 
	 * The event to set.
	*/
	EventGfx(EntityID _entA, EntityID _entB, GfxEvent _gfxEvent) : mEventType(_gfxEvent), mEntA(_entA), mEntB(_entB) {}

	EntityID mEntA, mEntB;
};

class EventPlayPause : public EventObject
{
public:
	enum PLAY_PAUSE_EVENT
	{
		PLAY,
		PAUSE,
		STOP
	} mEventType;

	/*!
	 * @brief 
	 * Default Ctor
	*/
	EventPlayPause() : mEventType(PLAY) {}

	/*!
	 * @brief 
	 * Overloaded Ctor
	 * @param _event 
	 * The input enum type
	*/
	EventPlayPause(PLAY_PAUSE_EVENT _event) : mEventType(_event) {}
};

#endif