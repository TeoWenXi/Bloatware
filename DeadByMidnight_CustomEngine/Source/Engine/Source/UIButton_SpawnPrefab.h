/*!
All content © 2024 DigiPen Institute of Technology Singapore, all rights reserved.
@file       UIButton_SpawnPrefab.h
@author     TAN Kang Zheng (100%)
@co-author

----------------------------------------------------------------------------

@course     CSD2451F24
@section    CSD2451F24-A
@project    CSD2451F24 Software Engineering Project 4

@brief      Button Component to spawn a given prefab
*//*______________________________________________________________________*/

#include "UIButton.h"
#include "EntityManager.h"
#include "GameUITextHandler.h"
#include "SoundComponent.h"

#ifndef UI_BUTTON_SPAWNPREFAB_H
#define UI_BUTTON_SPAWNPREFAB_H

class UIButtonSpawnPrefab : public UIButton
{
public:

	/*!
	 * @brief 
	 * Constructor for spawning prefab button
	 * @param _id 
	 * EntityID to initialise button with
	 * @param _prefab 
	 * The name of the prefab this button will spawn
	*/
	UIButtonSpawnPrefab(EntityID _id, std::string _prefab) : UIButton(_id), mPrefab(_prefab) {};

	/*!
	 * @brief 
	 * Destructor for spawning prefab button
	*/
	~UIButtonSpawnPrefab() {};

	/*!
	 * @brief 
	 * Called when the button is clicked.
	*/
	void OnClick();

	/*!
	 * @brief 
	 * Called when the button is held.
	*/
	void OnHold() {};

	/*!
	 * @brief 
	 * Called when the button being hovered.
	*/
	void OnHoverEnter();

	/*!
	 * @brief 
	 * Called when the button exiting hovered.
	*/
	void OnHoverExit();

	/*!
	 * @brief
	 * Sets the prefab that this button will spawn
	 * @param _prefab 
	 * The name of the prefab
	*/
	void SetPrefab(std::string _prefab);
private:
	std::string mPrefab;
};

#endif