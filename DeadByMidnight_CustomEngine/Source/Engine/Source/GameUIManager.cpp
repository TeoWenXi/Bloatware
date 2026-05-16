/*!
All content © 2023 DigiPen Institute of Technology Singapore, all rights reserved.
@file       GameUIManager.cpp
@author     TAN Kang Zheng (100%)
@co-author

----------------------------------------------------------------------------

@course     CSD2401F23
@section    CSD2401F23-A
@project    CSD2401F23 Software Engineering Project 3

@brief      Manages the in-game UI for the engine
*//*______________________________________________________________________*/

#include "GameUIManager.h"
#include "LevelLoader.h"
#include "GameUIObjectHandler.h"
#include "GameUITextHandler.h"
#include "EndGoal.h"
#include "DialogueTrigger.h"

void GameUIManager::Init()
{
	
}

void GameUIManager::Load()
{
}

void GameUIManager::Update()
{
	Shader* MMShader{ nullptr };
	AssetManager::GetInstance()->GetAsset("MainMenuShader", MMShader);
	MMShader->Use();
	AssetManager::ImageAsset* textureMM1{ nullptr };
	AssetManager::GetInstance()->GetAsset("MM_BG_BG", textureMM1);
	MMShader->SetUniform("uTex2d_BG", (GLint)textureMM1->mTexID);
	glBindTextureUnit(textureMM1->mTexID, textureMM1->mTexID);
	AssetManager::ImageAsset* textureMM2{ nullptr };
	AssetManager::GetInstance()->GetAsset("MM_BG_Noise", textureMM2);
	MMShader->SetUniform("uTex2d_Noise", (GLint)textureMM2->mTexID);
	glBindTextureUnit(textureMM2->mTexID, textureMM2->mTexID);
	MMShader->SetUniform("_Time", (float)glfwGetTime());
	MMShader->UnUse();

	for (int i = 0; i < mParentComponents.size(); i++)
	{
		if (mParentComponents[i] == 0 || EntityManager::GetInstance()->GetEntity(mParentComponents.at(i)) == nullptr)
			continue;

		GameUIObjectHandler* uiParent = EntityManager::GetInstance()->GetEntity(mParentComponents.at(i))->GetComponent<GameUIObjectHandler>();
		if (uiParent != nullptr)
		{
			uiParent->Update();
		}
	}

	TextManager::GetInstance()->ClearRenderInformation();

	for (int i = 0; i < mTextToRender.size(); i++)
	{
		if (mTextToRender[i] == 0 || EntityManager::GetInstance()->GetEntity(mTextToRender.at(i)) == nullptr)
			continue;

		GameUITextHandler* uiText = EntityManager::GetInstance()->GetEntity(mTextToRender.at(i))->GetComponent<GameUITextHandler>();
		if (uiText == nullptr)
			continue;

		Vector3 EntityPos = EntityManager::GetInstance()->GetEntity(mTextToRender.at(i))->GetComponent<Transform>()->GetPosition();

		TextManager::GetInstance()->DrawText(uiText->mFont,
											uiText->mText,
											(GLfloat)(uiText->mFontSize),
											EntityPos.mX + uiText->mPos.mX,
											EntityPos.mY + uiText->mPos.mY,
											uiText->mColor,
											static_cast<int>(uiText->mRenderLayer),
											uiText->mAlpha * WindowsManager::GetInstance()->mSceneTrueOpacity);
	}

	for (int i = 0; i < mButtons.size(); i++)
	{
		if (mButtons[i] == 0 || EntityManager::GetInstance()->GetEntity(mButtons.at(i)) == nullptr)
			continue;

		ButtonWrapper* buttonWrapper = EntityManager::GetInstance()->GetEntity(mButtons.at(i))->GetComponent<ButtonWrapper>();
		if (buttonWrapper == nullptr)
			continue;

		buttonWrapper->Update();
	}

	for (int i = 0; i < mEndGoal.size(); i++)
	{
		if (mEndGoal[i] == 0 || EntityManager::GetInstance()->GetEntity(mEndGoal.at(i)) == nullptr)
			continue;

		if (EntityManager::GetInstance()->GetEntity(mEndGoal.at(i))->GetActive() == false)
			continue;

		EndGoal* endGoal = EntityManager::GetInstance()->GetEntity(mEndGoal.at(i))->GetComponent<EndGoal>();
		if (endGoal == nullptr)
			continue;

		endGoal->Update();
	}

	for (int i = 0; i < mDialogueTriggers.size(); i++)
	{
		if (mDialogueTriggers[i] == 0 || EntityManager::GetInstance()->GetEntity(mDialogueTriggers.at(i)) == nullptr)
			continue;

		if (EntityManager::GetInstance()->GetEntity(mDialogueTriggers.at(i))->GetActive() == false)
			continue;

		DialogueTrigger* dialogueTrigger = EntityManager::GetInstance()->GetEntity(mDialogueTriggers.at(i))->GetComponent<DialogueTrigger>();
		if (dialogueTrigger == nullptr)
			continue;

		dialogueTrigger->Update();
	}
}

