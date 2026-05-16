/*!
All content © 2023 DigiPen Institute of Technology Singapore, all rights reserved.
@file       ComponentBase.h
@author     TEO Wen Xi (100%)
@co-author

----------------------------------------------------------------------------

@course     CSD2401F23
@section    CSD2401F23-A
@project    CSD2401F23 Software Engineering Project 3

@brief      Base Component for all component classes to inherit from
*//*______________________________________________________________________*/

#ifndef COMPONENT_BASE_H
#define COMPONENT_BASE_H

#include "ECS_Types.h"
#include "JSONManager.h"
#include <iostream>

class ComponentBase
{
public:
	virtual void Init() = 0;	//Init data
	virtual void Update() = 0;	//Update if required
	virtual void Exit() = 0;	//Exit if required

	virtual void Load() = 0;	//Load data from file
	virtual void Save() = 0;	//Load data from file

	virtual void DisplayProperties() = 0; //Displays the variables in ImGui

	//Entity ID of the entity tied to this component
	EntityID mEntID;
};

#endif