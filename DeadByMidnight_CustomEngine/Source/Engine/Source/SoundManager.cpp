/*!
All content © 2023 DigiPen Institute of Technology Singapore, all rights reserved.
@file       SoundManager.cpp
@author     TAN Kang Zheng (100%)
@co-author

----------------------------------------------------------------------------

@course     CSD2401F23
@section    CSD2401F23-A
@project    CSD2401F23 Software Engineering Project 3

@brief      Manages the sound for the engine
*//*______________________________________________________________________*/
#include "SoundManager.h"
#include "AssetManager.h"
#include "CoreEngine.h"
#include "PlayOnceSoundComponent.h"
#include "SoundComponent.h"
#include "EntityManager.h"

#define MAX_SFX_LENGTH_MS 15000 //Any audio longer than this time in milliseconds will be flagged as a BGM

int SoundManager::GetDSPBufferSize()
{
	if (mFmodSystem != nullptr)
	{
		unsigned int ret{};
		mFmodSystem->getDSPBufferSize(&ret, nullptr);
		return ret;
	}
	return 0;
}

SoundManager::SoundManager() : mFmodErr(FMOD_OK), mSFXVolume(0.7f), mBGMVolume(0.4f), mFmodSystem(NULL)
{
	mFmodErr = FMOD::System_Create(&mFmodSystem);      // Create the main system object.
	if (mFmodErr != FMOD_OK)
	{
		printf("FMOD system create error! (%d) %s\n", mFmodErr, FMOD_ErrorString(mFmodErr));
		exit(-1);
	}

	mFmodErr = mFmodSystem->init(512, FMOD_INIT_NORMAL, 0);    // Initialize FMOD.
	if (mFmodErr != FMOD_OK)
	{
		printf("FMOD system init error! (%d) %s\n", mFmodErr, FMOD_ErrorString(mFmodErr));
		exit(-1);
	}

	mFmodSystem->createChannelGroup("SFX", &mSFXChannelGroup);
	mFmodSystem->createChannelGroup("BGM", &mBGMChannelGroup);
	mFmodSystem->createChannelGroup("PAUSE", &mPauseOverrideChannelGroup);
	mSFXChannelGroup->setVolume(mSFXVolume);
	mBGMChannelGroup->setVolume(mBGMVolume);
	mPauseOverrideChannelGroup->setVolume(mSFXVolume);
}

SoundManager::~SoundManager()
{

}

void SoundManager::Init()
{

}

void SoundManager::Load()
{

}

void SoundManager::Update()
{
	//Do the FmodSystem update per frame
	mFmodSystem->update();

	if (CoreEngine::GetInstance()->mGameState == CoreEngine::GameState::GS_PAUSE)
	{
		if (!mLastFramePaused)
		{
			SetSFXPaused(true);
			mLastFramePaused = true;
		}
	}
	else if (CoreEngine::GetInstance()->mGameState == CoreEngine::GameState::GS_PLAY)
	{
		if (mLastFramePaused)
		{
			SetSFXPaused(false);
			mLastFramePaused = false;
		}
	}

	if (mMasterVolume < 0.1f)
		mMasterVolume = 0.0f;

	//Update the channels volumes
	mSFXChannelGroup->setVolume(mSFXVolume * mMasterVolume);
	mPauseOverrideChannelGroup->setVolume(mSFXVolume * mMasterVolume);
	if (mSFXVolume < 0.1f)
	{
		mSFXChannelGroup->setVolume(0.0f);
		mPauseOverrideChannelGroup->setVolume(0.0f);
	}

	mBGMChannelGroup->setVolume(mBGMVolume * mMasterVolume * mBGMDucking);
	if (mBGMVolume < 0.1f)
		mBGMChannelGroup->setVolume(0.0f);


	bool checkResult = false;
	//Go through the sfx channels vectors and check which are no longer playing AND not paused
	for (int i = 0; i < mSFXChannels.size(); i++)
	{
		mSFXChannels.at(i)->isPlaying(&checkResult);
		if (!checkResult)
		{
			mSFXChannels.at(i)->getPaused(&checkResult);
			if (!checkResult)
			{
				for (auto iter : mSoundComponents)
				{
					Entity* ent = EntityManager::GetInstance()->GetEntity(iter);
					if (ent == nullptr)
						continue;

					SoundComponent* soundComp = ent->GetComponent<SoundComponent>();
					if (soundComp != nullptr)
					{
						soundComp->RemoveChannel(mSFXChannels.at(i));
					}
				}

				mSFXChannels.at(i)->stop();
				mSFXChannels.erase(mSFXChannels.begin() + i);
			}
		}
	}

	checkResult = false;
	for (int i = 0; i < mBGMChannels.size(); i++)
	{
		mBGMChannels.at(i)->isPlaying(&checkResult);
		if (!checkResult)
		{
			mBGMChannels.at(i)->getPaused(&checkResult);
			if (!checkResult)
			{
				for (auto iter : mSoundComponents)
				{
					Entity* entTemp = EntityManager::GetInstance()->GetEntity(iter);
					if (entTemp == nullptr)
						continue;

					SoundComponent* soundComp = EntityManager::GetInstance()->GetEntity(iter)->GetComponent<SoundComponent>();
					if (soundComp != nullptr)
					{
						soundComp->RemoveChannel(mBGMChannels.at(i));
					}
				}

				mBGMChannels.at(i)->stop();
				mBGMChannels.erase(mBGMChannels.begin() + i);
			}
		}
	}

	for (auto i : mPlayOnceSoundComponents)
	{
		PlayOnceSoundComponent* _posc = EntityManager::GetInstance()->GetEntity(i)->GetComponent<PlayOnceSoundComponent>();
		if (_posc != nullptr)
		{
			if (_posc->HasPlayed())
				return;

			_posc->Update();
		}
	}

	if (CoreEngine::GetInstance()->mGameState == CoreEngine::GameState::GS_STOP)
	{
		bool playingCheck = false;
		//Stop SFX Channelgroup
		mSFXChannelGroup->isPlaying(&playingCheck);
		if (playingCheck)
			mSFXChannelGroup->stop();
		//Stop BGM Channelgroup
		mBGMChannelGroup->isPlaying(&playingCheck);
		if (playingCheck)
			mBGMChannelGroup->stop();
		//Stop Pause Override Channelgroup
		mPauseOverrideChannelGroup->isPlaying(&playingCheck);
		if (playingCheck)
			mPauseOverrideChannelGroup->stop();
	}
}

