/*!
All content © 2023 DigiPen Institute of Technology Singapore, all rights reserved.
@file       Key.h
@author     TEO Wen Xi (100%)
@co-author

----------------------------------------------------------------------------

@course     CSD2401F23
@section    CSD2401F23-A
@project    CSD2401F23 Software Engineering Project 3

@brief		Key class component, used for anything related to the key object
*//*______________________________________________________________________*/

#ifndef KEY_H
#define KEY_H

#include "ComponentBase.h"
#include "Vector3.h"

class Key : public ComponentBase
{
public:
	enum KeyState
	{
		CANCOLLECT,
		COLLECTEDHOVER,
		USED,
	};

	KeyState mKeyState = KeyState::CANCOLLECT;
	Vector3 mOffsetFromPlayer = Vector3{ 0, 7.f, 0 };
	Vector3 mSpawnLocation{};
	bool mHideImage = false;

	static std::vector<EntityID> mKeyIDs;
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
	 * Imgui display function
	*/
	void DisplayProperties();

	/*!
	 * @brief 
	 * Resets the key to its original position
	*/
	void Reset();

	/*!
	 * @brief 
	 * Cout overload function for component debug printout
	 * @param os 
	 * The output stream to write into
	 * @param _key 
	 * The input class object
	 * @return 
	 * The output stream with the written string
	*/
	friend std::ostream& operator<< (std::ostream& os, const Key& _key) 
	{ 
		os << "Key Script tied to Entity ID " << _key.mEntID;
		return os; 
	}
};

#endif