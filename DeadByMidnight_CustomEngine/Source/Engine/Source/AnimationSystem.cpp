/*!
All content © 2023 DigiPen Institute of Technology Singapore, all rights reserved.
@file       AnimationSystem.cpp
@author     CAI Wenjin (100%)
@co-author

----------------------------------------------------------------------------

@course     CSD2401F23
@section    CSD2401F23-A
@project    CSD2401F23 Software Engineering Project 3

@brief      Animation system class definitions
*//*______________________________________________________________________*/
#include "AnimationSystem.h"
#include "CoreEngine.h"

void AnimationSystem::Init()
{
}

void AnimationSystem::Update()
{
	if (CoreEngine::GetInstance()->mGameState != CoreEngine::GameState::GS_PLAY)
		return;

	for (auto& anim : mAnimationcComponents)	
		anim->Update();	
}

void AnimationSystem::Unload()
{
	for (auto& anim : mAnimationcComponents)	
		delete anim;	
}
