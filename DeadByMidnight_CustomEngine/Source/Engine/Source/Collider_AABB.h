/*!
All content © 2023 DigiPen Institute of Technology Singapore, all rights reserved.
@file       Collider_AABB.h
@author     SIA Xin Tian (40%)
@co-author	TEO Wen Xi (60%)

----------------------------------------------------------------------------

@course     CSD2401F23
@section    CSD2401F23-A
@project    CSD2401F23 Software Engineering Project 3

@brief      Stores essential data like min & max of bounding box, and renderer.
*/
/*______________________________________________________________________*/

#ifndef COLLIDER_AABB_H
#define COLLIDER_AABB_H

#include "ComponentBase.h"

#include "Vector3.h"
#include "Renderer.h"

enum ColliderType
{
	AABB, // 0
	OOBB, // 1
	NUM_COLLIDER_TYPES
};

class Collider_AABB : public ComponentBase
{
public:
	Vector3 mOffset;
	Vector3 mScale;
	Vector3 mFinalpos;

	Vector3 mMin;
	Vector3 mMax;

	Renderer mRdr;

	ColliderType mColliderType;

public:

	/*!
	 * @brief 
	 * Init function for Collider
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
	friend std::ostream &operator<<(std::ostream &_os, const Collider_AABB &_col);

	// getter function for collider type

	/*!
	 * @brief 
	 * Getter function for private variable mColliderType
	 * @return 
	 * ColliderType
	*/
	ColliderType GetType();

	/*!
	 * @brief 
	 * Getter function for private variable mIsCollided
	 * @return 
	 * Boolean value
	*/
	bool GetIsCollided() const;

	/*!
	 * @brief 
	 * Getter function for private variable mIsTriggered
	 * @return 
	 * Boolean value
	*/
	bool GetIsTriggered() const;

	/*!
	 * @brief 
	 * Getter function for private variable mIsEnabled
	 * @return 
	 * Boolean value
	*/
	bool GetIsEnabled() const;

	// setter functions

	/*!
	 * @brief 
	 * Set mIsCollided to a new value
	 * @param val 
	 * value to set to
	*/
	void SetIsCollided(bool _val);

	/*!
	 * @brief 
	 * Set mIsTriggered to a new value
	 * @param val 
	 * value to set to
	*/
	void SetIsTriggered(bool _val);

	/*!
	 * @brief 
	 * Set mIsEnabled to a new value
	 * @param val 
	 * value to set to
	*/
	void SetIsEnabled(bool _val);

private:
	bool mIsCollided = false;
	bool mIsTriggered = false;
	bool mIsEnabled = true;
	ColliderType Type = ColliderType::AABB;
	std::vector<std::string> mColliderTypesNames;
};

#endif