/*!
All content © 2023 DigiPen Institute of Technology Singapore, all rights reserved.
@file       EventManager.cpp
@author     TEO Wen Xi (100%)
@co-author

----------------------------------------------------------------------------

@course     CSD2401F23
@section    CSD2401F23-A
@project    CSD2401F23 Software Engineering Project 3

@brief      Event handling system for the engine.
*//*______________________________________________________________________*/

#include "EntityManager.h"
#include "ComponentsHeader.h"
#include "EventManager.h"

void EventManager::Update()
{
	//Iterate through the event queue and notify each observer
	while (false == mEventQueue.empty())
	{
		//Pop event from queue
		auto currEvent = mEventQueue.front();
		mEventQueue.pop();

		//Notify all observers
		NotifyObservers(currEvent.first, currEvent.second);

		//Delete event
		delete currEvent.second;
	}
}

void EventManager::Unload()
{
	//Clear queue data
	while (false == mEventQueue.empty())
	{
		//Pop event from queue
		auto currEvent = mEventQueue.front();
		mEventQueue.pop();

		//Delete event
		delete currEvent.second;
	}

	//Destroy instance
	Destroy();
}

void EventManager::NotifyObservers(const std::string& _typeName, EventObject* _evtObj)
{
	if (_typeName == typeid(EventPlayPause).name())
	{
		for (auto iter : mObserverMapPtrs[_typeName])
			if (iter != nullptr)
				iter->OnNotify(_evtObj);
	}
	else
		for (auto mCurrEntID : mObserverMap[_typeName])
		{
			if (mCurrEntID == 0 || EntityManager::GetInstance()->GetEntity(mCurrEntID) == nullptr || EntityManager::GetInstance()->GetEntity(mCurrEntID)->GetActive() == false)
				continue;

			Observer* ob = EntityManager::GetInstance()->GetEntity(mCurrEntID)->GetComponent<EnemyCollisionHandler>();
			if (ob != nullptr)
				ob->OnNotify(_evtObj);
			ob = nullptr;
			ob = EntityManager::GetInstance()->GetEntity(mCurrEntID)->GetComponent<KeyCollisionHandler>();
			if (ob != nullptr)
				ob->OnNotify(_evtObj);
			ob = nullptr;
			ob = EntityManager::GetInstance()->GetEntity(mCurrEntID)->GetComponent<GfxHandler>();
			if (ob != nullptr)
				ob->OnNotify(_evtObj);
			ob = nullptr;
			ob = EntityManager::GetInstance()->GetEntity(mCurrEntID)->GetComponent<PlayerCollisionHandler>();
			if (ob != nullptr)
				ob->OnNotify(_evtObj);
			ob = nullptr;
			ob = EntityManager::GetInstance()->GetEntity(mCurrEntID)->GetComponent<KeyWallCollisionHandler>();
			if (ob != nullptr)
				ob->OnNotify(_evtObj);
			ob = nullptr;
			ob = EntityManager::GetInstance()->GetEntity(mCurrEntID)->GetComponent<WindCollisionHandler>();
			if (ob != nullptr)
				ob->OnNotify(_evtObj);
			ob = nullptr;
			ob = EntityManager::GetInstance()->GetEntity(mCurrEntID)->GetComponent<NextCPCollisionHandler>();
			if (ob != nullptr)
				ob->OnNotify(_evtObj);
		}
}