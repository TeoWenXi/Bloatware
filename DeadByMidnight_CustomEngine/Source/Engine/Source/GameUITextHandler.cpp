/*!
All content © 2023 DigiPen Institute of Technology Singapore, all rights reserved.
@file       GameUIObjectHandler.cpp
@author     TAN Kang Zheng (100%)
@co-author

----------------------------------------------------------------------------

@course     CSD2401F23
@section    CSD2401F23-A
@project    CSD2401F23 Software Engineering Project 3

@brief		Component for game UI mText
*//*______________________________________________________________________*/
#include "GameUITextHandler.h"
#include "GameUIManager.h"
#include "Transform.h"

#if _ENGINE
#include "MyImGui.h"
#include <imgui.h>
#include <imgui_internal.h>
#endif // _ENGINE

void GameUITextHandler::Init()
{
	GameUIManager::GetInstance()->AddText(this->mEntID);
}

void GameUITextHandler::Exit()
{
	GameUIManager::GetInstance()->RemoveText(this->mEntID);
}

void GameUITextHandler::Load()
{
	mText = JSONManager::GetInstance()->GetString("Text");
	mFont = JSONManager::GetInstance()->GetString("Font");
	mFontSize = JSONManager::GetInstance()->GetInt("FontSize");
	mPos = JSONManager::GetInstance()->GetVec3("Pos");
	mColor = JSONManager::GetInstance()->GetVec3("Color");
	mAlpha = JSONManager::GetInstance()->GetFloat("Alpha");
	int renderLayer = JSONManager::GetInstance()->GetInt("RenderLayer");
	if (renderLayer == -1)
		mRenderLayer = Renderer::RENDER_LAYER::INTERFACE;
	else
		mRenderLayer = static_cast<Renderer::RENDER_LAYER>(renderLayer);
}

void GameUITextHandler::Save()
{
	JSONManager::GetInstance()->Save("Component", "GameUITextHandler");
	JSONManager::GetInstance()->Save("Text", mText);
	JSONManager::GetInstance()->Save("Font", mFont);
	JSONManager::GetInstance()->Save("FontSize", mFontSize);
	JSONManager::GetInstance()->Save("Pos", mPos);
	JSONManager::GetInstance()->Save("Color", mColor);
	JSONManager::GetInstance()->Save("Alpha", mAlpha);
	JSONManager::GetInstance()->Save("RenderLayer", static_cast<int>(mRenderLayer));
};

void GameUITextHandler::DisplayProperties()
{
#if _ENGINE
	//Text
	MyImGui::MyImGuiStringInput(mText, "Text");
	//Font
	ImGui::Text("Font");
	ImGui::SameLine();
	{//Box to be able to drag and drop
		char buff[1024];
		strcpy(buff, mFont.c_str());
		ImDrawList* draw_list = ImGui::GetForegroundDrawList();
		//Draw uneditable input box
		ImGui::PushStyleVar(ImGuiStyleVar_DisabledAlpha, 1);
		ImGui::BeginDisabled();
		ImGui::SameLine(ImGui::GetWindowWidth() - 300); ImGui::SetNextItemWidth(250);
		ImGui::InputText("##fontName", buff, 1024, ImGuiInputTextFlags_ReadOnly);
		ImGui::EndDisabled();
		ImGui::PopStyleVar();
		ImRect inner_rect = ImGui::GetCurrentContext()->LastItemData.Rect;
		if (ImGui::BeginDragDropTargetCustom(inner_rect, ImGui::GetID("##fontName")))//drag drop for sprites
		{
			const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("FontDrag", ImGuiDragDropFlags_AcceptBeforeDelivery);//need to check before delivery so we can draw the yellow box
			if (payload)
			{
				if (payload->IsPreview())
				{
					draw_list->AddRectFilled(inner_rect.Min, inner_rect.Max, ImGui::GetColorU32(ImGuiCol_DragDropTarget, 0.05f));//draw inner darker overlay
					draw_list->AddRect(inner_rect.Min, inner_rect.Max, ImGui::GetColorU32(ImGuiCol_DragDropTarget), 0.0f, 0, 2.0f);//draw the yellow outer box
				}
				if (payload->IsDelivery())
				{
					mFont = std::string(static_cast<const char*>(payload->Data), payload->DataSize);
				}
			}
			ImGui::EndDragDropTarget();
		}
	}
	//FontSize
	MyImGui::MyImGuiIntInput(mFontSize, "FontSize");//Change to dropdown enum list
	//PosX
	MyImGui::MyImGuiVector3Input(mPos, "Pos");
	//Color
	MyImGui::MyImGuiVector3Input(mColor, "Color");
	//Alpha
	MyImGui::MyImGuiFloatInput(mAlpha, "Alpha");
	//Render Layer
	const char* items[] = { "Background",
						"Midground",
						"Foreground",
						"AboveTiles",
						"Interface"
	};
	const char* current_item = items[(int)mRenderLayer];

	ImGui::Text("Render Layer:");
	if (ImGui::BeginCombo("##Render Layer", current_item)) // The second parameter is the label previewed before opening the combo.
	{
		for (int n = 0; n < IM_ARRAYSIZE(items); n++)
		{
			bool is_selected = (current_item == items[n]); // You can store your selection however you want, outside or inside your objects
			if (ImGui::Selectable(items[n], is_selected))
			{
				current_item = items[n];
				if (n != (int)mRenderLayer) //If choosen button type is different
				{
					mRenderLayer = Renderer::RENDER_LAYER(n);
				}
			}
			if (is_selected)
				ImGui::SetItemDefaultFocus();   // You may set the initial focus when opening the combo (scrolling + for keyboard navigation support)
		}
		ImGui::EndCombo();
	}
#endif // _ENGINE
}

std::ostream& operator<<(std::ostream& os, const GameUITextHandler& _handler)
{
	os << "GameUITextHandler tied to Ent ID " << _handler.mEntID;
	return os;
}
