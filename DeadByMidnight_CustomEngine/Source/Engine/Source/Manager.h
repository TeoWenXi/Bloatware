/*!
All content © 2023 DigiPen Institute of Technology Singapore, all rights reserved.
@file       Manager.h
@author     TAN Kang Zheng (100%)
@co-author

----------------------------------------------------------------------------

@course     CSD2401F23
@section    CSD2401F23-A
@project    CSD2401F23 Software Engineering Project 3

@brief      Manager template for all system managers, to be used for standardization of manager handling
*//*______________________________________________________________________*/

#ifndef MANAGER_TEMPLATE_H
#define MANAGER_TEMPLATE_H

class Manager
{
public:
	/*!
	 * @brief 
	 *	Virtual initialize function for manager class. Will be called when starting and creating the manager.
	*/
	virtual void Init() = 0;

	/*!
	 * @brief 
	 *	Virtual load function for manager class. Called after initialization, to create all necessary objects.
	*/
	virtual void Load() = 0;

	/*!
	 * @brief 
	 *	Virtual update function for manager class. Called every update cycle.
	*/
	virtual void Update() = 0;

	/*!
	 * @brief 
	 *	Virtual update function for manager class. Called when removing the class.
	*/
	virtual void Unload() = 0;
};

#endif