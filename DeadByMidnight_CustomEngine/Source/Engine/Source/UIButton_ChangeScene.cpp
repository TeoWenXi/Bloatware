/*!
All content © 2024 DigiPen Institute of Technology Singapore, all rights reserved.
@file       UIButton_ChangeScene.cpp
@author     TAN Kang Zheng (100%)
@co-author

----------------------------------------------------------------------------

@course     CSD2401F23
@section    CSD2401F23-A
@project    CSD2401F23 Software Engineering Project 3

@brief      Button Component to change scenes
*//*______________________________________________________________________*/

#include "UIButton_ChangeScene.h"
#include "SceneManager.h"

void UIButtonChangeScene::OnClick()
{
#if _ENGINE
	DebugLogger::GetInstance()->Log("Attempting to Load Scene: %s\n", mScene.c_str());
#endif // _ENGINE

	SceneManager::GetInstance()->SetActiveScene(SceneManager::GetSceneFromName(mScene));
}

void UIButtonChangeScene::OnHoverEnter()
{
	GameUITextHandler* _textHandler = EntityManager::GetInstance()->GetEntity(mEntID)->GetComponent<GameUITextHandler>();
	if (_textHandler != nullptr)
	{
		_textHandler->mFontSize += 1;
		_textHandler->mColor += Vec3{ 0.8f, 0.8f, 0.8f };
	}

	SoundComponent* _soundComp = EntityManager::GetInstance()->GetEntity(mEntID)->GetComponent<SoundComponent>();
	if (_soundComp != nullptr)
	{
		_soundComp->PlayOverride();
	}
}

void UIButtonChangeScene::OnHoverExit()
{
	GameUITextHandler* _textHandler = EntityManager::GetInstance()->GetEntity(mEntID)->GetComponent<GameUITextHandler>();
	if (_textHandler != nullptr)
	{
		_textHandler->mFontSize -= 1;
		_textHandler->mColor -= Vec3{ 0.8f, 0.8f, 0.8f };
	}
}

void UIButtonChangeScene::SetScene(std::string _scene)
{
	mScene = _scene;
}
