/*!
All content © 2024 DigiPen Institute of Technology Singapore, all rights reserved.
@file       ButtonSelectManager.cpp
@author     TAN Kang Zheng (100%)
@co-author

----------------------------------------------------------------------------

@course     CSD2451F24
@section    CSD2451F24-A
@project    CSD2451F24 Software Engineering Project 4

@brief      Manages using arrow keys to navigate buttons
*//*______________________________________________________________________*/

#include "ButtonSelectManager.h"
#include "ButtonWrapper.h"
#include "EntityManager.h"
#include "InputManager.h"
#include "DebugLogger.h"
#include "WindowsManager.h"

/*!
 * @brief
 *	Virtual update function for manager class.
*/
void ButtonSelectManager::Update()
{
	Shader* MMShader{ nullptr };
	AssetManager::GetInstance()->GetAsset("ButtonUIShader", MMShader);
	MMShader->Use();
	AssetManager::ImageAsset* textureMM1{ nullptr };
	AssetManager::GetInstance()->GetAsset("whitesquare", textureMM1);
	MMShader->SetUniform("uTex2d", (GLint)textureMM1->mTexID);
	MMShader->SetUniform("uTex2d_Blank", (GLint)textureMM1->mTexID);
	MMShader->SetUniform("uTextureEnabled", true);
	MMShader->SetUniform("_IsPressed", onButtonVal[0]);
	glBindTextureUnit(textureMM1->mTexID, textureMM1->mTexID);
	MMShader->UnUse();
	Shader* MM2Shader{ nullptr };
	AssetManager::GetInstance()->GetAsset("ButtonUIShader2", MM2Shader);
	MM2Shader->Use();
	MM2Shader->SetUniform("uTex2d", (GLint)textureMM1->mTexID);
	MM2Shader->SetUniform("uTex2d_Blank", (GLint)textureMM1->mTexID);
	MM2Shader->SetUniform("uTextureEnabled", true);
	MM2Shader->SetUniform("_IsPressed", onButtonVal[1]);
	glBindTextureUnit(textureMM1->mTexID, textureMM1->mTexID);
	MM2Shader->UnUse();
	Shader* MM3Shader{ nullptr };
	AssetManager::GetInstance()->GetAsset("ButtonUIShader3", MM3Shader);
	MM3Shader->Use();
	MM3Shader->SetUniform("uTex2d", (GLint)textureMM1->mTexID);
	MM3Shader->SetUniform("uTex2d_Blank", (GLint)textureMM1->mTexID);
	MM3Shader->SetUniform("uTextureEnabled", true);
	MM3Shader->SetUniform("_IsPressed", onButtonVal[2]);
	glBindTextureUnit(textureMM1->mTexID, textureMM1->mTexID);
	MM3Shader->UnUse();
	Shader* MM4Shader{ nullptr };
	AssetManager::GetInstance()->GetAsset("ButtonUIShader4", MM4Shader);
	MM4Shader->Use();
	MM4Shader->SetUniform("uTex2d", (GLint)textureMM1->mTexID);
	MM4Shader->SetUniform("uTex2d_Blank", (GLint)textureMM1->mTexID);
	MM4Shader->SetUniform("uTextureEnabled", true);
	MM4Shader->SetUniform("_IsPressed", onButtonVal[3]);
	glBindTextureUnit(textureMM1->mTexID, textureMM1->mTexID);
	MM4Shader->UnUse();

	for (unsigned int i = 0; i < sizeof(onButtonVal)/sizeof(onButtonVal[0]); ++i)
	{
		if (mButtonIndex == i)
		{
			onButtonVal[i] = std::clamp(onButtonVal[i] + (1.f / 0.25f) * (float)WindowsManager::GetInstance()->mDeltaTime, 0.f, 1.f);
		}
		else
		{
			onButtonVal[i] = std::clamp(onButtonVal[i] - (1.f / 0.25f) * (float)WindowsManager::GetInstance()->mDeltaTime, 0.f, 1.f);
		}
	}

	if (CoreEngine::GetInstance()->mGameState == CoreEngine::GameState::GS_STOP)
		return;

	if (mButtonList.size() == 0)
		return;

	if (mButtonIndex >= mButtonList.size())
		mButtonIndex = (unsigned int)(mButtonList.size() - 1);

	if (!mSelectedButton)
	{
		Entity* ent = EntityManager::GetInstance()->GetEntity(mButtonList.at(mButtonIndex));
		if (ent != nullptr)
		{
			UIButton* buttonObj = ent->GetComponent<ButtonWrapper>()->mButtonObj;
			if (buttonObj != nullptr)
			{
				buttonObj->OnHoverEnter();
				mSelectedButton = true;
			}
		}
	}

	//Button move upwards
	if (InputManager::GetInstance()->GetActionTriggered("MenuUp"))
	{
		if (mButtonIndex != 0)
		{
			//Play the current button's OnExit
			EntityManager::GetInstance()->GetEntity(mButtonList.at(mButtonIndex))->GetComponent<ButtonWrapper>()->mButtonObj->OnHoverExit();
			//Reduce the current button
			mButtonIndex--;
			//Play the new current button's OnEnter
			EntityManager::GetInstance()->GetEntity(mButtonList.at(mButtonIndex))->GetComponent<ButtonWrapper>()->mButtonObj->OnHoverEnter();
		}
	}

	//Button move downwards
	if (InputManager::GetInstance()->GetActionTriggered("MenuDown"))
	{
		if (mButtonIndex != mButtonList.size() - 1)
		{
			//Play the current button's OnExit
			EntityManager::GetInstance()->GetEntity(mButtonList.at(mButtonIndex))->GetComponent<ButtonWrapper>()->mButtonObj->OnHoverExit();
			//Reduce the current button
			mButtonIndex++;
			//Play the new current button's OnEnter
			EntityManager::GetInstance()->GetEntity(mButtonList.at(mButtonIndex))->GetComponent<ButtonWrapper>()->mButtonObj->OnHoverEnter();
		}
	}

	//Press Enter
	if (InputManager::GetInstance()->GetActionTriggered("MenuSelect"))
	{
		UIButton* buttonObj = EntityManager::GetInstance()->GetEntity(mButtonList.at(mButtonIndex))->GetComponent<ButtonWrapper>()->mButtonObj;
		if (buttonObj != nullptr)
		{
			buttonObj->OnClick();
		}
	}
}

