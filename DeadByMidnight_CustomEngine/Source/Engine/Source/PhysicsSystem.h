/*!
All content © 2023 DigiPen Institute of Technology Singapore, all rights reserved.
@file       PhysicsSystem.h  
@author     SIA Xin Tian (80%)
@co-author	TEO Wen Xi (20%)

----------------------------------------------------------------------------

@course     CSD2401F23
@section    CSD2401F23-A
@project    CSD2401F23 Software Engineering Project 3

@brief      Update physics for all physics based entities
*//*______________________________________________________________________*/

#ifndef PHYSICS_SYSTEM_H
#define PHYSICS_SYSTEM_H

#include "Matrix3x3.h"

#include "Singleton.h"
#include "Manager.h"
#include "SoundManager.h"
#include "PhysicsComponent.h"

class PhysicsSystem : public Singleton<PhysicsSystem>, public Manager
{
public:
	/*!
	 * @brief 
	 * Init function for Physics System
	*/
	void Init();

	/*!
	 * @brief 
	 * Load function for Physics System
	*/
	void Load();

	/*!
	 * @brief 
	 * Update functino for Physics System
	*/
	void Update();

	/*!
	 * @brief 
	 * Unload function for Physics System
	*/
	void Unload();

private:
	//This is to keep track of the states of the collisions of the entities
	enum COLLISION_STATES
	{
		NO_COLLISION = 0,
		FIRST_COLLIDED,
		COLLIDING,
		END_COLLISION
	};
	friend Singleton<PhysicsSystem>;
	std::unordered_map<PhysicsComponent*, std::unordered_map< PhysicsComponent*, COLLISION_STATES>> currentCollisionStates;

};

#endif