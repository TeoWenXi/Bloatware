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

#include "ComponentBase.h"

#ifndef PLAY_ONCE_SOUND_COMPONENT_H
#define PLAY_ONCE_SOUND_COMPONENT_H

#if _ENGINE
#include "MyImGui.h"
#include <imgui.h>
#include <imgui_internal.h>
#endif

class PlayOnceSoundComponent : public ComponentBase
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
	void Update();

	/*!
	 * @brief
	 * Update virtual function for components
	*/
	void Exit();

	/*!
	 * @brief
	 * Load virtual function for components
	*/
	void Load() 
	{
		//mHasPlayed = JSONManager::GetInstance()->GetBool("HasPlayed");
	};

	/*!
	 * @brief
	 * Save virtual function for components
	*/
	void Save()
	{
		JSONManager::GetInstance()->Save("Component", "PlayOnceSoundComponent");
		//JSONManager::GetInstance()->Save("HasPlayed", mHasPlayed);
	};

	/*!
	 * @brief
	 * Resets the state of the PlayOnceSoundComponent to not been played
	*/
	void Reset();

	/*!
	 * @brief
	 * Obtain the state of the PlayOnceSoundComponent
	 * @return 
	 * A boolean reflecting the state of the component. True = has already played, false = has not been played
	*/
	bool HasPlayed();

	/*!
	 * @brief
	 * Function that dictates what is shown in the properties tab in the editor
	*/
	void DisplayProperties()
	{
#if _ENGINE
		//ImGui::Checkbox("hasPlayed", &mHasPlayed);
		if (!mHasPlayed)
		{
			ImGui::Text("Component on standby to play...");
		}
		else
		{
			ImGui::Text("Component has played...");
		}
#endif
	}

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
	friend std::ostream& operator<< (std::ostream& os, const PlayOnceSoundComponent& _handler)
	{
		os << "PlayOnceSoundComponent tied to mEntID " << _handler.mEntID;
		return os;
	}

private:
	bool mHasPlayed = false;
};

#endif