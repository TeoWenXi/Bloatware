/*!
All content © 2023 DigiPen Institute of Technology Singapore, all rights reserved.
@file       Camera.cpp
@author     CAI Wenjin (80%)
@co-author	TEO Wen Xi (10%)
@co-author	Jovan Zhuo Wen LOW (10%)

----------------------------------------------------------------------------

@course     CSD2401F23
@section    CSD2401F23-A
@project    CSD2401F23 Software Engineering Project 3

@brief      Camera class definitions.
*//*______________________________________________________________________*/

#include "Camera.h"
#include "InputManager.h"
#include "ComponentsHeader.h"
#include "EntityManager.h"
#include "SceneManager.h"
#include "Scene.h"

#include <glm/gtc/matrix_transform.hpp>

Player* g_Player;
EntityID g_ParallaxA;
EntityID g_ParallaxB;
EntityID g_ParallaxC;
EntityID g_ParallaxZ;
EntityID g_AIEntity;
bool HasSetParallaxBG = false;

void Camera::Init(glm::vec3 _pos, glm::vec3 _up, glm::vec3 _tgt, GLfloat _top, GLfloat _btm, GLfloat _near, GLfloat _far)
{
	//Aspect ratio
	mAr = static_cast<GLfloat>(WindowsManager::GetInstance()->mWidth) / WindowsManager::GetInstance()->mHeight;

	//Set camera values
	mCurrCamInfo.mPosition = mOriginalPosition = mOldPosition = _pos;
	mUp = _up;
	mTarget = mOriginalTarget = mOldTarget = _tgt;

	//Set clipping plane values
	mTop = _top * (GraphicsSystem::GetInstance()->mWorldSize.y / 2);
	mBottom = _btm * (GraphicsSystem::GetInstance()->mWorldSize.y / 2);
	mLeft = _btm * mAr * (GraphicsSystem::GetInstance()->mWorldSize.x / 2);
	mRight = _top * mAr * (GraphicsSystem::GetInstance()->mWorldSize.x / 2);
	mNear = _near;
	mFar = _far;
	
	mViewXform = glm::lookAt(mCurrCamInfo.mPosition, mTarget, mUp);
	mProjectionXform = glm::ortho(mLeft, mRight, mBottom, mTop, mNear, mFar);

	SearchPlayer();
	SearchTextbox();
	HasSetParallaxBG = SearchParallaxBG();
}

bool Camera::SearchPlayer()
{
	for (size_t i{}; i < EntityManager::GetInstance()->mEntityList.size(); ++i)
	{
		if ("Player" == EntityManager::GetInstance()->mEntityList[i].GetName())
		{
			g_Player = EntityManager::GetInstance()->mEntityList[i].GetComponent<Player>();
			if (g_Player != nullptr)
			{
				mPlayerID = g_Player->mEntID;
				return true;
			}
		}
	}
	return false;
}

bool Camera::SearchTextbox()
{
	for (size_t i{}; i < EntityManager::GetInstance()->mEntityList.size(); ++i)
	{
		if ("TextOverlayBox" == EntityManager::GetInstance()->mEntityList[i].GetName())
		{
			mTextboxID = EntityManager::GetInstance()->mEntityList[i].GetID();
			return true;
		}
	}
	return false;
}

bool Camera::SearchParallaxBG()
{
	int counter = 0;
	for (size_t i{}; i < EntityManager::GetInstance()->mEntityList.size(); ++i)
	{
		if ("ParallaxBG_A" == EntityManager::GetInstance()->mEntityList[i].GetName())
		{
			g_ParallaxA = EntityManager::GetInstance()->mEntityList[i].GetID();
			if (EntityManager::GetInstance()->mEntityList[i].GetComponent<Transform>() != nullptr)
			{
				counter++;
				continue;
			}
		}
		if ("ParallaxBG_B" == EntityManager::GetInstance()->mEntityList[i].GetName())
		{
			g_ParallaxB = EntityManager::GetInstance()->mEntityList[i].GetID();
			if (EntityManager::GetInstance()->mEntityList[i].GetComponent<Transform>() != nullptr)
			{
				counter++;
				continue;
			}
		}
		if ("ParallaxBG_C" == EntityManager::GetInstance()->mEntityList[i].GetName())
		{
			g_ParallaxC = EntityManager::GetInstance()->mEntityList[i].GetID();
			if (EntityManager::GetInstance()->mEntityList[i].GetComponent<Transform>() != nullptr)
			{
				counter++;
				continue;
			}
		}
		if ("ParallaxBG_Z" == EntityManager::GetInstance()->mEntityList[i].GetName())
		{
			g_ParallaxZ = EntityManager::GetInstance()->mEntityList[i].GetID();
			if (EntityManager::GetInstance()->mEntityList[i].GetComponent<Transform>() != nullptr)
			{
				counter++;
				continue;
			}
		}
		if ("AIComponent" == EntityManager::GetInstance()->mEntityList[i].GetName())
		{
			g_AIEntity = EntityManager::GetInstance()->mEntityList[i].GetID();
			if (EntityManager::GetInstance()->mEntityList[i].GetComponent<Transform>() != nullptr)
			{
				counter++;
				continue;
			}
		}
	}

	if (counter >= 5) return true;
	return false;
}

