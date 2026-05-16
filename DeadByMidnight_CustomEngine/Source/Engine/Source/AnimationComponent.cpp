/*!
All content © 2023 DigiPen Institute of Technology Singapore, all rights reserved.
@file       AnimationComponent.cpp
@author     CAI Wenjin (100%)
@co-author

----------------------------------------------------------------------------

@course     CSD2401F23
@section    CSD2401F23-A
@project    CSD2401F23 Software Engineering Project 3

@brief      Animation Component class definitions.
*//*______________________________________________________________________*/
#include "AnimationComponent.h"
#include "GraphicsSystem.h"

#include <iostream>

void AnimationComponent::Init()
{
}

void AnimationComponent::Update()
{
	// update if the animation is not paused
	if (!mAnimation.mIsPaused)
	{
		// increment the elapsed time for the animation
		mAnimation.mElapsedTime += WindowsManager::GetInstance()->mDeltaTime;

		// check if the elapsed time hits the time delay for switching the frames
		if (mAnimation.mElapsedTime >= mAnimation.mTimeDelay)
		{
			// check if the current frame index will go over the total frames
			if (mAnimation.mCurrFrameIndex + 1 >= mAnimation.mTotalFrames)
			{
				// check if the animation is looping OR if it can loop for a limited number of times and has not exceeded that threshold
				if (mAnimation.mIsLooping || (mAnimationLoopLimit_Max > 0 && mAnimationLoopLimit_Current < mAnimationLoopLimit_Max))
				{
					// if so, reset the current frame to the first
					mAnimation.mCurrFrameIndex = 0;
					//increase number of times the animation has looped
					mAnimationLoopLimit_Current++;
				}
			}
			else
				// go to the next frame
				++mAnimation.mCurrFrameIndex;

			// reset the elapsed time
			mAnimation.mElapsedTime = 0;
		}
	}
}

bool AnimationComponent::IsPlaying()
{
	return !(!mAnimation.mIsLooping && mAnimation.mCurrFrameIndex + 1 >= mAnimation.mTotalFrames);
}

void AnimationComponent::Exit()
{
}

void AnimationComponent::Load()
{
}

void AnimationComponent::Save()
{
}

void AnimationComponent::ResetAnimation()
{
	mAnimation.mElapsedTime = 0;
	mAnimation.mCurrFrameIndex = 0;
	mAnimationLoopLimit_Current = 0;
}

void AnimationComponent::DisplayProperties()
{
#if _ENGINE
	MyImGui::MyImGuiIntInput(mAnimation.mTotalFrames, "Total Frames");
	MyImGui::MyImGuiDoubleInput(mAnimation.mTimeDelay, "Time to frame change");
#endif // _ENGINE
}

std::ostream& operator<<(std::ostream& _os, const AnimationComponent& _animationComponent)
{
	_os << "Animation component for ID: " << _animationComponent.mEntID << "\n";
	_os << "Animation current frame: " << _animationComponent.mAnimation.mCurrFrameIndex << "\n";
	_os << "Animation frame time delay: " << _animationComponent.mAnimation.mTimeDelay << "\n";

	return _os;
}
