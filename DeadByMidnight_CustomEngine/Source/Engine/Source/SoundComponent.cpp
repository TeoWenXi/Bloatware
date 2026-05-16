/*!
All content © 2023 DigiPen Institute of Technology Singapore, all rights reserved.
@file       SoundComponent.cpp
@author     TAN Kang Zheng (100%)
@co-author

----------------------------------------------------------------------------

@course     CSD2401F23
@section    CSD2401F23-A
@project    CSD2401F23 Software Engineering Project 3

@brief		Component for audio management for entities
*//*______________________________________________________________________*/

#include "SoundManager.h"
#include "SoundComponent.h"

#if _ENGINE
#include "MyImGui.h"
#include <imgui.h>
#include <imgui_internal.h>
#endif

void SoundComponent::Init()
{
	SoundManager::GetInstance()->AddSoundComponent(mEntID);
}

void SoundComponent::Exit()
{
	SoundManager::GetInstance()->RemoveSoundComponent(mEntID);
}

void SoundComponent::Load()
{
	mSoundName = JSONManager::GetInstance()->GetString("SoundName");
	mVolume = JSONManager::GetInstance()->GetFloat("Volume");
	mPitch = JSONManager::GetInstance()->GetFloat("Pitch");
}

void SoundComponent::Save()
{
	JSONManager::GetInstance()->Save("Component", "SoundComponent");
	JSONManager::GetInstance()->Save("SoundName", mSoundName);
	JSONManager::GetInstance()->Save("Volume", mVolume);
	JSONManager::GetInstance()->Save("Pitch", mPitch);
}

void SoundComponent::DisplayProperties()
{
#if _ENGINE
	ImGui::Text("SoundName");
	ImGui::SameLine();
	{//Box to be able to drag and drop
		char buff[1024];
		strcpy(buff, mSoundName.c_str());
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
					mSoundName = std::string(static_cast<const char*>(payload->Data), payload->DataSize);
				}
			}
			ImGui::EndDragDropTarget();
		}
	}
	MyImGui::MyImGuiFloatInput(mVolume, "mVolume");
	MyImGui::MyImGuiFloatInput(mPitch, "mPitch");
	if (ImGui::Button("Play Sound"))
	{
		Play();
	}
	ImGui::SameLine();
	if (ImGui::Button("Stop Sound"))
	{
		Stop();
	}
	ImGui::Text("Current Tracking Channels: %i", mChannelVec.size());
#endif // _ENGINE
}

void SoundComponent::Play()
{
	mChannelVec.push_back(SoundManager::GetInstance()->PlaySound(mSoundName, mVolume, mPitch));
}

void SoundComponent::Stop()
{
	for (auto i : mChannelVec)
	{
		SoundManager::GetInstance()->StopChannelSound(i);
	}
}

bool SoundComponent::RemoveChannel(FMOD::Channel* _channel)
{
	auto iter = std::find(mChannelVec.begin(), mChannelVec.end(), _channel);
	if (iter != mChannelVec.end())
	{
		mChannelVec.erase(iter);
		return true;
	}

	return false;
}

void SoundComponent::PlayOverride()
{
	SoundManager::GetInstance()->PlaySoundOverride(mSoundName, mVolume, mPitch);
}