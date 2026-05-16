/*!
All content © 2023 DigiPen Institute of Technology Singapore, all rights reserved.
@file       PlayerCollisionHandler.h
@author     TEO Wen Xi (100%)
@co-author

----------------------------------------------------------------------------

@course     CSD2401F23
@section    CSD2401F23-A
@project    CSD2401F23 Software Engineering Project 3

@brief		Player collision handler (observer). Handles all events for the player.
*//*______________________________________________________________________*/

#ifndef PLAYER_COLLISION_HANDLER_H
#define PLAYER_COLLISION_HANDLER_H

#include "ComponentBase.h"
#include "EventsHeader.h"
#include "ComponentsHeader.h"
#include "EntityManager.h"

#include <algorithm>

class PlayerCollisionHandler : public ComponentBase, public Observer
{
public:

	/*!
	 * @brief 
	 * Init function
	*/
	void Init();

	/*!
	 * @brief 
	 * Update function
	*/
	void Update();

	/*!
	 * @brief 
	 * Exit function
	*/
	void Exit();

	/*!
	 * @brief 
	 * Save function
	*/
	void Save();

	/*!
	 * @brief 
	 * Load function
	*/
	void Load();

	/*!
	 * @brief 
	 * Imgui display properties
	*/
	void DisplayProperties();

	/*!
	 * @brief 
	 * On notify function for the event handler
	 * @param _eventObj 
	 * The event object to read from
	*/
	void OnNotify(EventObject* _eventObj);

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
	friend std::ostream& operator<< (std::ostream& os, const PlayerCollisionHandler& _handler);
};

#endif