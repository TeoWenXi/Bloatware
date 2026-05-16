/*!
All content © 2023 DigiPen Institute of Technology Singapore, all rights reserved.
@file       Wind.cpp
@author     TEO Wen Xi (100%)
@co-author

----------------------------------------------------------------------------

@course     CSD2401F23
@section    CSD2401F23-A
@project    CSD2401F23 Software Engineering Project 3

@brief      Contains functions for the Wind component
*//*______________________________________________________________________*/

#include "Wind.h"
#include "EntityManager.h"
#include "ComponentsHeader.h"

FMOD::Channel* Wind::mWindBGMHandler = nullptr;
bool Wind::mPlayWindSound = false;

void Wind::UpdateWindSound()
{
	//Play wind sound
	float currentAudioVolume = 0;
	if (mWindBGMHandler != nullptr)
		mWindBGMHandler->getVolume(&currentAudioVolume);

	if (mPlayWindSound)
	{
		if (mWindBGMHandler == nullptr)
		{
			mWindBGMHandler = SoundManager::GetInstance()->PlaySound("SFX_EnterWindTunnel", SoundManager::GetInstance()->GetSFXVolume(), SoundManager::GetInstance()->GetChannelPitch());
			mWindBGMHandler->setVolume(std::clamp(currentAudioVolume + 3.f * (float)WindowsManager::GetInstance()->mDeltaTime, 0.f, 1.f));
			mWindBGMHandler->setLoopCount(-1);

		}
		else
		{
			mWindBGMHandler->setVolume(std::clamp(currentAudioVolume + 3.f * (float)WindowsManager::GetInstance()->mDeltaTime, 0.f, 1.f));
		}
	}
	else if (mWindBGMHandler != nullptr)
	{
		mWindBGMHandler->setVolume(std::clamp(currentAudioVolume - 3.f * (float)WindowsManager::GetInstance()->mDeltaTime, 0.f, 1.f));
	}
}