/*!
All content © 2023 DigiPen Institute of Technology Singapore, all rights reserved.
@file       GameUIObjectHandler.h
@author     TAN Kang Zheng (100%)
@co-author

----------------------------------------------------------------------------

@course     CSD2401F23
@section    CSD2401F23-A
@project    CSD2401F23 Software Engineering Project 3

@brief		Component that acts as a "Parent" for a group of UI components
*//*______________________________________________________________________*/

#include "GameUIObjectHandler.h"
#include "GraphicsSystem.h"

void GameUIObjectHandler::Init()
{
	//Adds the component's parent entity into the UIManager's list
	GameUIManager::GetInstance()->AddParent(this->mEntID);
	cameraOffset = GraphicsSystem::GetInstance()->mCamera.mCurrCamInfo.mPosition;
	EntityManager::GetInstance()->GetEntity(mEntID)->GetComponent<Transform>()->SetPosition(cameraOffset);
}

void GameUIObjectHandler::Update()
{
	cameraOffset = GraphicsSystem::GetInstance()->mCamera.mCurrCamInfo.mPosition;
	if (EntityManager::GetInstance()->GetEntity(mEntID)->GetComponent<Transform>() != nullptr)
		EntityManager::GetInstance()->GetEntity(mEntID)->GetComponent<Transform>()->SetPosition(cameraOffset);

	if (!onLoadrun)
		OnLoad();
}

void GameUIObjectHandler::Save()
{
	JSONManager::GetInstance()->Save("Component", "GameUIObjectHandler");
}

void GameUIObjectHandler::Exit()
{
	GameUIManager::GetInstance()->RemoveParent(this->mEntID);
}

void GameUIObjectHandler::DisplayProperties()
{
#if _ENGINE
	MyImGui::MyImGuiVector3Input(cameraOffset, "Current Camera Offset");
#endif // _ENGINE
}

void GameUIObjectHandler::OnLoad()
{
	SoundComponent* _soundComp = EntityManager::GetInstance()->GetEntity(mEntID)->GetComponent<SoundComponent>();
	if (_soundComp != nullptr)
		_soundComp->PlayOverride();
	onLoadrun = true;
}
