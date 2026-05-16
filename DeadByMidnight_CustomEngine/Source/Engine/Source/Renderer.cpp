/*!
All content © 2023 DigiPen Institute of Technology Singapore, all rights reserved.
@file       Renderer.cpp
@author     TEO Wen Xi (95%)
@co-author	CAI Wenjin (5%)

----------------------------------------------------------------------------

@course     CSD2401F23
@section    CSD2401F23-A
@project    CSD2401F23 Software Engineering Project 3

@brief      Renderer Component class for rendering purposes
*//*______________________________________________________________________*/
#include "Renderer.h"

#if _ENGINE
#include <imgui.h>
#include <imgui_internal.h>
#include "MyImGui.h"
#include "lib/imgui-1.89.9-docking/misc/cpp/imgui_stdlib.h"
#endif // _ENGINE

void Renderer::Init()
{
	for (int i{}; i < static_cast<int>(RENDER_LAYER::NUM_RENDER_LAYER); ++i)
	{
		switch (RENDER_LAYER(i))
		{
			case Renderer::RENDER_LAYER::BACKGROUND:
				mRenderLayerName.push_back("Background");
				break;
			case Renderer::RENDER_LAYER::MIDGROUND:
				mRenderLayerName.push_back("Midground");
				break;
			case Renderer::RENDER_LAYER::FOREGROUND:
				mRenderLayerName.push_back("Foreground");
				break;
			case Renderer::RENDER_LAYER::ABOVETILES:
				mRenderLayerName.push_back("AboveTiles");
				break;
			case Renderer::RENDER_LAYER::INTERFACE:
				mRenderLayerName.push_back("Interface");
				break;
			default:
				break;
		}
	}
}

void Renderer::Save()
{
	JSONManager::GetInstance()->Save("Component", "Renderer");
	JSONManager::GetInstance()->Save("ModelType", static_cast<int>(mModelType));
	JSONManager::GetInstance()->Save("ShaderName", mShaderName);
	JSONManager::GetInstance()->Save("TextureName", mTextureName);
	JSONManager::GetInstance()->Save("RenderLayer", static_cast<int>(mRenderLayer));
	JSONManager::GetInstance()->Save("Enabled", mEnabled);
	JSONManager::GetInstance()->Save("AlphaValue", mAlphaValue);
	JSONManager::GetInstance()->Save("CustomAlpha", mCustomAlphaEnabled);
}

void Renderer::Load()
{
	mModelType = static_cast<MODEL_TYPE>(JSONManager::GetInstance()->GetInt("ModelType"));
	mShaderName = JSONManager::GetInstance()->GetString("ShaderName");
	mTextureName = JSONManager::GetInstance()->GetString("TextureName");
	mRenderLayer = static_cast<RENDER_LAYER>(JSONManager::GetInstance()->GetInt("RenderLayer") == -1 ? 0 : JSONManager::GetInstance()->GetInt("RenderLayer"));
	mEnabled = JSONManager::GetInstance()->GetBool("Enabled");
	mAlphaValue = JSONManager::GetInstance()->GetFloat("AlphaValue");
	mCustomAlphaEnabled = JSONManager::GetInstance()->GetBool("CustomAlpha");
}