void Camera::DoScreenshake()
{
	mScreenShakeTimeLeft = 1.0f;
}

void Camera::DoPermanentScreenshake()
{
	mPermanentScreenShakeEnabled = true;
	mPermanentScreenShakeTicker = 0.0f;
}

void Camera::EndPermanentScreenshake()
{
	mPermanentScreenShakeEnabled = false;
	mPermanentScreenShakeTicker = 0.0f;
}

bool Camera::IsPlayerInLeftSideRange(float _range)
{
	if (EntityManager::GetInstance()->GetEntity(mPlayerID) != nullptr)
	{
		return EntityManager::GetInstance()->GetEntity(mPlayerID)->GetComponent<Transform>()->GetPosition().mX < mCurrCamInfo.mClampHoriRange.x + _range;
	}
	return false;
}

void Camera::Update()
{
#if _DEBUG

	if (mCurrCamInfo.mPosition != mOldPosition)
	{
#if _ENGINE
		DebugLogger::GetInstance()->Log("Camera Pos: {%f, %f, %f}\n", mCurrCamInfo.mPosition.x, mCurrCamInfo.mPosition.x, mCurrCamInfo.mPosition.x);
#endif // _ENGINE
		mOldPosition = mCurrCamInfo.mPosition;
	}

	if (mTarget != mOldTarget)
	{
#if _ENGINE
		DebugLogger::GetInstance()->Log("Camera Target: {%f, %f, %f}\n", mTarget.x, mTarget.x, mTarget.x);
#endif // _ENGINE
		mOldTarget = mTarget;
	}

#endif // _DEBUG


#if _BLOATWARE
	if (InputManager::GetInstance()->GetKey(KeyCode::ALPHA_0))
	{
		if (InputManager::GetInstance()->GetKey(KeyCode::W))
			mCurrCamInfo.mPosition.y += 1.f * (GLfloat)WindowsManager::GetInstance()->mDeltaTime * mCamMoveSpeed;
		if (InputManager::GetInstance()->GetKey(KeyCode::S))
			mCurrCamInfo.mPosition.y -= 1.f * (GLfloat)WindowsManager::GetInstance()->mDeltaTime * mCamMoveSpeed;
		if (InputManager::GetInstance()->GetKey(KeyCode::A))
			mCurrCamInfo.mPosition.x -= 1.f * (GLfloat)WindowsManager::GetInstance()->mDeltaTime * mCamMoveSpeed;
		if (InputManager::GetInstance()->GetKey(KeyCode::D))
			mCurrCamInfo.mPosition.x += 1.f * (GLfloat)WindowsManager::GetInstance()->mDeltaTime * mCamMoveSpeed;
	}
#else
	if (InputManager::GetInstance()->GetKey(KeyCode::UP))
		mCurrCamInfo.mPosition.y += 1.f * (GLfloat)WindowsManager::GetInstance()->mDeltaTime * mCamMoveSpeed;
	if (InputManager::GetInstance()->GetKey(KeyCode::DOWN))
		mCurrCamInfo.mPosition.y -= 1.f * (GLfloat)WindowsManager::GetInstance()->mDeltaTime * mCamMoveSpeed;
	if (InputManager::GetInstance()->GetKey(KeyCode::LEFT))
		mCurrCamInfo.mPosition.x -= 1.f * (GLfloat)WindowsManager::GetInstance()->mDeltaTime * mCamMoveSpeed;
	if (InputManager::GetInstance()->GetKey(KeyCode::RIGHT))
		mCurrCamInfo.mPosition.x += 1.f * (GLfloat)WindowsManager::GetInstance()->mDeltaTime * mCamMoveSpeed;


	if (InputManager::GetInstance()->GetKeyReleased(KeyCode::MINUS))
		ResetCameraPos();
	if (InputManager::GetInstance()->GetKeyReleased(KeyCode::EQUAL))
		ToggleFollowPlayer();
#endif // _BLOATWARE

	if (CoreEngine::GetInstance()->mGameState == CoreEngine::GameState::GS_PLAY && true == mCurrCamInfo.mTrackPlayer)
	{
		SearchPlayer();
		SearchTextbox();

		if (EntityManager::GetInstance()->GetEntity(mPlayerID) != nullptr)
		{
			//Get player position
			Transform* playerXform = EntityManager::GetInstance()->GetEntity(mPlayerID)->GetComponent<Transform>();

			glm::vec3 TargetPosition = glm::vec3(playerXform->GetPosition().mX, playerXform->GetPosition().mY, mCurrCamInfo.mPosition.z);
			

			//BOUNDARY-LIKES
			if (mCurrCamInfo.mHoldHorizontal)
				TargetPosition.x = SceneManager::GetInstance()->GetActiveScene()->mCamWaypoints[SceneManager::GetInstance()->GetActiveScene()->mCamWaypointIndex].mPosition.x;
			if (mCurrCamInfo.mHoldVertical)
				TargetPosition.y = SceneManager::GetInstance()->GetActiveScene()->mCamWaypoints[SceneManager::GetInstance()->GetActiveScene()->mCamWaypointIndex].mPosition.y;

			//Clamping
			if (true == mCurrCamInfo.mClampEnabled)
			{
				//Left Clamp
				if (TargetPosition.x < mCurrCamInfo.mClampHoriRange.x + (GraphicsSystem::GetInstance()->mWorldSize.x / 2))
					TargetPosition.x = mCurrCamInfo.mClampHoriRange.x + (GraphicsSystem::GetInstance()->mWorldSize.x / 2);
				//Right Clamp
				else if (TargetPosition.x > mCurrCamInfo.mClampHoriRange.y - (GraphicsSystem::GetInstance()->mWorldSize.x / 2))
					TargetPosition.x = mCurrCamInfo.mClampHoriRange.y - (GraphicsSystem::GetInstance()->mWorldSize.x / 2);
				//Bottom Clamp
				if (TargetPosition.y < mCurrCamInfo.mClampVertRange.x + (GraphicsSystem::GetInstance()->mWorldSize.y / 2))
					TargetPosition.y = mCurrCamInfo.mClampVertRange.x + (GraphicsSystem::GetInstance()->mWorldSize.y / 2);
				//Top Clamp
				else if (TargetPosition.y > mCurrCamInfo.mClampVertRange.y - (GraphicsSystem::GetInstance()->mWorldSize.y / 2))
					TargetPosition.y = mCurrCamInfo.mClampVertRange.y - (GraphicsSystem::GetInstance()->mWorldSize.y / 2);
			}
			

			glm::vec3 Position_Difference = TargetPosition - mCurrCamInfo.mPosition;
			float Position_Difference_Length = sqrtf(
				powf(Position_Difference.x, 2)
				+
				powf(Position_Difference.y, 2)
			);
			float diffAutochecker = CameraSmoothstep(0.0f, 1.0f, Position_Difference_Length / (mCameraFollowMaxDistanceThreshold - mCameraFollowMinDistanceThreshold));
			if (diffAutochecker <= 0.0001f) diffAutochecker = 0;

			float lerpval = std::clamp((Position_Difference_Length - mCameraFollowMinDistanceThreshold) / (mCameraFollowMaxDistanceThreshold * 100.f - mCameraFollowMinDistanceThreshold), 0.1f, 1.f);
			Position_Difference *= lerpval * (float)WindowsManager::GetInstance()->mDeltaTime * mCameraFollowSpeed;
			mCurrCamInfo.mPosition += Position_Difference;

			mCurrCamInfo.mPosition += glm::vec3(5.0f * (rand() % 100 - 50.0f) / 100.f * mScreenShakeTimeLeft, 5.0f * (rand() % 100 - 50.0f) / 100.f * mScreenShakeTimeLeft, 0);
			mScreenShakeTimeLeft = std::clamp(mScreenShakeTimeLeft - 5 * (float)WindowsManager::GetInstance()->mDeltaTime, 0.f, 1.f);

			if (mPermanentScreenShakeEnabled)
			{
				mPermanentScreenShakeTicker = std::clamp(mPermanentScreenShakeTicker + 0.15f * (float)WindowsManager::GetInstance()->mDeltaTime, 0.f, 20.f);
			}
			else
			{
				mPermanentScreenShakeTicker = std::clamp(mPermanentScreenShakeTicker - 5.f * (float)WindowsManager::GetInstance()->mDeltaTime, 0.f, 20.f);
			}
			mCurrCamInfo.mPosition += glm::vec3(0.5f * (rand() % 100 - 50.0f) / 100.f * mPermanentScreenShakeTicker, 0.5f * (rand() % 100 - 50.0f) / 100.f * mPermanentScreenShakeTicker, 0);

			//Update view matrix
			mViewXform = glm::lookAt(mCurrCamInfo.mPosition, mTarget, mUp);

			if (EntityManager::GetInstance()->GetEntity(mTextboxID) != nullptr)
			{
				Transform* textboxXform = EntityManager::GetInstance()->GetEntity(mTextboxID)->GetComponent<Transform>();
				glm::vec2 textboxScale = TextManager::GetInstance()->GetTextboxSize();
				if (textboxScale.x >= FLT_EPSILON)
				{
					textboxXform->SetPosition(Vector3(mCurrCamInfo.mPosition.x, mCurrCamInfo.mPosition.y - (GraphicsSystem::GetInstance()->mWorldSize.y / 2) * (1 / mCurrCamInfo.mZoom) + (textboxScale.y /*+ 15.0f*/ / 2), -10.f));
					textboxXform->mScale = Vector3(1.5f * textboxScale.x + 80.f, textboxScale.y /*+ 19.0f*/, 1.0f);
				}
				else
				{
					textboxXform->mScale = Vector3(0.f, 0.0f, 1.0f);
				}
			}
		}
	}
	
	UpdateXformMatrices();
	if (!HasSetParallaxBG)
	{
		HasSetParallaxBG = SearchParallaxBG();
	}
	//UpdateParallaxBG();
}

