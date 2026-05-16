/*!
All content © 2023 DigiPen Institute of Technology Singapore, all rights reserved.
@file       CinematicBars.cpp
@author     TEO Wen Xi (100%)
@co-author

----------------------------------------------------------------------------

@course     CSD2401F23
@section    CSD2401F23-A
@project    CSD2401F23 Software Engineering Project 3

@brief		Component to show cinematic bars for cinematics
*//*______________________________________________________________________*/

#include "CinematicBars.h"
#include "GraphicsSystem.h"
#include "EntityManager.h"

void CinematicBars::Init()
{
	//Get screen size
	Vec3 worldSize = GraphicsSystem::GetInstance()->mWorldSize * (1 / GraphicsSystem::GetInstance()->mCamera.mCurrCamInfo.mZoom);

	//Set cinematic bars to cover a certain percentage of the screen

	//Y AXIS (Vertical bars)
	if (mBarDir == BAR_DIRECTION::UP || mBarDir == BAR_DIRECTION::DOWN)
		mExpandLerpValue = (worldSize.mY * 0.25f) / mExpandDuration;
	//X AXIS (Horizontal bars)
	else
		mExpandLerpValue = (worldSize.mX * 0.25f) / mExpandDuration;

	//Init object position
	mXform = EntityManager::GetInstance()->GetEntity(mEntID)->GetComponent<Transform>();

	//Get camera position
	Vector3 camPos = GraphicsSystem::GetInstance()->mCamera.mCurrCamInfo.mPosition;
	Vector3 objScale{ 1,1,1 };

	switch (mBarDir)
	{
	case BAR_DIRECTION::UP:
		camPos.mY += worldSize.mY / 2;
		objScale.mX = worldSize.mX * 1.25f;
		break;
	case BAR_DIRECTION::DOWN:
		camPos.mY -= worldSize.mY / 2;
		objScale.mX = worldSize.mX * 1.25f;
		break;
	case BAR_DIRECTION::LEFT:
		camPos.mX -= worldSize.mX / 2;
		objScale.mY = worldSize.mY * 1.25f;
		break;
	case BAR_DIRECTION::RIGHT:
		camPos.mX += worldSize.mX / 2;
		objScale.mY = worldSize.mY * 1.25f;
		break;
	}

	//Set cinematic bar spawn location
	mXform->SetPosition(camPos);
	mXform->mScale = objScale;

	mTimer = 0;
}

void CinematicBars::Update() 
{
	//Increment timer
	mTimer += (float)WindowsManager::GetInstance()->mDeltaTime;

	switch (mCurrState)
	{
	case CURRENT_STATE::EXPANDING:
		//Y AXIS (Vertical bars)
		if (mBarDir == BAR_DIRECTION::UP || mBarDir == BAR_DIRECTION::DOWN)
			mXform->mScale.mY += mExpandLerpValue * (float)WindowsManager::GetInstance()->mDeltaTime;
		//X AXIS (Horizontal bars)
		else
			mXform->mScale.mX += mExpandLerpValue * (float)WindowsManager::GetInstance()->mDeltaTime;
		
		//Go next state
		if (mTimer > mExpandDuration)
		{
			mCurrState = CURRENT_STATE::HOLDING;
			mTimer = 0;
		}
		break;
	case CURRENT_STATE::HOLDING:
		//Go next state
		if (mTimer > mHoldDuration)
		{
			mCurrState = CURRENT_STATE::RETRACTING;
			mTimer = 0;
		}
		break;
	case CURRENT_STATE::RETRACTING:
		//Y AXIS (Vertical bars)
		if (mBarDir == BAR_DIRECTION::UP || mBarDir == BAR_DIRECTION::DOWN)
			mXform->mScale.mY -= mExpandLerpValue * (float)WindowsManager::GetInstance()->mDeltaTime;
		//X AXIS (Horizontal bars)
		else
			mXform->mScale.mX -= mExpandLerpValue * (float)WindowsManager::GetInstance()->mDeltaTime;

		//Clear object as cutscene has finished
		if (mTimer > mExpandDuration)
			EntityManager::GetInstance()->GetEntity(mEntID)->SetToDestroy(true);

		break;
	}

	//Set position
	Vector3 camPos = GraphicsSystem::GetInstance()->mCamera.mCurrCamInfo.mPosition;
	//Get screen size
	Vec3 worldSize = GraphicsSystem::GetInstance()->mWorldSize * (1 / GraphicsSystem::GetInstance()->mCamera.mCurrCamInfo.mZoom);
	switch (mBarDir)
	{
	case BAR_DIRECTION::UP:
		camPos.mY += worldSize.mY / 2;
		break;
	case BAR_DIRECTION::DOWN:
		camPos.mY -= worldSize.mY / 2;
		break;
	case BAR_DIRECTION::LEFT:
		camPos.mX -= worldSize.mX / 2;
		break;
	case BAR_DIRECTION::RIGHT:
		camPos.mX += worldSize.mX / 2;
		break;
	}
	mXform->SetPosition(camPos);
}

void CinematicBars::CreateCinematicBars(BAR_DIRECTION _dir, float _expandDura, float _holdDura)
{
	Entity* newEnt = EntityManager::GetInstance()->CreateEntity();

	if (newEnt == nullptr)
		return;

	switch (_dir)
	{
	case BAR_DIRECTION::UP:
		newEnt->SetName("Top Cinematic Bar");
		break;
	case BAR_DIRECTION::DOWN:
		newEnt->SetName("Bot Cinematic Bar");
		break;
	case BAR_DIRECTION::LEFT:
		newEnt->SetName("Left Cinematic Bar");
		break;
	case BAR_DIRECTION::RIGHT:
		newEnt->SetName("Right Cinematic Bar");
		break;
	}

	//Add transform
	newEnt->AddComponent<Transform>();

	//Add cinematic bars
	CinematicBars* newCineBar = newEnt->AddComponent<CinematicBars>();
	newCineBar->mBarDir = _dir;
	newCineBar->mExpandDuration = _expandDura;
	newCineBar->mHoldDuration = _holdDura;
	newCineBar->Init();

	//Add Renderer
	Renderer* rdr = newEnt->AddComponent<Renderer>();
	rdr->mModelType = Renderer::MODEL_TYPE::SPRITE;
	rdr->mRenderLayer = Renderer::RENDER_LAYER::INTERFACE;
	rdr->mTextureName = "blackSquare";
	rdr->mShaderName = "BasicShader";
}