void SoundManager::Unload()
{
	//Fmod handling of releasing memory
	mFmodSystem->release();

	//Destroy the singleton
	Destroy();
}

void SoundManager::CreateSound(std::string _referenceName, SOUND_TYPE _soundType)
{
	AssetManager::SoundAsset* soundAsset{};
	if (AssetManager::GetInstance()->GetAsset(_referenceName, soundAsset))
	{
		soundAsset->second = _soundType;
	}
}

FMOD::Channel* SoundManager::PlaySound(std::string _referenceName, float _volume, float _pitch)
{
	//Retrieve the sound asset
	AssetManager::SoundAsset* soundAsset{};

	if (AssetManager::GetInstance()->GetAsset(_referenceName, soundAsset))
	{
		unsigned long long dspClock{};
		int mixerRate{};
		switch (soundAsset->second) //Different play settings depending on the sound type
		{
		case SoundType_SFX:
#if _ENGINE
			DebugLogger::GetInstance()->Log("Playing SFX\n");
#endif // _ENGINE
			{
				soundAsset->first->setLoopCount(0); //Set to never loop
				FMOD::Channel* sfxChannel = 0;
				mFmodErr = mFmodSystem->playSound(soundAsset->first, 0, false, &sfxChannel);
				sfxChannel->setVolume(_volume);
				sfxChannel->setPitch(_pitch);
				sfxChannel->setChannelGroup(mSFXChannelGroup);
				mSFXChannels.push_back(sfxChannel);
				return sfxChannel;
			}
		case SoundType_BGM:
#if _ENGINE
			DebugLogger::GetInstance()->Log("Playing BGM\n");
#endif // _ENGINE
			{
				FMOD::Channel* bgmChannel = 0;
				mFmodErr = mFmodSystem->playSound(soundAsset->first, 0, false, &bgmChannel);
				bgmChannel->setVolume(_volume);
				bgmChannel->setPitch(_pitch);
				bgmChannel->setChannelGroup(mBGMChannelGroup);

				mFmodSystem->getSoftwareFormat(&mixerRate, 0, 0);					// Get mixer rate
				bgmChannel->getDSPClock(0, &dspClock);								// Get the reference clock, which is the parent channel group
				bgmChannel->addFadePoint(dspClock, 0.0f);							// Add a fade point at 'now' with 0 volume.
				bgmChannel->addFadePoint(dspClock + (mixerRate * 3), 1.0f);		// Add a fade point 3 seconds later at given volume.
				mBGMChannels.push_back(bgmChannel);
				return bgmChannel;
			}
		default:
			break;
		}

		if (mFmodErr != FMOD_OK)
		{
			printf("FMOD play sound error! (%d) %s\n", mFmodErr, FMOD_ErrorString(mFmodErr));
		}
	}
	else
	{
		printf("SoundManager error: Could not find sound with referenceName (%s)", _referenceName.c_str());
	}
	return nullptr;
}

