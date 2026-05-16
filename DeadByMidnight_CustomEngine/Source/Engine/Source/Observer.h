/*!
All content © 2023 DigiPen Institute of Technology Singapore, all rights reserved.
@file       Observer.h
@author     TEO Wen Xi (100%)
@co-author

----------------------------------------------------------------------------

@course     CSD2401F23
@section    CSD2401F23-A
@project    CSD2401F23 Software Engineering Project 3

@brief		Base component for all observers for the event handling system.
*//*______________________________________________________________________*/

#ifndef OBSERVER_H
#define OBSERVER_H

#include "EventList.h"

class Observer
{
public:

	/*!
	 * @brief
	 * Virtual function that handles whatever happens when an event has notified this handler.
	 * @param _eventObj
	 * Pointer to the event object that notified this handler.
	*/
	virtual void OnNotify(EventObject * _eventObj) = 0;
};

#endif
