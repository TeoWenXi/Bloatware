/*!
All content © 2023 DigiPen Institute of Technology Singapore, all rights reserved.
@file       KeyWall.h
@author     TEO Wen Xi (100%)
@co-author

----------------------------------------------------------------------------

@course     CSD2401F23
@section    CSD2401F23-A
@project    CSD2401F23 Software Engineering Project 3

@brief		KeyWall class component, used for anything related to the key wall object
*//*______________________________________________________________________*/

#ifndef KEY_WALL_H
#define KEY_WALL_H

#include "ComponentBase.h"
#include "Vector3.h"

class KeyWall : public ComponentBase
{
public:
	bool mLocked{ true };
	static std::vector<EntityID> mKeyWallIDs;
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
	 * Load function
	*/
	void Load();

	/*!
	 * @brief 
	 * Save function
	*/
	void Save();

	/*!
	 * @brief 
	 * Imgui display properties
	*/
	void DisplayProperties();

	/*!
	 * @brief 
	 * Resets the key wall to its original position and state
	*/
	void Reset();

	/*!
	 * @brief 
	 * Cout overload function for component debug printout
	 * @param os 
	 * The output stream to write into
	 * @param _keyWall 
	 * The input class object
	 * @return 
	 * The output stream with the written string
	*/
	friend std::ostream& operator<< (std::ostream& os, const KeyWall& _keyWall)
	{
		os << "Key Script tied to Entity ID " << _keyWall.mEntID;
		return os;
	}
};

#endif