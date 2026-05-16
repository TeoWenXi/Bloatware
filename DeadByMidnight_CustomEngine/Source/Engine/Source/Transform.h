/*!
All content © 2023 DigiPen Institute of Technology Singapore, all rights reserved.
@file       Transform.h
@author     TEO Wen Xi (100%)
@co-author

----------------------------------------------------------------------------

@course     CSD2401F23
@section    CSD2401F23-A
@project    CSD2401F23 Software Engineering Project 3

@brief		Transform class component
*//*______________________________________________________________________*/

#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "Vector3.h"
#include "ComponentBase.h"
#include "Renderer.h"

class Transform : public ComponentBase
{
private:
	Vector3 mPosition{};
	Vector3 mLocalPos{};
	
	Vector3 mPrevPos{};
public:
	Vector3 mScale{};
	float mRotation{};

	Renderer mRdr;

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
	void Update() {}

	/*!
	 * @brief 
	 * Exit function
	*/
	void Exit() {}

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
	 * Gets a reference to the position
	 * @return 
	 * Position Vector3 reference
	*/
	Vector3& GetPosition();

	/*!
	 * @brief 
	 * Gets a const reference to the position
	 * @return 
	 * Position const Vector3 reference
	*/
	const Vector3& GetPosition() const;

	/*!
	 * @brief 
	 * Set position value
	 * @param _newPosition 
	 * The new position to be set
	*/
	void SetPosition(const Vector3& _newPosition);

	/*!
	 * @brief 
	 * Set local position value
	 * @param _newPosition 
	 * The new local position to be set
	*/
	void SetLocalPosition(const Vector3& _newPosition);

	/*!
	 * @brief 
	 * Returns the local position
	 * @return 
	 * Local position
	*/
	Vector3 GetLocalPosition();

	/*!
	 * @brief 
	 * Gets the entity's parent position
	 * @return 
	 * Const reference to this entity's parent position
	*/
	const Vector3& GetParentPosition() const;

	/*!
	 * @brief 
	 * Set local position based on parent's position. Mainly used for editor adjustment
	*/
	void UpdateLocalPosition();

	/*!
	 * @brief 
	 * Updates position based on parent entity's movement 
	 * Also updates any child entities tied to this entity object
	*/
	void LoadPosFromParent();

	/*!
	 * @brief
	 * Cout overload function for component debug printout
	 * @param os
	 * The output stream to write into
	 * @param _transform
	 * The input class object
	 * @return
	 * The output stream with the written string
	*/
	friend std::ostream& operator<< (std::ostream& os, const Transform& _transform);
};

#endif