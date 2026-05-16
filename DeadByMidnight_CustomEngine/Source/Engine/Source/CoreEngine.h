/*!
All content © 2023 DigiPen Institute of Technology Singapore, all rights reserved.
@file       CoreEngine.h
@author     TEO Wen Xi (100%)
@co-author

----------------------------------------------------------------------------

@course     CSD2401F23
@section    CSD2401F23-A
@project    CSD2401F23 Software Engineering Project 3

@brief      Core Engine of the program. Contains the main game loop and assists
			in managing all registered systems/managers.
*//*______________________________________________________________________*/

#ifndef CORE_MANAGER_H
#define CORE_MANAGER_H

#include <vector>
#include <glad/glad.h>
#include <string>

#include "Singleton.h"
#include "Manager.h"

class CoreEngine : public Singleton<CoreEngine>, public Manager
{
public:
	enum class GameState
	{
		GS_STOP,
		GS_PLAY,
		GS_PAUSE
	};
public:
	//Manager

	/*!
	 * @brief
	 * Calls the init function of all managers tied to the core engine
	*/
	void Init();

	/*!
	 * @brief 
	 * Calls the load function of all managers tied to the core engine
	*/
	void Load();

	/*!
	 * @brief 
	 * Not Used
	*/
	void Update();

	/*!
	 * @brief 
	 * Calls the unload function of all managers tied to the core engine
	*/
	void Unload();

	/*!
	 * @brief 
	 * Closes the glfw window
	*/
	void Exit();

	/*!
	 * @brief 
	 * Main game loop of the entire system
	*/
	void MainGameLoop();

	//Registering managers to the vector array

	/*!
	 * @brief 
	 * Registers a system into the system list
	 * @tparam T 
	 * Type of the system to be used in measuring each system's efficiency
	 * @param _instance 
	 * Pointer to the manager's singleton instance
	*/
	template <typename T>
	void RegisterSystem(T* _instance)
	{
		//Add manager to list
		mSystemsList.push_back(dynamic_cast<Manager*>(_instance));

		//Add manager name to list
		mSystemsDeltaTime.push_back(std::make_pair(typeid(T).name(), NULL));
	}

	/*!
	 * @brief 
	 * Adds the system into the init sequence array to be init-ed later
	 * @tparam T 
	 * Typename of the system to be added
	 * @param _instance 
	 * Pointer to the manager's singleton instance
	*/
	template <typename T>
	void AddToInitSequence(T* _instance)
	{
		//Add manager to list
		mSystemInitSeq.push_back(dynamic_cast<Manager*>(_instance));
	}

	//For Update Capping
	GLfloat mTargetUpdateFrequency = 75.0;
	bool mEnableUpdateLoopTarget = false;

	//For FPS Capping
	double mRenderFPS;
	GLfloat mTargetRenderFPS = 60.0;
	bool mEnableFPSTarget = false;

	std::vector<std::pair<std::string, double>> mSystemsDeltaTime;
	double mTotalSystemsTime;

	//Pausing & Playing
	GameState mGameState = GameState::GS_STOP;
private:

	/*!
	 * @brief 
	 * Constructor of the CoreEngine class
	*/
	CoreEngine();

	/*!
	 * @brief 
	 * Destructor of the CoreEngine class
	*/
	~CoreEngine();

	friend Singleton<CoreEngine>;

	//Init Sequencing
	std::vector<Manager*> mSystemInitSeq;

	//System storage
	std::vector<Manager*> mSystemsList;
	
	//For Update Capping
	double mLastUpdateTime;

	//FPS capping
	GLuint mRenderCount;
	double mLastRenderTime;

	//FPS display
	double mLastFpsUpdateTime;
	double mRenderFpsUpdateInterval = 1.0;
};

#endif
