/*!
All content � 2023 DigiPen Institute of Technology Singapore, all rights reserved.
@file       DialogueTrigger.cpp
@author     TAN Kang Zheng (100%)
@co-author

----------------------------------------------------------------------------

@course     CSD2401F23
@section    CSD2401F23-A
@project    CSD2401F23 Software Engineering Project 3

@brief      Component to play dialogue if the player reaches a certain area
*//*______________________________________________________________________*/

#include "DialogueTrigger.h"

#include "Collision.h"
#include "Transform.h"
#include "EntityManager.h"
#include "Player.h"
#include "LevelLoader.h"
#include "GameUIManager.h"
#include "CinematicBars.h"
#include "AIManager.h"
#include <string.h>
#include "LevelManager.h"

//Rand
#include <time.h>
#include <stdlib.h>

void DialogueTrigger::Init()
{
	GameUIManager::GetInstance()->AddDialogueTrigger(mEntID);
	//mTriggerType = TriggerType::PROXIMITY;
	if (mTriggers.size() < 4)
	{
		for (int i = 0; i < 4; ++i)
			mTriggers.push_back(std::pair<std::string, std::string>("", ""));
	}
}

void DialogueTrigger::Load()
{
	mTriggerType = (TriggerType)(JSONManager::GetInstance()->GetInt("triggerType"));
	mTriggers.clear();
	std::stringstream loadedTriggers = std::stringstream(JSONManager::GetInstance()->GetString("triggers"));
	//TODO: Seperate string based on | per pair, and + between dialogue and audio
	std::string triggerSeperated = "";
	while (std::getline(loadedTriggers, triggerSeperated, '|'))
	{
		std::pair<std::string, std::string> triggerPair = ParsePair(triggerSeperated);
		mTriggers.push_back(triggerPair);
	}

	//Nothing was loaded (empty vector is bad)
	if (mTriggers.size() < 4)
	{
		for (int i = 0; i < 4; ++i)
			mTriggers.push_back(std::pair<std::string, std::string>("", ""));
	}
	mDeathThreshold = JSONManager::GetInstance()->GetInt("deathThreshold");
	mAudioDucking = JSONManager::GetInstance()->GetFloat("audioDucking");

	//For cutscene
	mCutsceneDuration = JSONManager::GetInstance()->GetFloat("CutsceneDuration");
	mCinematicBarExpandingDuration = JSONManager::GetInstance()->GetFloat("CinematicBarExpandingDuration");
	mDisablePlayerGravity = JSONManager::GetInstance()->GetFloat("DisablePlayerGravity");
	mDelayPlayerRespawnTimer = JSONManager::GetInstance()->GetFloat("DelayPlayerRespawnTimer");
	mVerticalBarsOnly = (bool)JSONManager::GetInstance()->GetInt("VerticalBarsOnly");
	mCutSceneNum = JSONManager::GetInstance()->GetInt("CutSceneNum");
	mFadeTimer = JSONManager::GetInstance()->GetFloat("FadeTimer");
}

std::pair<std::string, std::string> DialogueTrigger::ParsePair(std::string _input)
{
	size_t seperator = _input.find('+');

	//If fail to find seperator, just return an empty pair
	if (seperator == std::string::npos)
		return std::pair<std::string, std::string>("", "");

	std::pair<std::string, std::string> returnPair;
	
	//Check if there is a dialogue saved
	if (seperator != 0)
		returnPair.first = _input.substr(0, seperator);

	//Check if there is a audio file saved
	if ((_input.length() - seperator) > 1)
		returnPair.second = _input.substr(seperator+1, _input.length());

	return returnPair;
}

