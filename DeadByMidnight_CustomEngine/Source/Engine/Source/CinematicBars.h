/*!
All content © 2023 DigiPen Institute of Technology Singapore, all rights reserved.
@file       CinematicBars.h
@author     TEO Wen Xi (100%)
@co-author

----------------------------------------------------------------------------

@course     CSD2401F23
@section    CSD2401F23-A
@project    CSD2401F23 Software Engineering Project 3

@brief		Component to show cinematic bars for cinematics
*//*______________________________________________________________________*/

#ifndef CINEMATIC_BARS_H
#define CINEMATIC_BARS_H

#include "ComponentBase.h"
#include "Vector3.h"
#include "Transform.h"

class CinematicBars : public ComponentBase
{
public:
	enum BAR_DIRECTION
	{
		UP,
		DOWN,
		LEFT,
		RIGHT
	};

	BAR_DIRECTION mBarDir;
	float mExpandDuration;
	float mHoldDuration;
	
private:
	enum CURRENT_STATE
	{
		EXPANDING,
		HOLDING,
		RETRACTING
	};

	CURRENT_STATE mCurrState = EXPANDING;
	float mExpandLerpValue;
	float mTimer = 0;
	Transform* mXform;

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
	void Save() {};

	/*!
	 * @brief
	 * Imgui display properties
	*/
	void DisplayProperties() {};

	/*!
	 * @brief
	 * Static function to create cinematic bars objects for cutscenes
	*/
	static void CreateCinematicBars(BAR_DIRECTION _dir, float _expandDura, float _holdDura);

	/*!
	 * @brief
	 * Cout overload function for component debug printout
	 * @param os
	 * The output stream to write into
	 * @param _class
	 * The input class object
	 * @return
	 * The output stream with the written string
	*/
	friend std::ostream& operator<< (std::ostream& os, const CinematicBars& _class)
	{
		os << "CinematicBars Script tied to Entity ID " << _class.mEntID;
		return os;
	}
};
#endif