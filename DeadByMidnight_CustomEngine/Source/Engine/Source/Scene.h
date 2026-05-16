/*!
All content © 2023 DigiPen Institute of Technology Singapore, all rights reserved.
@file       Scene.h
@author     TAN Kang Zheng (100%)
@co-author

----------------------------------------------------------------------------

@course     CSD2401F23
@section    CSD2401F23-A
@project    CSD2401F23 Software Engineering Project 3

@brief      Header file for the Scene class
*//*______________________________________________________________________*/

#ifndef SCENE_H
#define SCENE_H

#include "CamWaypoint.h"
#include "GraphicsSystem.h"

class Scene
{
public:
	/*!
	 * @brief 
	 * Default constructor
	*/
	Scene() {};

	/*!
	 * @brief 
	 * Default destructor
	*/
	virtual ~Scene() {};

	/*!
	 * @brief 
	 * Load function of the scene
	*/
	virtual void Load() = 0;

	/*!
	 * @brief 
	 * Init function of the scene
	*/
	virtual void Init() = 0;

	/*!
	 * @brief
	 * Handles input for the scene
	 * @param dt
	 * System delta time
	*/
	virtual void Input(double dt) = 0;

	/*!
	 * @brief 
	 * Handles update for the scene
	 * @param dt 
	 * System delta time
	*/
	virtual void Update(double dt) = 0;

	/*!
	 * @brief 
	 * Handles the scene when it is paused
	 * @param dt 
	 * System delta time
	*/
	virtual void Pause(double dt) = 0;

	/*!
	 * @brief 
	 * Handles Render for the scene
	 * @param dt 
	 * System delta time
	*/
	virtual void Render(double dt) = 0;

	/*!
	 * @brief 
	 * Handles freeing of memory for the scene
	*/
	virtual void Free() = 0;

	/*!
	 * @brief 
	 * Handles the unloading of assets in the scene
	*/
	virtual void Exit() = 0;

	/*!
	 * @brief 
	 * Move Camera to next waypoint
	*/
	void MoveNextCamWaypoint()
	{
		//Check for last waypoint
		if (mCamWaypointIndex < (int)mCamWaypoints.size() - 1)
		{
			//Increment waypoint index
			++mCamWaypointIndex;

			//Update camera info
			//Do not update position if not first position as we have camera lerping movement
			glm::vec3 camPos{};
			if(mCamWaypointIndex > 0)
				camPos = GraphicsSystem::GetInstance()->mCamera.mCurrCamInfo.mPosition;

			GraphicsSystem::GetInstance()->mCamera.mCurrCamInfo = mCamWaypoints[mCamWaypointIndex];

			if (mCamWaypointIndex > 0)
				GraphicsSystem::GetInstance()->mCamera.mCurrCamInfo.mPosition = camPos;

			GraphicsSystem::GetInstance()->mCamera.UpdateCameraInfo();
		}

		Player* player = EntityManager::GetInstance()->GetEntity(Player::mPlayerID)->GetComponent<Player>();
		if (player != nullptr)
			player->ResetCurrentLevelDeaths();

		if (AIManager::GetInstance()->GetAIComponentVector().size())
		{
			AIComponent* aiEntity = EntityManager::GetInstance()->GetEntity(AIManager::GetInstance()->GetAIComponentVector().at(0))->GetComponent<AIComponent>();
			if (aiEntity != nullptr)
			{
				aiEntity->ResetLaser();
				aiEntity->DelayMovement();
			}
		}
	}
public:
	//Camera Positioning
	std::vector<CamWaypoint> mCamWaypoints;
	int mCamWaypointIndex = -1;
};

#endif