void DialogueTrigger::Update()
{
#if _BLOATWARE
	//If running in game mode, disable all the renderers if it is a proximity trigger
	if (mTriggerType == TriggerType::PROXIMITY)
	{
		Renderer* renderer = EntityManager::GetInstance()->GetEntity(mEntID)->GetComponent<Renderer>();
		if ((renderer != nullptr) && renderer->mEnabled)
			renderer->mEnabled = false;
	}
#endif
	if (CoreEngine::GetInstance()->mGameState != CoreEngine::GameState::GS_PLAY)
		return;

	switch (mTriggerType)
	{
	case TriggerType::PROXIMITY:
	{
		if (mTriggered)
			return;

		Transform* transform = EntityManager::GetInstance()->GetEntity(mEntID)->GetComponent<Transform>();
		Vec3 playerPos = EntityManager::GetInstance()->GetEntity(Player::mPlayerID)->GetComponent<Transform>()->GetPosition();

		if (transform == nullptr)
			return;

		if (Collision::CollisionStaticAABB(playerPos, *transform))
		{
			mTriggered = true;
			PlayTrigger();
		}
	}
		break;
	case TriggerType::RANDOM:
		break;
	case TriggerType::PROXIMITY_DEATH:
	{
		if (mTriggered)
			return;

		Transform* transform = EntityManager::GetInstance()->GetEntity(mEntID)->GetComponent<Transform>();
		Vec3 playerPos = EntityManager::GetInstance()->GetEntity(Player::mPlayerID)->GetComponent<Transform>()->GetPosition();

		if (transform == nullptr)
			return;

		if (Collision::CollisionStaticAABB(playerPos, *transform))
		{
			Player* player = EntityManager::GetInstance()->GetEntity(Player::mPlayerID)->GetComponent<Player>();
			if (player != nullptr)
			{
				if ((int)player->GetCurrentLevelDeaths() >= mDeathThreshold)
				{
					mTriggered = true;
					return;
				}
			}
			mTriggered = true;
			PlayTrigger();
		}
	}
		break;
	case TriggerType::CUTSCENE:
	{
		if (mTriggered)
		{
			mCutsceneTextTimer += (float)WindowsManager::GetInstance()->mDeltaTime;

			switch (mCutSceneNum)
			{
			case 1:
				{
					//What?! How are you doing that
					if (mCutsceneTextTimer > 4.f && mCutscenePart2Triggered == false)
					{
						AIManager::GetInstance()->InitiateTalking(mTriggers[1].first, mTriggers[1].second);
						SoundComponent* _soundComp = EntityManager::GetInstance()->GetEntity(mEntID)->GetComponent<SoundComponent>();
						if (_soundComp != nullptr)
						{
							_soundComp->Play();
						}
						mCutscenePart2Triggered = true;
					}
					//No matter. This is even better than I intended.
					else if (mCutsceneTextTimer > 7.5f && mCutscenePart3Triggered == false)
					{
						AIManager::GetInstance()->InitiateTalking(mTriggers[2].first, mTriggers[2].second);
						mCutscenePart3Triggered = true;
					}
					//Now, I get to watch you suffer forever
					else if (mCutsceneTextTimer > 11.f && mCutscenePart4Triggered == false)
					{
						AIManager::GetInstance()->InitiateTalking(mTriggers[3].first, mTriggers[3].second);
						mCutscenePart4Triggered = true;
					}
				}
				break;
			case 2:
				{
					//Move player to the right
					if (mCutsceneTextTimer > 4.f)
					{
						Entity* player = EntityManager::GetInstance()->GetEntity(Player::mPlayerID);
						if (player != nullptr)
						{
							player->GetComponent<Player>()->MoveRight();
						}
					}
				}
				break;
			case 3:
				{
					//Well enough games, now you’ll face a REAL challenge
					if (mCutsceneTextTimer > 4.f && mCutscenePart2Triggered == false)
					{
						AIManager::GetInstance()->InitiateTalking(mTriggers[1].first, mTriggers[1].second);
						
						mCutscenePart2Triggered = true;
					}
					//Enable boss
					else if (mCutsceneTextTimer > 10.5f && mCutscenePart3Triggered == false)
					{
						AIManager::GetInstance()->EnableAIBossMode();
						mCutscenePart3Triggered = true;
					}
					//Start bgm
					else if (mCutsceneTextTimer > 15.f && mCutscenePart4Triggered == false)
					{
						SoundComponent* _soundComp = EntityManager::GetInstance()->GetEntity(mEntID)->GetComponent<SoundComponent>();
						if (_soundComp != nullptr)
							_soundComp->Play();

						mCutscenePart4Triggered = true;
					}
				}
				break;
			case 4:
				{
					//Cutscene lerping
					if (mCutsceneTextTimer < mCutsceneDuration)
					{
						//Slow down player movespeed
						EntityManager::GetInstance()->GetEntity(Player::mPlayerID)->GetComponent<Player>()->mMoveSpeedMultiplier -= mPlayerMsStep * (float)WindowsManager::GetInstance()->mDeltaTime;
					
						//Shader stuff
						GraphicsSystem::GetInstance()->mParticleAlpha += mEndShaderAlpha * (float)WindowsManager::GetInstance()->mDeltaTime;
						GraphicsSystem::GetInstance()->mParticleSpeed += mEndShaderSpeed * (float)WindowsManager::GetInstance()->mDeltaTime;
						GraphicsSystem::GetInstance()->mParticleBloom += mEndShaderBloom * (float)WindowsManager::GetInstance()->mDeltaTime;

						if (EntityManager::GetInstance()->GetEntity(Player::mPlayerID)->GetComponent<Player>()->mMoveSpeedMultiplier < 0.1f)
							EntityManager::GetInstance()->GetEntity(Player::mPlayerID)->GetComponent<Player>()->mMoveSpeedMultiplier = 0.1f;

						if (GraphicsSystem::GetInstance()->mParticleAlpha > 0.5f)
							GraphicsSystem::GetInstance()->mParticleAlpha = 0.5f;
						if (GraphicsSystem::GetInstance()->mParticleSpeed > 1.f)
							GraphicsSystem::GetInstance()->mParticleSpeed = 1.f;
						if (GraphicsSystem::GetInstance()->mParticleBloom > 1.f)
							GraphicsSystem::GetInstance()->mParticleBloom = 1.f;

						EntityManager::GetInstance()->GetEntity(Player::mPlayerID)->GetComponent<Player>()->MoveRight();
						SoundManager::GetInstance()->SetBGMDucking(1.0f);
					}

					if (mCutscenePart2Triggered == false)
					{
						SoundManager::GetInstance()->StopChannel(SoundManager::SoundType_BGM);
						AIManager::GetInstance()->DisableAIBossMode();
						LevelManager::GetInstance()->DoPermanentScreenshake();

						//Set step values
						mPlayerMsStep = (0.23f - 0.1f) / mCutsceneDuration;
						EntityManager::GetInstance()->GetEntity(Player::mPlayerID)->GetComponent<Player>()->mMoveSpeedMultiplier = 0.23f;

						mEndShaderAlpha = 0.5f / mCutsceneDuration;
						mEndShaderSpeed = 1.f / mCutsceneDuration;
						mEndShaderBloom = 1.f / (mCutsceneDuration - 1.f);

						mCutscenePart2Triggered = true;
					}

					if (mCutsceneTextTimer > 0.65f && mCutscenePart3Triggered == false)
					{
						SoundComponent* _soundComp = EntityManager::GetInstance()->GetEntity(mEntID)->GetComponent<SoundComponent>();
						if (_soundComp != nullptr)
							_soundComp->Play();

						mCutscenePart3Triggered = true;
					}

					if (mCutsceneTextTimer > mCutsceneDuration)
					{
						SceneManager::GetInstance()->SetActiveScene(SceneManager::SCENE_CREDITS);
						SoundManager::GetInstance()->StopChannel(SoundManager::SoundType_BGM);
					}

					if (mCutsceneTextTimer >= mCutsceneDuration - 1.0f)
					{
						WindowsManager::GetInstance()->mSceneOpacity = 0.0f;
					}
				}
				break;
			}
			
			return;
		}

		Transform* transform = EntityManager::GetInstance()->GetEntity(mEntID)->GetComponent<Transform>();
		Vec3 playerPos = EntityManager::GetInstance()->GetEntity(Player::mPlayerID)->GetComponent<Transform>()->GetPosition();

		if (transform == nullptr)
			return;

		if (Collision::CollisionStaticAABB(playerPos, *transform))
		{
			mTriggered = true;
			PlayTrigger();
		}
	}
		break;
	case TriggerType::PROXIMITY_IMAGE:
	{
		Transform* transform = EntityManager::GetInstance()->GetEntity(mEntID)->GetComponent<Transform>();
		Vec3 playerPos = EntityManager::GetInstance()->GetEntity(Player::mPlayerID)->GetComponent<Transform>()->GetPosition();

		if (transform == nullptr)
			return;

		mTriggered = Collision::CollisionStaticAABB(playerPos, *transform);
		PlayTrigger();
	}
		break;
	default:
		break;
	}
}

