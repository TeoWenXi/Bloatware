/*!
All content © 2023 DigiPen Institute of Technology Singapore, all rights reserved.
@file       CreditsScene.h
@author		Jovan Zhuo Wen LOW (100%)
@co-author

----------------------------------------------------------------------------

@course     CSD2401F23
@section    CSD2401F23-A
@project    CSD2401F23 Software Engineering Project 3

@brief      Credits Scene for the game
*//*______________________________________________________________________*/

#ifndef CREDITS_SCENE_H
#define CREDITS_SCENE_H

#include "Scene.h"
#include "SceneManager.h"
#include "EntityManager.h"
#include "ComponentsHeader.h"
#include "LevelLoader.h"

class CreditsScene : public Scene
{
public:
	//Scene functions
	/*!
	 * @brief
	 * Load function for the credits scene
	*/
	void Load();

	/*!
	 * @brief
	 * Init function for the credits scene
	*/
	void Init();

	/*!
	 * @brief
	 * Input function for the credits scene
	 * @param _dt
	 * Delta time for the program
	*/
	void Input(double _dt);

	/*!
	 * @brief 
	 * Update function for the credits scene
	 * @param _dt
	 * Delta time for the program
	*/
	void Update(double _dt);

	/*!
	 * @brief
	 * Pause function loop for the credits scene
	 * @param _dt
	 * Delta time for the program
	*/
	void Pause(double _dt);

	/*!
	 * @brief
	 * Render function for the credits scene
	 * @param _dt
	 * Delta time for the program
	*/
	void Render(double _dt);

	/*!
	 * @brief 
	 * Free function for the credits scene. Should clear all memory at this stage.
	*/
	void Free();

	/*!
	 * @brief Exit function for the credits scene.
	*/
	void Exit();

	/*!
	 * @brief Loads Main Menu Scene
	*/
	void GoToMainMenuScene();


	CreditsScene();

private:
	std::string mScene = "EndingDialogueScene";

	float mChangeSceneDelayTime = 0.4f;
	float mChangeSceneDelayTimer = 0.0f;

	float mSceneScrollDelayTime = 1.5f;
	float mSceneScrollDelayTimer = 0.0f;

	std::string fontName = "IosevkaNerdFont-Bold";

	Entity* ent_SideImageA;
	Entity* ent_SideImageB;

	int showDeathCounterState = -1; //-1 = nothing | 0 = instantly tp + fade in + tally counting process | 1 = show the number | 2 = fade out
	float deathCounterTimer = 0.0f;
	float deathCounterTime_0 = 1.8f;
	float deathCounterTime_1 = 2.0f;
	float deathCounterTime_2 = 0.6f;
	EntityID TitleID;
};

#endif