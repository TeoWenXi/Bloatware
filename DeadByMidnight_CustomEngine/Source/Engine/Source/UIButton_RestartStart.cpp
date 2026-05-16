/*!
All content © 2023 DigiPen Institute of Technology Singapore, all rights reserved.
@file       UIButton_RestartStart.cpp
@author     TAN Kang Zheng (100%)
@co-author

----------------------------------------------------------------------------

@course     CSD2401F23
@section    CSD2401F23-A
@project    CSD2401F23 Software Engineering Project 3

@brief      Button Component to restart the game
*//*______________________________________________________________________*/

#include "UIButton_RestartStart.h"

#include "GameUITextHandler.h"
#include "EntityManager.h"
#include "CoreEngine.h"
#include "SoundComponent.h"
#include "LevelLoader.h"
#include "GameScene.h"
#include "SoundManager.h"

void UIButtonRestartStart::OnClick()
{
	SoundManager::GetInstance()->StopChannel(SoundManager::SoundType_BGM);
	SceneManager::GetInstance()->SetSceneToRestart(true);
}

void UIButtonRestartStart::OnHoverEnter()
{
	GameUITextHandler* _textHandler = EntityManager::GetInstance()->GetEntity(mEntID)->GetComponent<GameUITextHandler>();
	if (_textHandler != nullptr)
	{
		_textHandler->mFontSize += 1;
		_textHandler->mColor -= Vec3{ 0.1f, 0.1f, 0.1f };
	}
	SoundComponent* _soundComp = EntityManager::GetInstance()->GetEntity(mEntID)->GetComponent<SoundComponent>();
	if (_soundComp != nullptr)
	{
		_soundComp->PlayOverride();
	}
}

void UIButtonRestartStart::OnHoverExit()
{
	GameUITextHandler* _textHandler = EntityManager::GetInstance()->GetEntity(mEntID)->GetComponent<GameUITextHandler>();
	if (_textHandler != nullptr)
	{
		_textHandler->mFontSize -= 1;
		_textHandler->mColor += Vec3{ 0.1f, 0.1f, 0.1f };
	}
}