void DialogueTrigger::PlayTrigger()
{
	switch (mTriggerType)
	{
	case TriggerType::PROXIMITY:
		AIManager::GetInstance()->SetAudioDucking(mAudioDucking);
		AIManager::GetInstance()->InitiateTalking(mTriggers[0].first, mTriggers[0].second);
		break;
	case TriggerType::RANDOM:
	{
		srand((unsigned int)time(NULL));
		int randOutcome = rand() % mTriggers.size(); //Generate a random number between 0 and (size - 1)
		AIManager::GetInstance()->SetAudioDucking(mAudioDucking);
		AIManager::GetInstance()->InitiateTalking(mTriggers[randOutcome].first, mTriggers[randOutcome].second);
	}
		break;
	case TriggerType::PROXIMITY_DEATH:
	{
		srand((unsigned int)time(NULL));
		int randOutcome = rand() % mTriggers.size(); //Generate a random number between 0 and (size - 1)
		AIManager::GetInstance()->SetAudioDucking(mAudioDucking);
		AIManager::GetInstance()->InitiateTalking(mTriggers[randOutcome].first, mTriggers[randOutcome].second);
	}
		break;
	case TriggerType::CUTSCENE:
	{
		Player* player = EntityManager::GetInstance()->GetEntity(Player::mPlayerID)->GetComponent<Player>();
		if (player != nullptr)
		{
			player->mCutsceneTimer = mCutsceneDuration;
			player->mDisableGravityTimer = mDisablePlayerGravity;
			player->mDelayRespawnTimer = mDelayPlayerRespawnTimer;
			player->ResetHorizontalVel();

			if(mCutSceneNum == 1)
				player->Kill();
		}

		//Spawn cutscene objects
		if (mVerticalBarsOnly == false)
		{
			CinematicBars::CreateCinematicBars(CinematicBars::LEFT, mCinematicBarExpandingDuration, mCutsceneDuration - mCinematicBarExpandingDuration * 2);
			CinematicBars::CreateCinematicBars(CinematicBars::RIGHT, mCinematicBarExpandingDuration, mCutsceneDuration - mCinematicBarExpandingDuration * 2);
		}

		CinematicBars::CreateCinematicBars(CinematicBars::UP, mCinematicBarExpandingDuration, mCutsceneDuration - mCinematicBarExpandingDuration * 2);
		CinematicBars::CreateCinematicBars(CinematicBars::DOWN, mCinematicBarExpandingDuration, mCutsceneDuration - mCinematicBarExpandingDuration * 2);

		AIManager::GetInstance()->SetAudioDucking(mAudioDucking);
		AIManager::GetInstance()->InitiateTalking(mTriggers[0].first, mTriggers[0].second);
		break;
	}
	case TriggerType::PROXIMITY_IMAGE:
	{
		std::vector<EntityID> childEnt = EntityManager::GetInstance()->GetEntity(mEntID)->GetChildEntList();
		if (childEnt.size() == 0) //childEnt list is empty
			return;

		//Calculate value to reduce by
		float changeVal = (float)(WindowsManager::GetInstance()->mDeltaTime / mFadeTimer);
		if (!mTriggered)
			changeVal *= -1;

		for (auto i : childEnt)
		{
			Renderer* childRenderer = EntityManager::GetInstance()->GetEntity(i)->GetComponent<Renderer>();
			if (childRenderer != nullptr)
			{
				childRenderer->mAlphaValue += changeVal;
				childRenderer->mAlphaValue = std::clamp(childRenderer->mAlphaValue, 0.0f, 1.0f);
			}

			GameUITextHandler* uiText = EntityManager::GetInstance()->GetEntity(i)->GetComponent<GameUITextHandler>();
			if (uiText != nullptr)
			{
				uiText->mAlpha += changeVal;
				uiText->mAlpha = std::clamp(uiText->mAlpha, 0.0f, 1.0f);
			}
		}
	}
		break;
	default:
		break;
	}
}

