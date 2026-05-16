/*!
All content © 2023 DigiPen Institute of Technology Singapore, all rights reserved.
@file       Animation.h
@author     CAI Wenjin (100%)
@co-author

----------------------------------------------------------------------------

@course     CSD2401F23
@section    CSD2401F23-A
@project    CSD2401F23 Software Engineering Project 3

@brief      Animation class declarations
*//*______________________________________________________________________*/
#ifndef ANIMATION_H
#define ANIMATION_H

#include "Vector3.h"
#include "Texture.h"
#include <vector>

/*!
 * @brief 
 * Animation struct
*/
struct Animation
{
	// current playing frame index
	int mCurrFrameIndex;
	// total number of frames
	int mTotalFrames;
	// flag to see if animation will loop
	bool mIsLooping;
	// flag to see if animation is paused
	bool mIsPaused;
	// time delay to switch to the next frame
	double mTimeDelay;
	// elapsed time
	double mElapsedTime;
};

#endif // !ANIMATION_H