FMOD::Channel* SoundManager::PlaySoundOverride(std::string _referenceName, float _volume, float _pitch)
{
	//Retrieve the sound asset
	AssetManager::SoundAsset* soundAsset{};

	if (AssetManager::GetInstance()->GetAsset(_referenceName, soundAsset))
	{
	#if _ENGINE
			DebugLogger::GetInstance()->Log("Playing Sound in Override Channel\n");
	#endif // _ENGINE
			if (soundAsset->second != SoundType_BGM)
			{
				soundAsset->first->setLoopCount(0); //Set to never loop
			}
			FMOD::Channel* sfxChannel = 0;
			mFmodErr = mFmodSystem->playSound(soundAsset->first, 0, false, &sfxChannel);
			sfxChannel->setVolume(_volume);
			sfxChannel->setPitch(_pitch);
			sfxChannel->setChannelGroup(mPauseOverrideChannelGroup);
			mPauseOverrideChannels.push_back(sfxChannel);
			return sfxChannel;
	}
	else
	{
		printf("SoundManager error: Could not find sound with referenceName (%s)", _referenceName.c_str());
	}
	return nullptr;
}

float SoundManager::GetBGMVolume()
{
	return mBGMVolume;
}

void SoundManager::SetBGMVolume(float _val)
{
	mBGMVolume = std::clamp(_val, 0.f, 1.f);
}

bool SoundManager::GetBGMPaused()
{
	bool toReturn;
	mBGMChannelGroup->getPaused(&toReturn);
	return toReturn;
}

void SoundManager::SetBGMPaused(bool _val)
{
	mBGMChannelGroup->setPaused(_val);
}

float SoundManager::GetSFXVolume()
{
	return mSFXVolume;
}

void SoundManager::SetSFXVolume(float _val)
{
	mSFXVolume = std::clamp(_val, 0.f, 1.f);
}

float SoundManager::GetMasterVolume()
{
	return mMasterVolume;
}

void SoundManager::SetMasterVolume(float _val)
{
	mMasterVolume = std::clamp(_val, 0.f, 1.f);
}

void SoundManager::SetBGMDucking(float _val)
{
	mBGMDucking = std::clamp(_val, 0.f, 1.f);
}

float SoundManager::GetChannelVolume()
{
	return mChannelVolume;
}

void SoundManager::SetChannelVolume(float _val)
{
	mChannelVolume = _val;
}

float SoundManager::GetChannelPitch()
{
	return mChannelPitch;
}

void SoundManager::SetChannelPitch(float _val)
{
	mChannelPitch = _val;
}

bool SoundManager::GetSFXPaused()
{
	bool toReturn;
	mSFXChannelGroup->getPaused(&toReturn);
	return toReturn;
}

void SoundManager::SetSFXPaused(bool _val)
{
	mSFXChannelGroup->setPaused(_val);
}

void SoundManager::StopChannel(SOUND_TYPE _soundType)
{
	//Stops individual channels depending on the given SOUND_TYPE enum

	unsigned long long dspClock{};
	int mixerRate{};

	switch (_soundType)
	{
	case SoundType_SFX:
		mSFXChannelGroup->stop();
		break;
	case SoundType_BGM:
		for (auto i : mBGMChannels)
		{
			mFmodSystem->getSoftwareFormat(&mixerRate, 0, 0);						// Get mixer rate
			i->getDSPClock(0, &dspClock);							// Get the reference clock, which is the parent channel group
			i->addFadePoint(dspClock, 1.0f);							// Add a fade point at 'now' with current volume.
			i->addFadePoint(dspClock + (mixerRate * 3), 0.0f);		// Add a fade point 3 seconds later at 0 volume.
			i->setDelay(0, dspClock + (mixerRate * 3), true);		// Add a delayed stop command at 3 seconds ('stopchannels = true')
		}

		break;
	default:
		break;
	}
}