/*!
 * @brief
 *	Adds a button to the list for tracking.
*/
void ButtonSelectManager::AddButton(EntityID _entityID)
{
	mButtonList.push_back(_entityID);
#if _ENGINE
	DebugLogger::GetInstance()->Log("ButtonSelectManager added Button of ID: %d\n", _entityID);
	DebugLogger::GetInstance()->Log("ButtonSelectManager now tracking %d Buttons\n", mButtonList.size());
#endif
	SortButtons();
}

/*!
 * @brief
 *	Removes a button to the list for tracking.
*/
void ButtonSelectManager::RemoveButton(EntityID _entityID)
{
	for (int i = 0; i < mButtonList.size(); i++)
	{
		if (mButtonList.at(i) == _entityID)
		{
			mButtonList.erase(mButtonList.begin() + i);
#if _ENGINE
			DebugLogger::GetInstance()->Log("ButtonSelectManager removed Button of ID: %d\n", _entityID);
			DebugLogger::GetInstance()->Log("ButtonSelectManager now tracking %d Buttons\n", mButtonList.size());
#endif
			break;
		}
	}
	SortButtons();
}

/*!
 * @brief
 *	Creates a backup of all the current mButtonList
*/
void ButtonSelectManager::CreateBackup()
{
	mButtonListBackup = mButtonList;
}

/*!
 * @brief
 *	Laods the backup of the mButtonList
*/
void ButtonSelectManager::LoadBackup()
{
	mButtonList = mButtonListBackup;
}

/*!
 * @brief
 *	Sorts the current list of buttons by their ButtonOrderPriority.
*/
void ButtonSelectManager::SortButtons()
{
	if (mButtonList.size() == 0)
		return;

	for(int i = 0; i < mButtonList.size(); i++)
	{
		for (int j = i; j < mButtonList.size(); j++)
		{
			if (i < mButtonList.size() - 1)
			{
				Entity* ent1 = EntityManager::GetInstance()->GetEntity(mButtonList.at(i));
				Entity* ent2 = EntityManager::GetInstance()->GetEntity(mButtonList.at(i + 1));
				if (ent1 == nullptr || ent2 == nullptr)
					continue;

				if (EntityManager::GetInstance()->GetEntity(mButtonList.at(i))->GetComponent<ButtonWrapper>()->mButtonOrderPosition >
					EntityManager::GetInstance()->GetEntity(mButtonList.at(i + 1))->GetComponent<ButtonWrapper>()->mButtonOrderPosition)
				{
					std::swap(mButtonList.at(i), mButtonList.at(i + 1));
				}
			}
		}
	}

	mButtonIndex = 0;
	mSelectedButton = false;
}