/*!
All content © 2023 DigiPen Institute of Technology Singapore, all rights reserved.
@file       SoundComponent.h
@author     TAN Kang Zheng (100%)
@co-author

----------------------------------------------------------------------------

@course     CSD2401F23
@section    CSD2401F23-A
@project    CSD2401F23 Software Engineering Project 3

@brief		Component for audio management for entities
*//*______________________________________________________________________*/

#ifndef SOUND_COMPONENT_H
#define SOUND_COMPONENT_H

#include "ComponentBase.h"

class SoundComponent : public ComponentBase
{
public:

	/*!
	 * @brief
	 * Init virtual function for components
	*/
	void Init();

	/*!
	 * @brief
	 * Update virtual function for components
	*/
	void Update() {};

	/*!
	 * @brief
	 * Update virtual function for components
	*/
	void Exit();

	/*!
	 * @brief
	 * Load virtual function for components
	*/
	void Load();

	/*!
	 * @brief
	 * Save virtual function for components
	*/
	void Save();

	/*!
	 * @brief
	 * DisplayProperties virtual function for components
	*/
	void DisplayProperties();

	/*!
	 * @brief
	 * Signals the SoundManager to play the sound saved to this component
	*/
	void Play();

	/*!
	 * @brief
	 * Signals the SoundManager to stop all channels with sounds played by this component
	*/
	void Stop();

	/*!
	 * @brief
	 * Signals the SoundManager to remove a channel from its channel list
	 * @param _channel
	 * Pointer to the channel that is to be removed from the sound component's channel list
	 * @return
	 * A boolean determining if the channel was removed
	*/
	bool RemoveChannel(FMOD::Channel* _channel);

	/*!
	 * @brief
	 * Signals the SoundManager to play the sound saved to this component in the override channel
	*/
	void PlayOverride();

	/*!
	 * @brief
	 *	Operator << overload to print to logs on creation
	 * @param os
	 *	Output stream to print to
	 * @param _handler
	 *	Reference to the GameUIObjectHandler
	 * @return
	 *	The output stream to continue printing to
	*/
	friend std::ostream& operator<< (std::ostream& os, const SoundComponent& _handler)
	{
		os << "SoundComponent tied to Ent ID " << _handler.mEntID << " | SoundName: " << _handler.mSoundName << " | Volume: " << _handler.mVolume << " | Pitch: " << _handler.mPitch << " |";
		return os;
	}

	//String that stores the name of audio
	std::string mSoundName{};
	//Volume of individual volume
	float mVolume = 1.0f;
	//Pitch of individual volume
	float mPitch = 1.0f;

	std::vector<FMOD::Channel*> mChannelVec;
};


#endif
