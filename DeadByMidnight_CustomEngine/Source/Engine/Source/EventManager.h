/*!
All content © 2023 DigiPen Institute of Technology Singapore, all rights reserved.
@file       EventManager.h
@author     TEO Wen Xi (100%)
@co-author	

----------------------------------------------------------------------------

@course     CSD2401F23
@section    CSD2401F23-A
@project    CSD2401F23 Software Engineering Project 3

@brief      Event handling system for the engine.
*//*______________________________________________________________________*/

#ifndef EVENT_MANAGER_H
#define EVENT_MANAGER_H

#include "Singleton.h"
#include "Manager.h"

#include "EventList.h"
#include "Observer.h"

#include <unordered_map>
#include <vector>
#include <queue>
#include <string>
#include <typeinfo>
#include <algorithm>

class EventManager : public Singleton<EventManager>, public Manager
{
public:
	//Manager

	/*!
	 * @brief 
	 * Init function
	*/
	void Init() {}

	/*!
	 * @brief 
	 * Load function
	*/
	void Load() {}

	/*!
	 * @brief 
	 * Update function
	*/
	void Update();

	/*!
	 * @brief 
	 * Unload function
	*/
	void Unload();

	//Observer Subscriber Stuff

	/*!
	 * @brief 
	 * Adds an observer tied to the input event type
	 * @tparam EventType 
	 * Type name of the event type
	 * @param _eventType 
	 * Event type the observer is tied to
	 * @param _observerEntID 
	 * EntityID of the observer entity
	*/
	template <typename EventType>
	void AddObserver(EventType _eventType, EntityID _observerEntID)
	{
		mObserverMap[typeid(_eventType).name()].emplace_back(_observerEntID);
	}

	/*!
	 * @brief 
	 * Adds an observer tied to the input event type
	 * @tparam EventType 
	 * Type name of the event type
	 * @param _eventType 
	 * Event type the observer is tied to
	 * @param _observer 
	 * Pointer to the observer object to be added
	*/
	template <typename EventType>
	void AddObserver(EventType _eventType, Observer* _observer)
	{
		mObserverMapPtrs[typeid(_eventType).name()].emplace_back(_observer);
	}

	/*!
	 * @brief 
	 * Removes an observer tied to the input event type
	 * @tparam EventType 
	 * Type name of the event type
	 * @param _eventType 
	 * Event type the observer is tied to
	 * @param _observerEntID 
	 * Observer object ID
	*/
	template <typename EventType>
	void RemoveObserver(EventType _eventType, EntityID _observerEntID)
	{
		std::string typeName{ typeid(_eventType).name() };

		//Search and remove observer
		auto iter = std::find(mObserverMap[typeName].begin(), mObserverMap[typeName].end(), _observerEntID);
		if (iter != mObserverMap[typeName].end())
			mObserverMap[typeName].erase(iter);
	}

	/*!
	 * @brief 
	 * Removes an observer tied to the input event type
	 * @tparam EventType 
	 * EventType class
	 * @param _eventType
	 * Event type of the observer to be removed
	 * @param _observer 
	 * Observer to be removed from the input event type
	*/
	template <typename EventType>
	void RemoveObserver(EventType _eventType, Observer* _observer)
	{
		std::string typeName{ typeid(_eventType).name() };

		//Search and remove observer
		auto iter = std::find(mObserverMapPtrs[typeName].begin(), mObserverMapPtrs[typeName].end(), _observer);
		if (iter != mObserverMapPtrs[typeName].end())
			mObserverMapPtrs[typeName].erase(iter);
	}

	/*!
	 * @brief 
	 * Adds an event to the event queue
	 * @tparam EventType 
	 * EventType class
	 * @param _newEvent
	 * Event object to be added
	*/
	template <typename EventType>
	void AddEvent(EventType _newEvent)
	{
		//Create event, cast it to base type and add to queue
		EventType* newEvtObj = new EventType(_newEvent);
		mEventQueue.push(std::make_pair(typeid(_newEvent).name(), dynamic_cast<EventObject *>(newEvtObj)));
	}

private:
	friend Singleton<EventManager>;

	/*!
	 * @brief 
	 * Notifies all observers of the respective event object
	 * @param _typeName 
	 * Class name of the event object called
	 * @param _evtObj 
	 * The event object
	*/
	void NotifyObservers(const std::string& _typeName, EventObject* _evtObj);

	//Queue containing messages to be sent to each observer
	std::queue<std::pair<std::string, EventObject *>> mEventQueue;

	//Container containing all observers and their respective subscribed events
	std::unordered_map<std::string, std::vector<EntityID>> mObserverMap;

	//MonoManager
	std::unordered_map<std::string, std::vector<Observer*>> mObserverMapPtrs;
};

#endif