/*!
All content © 2023 DigiPen Institute of Technology Singapore, all rights reserved.
@file       AnimationComponent.h
@author     CAI Wenjin (100%)
@co-author

----------------------------------------------------------------------------

@course     CSD2401F23
@section    CSD2401F23-A
@project    CSD2401F23 Software Engineering Project 3

@brief      Animation component class declarations
*//*______________________________________________________________________*/

#ifndef ANIMATION_COMPONENT_H
#define ANIMATION_COMPONENT_H

#include "ComponentBase.h"
#include "Texture.h"
#include "Animation.h"
#include <vector>
#include <unordered_map>
#include <iostream>

class AnimationComponent : public ComponentBase
{
public:
	Animation mAnimation{};
	int mAnimationLoopLimit_Max = 0, mAnimationLoopLimit_Current = 0;
	
	// must declare these functions since inheriting from ComponentBase class

	/*!
	 * @brief 
	 * Init function for the animation component.
	*/
	void Init();

	/*!
	 * @brief 
	 * Update function for the animation component.
	*/
	void Update();

	/*!
	 * @brief 
	 * Exit function for the animation component.
	*/
	void Exit();

	/*!
	 * @brief 
	 * Load function for the animation component.
	*/
	void Load();

	/*!
	 * @brief 
	 * Save function for the animation component.
	*/
	void Save();

	/*!
	 * @brief 
	 * Displays the animation component properties for ImGui.
	*/
	void DisplayProperties();

	/*!
	 * @brief
	 * Checks if the animation is looping, or in the case of non-looping animations, if it has not reached the last frame.
	 * @return 
	 * True if above conditions are satisfied, False otherwise.
	*/
	bool IsPlaying();

	/*!
	 * @brief
	 * Resets animation frame to the beginning.
	*/
	void ResetAnimation();

	/*!
	 * @brief 
	 * Output stream operator overload to print out information about the animation component.
	 * @param _os 
	 * The output stream.
	 * @param _animationComponent 
	 * The animation component object.
	 * @return 
	 * The printed output stream.
	*/
	friend std::ostream& operator<< (std::ostream& _os, const AnimationComponent& _animationComponent);
};

#endif // !ANIMATION_COMPONENT_H



