/*!
All content © 2023 DigiPen Institute of Technology Singapore, all rights reserved.
@file       PhysicsComponent.h
@author     SIA Xin Tian (40%)
@co-author	TEO Wen Xi (60%)

----------------------------------------------------------------------------

@course     CSD2401F23
@section    CSD2401F23-A
@project    CSD2401F23 Software Engineering Project 3

@brief      Stores essential data like size, position, velocity.
*//*______________________________________________________________________*/

#ifndef PHYSICS_COMPONENT_H
#define PHYSICS_COMPONENT_H

#include "ComponentBase.h"
#include "Collision.h"
#include "Vector3.h"

class PhysicsComponent : public ComponentBase
{
public:
	Vec3 mCurrentVelocity{};
	Vec3 mGravity{}; //positive
	Vec3 mWindTunnelForce{};
	Vec3 mCurrentAcceleration{};
	Vec3 mMoveForce{};
	Vec3 mJumpForce{};
	Vec3 mObjBigScale{};
	Collision::CollisionDir mCollisionDir{Collision::CollisionDir::CD_NONE};
	float mCurrentA{}; 
	float mForce{}; 
	float mMass{1.0f}; 
	float mObjCurrentMass{};
	float mJumpMultiplier{};
	float mJumpVelocity{};
	float mCoefficientOfFriction{};
	bool mIsCharacterSmall{}; //check if character is small
	bool mIsCharacterWindTunnelUp = false;
	bool mIsCharacterWindTunnelLeft = false;
	bool mIsCharacterWindTunnelRight = false;

	bool mEnabled = true;

	float mCharacterSizeFactor{ 1.f };
	float mCharacterWindFactor{ 0.f };

	//Renderer mRdr; // for gfx debugging

public:

	/*!
	 * @brief 
	 * Init function for Physics Component
	*/
	void Init();

	/*!
	 * @brief 
	 * Update function for Physics Component
	*/
	void Update();

	/*!
	 * @brief 
	 * Exit function for Physics Component
	*/
	void Exit();

	/*!
	 * @brief 
	 * Load function for Physics Component
	*/
	void Load();

	/*!
	 * @brief 
	 * Save function for Physics Component
	*/
	void Save();

	/*!
	 * @brief 
	 * Function to display properites
	*/
	void DisplayProperties();

	/*!
	 * @brief 
	 * Set force and velocity to zero
	*/
	void SetOnGround();

	/*!
	 * @brief 
	 * operator<<
	 * @param os 
	 * Output stream object
	 * @param _phy 
	 * Variable to print out
	 * @return 
	 * Output stream object
	*/
	friend std::ostream& operator<< (std::ostream& _os, const PhysicsComponent& _phy);
};

#endif