/*!
All content © 2023 DigiPen Institute of Technology Singapore, all rights reserved.
@file       Collider_Circle.h
@author     SIA Xin Tian (100%)
@co-author

----------------------------------------------------------------------------

@course     CSD2401F23
@section    CSD2401F23-A
@project    CSD2401F23 Software Engineering Project 3

@brief      Stores essential data like center and radius of bounding circle, and renderer.
*//*______________________________________________________________________*/

#ifndef COLLIDER_CIRCLE_H
#define COLLIDER_CIRCLE_H

#include "ComponentBase.h"

#include "Vector3.h"
#include "Renderer.h"

class Collider_Circle : public ComponentBase
{
public:
	
	Vector3 mCenter;
	float mRadius;

	Renderer mRdr;

public:

	/*!
	 * @brief 
	 * Init function for collider
	*/
	void Init();

	/*!
	 * @brief 
	 * Update function for collider
	*/
	void Update();

	/*!
	 * @brief 
	 * Exit function for collider
	*/
	void Exit();

	/*!
	 * @brief 
	 * Load function for collider
	*/
	void Load();

	/*!
	 * @brief 
	 * Save function for collider
	*/
	void Save();

	/*!
	 * @brief 
	 * Function to display properites
	*/
	void DisplayProperties();

	/*!
	 * @brief 
	 * operator<<
	 * @param os 
	 * Output stream object
	 * @param _col 
	 * Variable to print out
	 * @return 
	 * Output stream object
	*/
	friend std::ostream& operator<< (std::ostream& _os, const Collider_Circle& _col);
};

#endif