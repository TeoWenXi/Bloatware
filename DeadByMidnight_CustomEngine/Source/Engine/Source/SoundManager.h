/*!
All content © 2023 DigiPen Institute of Technology Singapore, all rights reserved.
@file       SoundManager.h
@author     TAN Kang Zheng (100%)
@co-author  

----------------------------------------------------------------------------

@course     CSD2401F23
@section    CSD2401F23-A
@project    CSD2401F23 Software Engineering Project 3

@brief      Manages the sound for the engine
*//*______________________________________________________________________*/

#ifndef SOUND_MANAGER_H
#define SOUND_MANAGER_H

//Other Includes
#include "Singleton.h"
#include "Manager.h"
#include "ECS_Types.h"

#include <iostream>
#include <string>
#include <vector>

//FMod includes
#pragma warning(push, 0)
#include <fmod.h>
#include <fmod.hpp>
#include <fmod_errors.h>
#pragma warning(pop)

class SoundManager : public Singleton <SoundManager>, public Manager
{
public:
	/*!
	 * @brief 
	 *	Virtual init function for manager class.
	*/
	void Init();

	/*!
	 * @brief 
	 *	Virtual load function for manager class.
	*/
	void Load();

	/*!
	 * @brief 
	 *	Virtual update function for manager class.
	*/
	void Update();

	/*!
	 * @brief 
	 *	Virtual unload function for the manager class.
	*/
	void Unload();

	enum SOUND_TYPE : int
	{
		SoundType_SFX = 0,
		SoundType_BGM = 1
	};

	/*!
	 * @brief 
	 *	Helper function for asset manager to create sound objects
	 * @param _referenceName 
	 *	The name to refer to when calling the sound
	 * @param _soundType 
	 *	The type of sound, BGM or VFX
	*/
	void CreateSound(std::string _referenceName, SOUND_TYPE _soundType);

	/*!
	 * @brief 
	 *	Plays the sound
	 * @param _referenceName
		The name of the sound
	 * @param _volume
		The _volume of the sound between 0 and 1
	 * @param _pitch
		The pitch of the sound between 0 and ? (above 1 will be undefined)
	 * @return 
	 *	The exact sound channel which is playing the given sound.
	*/
	FMOD::Channel* PlaySound(std::string _referenceName, float _volume, float _pitch);

	/*!
	 * @brief
	 *	Plays the sound irrespective of the current game state (pause, play etc)
	 * @param _referenceName
		The name of the sound
	* @param _volume
		The _volume of the sound between 0 and 1
	 * @param _pitch
		The pitch of the sound between 0 and ? (above 1 will be undefined)
	 * @return
	 *	The exact sound channel which is playing the given sound.
	*/		
	FMOD::Channel* PlaySoundOverride(std::string _referenceName, float _volume, float _pitch);

	/*!
	 * @brief 
	 *	Gets the current volume setting of the BGM channel
	 * @return 
	 *	The current volume of the BGM channel
	*/
	float GetBGMVolume();

	/*!
	 * @brief 
	 *	Sets the current volume setting of the BGM channel
	 * @param _val 
	 *	The new volume of the BGM channel
	*/
	void SetBGMVolume(float _val);

	/*!
	 * @brief 
	 *	Gets the pause state of the BGM channel
	 * @return 
	 *	The current pause state of the BGM channel
	*/
	bool GetBGMPaused();

	/*!
	 * @brief 
	 *	Sets the pause state of the BGM channel
	 * @param _val 
	 *	The new pause state of the BGM channel
	*/
	void SetBGMPaused(bool _val);

	/*!
	 * @brief
	 *	Gets the current volume setting of the SFX channel
	 * @return
	 *	The current volume of the SFX channel
	*/
	float GetSFXVolume();

	/*!
	 * @brief
	 *	Sets the current volume setting of the SFX channel
	 * @param _val
	 *	The new volume of the SFX channel
	*/
	void SetSFXVolume(float _val);

	/*!
	 * @brief
	 *	Gets the current volume setting of the Master Volume
	 * @return
	 *	The current volume of the Master Volume
	*/
	float GetMasterVolume();

	/*!
	 * @brief
	 *	Sets the current volume setting of the Master Volume
	 * @param _val
	 *	The new volume of the Master Volume
	*/
	void SetMasterVolume(float _val);

	/*!
	 * @brief
	 *	Sets the current volume setting of the BGM ducking
	 * @param _val
	 *	The new volume of the BGM Ducking
	*/
	void SetBGMDucking(float _val);

	/*!
	 * @brief
	 *	Gets the current volume setting of the Sound channel
	 * @return
	 *	The current volume of the Sound channel
	*/
	float GetChannelVolume();

