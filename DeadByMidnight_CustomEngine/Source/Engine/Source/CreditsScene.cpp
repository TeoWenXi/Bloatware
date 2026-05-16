/*!
All content © 2023 DigiPen Institute of Technology Singapore, all rights reserved.
@file       CreditsScene.cpp
@author		Jovan Zhuo Wen LOW (100%)
@co-author

----------------------------------------------------------------------------

@course     CSD2401F23
@section    CSD2401F23-A
@project    CSD2401F23 Software Engineering Project 3

@brief      Credits Scene for the game
*//*______________________________________________________________________*/

#define UNREFERENCED_PARAMETER(P) (P)

#include "CoreEngine.h"
#include "InputManager.h"

#include "CreditsScene.h"

#include "WindowsManager.h"

CreditsScene::CreditsScene()
{

}

void CreditsScene::Init()
{
	//Reset camera zoom
	GraphicsSystem::GetInstance()->mCamera.mCurrCamInfo.mZoom = 1.f;
	GraphicsSystem::GetInstance()->mCamera.UpdateCameraInfo();

	TextManager::GetInstance()->DialogueTextOffsetX = -850 - 232 + 55 - 58;
	TextManager::GetInstance()->DialogueTextOffsetY = -1250 - 343 - 110;

	mSceneScrollDelayTimer = mSceneScrollDelayTime;
	mChangeSceneDelayTimer = mChangeSceneDelayTime;

	WindowsManager::GetInstance()->mSceneOpacity = 1.0f;

	GraphicsSystem::GetInstance()->mParticleBloom = 0.0f;
	GraphicsSystem::GetInstance()->mParticleAlpha = 0.0f;
	GraphicsSystem::GetInstance()->mParticleSpeed = 0.0f;

	std::string fulltext = "";
	fulltext.append("\n");
	fulltext.append("\n");
	fulltext.append("\n");
	fulltext.append("\n");
	fulltext.append("\nPRODUCER & PROGRAMMER");
	fulltext.append("\nSim Mong Chuan");
	fulltext.append("\n");
	fulltext.append("\n");
	fulltext.append("\nGRAPHICS PROGRAMMER");
	fulltext.append("\nCai Wenjin");
	fulltext.append("\n");
	fulltext.append("\n");
	fulltext.append("\nTECHNICAL LEAD & ENGINE PROGRAMMER");
	fulltext.append("\nTeo Wen Xi");
	fulltext.append("\n");
	fulltext.append("\n");
	fulltext.append("\nLEVEL PROGRAMMER");
	fulltext.append("\nHear Li Heng");
	fulltext.append("\n");
	fulltext.append("\n");
	fulltext.append("\nPHYSICS PROGRAMMER");
	fulltext.append("\nSia Xin Tian");
	fulltext.append("\n");
	fulltext.append("\n");
	fulltext.append("\nGAME PROGRAMMER");
	fulltext.append("\nTan Kang Zheng");
	fulltext.append("\n");
	fulltext.append("\n");
	fulltext.append("\nVFX ARTIST & DESIGNER");
	fulltext.append("\nLow Jovan Zhuo Wen");
	fulltext.append("\n");
	fulltext.append("\n");
	fulltext.append("\nDESIGN LEAD & ART LEAD");
	fulltext.append("\nAUDIO LEAD & VOICE OF AXIOM");
	fulltext.append("\nWong Weng Kit Zephan");
	fulltext.append("\n");
	fulltext.append("\n");

	fulltext.append("\n");
	fulltext.append("\n");
	fulltext.append("\n");
	fulltext.append("\n");

	fulltext.append("\nPlaytesters");
	fulltext.append("\n");
	fulltext.append("\nAlwyn Lee");
	fulltext.append("\nJing Ying Goh");
	fulltext.append("\nKok Timothy");
	fulltext.append("\nKwok Wayne");
	fulltext.append("\nLeong Aixian");
	fulltext.append("\nLim Isaiah");
	fulltext.append("\nMalcolm");
	fulltext.append("\nTham Tristan");
	fulltext.append("\nTheong Nichola");
	fulltext.append("\nWayne Kwok");
	fulltext.append("\n");
	fulltext.append("\n");

	fulltext.append("\n");
	fulltext.append("\n");
	fulltext.append("\n");
	fulltext.append("\n");

	fulltext.append("\nFaculty and Advisors");
	fulltext.append("\n");
	fulltext.append("\nTech");
	fulltext.append("\nElie HOSRY");
	fulltext.append("\nDian Yang GOH");
	fulltext.append("\n");
	fulltext.append("\n");
	fulltext.append("\nDesign and Production");
	fulltext.append("\nAlwyn LEE");
	fulltext.append("\nJing Ying GOH");
	fulltext.append("\nHolger LIEBNITZ");
	fulltext.append("\nDanielle FONG");
	fulltext.append("\n");
	fulltext.append("\n");
	fulltext.append("\nArt");
	fulltext.append("\nChoon Wee KEH");
	fulltext.append("\n");
	fulltext.append("\n");
	fulltext.append("\nAudio");
	fulltext.append("\nVuk KRAKOVIC");
	fulltext.append("\nGavin PARKER");
	fulltext.append("\n");
	fulltext.append("\n");
	fulltext.append("\n");
	fulltext.append("\n");
	fulltext.append("\n");
	fulltext.append("\n");
	fulltext.append("\n");
	fulltext.append("\n");
	fulltext.append("\nCreated at");
	fulltext.append("\nDigiPen Institute of Technology Singapore");
	fulltext.append("\n");
	fulltext.append("\n");
	fulltext.append("\nPRESIDENT");
	fulltext.append("\nClaude Comair");
	fulltext.append("\n");
	fulltext.append("\n");
	fulltext.append("\nEXECUTIVES");
	fulltext.append("\nJason Chu");
	fulltext.append("\nChristopher Comair");
	fulltext.append("\nMichael Gats");
	fulltext.append("\nMichele Comair");
	fulltext.append("\nRaymond Yan");
	fulltext.append("\nSamir Abou Samra");
	fulltext.append("\nPrasanna Ghali");
	fulltext.append("\n");
	fulltext.append("\n");

	//

	fulltext.append("\n");//copyright text for digipen
	fulltext.append("\n");
	fulltext.append("\n");
	fulltext.append("\n");
	fulltext.append("\n");

	fulltext.append("\n"); //DigiPen Logo
	fulltext.append("\n");
	fulltext.append("\n");
	fulltext.append("\n");
	fulltext.append("\n");
	fulltext.append("\n");

	fulltext.append("\n"); //OpenGL
	fulltext.append("\n");
	fulltext.append("\n");
	fulltext.append("\n");
	fulltext.append("\n");
	fulltext.append("\n");

	fulltext.append("\n"); //FMOD
	fulltext.append("\n");
	fulltext.append("\n");
	fulltext.append("\n");
	fulltext.append("\n");
	fulltext.append("\n");

	fulltext.append("\n"); //Krita
	fulltext.append("\n");
	fulltext.append("\n");
	fulltext.append("\n");
	fulltext.append("\n");
	fulltext.append("\n");


	//

	fulltext.append("\n");
	fulltext.append("\n");
	fulltext.append("\n");
	fulltext.append("\n");
	fulltext.append("\nSpecial Thanks to our friends, family, and lecturers for their continued support!");
	fulltext.append("\n");
	fulltext.append("\nThank you for playing our game!");
	fulltext.append("\n");
	fulltext.append("\n");
	fulltext.append("\n");
	fulltext.append("\n");
	fulltext.append("\n");
	fulltext.append("\n");
	fulltext.append("\n");
	fulltext.append("\n");
	fulltext.append("\n");
	fulltext.append("\n");

	TextManager::GetInstance()->SetDialogueText(fulltext);

	GraphicsSystem::GetInstance()->mCamera.mCurrCamInfo.mPosition.y = 0;
	GraphicsSystem::GetInstance()->mCamera.mCurrCamInfo.mPosition.x = 0;

	ent_SideImageA = EntityManager::GetInstance()->CreateEntity();
	ent_SideImageA->SetName("SideImageA");
	Transform newXform{};
	newXform.SetPosition(GraphicsSystem::GetInstance()->mCamera.mCurrCamInfo.mPosition + Vector3(-120.0f, 0, -0.1f));
	newXform.mScale = Vector3{ 80, 80, 1 };
	ent_SideImageA->AddComponent(newXform);
	Renderer* _renderer = ent_SideImageA->AddComponent<Renderer>();
	_renderer->mModelType = Renderer::MODEL_TYPE::SPRITE;
	_renderer->mShaderName = "BasicShader";
	_renderer->mTextureName = "Credits_0";
	_renderer->mEnabled = true;
	_renderer->mRenderLayer = Renderer::RENDER_LAYER::ABOVETILES;
	_renderer->mCustomAlphaEnabled = true;
	_renderer->mAlphaValue = 0.0f; //0.7f
	ent_SideImageA->SetSaveInScene(false);


	ent_SideImageB = EntityManager::GetInstance()->CreateEntity();
	ent_SideImageB->SetName("SideImageB");
	newXform.SetPosition(GraphicsSystem::GetInstance()->mCamera.mCurrCamInfo.mPosition + Vector3(120.0f, 0, -0.1f));
	newXform.mScale = Vector3{ 80, 80, 1 };
	ent_SideImageB->AddComponent(newXform);
	_renderer = ent_SideImageB->AddComponent<Renderer>();
	_renderer->mModelType = Renderer::MODEL_TYPE::SPRITE;
	_renderer->mShaderName = "BasicShader";
	_renderer->mTextureName = "Credits_1";
	_renderer->mEnabled = true;
	_renderer->mRenderLayer = Renderer::RENDER_LAYER::ABOVETILES;
	_renderer->mCustomAlphaEnabled = true;
	_renderer->mAlphaValue = 0.0f;
	ent_SideImageB->SetSaveInScene(false);
}

