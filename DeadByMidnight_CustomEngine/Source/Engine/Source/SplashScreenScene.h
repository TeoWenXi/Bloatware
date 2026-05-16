/*!
All content © 2023 DigiPen Institute of Technology Singapore, all rights reserved.
@file       SplashScreenScene.h
@author     TAN Kang Zheng (100%)
@co-author

----------------------------------------------------------------------------

@course     CSD2401F23
@section    CSD2401F23-A
@project    CSD2401F23 Software Engineering Project 3

@brief      Splash Screen Scene for the game
*//*______________________________________________________________________*/

#ifndef SPLASH_SCREEN_SCENE_H
#define SPLASH_SCREEN_SCENE_H

#include "Scene.h"
#include "SceneManager.h"
#include "EntityManager.h"
#include "ComponentsHeader.h"
#include "LevelLoader.h"

class SplashScreenScene : public Scene
{
public:
	//Scene functions
	/*!
	 * @brief
	 * Load function for the splash screen scene
	*/
	void Load();

	/*!
	 * @brief
	 * Init function for the splash screen scene
	*/
	void Init();

	/*!
	 * @brief
	 * Input function for the splash screen scene
	 * @param _dt
	 * Delta time for the program
	*/
	void Input(double _dt);

	/*!
	 * @brief
	 * Update function for the splash screen scene
	 * @param _dt
	 * Delta time for the program
	*/
	void Update(double _dt);

	/*!
	 * @brief
	 * Pause function loop for the splash screen scene
	 * @param _dt
	 * Delta time for the program
	*/
	void Pause(double _dt);

	/*!
	 * @brief
	 * Render function for the splash screen scene
	 * @param _dt
	 * Delta time for the program
	*/
	void Render(double _dt);

	/*!
	 * @brief
	 * Free function for the splash screen scene. Should clear all memory at this stage.
	*/
	void Free();

	/*!
	 * @brief Exit function for the splash screen scene.
	*/
	void Exit();
};

#endif