	/*!
	 * @brief
	 *	Sets the current volume setting of the Sound channel
	 * @param _val
	 *	The new volume of the Sound channel
	*/
	void SetChannelVolume(float _val);

	/*!
	 * @brief
	 *	Gets the current pitch setting of the Sound channel
	 * @return
	 *	The current pitch of the Sound channel
	*/
	float GetChannelPitch();

	/*!
	 * @brief
	 *	Sets the current pitch setting of the Sound channel
	 * @param _val
	 *	The new pitch of the Sound channel
	*/
	void SetChannelPitch(float _val);

	/*!
	 * @brief
	 *	Gets the pause state of the SFX channel
	 * @return
	 *	The current pause state of the SFX channel
	*/
	bool GetSFXPaused();

	/*!
	 * @brief
	 *	Sets the pause state of the SFX channel
	 * @param _val
	 *	The new pause state of the SFX channel
	*/
	void SetSFXPaused(bool _val);

	/*!
	 * @brief 
	 *	Stops all audio of a specific type. BGMs have a fade out.
	 * @param _soundType 
	 *	The channel to stop
	*/
	void StopChannel(SOUND_TYPE _soundType);

	/*!
	 * @brief
	 *	Stops a specific channel's audio.
	 * @param _soundType
	 *	The channel to stop
	*/
	void StopChannelSound(FMOD::Channel* _channel);

	/*!
	 * @brief 
	 *	Gets the number of all currently active audio channels
	 * @return 
	 *	The number of currently active audio channels
	*/
	int GetActiveChannels();

	/*!
	 * @brief 
	 *	Creates audio object for insertion into asset manager
	 * @param _fullPath 
	 *	The file path of the audio
	 * @param _dataPtr 
	 *	Pointer to information about the audio
	*/
	void CreateSoundAsset(std::string _fullPath, void*& _dataPtr);

	/*!
	 * @brief
	 * Adds an entity ID that has a PlayOnceSoundComponent to call update
	 * @param _mEntID
	 *	The entity ID of the entity
	*/
	void AddPlayOnceComponent(EntityID _mEntID);

	/*!
	 * @brief Resets all PlayOnceComponents that SoundManager is tracking
	*/
	void ResetPlayOnceComponent();

	/*!
	 * @brief Removes a PlayOnceComponent from the SoundManager tracking list
	 * @param _mEntID 
	*/
	void RemovePlayOnceComponent(EntityID _mEntID);

	/*!
	 * @brief
	 * Adds an entity ID that has a SoundComponent to track
	 * @param _mEntID
	 *	The entity ID of the entity
	*/
	void AddSoundComponent(EntityID _mEntID);

	/*!
	 * @brief Removes a SoundComponent from the SoundManager tracking list
	 * @param _mEntID
	*/
	void RemoveSoundComponent(EntityID _mEntID);

	/*!
	 * @brief Creates a backup of all entities tracked by SoundManager
	*/
	void CreateBackup();

	/*!
	 * @brief Loads from backup all entities tracked by SoundManager
	*/
	void LoadBackup();

	/*!
	 * @brief Clears all components from lists
	*/
	void ClearComponents();

	/*!
	 * @brief 
	 *  Gets the audio length of a given audio file name
	 * @param _audioFile
	 *  The name of the Audio File
	 * @return
	 *  The length of the audio file in seconds
	 * 
	*/
	float GetAudioLength(std::string _audioFile);

	/*!
	 * @brief
	 *  Gets the DSP Buffer size of the current FMod system
	 * @return
	 *  The DSP Buffer Size
	*/
	int GetDSPBufferSize();
private:

	/*!
	 * @brief 
	 *	Default constructor of the SoundManager
	*/
	SoundManager();

	/*!
	 * @brief
	 *	Default destructor of the SoundManager
	*/
	~SoundManager();

	friend Singleton <SoundManager>;

	FMOD_RESULT mFmodErr;
	FMOD::ChannelGroup* mSFXChannelGroup;
	FMOD::ChannelGroup* mBGMChannelGroup;
	FMOD::ChannelGroup* mPauseOverrideChannelGroup;
	std::vector<FMOD::Channel*> mSFXChannels;
	std::vector<FMOD::Channel*> mBGMChannels;
	std::vector<FMOD::Channel*> mPauseOverrideChannels;
	float mSFXVolume, mBGMVolume, mChannelVolume{ 1.0f }, mChannelPitch{ 1.0f }, mMasterVolume{ 1.0f }, mBGMDucking{ 1.0f };

	FMOD::System* mFmodSystem;

	bool mLastFramePaused = false;

	std::vector<EntityID> mPlayOnceSoundComponents, mPlayOnceSoundComponentsBackup;
	std::vector<EntityID> mSoundComponents, mSoundComponentsBackup;
};

#endif