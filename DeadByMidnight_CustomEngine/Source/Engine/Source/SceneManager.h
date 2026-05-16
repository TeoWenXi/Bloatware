/*!
All content © 2023 DigiPen Institute of Technology Singapore, all rights reserved.
@file       SceneManager.h
@author     TAN Kang Zheng (100%)
@co-author

----------------------------------------------------------------------------
@course     CSD2401F23
@section    CSD2401F23-A
@project    CSD2401F23 Software Engineering Project 3

@brief      Header file for the SceneManager, for handling the changing of scenes
*//*______________________________________________________________________*/

#ifndef SCENE_MANAGER_H
#define SCENE_MANAGER_H

#include "Singleton.h"
#include "Manager.h"

#include <map>
#include <string>

class Scene;

class SceneManager : public Singleton<SceneManager>, public Manager
{
public:
	
	/*!
	 * @brief 
	 *	Initializes the variavles within SceneManager
	*/
	void Init();

	/*!
	 * @brief 
	 *	Calls the Init and Load functions from the active scene
	*/
	void Load();

	/*!
	 * @brief 
	 *	Calls all update functions required by active Scene object
	*/
	void Update();

	/*!
	 * @brief
	 *	Calls the unloac function for the currently active scene
	*/
	void Unload();

	/*!
	 * @brief 
	 *	Calls the render function for the currently active scene
	*/
	void Render();

	enum SCENES
	{
		SCENE_EDITOR_MAIN,
		SCENE_MAIN_MENU,
		SCENE_GAME,
		SCENE_OPENING_DIALOGUE,
		SCENE_BOSS,
		SCENE_MAIN_MENU_TRANSITION,
		SCENE_END,
		SCENE_ENDING_DIALOGUE,
		SCENE_CREDITS,
		SCENE_SPLASH_SCREEN,
		SCENE_COUNT
	};

	/*!
	 * @brief 
	 *	Sets the manager's quit flag to true
	*/
	void Quit();

	/*!
	 * @brief 
	 *	Initializes the SceneManage with the first scene
	 * @param firstScene 
	 *	The Enum value of the scene
	*/
	void InitFirstScene(SCENES firstScene);

	/*!
	 * @brief 
	 *	Signals to the SceneManager to swap to the next scene
	 * @param nextScene 
	 *	Enum of the next scene to create
	 * @return 
	 *	Pointer to the next scene
	*/
	Scene* SetActiveScene(SCENES nextScene);

	/*!
	 * @brief 
	 *	bool operator overload to return the manager's quit flag
	*/
	operator bool() const;

	std::string mActiveSceneName;

	/*!
	 * @brief 
	 * Obtain a scene from a given name
	 * @param sceneName 
	 * The name of the scene
	 * @return 
	 * A SCENES enum representation of the scene
	*/
	static SCENES GetSceneFromName(const std::string& sceneName);

	/*!
	 * @brief
	 * Parses an input to return the scene name
	 * @param inputStr 
	 * The input to parsed
	 * @return 
	 * The parsed scene name
	*/
	static std::string DemangleSceneName(const std::string& inputStr);

	/*!
	 * @brief 
	 * Sets the signal for the scene to restart to the given input
	 * @param _restart 
	 * The state of restart in next frame. True = will restart, False = will not restart
	*/
	void SetSceneToRestart(bool _restart);

	/*!
	 * @brief
	 * Begins Scene Transition to given scene
	 * @param _sceneName
	 * GivenScene
	*/
	void SetSceneToTransitionTo(std::string _sceneName);
	
	/*!
	 * @brief 
	 * Gets the currently active scene
	 * @return 
	 * A pointer to the currently active scene
	*/
	Scene* GetActiveScene();

	Scene* mEditorScene;	//Editor with all imgui stuff

	int totalDeathCount = 0; //player's total death count from start of game

private:
	/*!
	 * @brief 
	 *	Default constructor of the manager
	*/
	SceneManager();

	/*!
	 * @brief
	 *	Default destructor of the manager.
	*/
	~SceneManager();

	friend Singleton<SceneManager>;

	Scene* mActiveScene, * mNextScene;
	bool mQuit;
	bool mRestartScene;

	bool isChangingScene = false;
	float mChangeSceneDelayTime = 0.4f;
	float mChangeSceneDelayTimer = 0.0f;
	std::string mChangeSceneName = "MainMenuScene";
};

#endif