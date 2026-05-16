/*!
All content © 2023 DigiPen Institute of Technology Singapore, all rights reserved.
@file       GameScene.h
@author		TEO Wen Xi (75%)
@co-author	TAN Kang Zheng (25%)

----------------------------------------------------------------------------

@course     CSD2401F23
@section    CSD2401F23-A
@project    CSD2401F23 Software Engineering Project 3

@brief      Main Game Scene for the game
*//*______________________________________________________________________*/

#ifndef GAME_SCENE_H
#define GAME_SCENE_H

#include "Scene.h"
#include "SceneManager.h"
#include "EntityManager.h"
#include "ComponentsHeader.h"
#include "LevelLoader.h"

class GameScene : public Scene
{
public:
	//Scene functions
	/*!
	 * @brief 
	 * Load function of this scene
	*/
	void Load();

	/*!
	 * @brief 
	 * Init function of this scene
	*/
	void Init();

	/*!
	 * @brief 
	 * Input function of this scene
	 * @param _dt 
	 * Deltatime
	*/
	void Input(double _dt);

	/*!
	 * @brief 
	 * Update function of this scene
	 * @param _dt 
	 * Deltatime
	*/
	void Update(double _dt);

	/*!
	 * @brief 
	 * Pause function of this scene
	 * @param _dt 
	 * Deltatime
	*/
	void Pause(double _dt);

	/*!
	 * @brief 
	 * Render function of this scene
	 * @param _dt 
	 * Deltatime
	*/
	void Render(double _dt);

	/*!
	 * @brief 
	 * Free function of this scene
	*/
	void Free();

	/*!
	 * @brief 
	 * Exit function of this scene
	*/
	void Exit();

public:

private:
	//Player
	EntityID mPlayerID{ UINT_MAX };
};

#endif