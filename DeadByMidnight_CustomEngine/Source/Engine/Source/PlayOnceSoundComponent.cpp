/*!
All content © 2023 DigiPen Institute of Technology Singapore, all rights reserved.
@file       PlayOnceSoundComponent.h
@author     TAN Kang Zheng (100%)
@co-author

----------------------------------------------------------------------------

@course     CSD2401F23
@section    CSD2401F23-A
@project    CSD2401F23 Software Engineering Project 3

@brief		Component to play a sound component once upon play scene
*//*______________________________________________________________________*/

#include "PlayOnceSoundComponent.h"

#include "SoundComponent.h"
#include "SoundManager.h"
#include "CoreEngine.h"
#include "EntityManager.h"

void PlayOnceSoundComponent::Init()
{
	SoundManager::GetInstance()->AddPlayOnceComponent(mEntID);
	mHasPlayed = false;
}

void PlayOnceSoundComponent::Update()
{
	if ((CoreEngine::GetInstance()->mGameState == CoreEngine::GameState::GS_PLAY) && !mHasPlayed)
	{
		SoundComponent* _soundComp = EntityManager::GetInstance()->GetEntity(mEntID)->GetComponent<SoundComponent>();
		if (_soundComp != nullptr)
		{
			_soundComp->Play();
#if _ENGINE
			DebugLogger::GetInstance()->Log("PlaceOnceSoundComponent: Playing...\n");
#endif // _ENGINE

			mHasPlayed = true;
		}
	}
};

void PlayOnceSoundComponent::Reset()
{
	mHasPlayed = false;
}

bool PlayOnceSoundComponent::HasPlayed()
{
	return mHasPlayed;
}

void PlayOnceSoundComponent::Exit()
{
	SoundManager::GetInstance()->RemovePlayOnceComponent(mEntID);
}