void CreditsScene::Load()
{
	LevelLoader::LoadLevel(SceneManager::DemangleSceneName(typeid(CreditsScene).name()), GraphicsSystem::GetInstance()->mBackgroundColor);
}

void CreditsScene::Input(double _dt)
{
	UNREFERENCED_PARAMETER(_dt);
	if (InputManager::GetInstance()->GetActionTriggered("MenuPause") && (CoreEngine::GetInstance()->mGameState == CoreEngine::GameState::GS_PLAY))
	{
		CoreEngine::GetInstance()->mGameState = CoreEngine::GameState::GS_PAUSE;
		EventManager::GetInstance()->AddEvent(EventPlayPause(EventPlayPause::PAUSE));
		LevelLoader::LoadPrefab("PauseMenu Prefab");
	}
}

void CreditsScene::Update(double _dt)
{
	UNREFERENCED_PARAMETER(_dt);

	deathCounterTimer = std::clamp(deathCounterTimer - (float)_dt, 0.0f, 999.0f);
	float randomNumberPower;
	int randomNumber;
	switch (showDeathCounterState)
	{
	case 0:
		EntityManager::GetInstance()->GetEntity(TitleID)->GetComponent<GameUITextHandler>()->mAlpha = 1.0f - std::clamp((deathCounterTimer - 0.66f * deathCounterTime_0) / (0.34f * deathCounterTime_0), 0.0f, 1.0f);
		randomNumberPower = 1;
		if (deathCounterTimer >= 0.66f * deathCounterTime_0) randomNumberPower = 3;
		else if (deathCounterTimer >= 0.33f * deathCounterTime_0) randomNumberPower = 2;
		randomNumber = (int)(rand() % (int)(9 * powf(10, randomNumberPower) - 2) + 1 * powf(10, randomNumberPower) + 1); //100 ~ 999
		EntityManager::GetInstance()->GetEntity(TitleID)->GetComponent<GameUITextHandler>()->mText = "Total Death Count: " + std::to_string(randomNumber);
		if (deathCounterTimer <= FLT_EPSILON)
		{
			EntityManager::GetInstance()->GetEntity(TitleID)->GetComponent<GameUITextHandler>()->mText = "Total Death Count: " + std::to_string(SceneManager::GetInstance()->totalDeathCount);
			showDeathCounterState = 1;
			deathCounterTimer = deathCounterTime_1;
		}
		break;
	case 1:
		if (deathCounterTimer <= FLT_EPSILON)
		{
			showDeathCounterState = 2;
			deathCounterTimer = deathCounterTime_2;
		}
		break;
	case 2: 
		WindowsManager::GetInstance()->mSceneOpacity = 0.0f;
		mChangeSceneDelayTimer = std::clamp(mChangeSceneDelayTimer - (float)_dt, 0.0f, 10.f);
		if (mChangeSceneDelayTimer <= FLT_EPSILON && deathCounterTimer <= FLT_EPSILON)
		{
			GoToMainMenuScene();
		}
		break;
	}


	float thevalue = 1250.0f + 343.0f + 110.0f;
	GraphicsSystem::GetInstance()->mCamera.mCurrCamInfo.mPosition.y = -thevalue - TextManager::GetInstance()->DialogueTextOffsetY;

	float alphaimage = 1.0f;
	float progress = fabsf(GraphicsSystem::GetInstance()->mCamera.mCurrCamInfo.mPosition.y) / thevalue;
	if (0.04f <= progress && progress <= 0.12f)
	{
		ent_SideImageA->GetComponent<Renderer>()->mTextureName = "Credits_0";
		ent_SideImageA->GetComponent<Renderer>()->mAlphaValue = alphaimage * std::clamp(2.0f - fabsf(0.08f - progress) / (0.02f), 0.0f, 1.0f);

		ent_SideImageB->GetComponent<Renderer>()->mAlphaValue = 0.0f;
	}
	else if (0.14f <= progress && progress <= 0.22f)
	{
		ent_SideImageB->GetComponent<Renderer>()->mTextureName = "Credits_4";
		ent_SideImageB->GetComponent<Renderer>()->mAlphaValue = alphaimage * std::clamp(2.0f - fabsf(0.18f - progress) / (0.02f), 0.0f, 1.0f);

		ent_SideImageA->GetComponent<Renderer>()->mAlphaValue = 0.0f;
	}
	else if (0.24f <= progress && progress <= 0.32f)
	{
		ent_SideImageA->GetComponent<Renderer>()->mTextureName = "Credits_2";
		ent_SideImageA->GetComponent<Renderer>()->mAlphaValue = alphaimage * std::clamp(2.0f - fabsf(0.28f - progress) / (0.02f), 0.0f, 1.0f);

		ent_SideImageB->GetComponent<Renderer>()->mAlphaValue = 0.0f;
	}
	else if (0.34f <= progress && progress <= 0.42f)
	{
		ent_SideImageB->GetComponent<Renderer>()->mTextureName = "Credits_3";
		ent_SideImageB->GetComponent<Renderer>()->mAlphaValue = alphaimage * std::clamp(2.0f - fabsf(0.38f - progress) / (0.02f), 0.0f, 1.0f);

		ent_SideImageA->GetComponent<Renderer>()->mAlphaValue = 0.0f;
	}
	else if (0.44f <= progress && progress <= 0.52f)
	{
		ent_SideImageA->GetComponent<Renderer>()->mTextureName = "Credits_6";
		ent_SideImageA->GetComponent<Renderer>()->mAlphaValue = alphaimage * std::clamp(2.0f - fabsf(0.48f - progress) / (0.02f), 0.0f, 1.0f);

		ent_SideImageB->GetComponent<Renderer>()->mAlphaValue = 0.0f;
	}
	else if (0.54f <= progress && progress <= 0.62f)
	{
		ent_SideImageB->GetComponent<Renderer>()->mTextureName = "Credits_5";
		ent_SideImageB->GetComponent<Renderer>()->mAlphaValue = alphaimage * std::clamp(2.0f - fabsf(0.58f - progress) / (0.02f), 0.0f, 1.0f);

		ent_SideImageA->GetComponent<Renderer>()->mAlphaValue = 0.0f;
	}
	else if (0.64f <= progress && progress <= 0.72f)
	{
		ent_SideImageA->GetComponent<Renderer>()->mTextureName = "Credits_7";
		ent_SideImageA->GetComponent<Renderer>()->mAlphaValue = alphaimage * std::clamp(2.0f - fabsf(0.68f - progress) / (0.02f), 0.0f, 1.0f);

		ent_SideImageB->GetComponent<Renderer>()->mAlphaValue = 0.0f;
	}
	else if (0.74f <= progress && progress <= 0.82f)
	{
		ent_SideImageB->GetComponent<Renderer>()->mTextureName = "Credits_1";
		ent_SideImageB->GetComponent<Renderer>()->mAlphaValue = alphaimage * std::clamp(2.0f - fabsf(0.78f - progress) / (0.02f), 0.0f, 1.0f);

		ent_SideImageA->GetComponent<Renderer>()->mAlphaValue = 0.0f;
	}
	else
	{
		ent_SideImageA->GetComponent<Renderer>()->mAlphaValue = 0.0f;
		ent_SideImageB->GetComponent<Renderer>()->mAlphaValue = 0.0f;
	}

	if (mSceneScrollDelayTimer < FLT_EPSILON)
	{
		if (InputManager::GetInstance()->GetAction("CheatFastforward"))
			TextManager::GetInstance()->DialogueTextOffsetY = std::clamp(TextManager::GetInstance()->DialogueTextOffsetY + 250.0f * (float)_dt, -2000.0f, 30.f);
		else
			TextManager::GetInstance()->DialogueTextOffsetY = std::clamp(TextManager::GetInstance()->DialogueTextOffsetY + 20.f * (float)_dt, -2000.0f, 30.f);
	}
	else
	{
		if (InputManager::GetInstance()->GetAction("CheatFastforward"))
			mSceneScrollDelayTimer = std::clamp(mSceneScrollDelayTimer - 10.0f * (float)_dt, 0.0f, mSceneScrollDelayTime);
		else
			mSceneScrollDelayTimer = std::clamp(mSceneScrollDelayTimer - (float)_dt, 0.0f, mSceneScrollDelayTime);

		if (mSceneScrollDelayTimer < FLT_EPSILON)
		{
			for (size_t i{}; i < EntityManager::GetInstance()->mEntityList.size(); ++i)
			{
				if ("Sprite Prefab" == EntityManager::GetInstance()->mEntityList[i].GetName() && EntityManager::GetInstance()->mEntityList[i].GetComponent<SoundComponent>() != nullptr)
				{
					EntityManager::GetInstance()->mEntityList[i].GetComponent<SoundComponent>()->Play();
					break;
				}
			}
		}
	}

	if (showDeathCounterState == -1 && TextManager::GetInstance()->DialogueTextOffsetY >= 30.0f - FLT_EPSILON)
	{
		showDeathCounterState = 0;
		deathCounterTimer = deathCounterTime_0;
		for (size_t i{}; i < EntityManager::GetInstance()->mEntityList.size(); ++i)
		{
			if ("Title" == EntityManager::GetInstance()->mEntityList[i].GetName())
			{
				TitleID = EntityManager::GetInstance()->mEntityList[i].GetID();

				EntityManager::GetInstance()->mEntityList[i].GetComponent<Transform>()->SetPosition(Vector3(-75, -1300.0f - 343.0f - 110.0f, 0));
				EntityManager::GetInstance()->mEntityList[i].GetComponent<GameUITextHandler>()->mAlpha = 0;
				EntityManager::GetInstance()->mEntityList[i].GetComponent<GameUITextHandler>()->mFontSize = 16;
				EntityManager::GetInstance()->mEntityList[i].GetComponent<GameUITextHandler>()->mText = "Total Death Count: 9999";

				break;
			}
		}
		//WindowsManager::GetInstance()->mSceneOpacity = 0.0f;
		//mChangeSceneDelayTimer = std::clamp(mChangeSceneDelayTimer - (float)_dt, 0.0f, 10.f);
		//if (mChangeSceneDelayTimer <= FLT_EPSILON)
		//{
		//	GoToMainMenuScene();
		//}
	}

	ent_SideImageA->GetComponent<Transform>()->SetPosition(GraphicsSystem::GetInstance()->mCamera.mCurrCamInfo.mPosition + Vector3(-120.0f, 0, -0.1f));
	ent_SideImageB->GetComponent<Transform>()->SetPosition(GraphicsSystem::GetInstance()->mCamera.mCurrCamInfo.mPosition + Vector3(120.0f, 0, -0.1f));

}

void CreditsScene::Pause(double _dt)
{
	UNREFERENCED_PARAMETER(_dt);
}

void CreditsScene::Render(double _dt)
{
	UNREFERENCED_PARAMETER(_dt);
}

void CreditsScene::Free()
{

}

void CreditsScene::Exit()
{
	SoundManager::GetInstance()->StopChannel(SoundManager::SoundType_SFX);
	SoundManager::GetInstance()->StopChannel(SoundManager::SoundType_BGM);
	TextManager::GetInstance()->DialogueTextOffsetX = 0;
	TextManager::GetInstance()->DialogueTextOffsetY = 0;
}

void CreditsScene::GoToMainMenuScene()
{
#if _ENGINE
	DebugLogger::GetInstance()->Log("Attempting to Load Scene: %s\n", mScene.c_str());
#endif // _ENGINE

	TextManager::GetInstance()->SetOpeningDialogueText("", fontName);
	SceneManager::GetInstance()->SetActiveScene(SceneManager::GetSceneFromName(mScene));
	TextManager::GetInstance()->DialogueTextOffsetX = 0;
	TextManager::GetInstance()->DialogueTextOffsetY = 0;
}