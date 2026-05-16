/*!
All content © 2023 DigiPen Institute of Technology Singapore, all rights reserved.
@file       EndGoal.h
@author     TAN Kang Zheng (100%)
@co-author

----------------------------------------------------------------------------

@course     CSD2401F23
@section    CSD2401F23-A
@project    CSD2401F23 Software Engineering Project 3

@brief      Component to check if player has reached a target location
*//*______________________________________________________________________*/

#ifndef END_GOAL_COMPONENT_H
#define END_GOAL_COMPONENT_H

#include "ComponentBase.h"

class EndGoal : public ComponentBase
{
public:

	/*!
	 * @brief
	 * Init virtual function for components
	*/
	void Init();

	/*!
	 * @brief
	 * Load virtual function for components
	*/
	void Load();

	/*!
	 * @brief
	 * Save virtual function for components
	 * Each individual button type must save its name
	*/
	void Save();

	/*!
	 * @brief
	 * Update virtual function for components
	*/
	void Update();

	/*!
	 * @brief
	 * Exit virtual function for components
	*/
	void Exit();

	/*!
	 * @brief
	 * DisplayProperties virtual function for components
	*/
	void DisplayProperties();

	/*!
	 * @brief
	 * Output stream operator overload to print out information about the endgoal component.
	 * @param _os
	 * The output stream.
	 * @param _handler
	 * The animation component object.
	 * @return
	 * The printed output stream.
	*/
	friend std::ostream& operator<< (std::ostream& _os, const EndGoal& _handler)
	{
		_os << "EndGoal Component tied to Ent ID " << _handler.mEntID;
		return _os;
	}

	std::string mPrefab{};
};

#endif
