/*!
All content © 2023 DigiPen Institute of Technology Singapore, all rights reserved.
@file       AnimationSystem.h
@author     CAI Wenjin (100%)
@co-author

----------------------------------------------------------------------------

@course     CSD2401F23
@section    CSD2401F23-A
@project    CSD2401F23 Software Engineering Project 3

@brief      Animation system class declarations
*//*______________________________________________________________________*/

#ifndef ANIMAITON_SYSTEM_H
#define ANIMAITON_SYSTEM_H

//Utility
#include "Singleton.h"
#include "Manager.h"

#include "AnimationComponent.h"
#include <vector>

class AnimationSystem : public Singleton<AnimationSystem> , public Manager
{
public:
	std::vector<AnimationComponent*> mAnimationcComponents;
	
	/*!
	 * @brief 
	 * Init function for animation system.
	*/
	void Init();

	/*!
	 * @brief
	 * Update function for animation system.
	*/
	void Update();

	/*!
	 * @brief
	 * Unload function for animation system.
	*/
	void Unload();
};

#endif // !ANIMAITON_SYSTEM_H