void GameUIManager::Unload()
{
	//Destroy Instance
	Destroy();
}

void GameUIManager::Render()
{

}

/*-------------------------------------------------------*/

void GameUIManager::AddParent(EntityID _entityID)
{
	mParentComponents.push_back(_entityID);
}

void GameUIManager::AddText(EntityID _entityID)
{
	mTextToRender.push_back(_entityID);
}

void GameUIManager::AddButton(EntityID _entityID)
{
	mButtons.push_back(_entityID);
}

void GameUIManager::AddEndGoal(EntityID _entityID)
{
	mEndGoal.push_back(_entityID);
}

void GameUIManager::AddDialogueTrigger(EntityID _entityID)
{
	mDialogueTriggers.push_back(_entityID);
}

void GameUIManager::RemoveParent(EntityID _entityID)
{
	for (int i = 0; i < mParentComponents.size(); i++)
	{
		if (_entityID == mParentComponents.at(i))
		{
			mParentComponents.erase(mParentComponents.begin() + i);
			return;
		}
	}
}

void GameUIManager::RemoveText(EntityID _entityID)
{
	for (int i = 0; i < mTextToRender.size(); i++)
	{
		if (_entityID == mTextToRender.at(i))
		{
			mTextToRender.erase(mTextToRender.begin() + i);
			return;
		}
	}
}

void GameUIManager::RemoveButton(EntityID _entityID)
{
	for (int i = 0; i < mButtons.size(); i++)
	{
		if (_entityID == mButtons.at(i))
		{
			mButtons.erase(mButtons.begin() + i);
			return;
		}
	}
}

void GameUIManager::RemoveEndGoal(EntityID _entityID)
{
	for (int i = 0; i < mEndGoal.size(); i++)
	{
		if (_entityID == mEndGoal.at(i))
		{
			mEndGoal.erase(mEndGoal.begin() + i);
			return;
		}
	}
}

void GameUIManager::RemoveDialogueTrigger(EntityID _entityID)
{
	for (int i = 0; i < mDialogueTriggers.size(); i++)
	{
		if (_entityID == mDialogueTriggers.at(i))
		{
			mDialogueTriggers.erase(mDialogueTriggers.begin() + i);
			return;
		}
	}
}

Entity* GameUIManager::GetParentEntity(std::string _entityName)
{
	for (size_t i = 0; i < mParentComponents.size(); i++)
	{
		Entity* currEnt = EntityManager::GetInstance()->GetEntity(mParentComponents.at(i));
		if (currEnt->GetName() == _entityName)
			return currEnt;
	}

	return nullptr;
}

Entity* GameUIManager::GetTextEntity(std::string _entityName)
{
	for (size_t i = 0; i < mTextToRender.size(); i++)
	{
		Entity* currEnt = EntityManager::GetInstance()->GetEntity(mTextToRender.at(i));
		if (currEnt->GetName() == _entityName)
			return currEnt;
	}

	return nullptr;
}

Entity* GameUIManager::GetButtonEntity(std::string _entityName)
{
	for (size_t i = 0; i < mButtons.size(); i++)
	{
		Entity* currEnt = EntityManager::GetInstance()->GetEntity(mButtons.at(i));
		if (currEnt->GetName() == _entityName)
			return currEnt;
	}

	return nullptr;
}

void GameUIManager::ClearAllUI()
{
	mButtons.clear();
	mParentComponents.clear();
	mTextToRender.clear();
	mEndGoal.clear();
}

void GameUIManager::CreateBackup()
{
	mParentComponentsBackup = mParentComponents;
	mTextToRenderBackup = mTextToRender;
	mButtonsBackup = mButtons;
	mEndGoalBackup = mEndGoal;
	mDialogueTriggersBackup = mDialogueTriggers;
}

void GameUIManager::LoadBackup()
{
	mParentComponents = mParentComponentsBackup;
	mTextToRender = mTextToRenderBackup;
	mButtons = mButtonsBackup;
	mEndGoal = mEndGoalBackup;
	mDialogueTriggers = mDialogueTriggersBackup;
}