float Camera::CameraSmoothstep(float _leftEdge, float _rightEdge, float _x)
{
	_x = std::clamp((_x - _leftEdge) / (_rightEdge - _leftEdge), 0.f, 1.f);
	return _x * _x * (3.0f - 2.0f * _x);
}

void Camera::UpdateXformMatrices()
{
	//mViewXform = glm::lookAt(mPosition, mTarget, mUp);
	mViewXform = glm::mat3
	{
		1, 0, 0,
		0, 1, 0,
		-mCurrCamInfo.mPosition.x, -mCurrCamInfo.mPosition.y, 1
	};

	mProjectionXform = glm::ortho(mLeft, mRight, mBottom, mTop, mNear, mFar);
	
	//Viewport Matrix, aka how many pixels = 1 unit in world position
	GLfloat width = (GLfloat)WindowsManager::GetInstance()->mWidth;
	GLfloat height = (GLfloat)WindowsManager::GetInstance()->mHeight;

	glm::mat4 ndcToWin
	{ 
		width / 2,			0,			0,		0,
		0,				-height / 2,	0,		0,
		0,					0,			0.5,	0,
		width / 2,		height / 2,		0.5,	1
	};

	//Concatenate transform matrices
	mXformMat = ndcToWin * mProjectionXform * mViewXform;
}