void Renderer::DisplayProperties()
{
#if _ENGINE
	ImGui::Checkbox("Enabled? ", &mEnabled);
	//Shader
	MyImGui::MyImGuiStringInput(mShaderName, "Shader Name: ");
	MyImGui::MyImGuiFloatInput(mAlphaValue, "Alpha Value: ");
	MyImGui::MyImGuiBoolInput(mCustomAlphaEnabled, "Using custom alpha: ");

	ImGui::Text("Sprite"); ImGui::SameLine();
	{//Box to be able to drag and drop
		char buff[1024];
		strcpy(buff, mTextureName.c_str());
		ImDrawList* draw_list = ImGui::GetForegroundDrawList();
		//Draw uneditable input box
		ImGui::PushStyleVar(ImGuiStyleVar_DisabledAlpha, 1);
		ImGui::BeginDisabled();
		ImGui::SameLine(ImGui::GetWindowWidth() - 300); ImGui::SetNextItemWidth(250);
		ImGui::InputText("##spriteName", buff, 1024, ImGuiInputTextFlags_ReadOnly);
		ImGui::EndDisabled();
		ImGui::PopStyleVar();
		ImRect inner_rect = ImGui::GetCurrentContext()->LastItemData.Rect;
		if (ImGui::BeginDragDropTargetCustom(inner_rect, ImGui::GetID("##spriteName")))//drag drop for sprites
		{
			const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("SpriteDrag", ImGuiDragDropFlags_AcceptBeforeDelivery);//need to check before delivery so we can draw the yellow box
			if (payload)
			{
				if (payload->IsPreview())
				{
					draw_list->AddRectFilled(inner_rect.Min, inner_rect.Max, ImGui::GetColorU32(ImGuiCol_DragDropTarget, 0.05f));//draw inner darker overlay
					draw_list->AddRect(inner_rect.Min, inner_rect.Max, ImGui::GetColorU32(ImGuiCol_DragDropTarget), 0.0f, 0, 2.0f);//draw the yellow outer box
				}
				if (payload->IsDelivery())
				{
					mTextureName = std::string(static_cast<const char*>(payload->Data), payload->DataSize);
				}
			}
			ImGui::EndDragDropTarget();
		}
	}
	ImGui::SameLine();
	if (ImGui::Button("0"))//Display all available images
	{
		ImGui::OpenPopup("##SpriteList");
	}

	std::string str{};
	if (mRenderLayerName.empty())
	{
		switch (mRenderLayer)
		{
			case Renderer::RENDER_LAYER::BACKGROUND:
				str = "Background";
				break;
			case Renderer::RENDER_LAYER::MIDGROUND:
				str = "Midground";
				break;
			case Renderer::RENDER_LAYER::FOREGROUND:
				str = "Forground";
				break;
			default:
				break;
		}
	}
	else
		str = mRenderLayerName[static_cast<int>(mRenderLayer)];

	MyImGui::MyImGuiTextOutput("Render Layer");

	if (ImGui::BeginCombo("##RenderLayer", str.c_str(), 0))
	{
		for (int i{}; i < mRenderLayerName.size(); ++i)
		{
			const bool is_selected = (static_cast<int>(mRenderLayer) == i);
			if (ImGui::Selectable(mRenderLayerName[i].c_str(), is_selected))
				mRenderLayer = RENDER_LAYER(i);

			if (is_selected)
				ImGui::SetItemDefaultFocus();
		}
		ImGui::EndCombo();
	}
#endif // _ENGINE
}

const char* Renderer::ToString(RENDER_LAYER layer)
{
	switch (layer)
	{
		case Renderer::RENDER_LAYER::BACKGROUND:
			return "Background";
			break;
		case Renderer::RENDER_LAYER::MIDGROUND:
			return "Midground";
			break;
		case Renderer::RENDER_LAYER::FOREGROUND:
			return "Foreground";
			break;
		default:
			return "";
			break;
	}
}

std::ostream& operator<<(std::ostream& os, const Renderer& _renderer)
{
	os << "Model Type: ";
	switch (_renderer.mModelType)
	{
	case Renderer::MODEL_TYPE::POINT:
		os << "Point";
		break;
	case Renderer::MODEL_TYPE::LINE:
		os << "Line";
		break;
	case Renderer::MODEL_TYPE::CIRCLE:
		os << "Circle";
		break;
	case Renderer::MODEL_TYPE::QUAD:
		os << "Quad";
		break;
	case Renderer::MODEL_TYPE::BOX:
		os << "Box";
		break;
	case Renderer::MODEL_TYPE::TRIANGLE:
		os << "Triangle";
		break;
	case Renderer::MODEL_TYPE::SPRITE:
		os << "Image";
		break;
	case Renderer::MODEL_TYPE::SPRITE_ANIMATION:
		os << "Sprite Animation";
		break;
	case Renderer::MODEL_TYPE::COLLIDABLE:
		os << "Collidable";
		break;
	case Renderer::MODEL_TYPE::PARALLAX:
		os << "Parallax";
		break;
	case Renderer::MODEL_TYPE::AI_IRIS:
		os << "AI Iris";
		break;
	case Renderer::MODEL_TYPE::LASER:
		os << "Laser";
		break;
	}

	os << " Shader: " << _renderer.mShaderName;
	return os;
}