void DialogueTrigger::Save()
{
	JSONManager::GetInstance()->Save("Component", "DialogueTrigger");
	JSONManager::GetInstance()->Save("triggerType", (int)(mTriggerType));
	//concatenate all the text and audio together
	std::string toSave{};
	for (auto i : mTriggers)
	{
		toSave += i.first + "+" + i.second + "|";
	}
	JSONManager::GetInstance()->Save("triggers", toSave);
	JSONManager::GetInstance()->Save("deathThreshold", mDeathThreshold);
	JSONManager::GetInstance()->Save("audioDucking", mAudioDucking);

	JSONManager::GetInstance()->Save("CutsceneDuration", mCutsceneDuration);
	JSONManager::GetInstance()->Save("CinematicBarExpandingDuration", mCinematicBarExpandingDuration);
	JSONManager::GetInstance()->Save("VerticalBarsOnly", (int)mVerticalBarsOnly);
	JSONManager::GetInstance()->Save("DisablePlayerGravity", mDisablePlayerGravity);
	JSONManager::GetInstance()->Save("DelayPlayerRespawnTimer", mDelayPlayerRespawnTimer);
	JSONManager::GetInstance()->Save("CutSceneNum", mCutSceneNum);

	JSONManager::GetInstance()->Save("FadeTimer", mFadeTimer);
}

