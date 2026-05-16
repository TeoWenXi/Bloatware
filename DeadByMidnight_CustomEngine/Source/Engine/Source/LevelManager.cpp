/*!
All content © 2023 DigiPen Institute of Technology Singapore, all rights reserved.
@file       LevelManager.cpp
@author     Jovan Zhuo Wen LOW (100%)
@co-author

----------------------------------------------------------------------------

@course     CSD2401F23
@section    CSD2401F23-A
@project    CSD2401F23 Software Engineering Project 3

@brief      Contains all level sub-sections' boundaries and checkpoints
*/
/*______________________________________________________________________*/

#include "LevelManager.h"
#include "EntityManager.h"
#include "WindowsManager.h"
#include "EventManager.h"
#include "CoreEngine.h"
#include "InputManager.h"

// Components
#include "Transform.h"
#include "Player.h"
#include "SceneManager.h"
#include "Scene.h"

void LevelManager::Init()
{

}

void LevelManager::Load()
{
}

Vector3 LevelManager::RetrieveSpawnPosition(Vector3 _currPos)
{
	for (size_t i = LastScenePresent; i < SceneManager::GetInstance()->GetActiveScene()->mCamWaypoints.size(); ++i)
	{
		CamWaypoint camPoint = SceneManager::GetInstance()->GetActiveScene()->mCamWaypoints[i];
		if (camPoint.mClampHoriRange.x <= _currPos.mX
			&& _currPos.mX <= camPoint.mClampHoriRange.y
			&& camPoint.mClampVertRange.x <= _currPos.mY
			&& _currPos.mY <= camPoint.mClampVertRange.y)
		{
			LastScenePresent = (int)i;
			return camPoint.mPosition;
		}
	}

	if (SceneManager::GetInstance()->GetActiveScene()->mCamWaypoints.size() > LastScenePresent)
		return SceneManager::GetInstance()->GetActiveScene()->mCamWaypoints[LastScenePresent].mPosition;
	else
		return Vector3{ 0, 0, 0 };
}

void LevelManager::UpdateCameraInfo(Vector3 _currPos)
{
	CamWaypoint currWaypoint;

	for (size_t i = 0; i < SceneManager::GetInstance()->GetActiveScene()->mCamWaypoints.size(); ++i)
	{
		CamWaypoint camPoint = SceneManager::GetInstance()->GetActiveScene()->mCamWaypoints[i];
		if (camPoint.mClampHoriRange.x <= _currPos.mX
			&& _currPos.mX <= camPoint.mClampHoriRange.y
			&& camPoint.mClampVertRange.x <= _currPos.mY
			&& _currPos.mY <= camPoint.mClampVertRange.y)
		{
			if (i < LastScenePresent)
			{
				if (EntityManager::GetInstance()->GetEntity(GraphicsSystem::GetInstance()->mCamera.mPlayerID) != nullptr)
				{
					EntityManager::GetInstance()->GetEntity(GraphicsSystem::GetInstance()->mCamera.mPlayerID)->GetComponent<Player>()->Kill();
				}
				break;
			}
			LastScenePresent = (int)i;
			currWaypoint = camPoint;
			break;
		}

		if (i == SceneManager::GetInstance()->GetActiveScene()->mCamWaypoints.size() - 1)
		{
			if (SceneManager::GetInstance()->GetActiveScene()->mCamWaypoints.size() > LastScenePresent)
				currWaypoint = SceneManager::GetInstance()->GetActiveScene()->mCamWaypoints[LastScenePresent];
			else
				return;
		}
	}

	glm::vec3 prevPos = GraphicsSystem::GetInstance()->mCamera.mCurrCamInfo.mPosition;
	GraphicsSystem::GetInstance()->mCamera.mCurrCamInfo = currWaypoint;
	GraphicsSystem::GetInstance()->mCamera.UpdateCameraInfo();
	GraphicsSystem::GetInstance()->mCamera.mCurrCamInfo.mPosition = prevPos;
}

void LevelManager::DoScreenshake()
{
	GraphicsSystem::GetInstance()->mCamera.DoScreenshake();
}

void LevelManager::DoPermanentScreenshake()
{
	GraphicsSystem::GetInstance()->mCamera.DoPermanentScreenshake();
}

void LevelManager::Update()
{
	if (CoreEngine::GetInstance()->mGameState != CoreEngine::GameState::GS_PLAY)
		return;

	if (InputManager::GetInstance()->GetKeyReleased(KeyCode::MINUS))
		LastScenePresent = 0;
}

void LevelManager::Unload()
{
	// Destroy instance
	Destroy();
}