void SoundManager::StopChannelSound(FMOD::Channel* _channel)
{
	//Remove channel from all sound components
	for (auto i : mSoundComponents)
	{
		SoundComponent* soundComp = EntityManager::GetInstance()->GetEntity(i)->GetComponent<SoundComponent>();
		if (soundComp != nullptr)
			soundComp->RemoveChannel(_channel);
	}

	auto sfxIter = std::find(mSFXChannels.begin(), mSFXChannels.end(), _channel);
	if (sfxIter != mSFXChannels.end())
	{
		mSFXChannels.erase(sfxIter);
		_channel->stop();
	}

	auto bgmIter = std::find(mBGMChannels.begin(), mBGMChannels.end(), _channel);
	if (bgmIter != mBGMChannels.end())
	{
		mBGMChannels.erase(bgmIter);
		_channel->stop();
	}
}

int SoundManager::GetActiveChannels()
{
	int toReturn{};
	mFmodSystem->getChannelsPlaying(&toReturn);
	return toReturn;
}

//Asset creation
void SoundManager::CreateSoundAsset(std::string _fullPath, void*& _dataPtr)
{
	_dataPtr = new std::pair<FMOD::Sound*, SoundManager::SOUND_TYPE>;

	mFmodErr = mFmodSystem->createSound(_fullPath.c_str(), FMOD_LOOP_NORMAL, 0, &static_cast<std::pair<FMOD::Sound*, SoundManager::SOUND_TYPE>*>(_dataPtr)->first);
	if (mFmodErr != FMOD_OK)
	{
#if _ENGINE
		DebugLogger::GetInstance()->Log("FMOD create sound error! (%d) %s\n", mFmodErr, FMOD_ErrorString(mFmodErr));
#endif // _ENGINE
	}

	unsigned int soundLength{};
	static_cast<std::pair<FMOD::Sound*, SoundManager::SOUND_TYPE>*>(_dataPtr)->first->getLength(&soundLength, FMOD_TIMEUNIT_MS);
#if _ENGINE
	DebugLogger::GetInstance()->Log("Sound created with length of %i\n", soundLength);
#endif // _ENGINE

	if (soundLength > MAX_SFX_LENGTH_MS)
	{
		static_cast<std::pair<FMOD::Sound*, SoundManager::SOUND_TYPE>*>(_dataPtr)->second = SoundType_BGM;
	}

}

void SoundManager::AddPlayOnceComponent(EntityID _mEntID)
{
	mPlayOnceSoundComponents.push_back(_mEntID);
}

void SoundManager::ResetPlayOnceComponent()
{
	for (auto i : mPlayOnceSoundComponents)
	{
		PlayOnceSoundComponent* playOnceComp = EntityManager::GetInstance()->GetEntity(i)->GetComponent<PlayOnceSoundComponent>();
		if (playOnceComp != nullptr)
			playOnceComp->Reset();
	}
}

void SoundManager::RemovePlayOnceComponent(EntityID _mEntID)
{
	for (int i = 0; i < mPlayOnceSoundComponents.size(); i++)
	{
		if (mPlayOnceSoundComponents.at(i) == _mEntID)
		{
			mPlayOnceSoundComponents.erase(mPlayOnceSoundComponents.begin() + i);
			return;
		}
	}
}

void SoundManager::AddSoundComponent(EntityID _mEntID)
{
	mSoundComponents.push_back(_mEntID);
}

void SoundManager::RemoveSoundComponent(EntityID _mEntID)
{
	for (int i = 0; i < mSoundComponents.size(); i++)
	{
		if (mSoundComponents.at(i) == _mEntID)
		{
			mSoundComponents.erase(mSoundComponents.begin() + i);
			return;
		}
	}
}

void SoundManager::CreateBackup()
{
	mPlayOnceSoundComponentsBackup = mPlayOnceSoundComponents;
}

void SoundManager::LoadBackup()
{
	mPlayOnceSoundComponents = mPlayOnceSoundComponentsBackup;
}

void SoundManager::ClearComponents()
{
	mPlayOnceSoundComponents.clear();
	mPlayOnceSoundComponentsBackup.clear();

	mSoundComponents.clear();
	mSoundComponentsBackup.clear();
}

float SoundManager::GetAudioLength(std::string _audioFile)
{
	AssetManager::SoundAsset* soundAsset{};

	if (AssetManager::GetInstance()->GetAsset(_audioFile, soundAsset))
	{
		unsigned int soundLength{};
		static_cast<std::pair<FMOD::Sound*, SoundManager::SOUND_TYPE>*>(soundAsset)->first->getLength(&soundLength, FMOD_TIMEUNIT_MS);

		return (float)soundLength / 1000;
	}

	return 0.0f;
}