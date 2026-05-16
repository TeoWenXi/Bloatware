/*!
All content © 2023 DigiPen Institute of Technology Singapore, all rights reserved.
@file       Key.cpp
@author     TEO Wen Xi (100%)
@co-author

----------------------------------------------------------------------------

@course     CSD2401F23
@section    CSD2401F23-A
@project    CSD2401F23 Software Engineering Project 3

@brief      Contains functions for the Key component
*//*______________________________________________________________________*/

#include "Key.h"
#include "EntityManager.h"
#include "ComponentsHeader.h"

#if _ENGINE
#include "MyImGui.h"
#endif // _ENGINE

std::vector<EntityID> Key::mKeyIDs{};

void Key::Init()
{
	mKeyIDs.push_back(mEntID);
}

void Key::Load()
{
	mKeyState = KeyState(JSONManager::GetInstance()->GetInt("KeyState"));
	mOffsetFromPlayer = JSONManager::GetInstance()->GetVec3("mOffsetFromPlayer");
	mSpawnLocation = JSONManager::GetInstance()->GetVec3("mSpawnLocation");
	mHideImage = JSONManager::GetInstance()->GetBool("HideImage");
}

void Key::Save()
{
	JSONManager::GetInstance()->Save("Component", "Key");
	JSONManager::GetInstance()->Save("KeyState", (int)mKeyState);
	JSONManager::GetInstance()->Save("mOffsetFromPlayer", mOffsetFromPlayer);
	JSONManager::GetInstance()->Save("mSpawnLocation", mSpawnLocation);
	JSONManager::GetInstance()->Save("HideImage", mHideImage);
}

void Key::Update()
{
	if (mKeyState != KeyState::COLLECTEDHOVER ||
		Player::mPlayerID == 0 ||
		EntityManager::GetInstance()->GetEntity(Player::mPlayerID) == nullptr ||
		EntityManager::GetInstance()->GetEntity(Player::mPlayerID)->GetComponent<Player>() == nullptr)
		return;

	Vector3 parentPos = EntityManager::GetInstance()->GetEntity(Player::mPlayerID)->GetComponent<Transform>()->GetPosition();
	EntityManager::GetInstance()->GetEntity(mEntID)->GetComponent<Transform>()->SetPosition(parentPos + mOffsetFromPlayer);
}

void Key::Exit()
{
	auto iter = std::find(mKeyIDs.begin(), mKeyIDs.end(), mEntID);
	if (iter != mKeyIDs.end())
		mKeyIDs.erase(iter);
}

void Key::DisplayProperties()
{
#if _ENGINE
	MyImGui::MyImGuiVector3Input(mOffsetFromPlayer, "Offset from player");
	MyImGui::MyImGuiBoolInput(mHideImage, "Hide image");
#endif // _ENGINE
}

void Key::Reset()
{
	if (mSpawnLocation != Vector3(0, 0, 0))
		EntityManager::GetInstance()->GetEntity(mEntID)->GetComponent<Transform>()->SetPosition(mSpawnLocation);
	
	mKeyState = KeyState::CANCOLLECT;
	EntityManager::GetInstance()->GetEntity(mEntID)->SetActive(true);

	if(mHideImage == false)
		EntityManager::GetInstance()->GetEntity(mEntID)->GetComponent<Renderer>()->mEnabled = true;
}