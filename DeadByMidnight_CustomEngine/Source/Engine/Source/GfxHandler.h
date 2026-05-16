/*!
All content © 2023 DigiPen Institute of Technology Singapore, all rights reserved.
@file       GfxHandler.h
@author     CAI Wenjin (100%)
@co-author	

----------------------------------------------------------------------------

@course     CSD2401F23
@section    CSD2401F23-A
@project    CSD2401F23 Software Engineering Project 3

@brief			Handles the graphical side of the events that occur.
*//*______________________________________________________________________*/

#ifndef GFX_HANDLER_H
#define GFX_HANDLER_H

#include "ComponentBase.h"
#include "EventsHeader.h"
#include "ComponentsHeader.h"
#include "EntityManager.h"

class GfxHandler : public ComponentBase, public Observer
{
public:

	/*!
	 * @brief 
	 * Init function for the graphics handler.
	*/
	void Init();
	/*!
	 * @brief 
	 * Update function for the graphics handler.
	*/
	void Update();
	/*!
	 * @brief 
	 * Exit function for the graphics handler.
	*/
	void Exit();
	/*!
	 * @brief 
	 * Save function for the the graohics handler.
	*/
	void Save();
	/*!
	 * @brief 
	 * Load function for the graphics handler.
	*/
	void Load();
	/*!
	 * @brief 
	 * Displays the properties for the graphics handler.
	*/
	void DisplayProperties();

	/*!
	 * @brief 
	 * Handles whatever happens when an event has notified this handler.
	 * @param _eventObj 
	 * Pointer to the event object that notified this handler.
	*/
	void OnNotify(EventObject* _eventObj);

	/*!
	 * @brief 
	 * Output stream operator overload for the graphics handler to print out information.
	 * @param _os 
	 * The output stream object.
	 * @param _handler 
	 * The reference to the graphics handler object.
	 * @return 
	 * The output steam.
	*/
	friend std::ostream& operator<< (std::ostream& _os, const GfxHandler& _handler);
};

#endif // !GFX_HANDLER_H


