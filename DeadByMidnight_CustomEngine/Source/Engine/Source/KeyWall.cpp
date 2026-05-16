/*!
All content © 2023 DigiPen Institute of Technology Singapore, all rights reserved.
@file       KeyWall.cpp
@author     TEO Wen Xi (100%)
@co-author

----------------------------------------------------------------------------

@course     CSD2401F23
@section    CSD2401F23-A
@project    CSD2401F23 Software Engineering Project 3

@brief      Contains functions for the Key component
*//*______________________________________________________________________*/

#include "KeyWall.h"
#include "EntityManager.h"
#include "ComponentsHeader.h"

#if _ENGINE
#include "MyImGui.h"
#endif // _ENGINE

std::vector<EntityID> KeyWall::mKeyWallIDs{};

void KeyWall::Init()
{
	mKeyWallIDs.push_back(mEntID);
}

void KeyWall::Load()
{
	mLocked = JSONManager::GetInstance()->GetBool("mLocked");
}

void KeyWall::Save()
{
	JSONManager::GetInstance()->Save("Component", "KeyWall");
	JSONManager::GetInstance()->Save("mLocked", mLocked);
}

void KeyWall::Update()
{
	//Lock door if player doesnt have key anymore
	if (Player::mPlayerID != 0 &&
		EntityManager::GetInstance()->GetEntity(Player::mPlayerID) != nullptr &&
		EntityManager::GetInstance()->GetEntity(Player::mPlayerID)->GetComponent<Player>() != nullptr)
		if (EntityManager::GetInstance()->GetEntity(Player::mPlayerID)->GetComponent<Player>()->mHasKey == false)
			mLocked = true;

	//Shader stuff
	Shader* currShader{ nullptr };
	AssetManager::GetInstance()->GetAsset("KeyWallShader", currShader);
	currShader->Use();
	AssetManager::ImageAsset* texture{ nullptr };
	AssetManager::GetInstance()->GetAsset("Misc_NoiseTextureGeneral", texture);
	currShader->SetUniform("uTex2d_Base", (GLint)texture->mTexID);
	glBindTextureUnit(texture->mTexID, texture->mTexID);
	AssetManager::ImageAsset* texture2{ nullptr };
	AssetManager::GetInstance()->GetAsset("Misc_NoiseTextureFractal", texture2);
	currShader->SetUniform("uTex2d_Noise", (GLint)texture2->mTexID);
	glBindTextureUnit(texture2->mTexID, texture2->mTexID);
	currShader->SetUniform("_Time", (float)glfwGetTime());
	currShader->SetUniform("_WindThreshold", 0.35f);
	if (mLocked)
	{
		currShader->SetUniform("_AlphaMultiplier", 1.0f);
		if (Player::mPlayerID == 0 || 
			EntityManager::GetInstance()->GetEntity(Player::mPlayerID) == nullptr || 
			EntityManager::GetInstance()->GetEntity(Player::mPlayerID)->GetComponent<Player>() == nullptr ||
			EntityManager::GetInstance()->GetEntity(Player::mPlayerID)->GetComponent<Player>()->mHasKey == false)
			currShader->SetUniform("_IsOpen", 0.0f);
		else
			currShader->SetUniform("_IsOpen", 1.0f);
	}
	else
	{
		currShader->SetUniform("_AlphaMultiplier", 0.0f);
		currShader->SetUniform("_IsOpen", 1.0f);
	}
	currShader->UnUse();
}

void KeyWall::Exit()
{
	auto iter = std::find(mKeyWallIDs.begin(), mKeyWallIDs.end(), mEntID);
	if (iter != mKeyWallIDs.end())
		mKeyWallIDs.erase(iter);
}

void KeyWall::DisplayProperties()
{
#if _ENGINE
	ImGui::Checkbox("Locked?", &mLocked);
#endif // _ENGINE
}

void KeyWall::Reset()
{
	mLocked = true;
	EntityManager::GetInstance()->GetEntity(mEntID)->SetActive(true);
	EntityManager::GetInstance()->GetEntity(mEntID)->GetComponent<Renderer>()->mEnabled = true;
}