void Camera::ToggleFollowPlayer()
{
	mCurrCamInfo.mTrackPlayer = !mCurrCamInfo.mTrackPlayer;
	if (!SearchPlayer())
	{
		mCurrCamInfo.mTrackPlayer = false;
		ResetCameraPos();
	}
}

void Camera::ResetCameraPos()
{
	mCurrCamInfo.mPosition = mOriginalPosition;
	mTarget = mOriginalTarget;
}

void Camera::UpdateOrtho()
{
	//Aspect ratio
	mAr = static_cast<GLfloat>(WindowsManager::GetInstance()->mWidth) / WindowsManager::GetInstance()->mHeight;
	GLfloat camZoomVal = mCurrCamInfo.mZoom;
	camZoomVal = 1 / camZoomVal;

	//New viewport size
	GLfloat screenTop = (GLfloat)GraphicsSystem::GetInstance()->mVp.mHeight / 2;
	GLfloat screenBot = (GLfloat)GraphicsSystem::GetInstance()->mVp.mHeight / -2;

	//Clipping plane values
	mTop = screenTop * (GraphicsSystem::GetInstance()->mWorldSize.y / 2) * camZoomVal;
	mBottom = screenBot * (GraphicsSystem::GetInstance()->mWorldSize.y / 2) * camZoomVal;
	mLeft = screenBot * mAr * (GraphicsSystem::GetInstance()->mWorldSize.x / 2) * camZoomVal;
	mRight = screenTop * mAr * (GraphicsSystem::GetInstance()->mWorldSize.x / 2) * camZoomVal;
}

void Camera::UpdateCameraInfo()
{
	UpdateOrtho();
	UpdateXformMatrices();
}