void DialogueTrigger::Exit()
{
	GameUIManager::GetInstance()->RemoveDialogueTrigger(mEntID);
}

void DialogueTrigger::DisplayProperties()
{
#if _ENGINE
	//Select Trigger Type
	const char* items[] = { "Proximity",
						"Random",
						"Proximity-Deaths",
						"Cutscene",
						"Proximity-Image"
	};
	const char* current_item = items[(int)mTriggerType];

	ImGui::Text("Trigger Type:");
	if (ImGui::BeginCombo("##Trigger Type", current_item)) // The second parameter is the label previewed before opening the combo.
	{
		for (int n = 0; n < IM_ARRAYSIZE(items); n++)
		{
			bool is_selected = (current_item == items[n]); // You can store your selection however you want, outside or inside your objects
			if (ImGui::Selectable(items[n], is_selected))
			{
				current_item = items[n];
				if (n != (int)mTriggerType) //If choosen button type is different
				{
					mTriggerType = TriggerType(n);
				}
			}
			if (is_selected)
				ImGui::SetItemDefaultFocus();   // You may set the initial focus when opening the combo (scrolling + for keyboard navigation support)
		}
		ImGui::EndCombo();
	}

	if (ImGui::Button("Activate Trigger"))
	{
		PlayTrigger();
	}

	switch (mTriggerType)
	{
	case TriggerType::PROXIMITY:
		//If the type is proximity (aka only once)
		MyImGui::MyImGuiStringInput(mTriggers[0].first, "Dialogue");
		ImGui::Text("Audio");
		ImGui::SameLine();
		{
			//Box to be able to drag and drop
			char buff[1024];
			strcpy(buff, mTriggers[0].second.c_str());
			ImDrawList* draw_list = ImGui::GetForegroundDrawList();
			//Draw uneditable input box
			ImGui::PushStyleVar(ImGuiStyleVar_DisabledAlpha, 1);
			ImGui::BeginDisabled();
			ImGui::SameLine(ImGui::GetWindowWidth() - 300); ImGui::SetNextItemWidth(250);
			ImGui::InputText("##audioName", buff, 1024, ImGuiInputTextFlags_ReadOnly);
			ImGui::EndDisabled();
			ImGui::PopStyleVar();
			ImRect inner_rect = ImGui::GetCurrentContext()->LastItemData.Rect;
			if (ImGui::BeginDragDropTargetCustom(inner_rect, ImGui::GetID("##audioame")))//drag drop for sprites
			{
				const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("AudioDrag", ImGuiDragDropFlags_AcceptBeforeDelivery);//need to check before delivery so we can draw the yellow box
				if (payload)
				{
					if (payload->IsPreview())
					{
						draw_list->AddRectFilled(inner_rect.Min, inner_rect.Max, ImGui::GetColorU32(ImGuiCol_DragDropTarget, 0.05f));//draw inner darker overlay
						draw_list->AddRect(inner_rect.Min, inner_rect.Max, ImGui::GetColorU32(ImGuiCol_DragDropTarget), 0.0f, 0, 2.0f);//draw the yellow outer box
					}
					if (payload->IsDelivery())
					{
						mTriggers[0].second = std::string(static_cast<const char*>(payload->Data), payload->DataSize);
					}
				}
				ImGui::EndDragDropTarget();
			}
		}
		MyImGui::MyImGuiFloatInput(mAudioDucking, "Audio Ducking");
		ImGui::Checkbox("Activated?", &mTriggered);
		break;
	case TriggerType::RANDOM:
		for (int i = 0; i < mTriggers.size(); i++)
		{
			//Trigger text
			ImGui::Text("Trigger #%i", i);
			MyImGui::MyImGuiStringInput(mTriggers[i].first, "Dialogue" + std::to_string(i));
			ImGui::Text("Audio");
			ImGui::SameLine();
			{
				//Box to be able to drag and drop
				char buff[1024];
				strcpy(buff, mTriggers[i].second.c_str());
				ImDrawList* draw_list = ImGui::GetForegroundDrawList();
				//Draw uneditable input box
				ImGui::PushStyleVar(ImGuiStyleVar_DisabledAlpha, 1);
				ImGui::BeginDisabled();
				ImGui::SameLine(ImGui::GetWindowWidth() - 300); ImGui::SetNextItemWidth(250);
				ImGui::InputText("##audioName", buff, 1024, ImGuiInputTextFlags_ReadOnly);
				ImGui::EndDisabled();
				ImGui::PopStyleVar();
				ImRect inner_rect = ImGui::GetCurrentContext()->LastItemData.Rect;
				if (ImGui::BeginDragDropTargetCustom(inner_rect, ImGui::GetID("##audioame")))//drag drop for sprites
				{
					const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("AudioDrag", ImGuiDragDropFlags_AcceptBeforeDelivery);//need to check before delivery so we can draw the yellow box
					if (payload)
					{
						if (payload->IsPreview())
						{
							draw_list->AddRectFilled(inner_rect.Min, inner_rect.Max, ImGui::GetColorU32(ImGuiCol_DragDropTarget, 0.05f));//draw inner darker overlay
							draw_list->AddRect(inner_rect.Min, inner_rect.Max, ImGui::GetColorU32(ImGuiCol_DragDropTarget), 0.0f, 0, 2.0f);//draw the yellow outer box
						}
						if (payload->IsDelivery())
						{
							mTriggers[i].second = std::string(static_cast<const char*>(payload->Data), payload->DataSize);
						}
					}
					ImGui::EndDragDropTarget();
				}
			}
			char deleteButtonStr[20] = "Delete Trigger #";
			strcat(deleteButtonStr, std::to_string(i).c_str());
			if (ImGui::Button(deleteButtonStr))
			{
				mTriggers.erase(mTriggers.begin() + i);
			}
		}
		if (ImGui::Button("Add Trigger"))
		{
			mTriggers.push_back(std::pair<std::string, std::string>("", ""));
		}
		MyImGui::MyImGuiFloatInput(mAudioDucking, "Audio Ducking");
		break;
	case TriggerType::PROXIMITY_DEATH:
		//If the type is proximity + death counter (aka only once)
		for (int i = 0; i < mTriggers.size(); i++)
		{
			//Trigger text
			ImGui::Text("Trigger #%i", i);
			MyImGui::MyImGuiStringInput(mTriggers[i].first, "Dialogue" + std::to_string(i));
			ImGui::Text("Audio");
			ImGui::SameLine();
			{
				//Box to be able to drag and drop
				char buff[1024];
				strcpy(buff, mTriggers[i].second.c_str());
				ImDrawList* draw_list = ImGui::GetForegroundDrawList();
				//Draw uneditable input box
				ImGui::PushStyleVar(ImGuiStyleVar_DisabledAlpha, 1);
				ImGui::BeginDisabled();
				ImGui::SameLine(ImGui::GetWindowWidth() - 300); ImGui::SetNextItemWidth(250);
				ImGui::InputText("##audioName", buff, 1024, ImGuiInputTextFlags_ReadOnly);
				ImGui::EndDisabled();
				ImGui::PopStyleVar();
				ImRect inner_rect = ImGui::GetCurrentContext()->LastItemData.Rect;
				if (ImGui::BeginDragDropTargetCustom(inner_rect, ImGui::GetID("##audioame")))//drag drop for sprites
				{
					const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("AudioDrag", ImGuiDragDropFlags_AcceptBeforeDelivery);//need to check before delivery so we can draw the yellow box
					if (payload)
					{
						if (payload->IsPreview())
						{
							draw_list->AddRectFilled(inner_rect.Min, inner_rect.Max, ImGui::GetColorU32(ImGuiCol_DragDropTarget, 0.05f));//draw inner darker overlay
							draw_list->AddRect(inner_rect.Min, inner_rect.Max, ImGui::GetColorU32(ImGuiCol_DragDropTarget), 0.0f, 0, 2.0f);//draw the yellow outer box
						}
						if (payload->IsDelivery())
						{
							mTriggers[i].second = std::string(static_cast<const char*>(payload->Data), payload->DataSize);
						}
					}
					ImGui::EndDragDropTarget();
				}
			}
			char deleteButtonStr[20] = "Delete Trigger #";
			strcat(deleteButtonStr, std::to_string(i).c_str());
			if (ImGui::Button(deleteButtonStr))
			{
				mTriggers.erase(mTriggers.begin() + i);
			}
		}
		if (ImGui::Button("Add Trigger"))
		{
			mTriggers.push_back(std::pair<std::string, std::string>("", ""));
		}
		MyImGui::MyImGuiIntInput(mDeathThreshold, "Death Threshold");
		MyImGui::MyImGuiFloatInput(mAudioDucking, "Audio Ducking");
		ImGui::Checkbox("Activated?", &mTriggered);
		break;
	case CUTSCENE:
		MyImGui::MyImGuiIntInput(mCutSceneNum, "Cutscene Number");
		MyImGui::MyImGuiFloatInput(mCutsceneDuration, "Cutscene duration");
		MyImGui::MyImGuiFloatInput(mCinematicBarExpandingDuration, "Cinematic bars expand duration");
		MyImGui::MyImGuiFloatInput(mDisablePlayerGravity, "Disable Player Gravity Duration");
		MyImGui::MyImGuiFloatInput(mDelayPlayerRespawnTimer, "Delay player respawn duration");
		ImGui::Checkbox("Vertical Bars only?", &mVerticalBarsOnly);

		//If the type is proximity (aka only once)
		{
			MyImGui::MyImGuiStringInput(mTriggers[0].first, "Dialogue1");
			ImGui::Text("Audio1");
			ImGui::SameLine();
			{
				//Box to be able to drag and drop
				char buff[1024];
				strcpy(buff, mTriggers[0].second.c_str());
				ImDrawList* draw_list = ImGui::GetForegroundDrawList();
				//Draw uneditable input box
				ImGui::PushStyleVar(ImGuiStyleVar_DisabledAlpha, 1);
				ImGui::BeginDisabled();
				ImGui::SameLine(ImGui::GetWindowWidth() - 300); ImGui::SetNextItemWidth(250);
				ImGui::InputText("##audioName1", buff, 1024, ImGuiInputTextFlags_ReadOnly);
				ImGui::EndDisabled();
				ImGui::PopStyleVar();
				ImRect inner_rect = ImGui::GetCurrentContext()->LastItemData.Rect;
				if (ImGui::BeginDragDropTargetCustom(inner_rect, ImGui::GetID("##audioame1")))//drag drop for sprites
				{
					const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("AudioDrag", ImGuiDragDropFlags_AcceptBeforeDelivery);//need to check before delivery so we can draw the yellow box
					if (payload)
					{
						if (payload->IsPreview())
						{
							draw_list->AddRectFilled(inner_rect.Min, inner_rect.Max, ImGui::GetColorU32(ImGuiCol_DragDropTarget, 0.05f));//draw inner darker overlay
							draw_list->AddRect(inner_rect.Min, inner_rect.Max, ImGui::GetColorU32(ImGuiCol_DragDropTarget), 0.0f, 0, 2.0f);//draw the yellow outer box
						}
						if (payload->IsDelivery())
						{
							mTriggers[0].second = std::string(static_cast<const char*>(payload->Data), payload->DataSize);
						}
					}
					ImGui::EndDragDropTarget();
				}
			}
		}
		{
			MyImGui::MyImGuiStringInput(mTriggers[1].first, "Dialogue2");
			ImGui::Text("Audio2");
			ImGui::SameLine();
			{
				//Box to be able to drag and drop
				char buff[1024];
				strcpy(buff, mTriggers[1].second.c_str());
				ImDrawList* draw_list = ImGui::GetForegroundDrawList();
				//Draw uneditable input box
				ImGui::PushStyleVar(ImGuiStyleVar_DisabledAlpha, 1);
				ImGui::BeginDisabled();
				ImGui::SameLine(ImGui::GetWindowWidth() - 300); ImGui::SetNextItemWidth(250);
				ImGui::InputText("##audioName2", buff, 1024, ImGuiInputTextFlags_ReadOnly);
				ImGui::EndDisabled();
				ImGui::PopStyleVar();
				ImRect inner_rect = ImGui::GetCurrentContext()->LastItemData.Rect;
				if (ImGui::BeginDragDropTargetCustom(inner_rect, ImGui::GetID("##audioame2")))//drag drop for sprites
				{
					const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("AudioDrag", ImGuiDragDropFlags_AcceptBeforeDelivery);//need to check before delivery so we can draw the yellow box
					if (payload)
					{
						if (payload->IsPreview())
						{
							draw_list->AddRectFilled(inner_rect.Min, inner_rect.Max, ImGui::GetColorU32(ImGuiCol_DragDropTarget, 0.05f));//draw inner darker overlay
							draw_list->AddRect(inner_rect.Min, inner_rect.Max, ImGui::GetColorU32(ImGuiCol_DragDropTarget), 0.0f, 0, 2.0f);//draw the yellow outer box
						}
						if (payload->IsDelivery())
						{
							mTriggers[1].second = std::string(static_cast<const char*>(payload->Data), payload->DataSize);
						}
					}
					ImGui::EndDragDropTarget();
				}
			}
		}
		{
			MyImGui::MyImGuiStringInput(mTriggers[2].first, "Dialogue3");
			ImGui::Text("Audio3");
			ImGui::SameLine();
			{
				//Box to be able to drag and drop
				char buff[1024];
				strcpy(buff, mTriggers[2].second.c_str());
				ImDrawList* draw_list = ImGui::GetForegroundDrawList();
				//Draw uneditable input box
				ImGui::PushStyleVar(ImGuiStyleVar_DisabledAlpha, 1);
				ImGui::BeginDisabled();
				ImGui::SameLine(ImGui::GetWindowWidth() - 300); ImGui::SetNextItemWidth(250);
				ImGui::InputText("##audioName3", buff, 1024, ImGuiInputTextFlags_ReadOnly);
				ImGui::EndDisabled();
				ImGui::PopStyleVar();
				ImRect inner_rect = ImGui::GetCurrentContext()->LastItemData.Rect;
				if (ImGui::BeginDragDropTargetCustom(inner_rect, ImGui::GetID("##audioame3")))//drag drop for sprites
				{
					const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("AudioDrag", ImGuiDragDropFlags_AcceptBeforeDelivery);//need to check before delivery so we can draw the yellow box
					if (payload)
					{
						if (payload->IsPreview())
						{
							draw_list->AddRectFilled(inner_rect.Min, inner_rect.Max, ImGui::GetColorU32(ImGuiCol_DragDropTarget, 0.05f));//draw inner darker overlay
							draw_list->AddRect(inner_rect.Min, inner_rect.Max, ImGui::GetColorU32(ImGuiCol_DragDropTarget), 0.0f, 0, 2.0f);//draw the yellow outer box
						}
						if (payload->IsDelivery())
						{
							mTriggers[2].second = std::string(static_cast<const char*>(payload->Data), payload->DataSize);
						}
					}
					ImGui::EndDragDropTarget();
				}
			}
		}
		{
			MyImGui::MyImGuiStringInput(mTriggers[3].first, "Dialogue4");
			ImGui::Text("Audio4");
			ImGui::SameLine();
			{
				//Box to be able to drag and drop
				char buff[1024];
				strcpy(buff, mTriggers[3].second.c_str());
				ImDrawList* draw_list = ImGui::GetForegroundDrawList();
				//Draw uneditable input box
				ImGui::PushStyleVar(ImGuiStyleVar_DisabledAlpha, 1);
				ImGui::BeginDisabled();
				ImGui::SameLine(ImGui::GetWindowWidth() - 300); ImGui::SetNextItemWidth(250);
				ImGui::InputText("##audioName4", buff, 1024, ImGuiInputTextFlags_ReadOnly);
				ImGui::EndDisabled();
				ImGui::PopStyleVar();
				ImRect inner_rect = ImGui::GetCurrentContext()->LastItemData.Rect;
				if (ImGui::BeginDragDropTargetCustom(inner_rect, ImGui::GetID("##audioame4")))//drag drop for sprites
				{
					const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("AudioDrag", ImGuiDragDropFlags_AcceptBeforeDelivery);//need to check before delivery so we can draw the yellow box
					if (payload)
					{
						if (payload->IsPreview())
						{
							draw_list->AddRectFilled(inner_rect.Min, inner_rect.Max, ImGui::GetColorU32(ImGuiCol_DragDropTarget, 0.05f));//draw inner darker overlay
							draw_list->AddRect(inner_rect.Min, inner_rect.Max, ImGui::GetColorU32(ImGuiCol_DragDropTarget), 0.0f, 0, 2.0f);//draw the yellow outer box
						}
						if (payload->IsDelivery())
						{
							mTriggers[3].second = std::string(static_cast<const char*>(payload->Data), payload->DataSize);
						}
					}
					ImGui::EndDragDropTarget();
				}
			}
		}
		MyImGui::MyImGuiFloatInput(mAudioDucking, "Audio Ducking");
		ImGui::Checkbox("Activated?", &mTriggered);
		break;
	case TriggerType::PROXIMITY_IMAGE:
	{
		MyImGui::MyImGuiFloatInput(mFadeTimer, "Fade Timer");
	}
		break;
	default:
		break;
	}
	
#endif // _ENGINE
};