/*!
All content © 2024 DigiPen Institute of Technology Singapore, all rights reserved.
@file       ButtonSelectManager.h
@author     TAN Kang Zheng (100%)
@co-author

----------------------------------------------------------------------------

@course     CSD2451F24
@section    CSD2451F24-A
@project    CSD2451F24 Software Engineering Project 4

@brief      Manages using arrow keys to navigate buttons
*//*______________________________________________________________________*/

#ifndef BUTTON_SELECT_MANAGER_H
#define BUTTON_SELECT_MANAGER_H

//Manager Includes
#include "Singleton.h"
#include "Manager.h"
#include "ECS_Types.h"

//Other Includes
#include <vector>

class ButtonSelectManager : public Singleton <ButtonSelectManager>, public Manager
{
public:
	/*!
	 * @brief
	 *	Virtual init function for manager class.
	*/
	void Init() {};

	/*!
	 * @brief
	 *	Virtual load function for manager class.
	*/
	void Load() {};

	/*!
	 * @brief
	 *	Virtual update function for manager class.
	*/
	void Update();

	/*!
	 * @brief
	 *	Virtual unload function for the manager class.
	*/
	void Unload() 
	{
		Destroy();
	}

	/*!
	 * @brief
	 *	Adds a button to the list for tracking.
	*/
	void AddButton(EntityID _entityID);

	/*!
	 * @brief
	 *	Removes a button to the list for tracking.
	*/
	void RemoveButton(EntityID _entityID);

	/*!
	 * @brief
	 *	Creates a backup of all the current mButtonList
	*/
	void CreateBackup();

	/*!
	 * @brief
	 *	Loads the backup of the mButtonList
	*/
	void LoadBackup();
private:

	/*!
	 * @brief
	 *	Sorts the current list of buttons by their ButtonOrderPriority.
	*/
	void SortButtons();

	std::vector<EntityID> mButtonList;
	std::vector<EntityID> mButtonListBackup;
	unsigned int mButtonIndex = 0;
	bool mSelectedButton = false;

	float onButtonVal[4] = {1, 0, 0, 0};
};

#endif