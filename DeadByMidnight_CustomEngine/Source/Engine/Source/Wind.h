/*!
All content © 2023 DigiPen Institute of Technology Singapore, all rights reserved.
@file       Wind.h
@author     TEO Wen Xi (100%)
@co-author

----------------------------------------------------------------------------

@course     CSD2401F23
@section    CSD2401F23-A
@project    CSD2401F23 Software Engineering Project 3

@brief		Wind class component, used for anything related to the wind object
*//*______________________________________________________________________*/

#ifndef WIND_H
#define WIND_H

#include "ComponentBase.h"
#include "Vector3.h"
#include "SoundManager.h"

class Wind : public ComponentBase
{
public:
	static FMOD::Channel* mWindBGMHandler;
	static bool mPlayWindSound;
public:

	/*!
	 * @brief 
	 * Init function
	*/
	void Init() {};

	/*!
	 * @brief 
	 * Update function
	*/
	void Update() {};

	/*!
	 * @brief 
	 * Plays the wind sound if player is in wind
	*/
	static void UpdateWindSound();

	/*!
	 * @brief 
	 * Exit function
	*/
	void Exit() {};

	/*!
	 * @brief 
	 * Load function
	*/
	void Load() {};

	/*!
	 * @brief 
	 * Save function
	*/
	void Save() { JSONManager::GetInstance()->Save("Component", "Wind"); };

	/*!
	 * @brief 
	 * Imgui display properties
	*/
	void DisplayProperties() {};

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
	friend std::ostream& operator<< (std::ostream& os, const Wind& _wind)
	{
		os << "Wind Script tied to Entity ID " << _wind.mEntID;
		return os;
	}
};
#endif