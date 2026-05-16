/*!
All content © 2023 DigiPen Institute of Technology Singapore, all rights reserved.
@file       GameUIObjectHandler.h
@author     TAN Kang Zheng (100%)
@co-author

----------------------------------------------------------------------------

@course     CSD2401F23
@section    CSD2401F23-A
@project    CSD2401F23 Software Engineering Project 3

@brief		Component that acts as a "Parent" for a group of UI components
*//*______________________________________________________________________*/
#ifndef GAMEUI_OBJECT_HANDLER_H
#define GAMEUI_OBJECT_HANDLER_H

#include "ComponentBase.h"
#include "GameUIManager.h"


class GameUIObjectHandler : public ComponentBase
{
public:
	/*!
	 * @brief
	 * Init virtual function for components
	*/
	void Init();

	/*!
	 * @brief
	 * Update virtual function for components
	*/
	void Update();

	/*!
	 * @brief
	 * Update virtual function for components
	*/
	void Exit();

	/*!
	 * @brief
	 * Load virtual function for components
	*/
	void Load() {};

	/*!
	 * @brief
	 * Save virtual function for components
	*/
	void Save();

	/*!
	 * @brief
	 * DisplayProperties virtual function for components
	*/
	void DisplayProperties();

	/*!
	 * @brief
	 * Function to run once upon loading
	*/
	void OnLoad();

	/*!
	 * @brief 
	 *	Operator << overload to print to logs on creation
	 * @param _os 
	 *	Output stream to print to
	 * @param _handler 
	 *	Reference to the GameUIObjectHandler 
	 * @return 
	 *	The output stream to continue printing to
	*/
	friend std::ostream& operator<< (std::ostream& _os, const GameUIObjectHandler& _handler)
	{
		_os << "GameUIObjectHandler tied to Ent ID " << _handler.mEntID;
		return _os;
	}

	Vec3 cameraOffset{};
	bool onLoadrun = false;
};

#endif