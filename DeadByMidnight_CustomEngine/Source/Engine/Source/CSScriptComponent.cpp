/*!
All content © 2023 DigiPen Institute of Technology Singapore, all rights reserved.
@file       CSScriptComponent.cpp
@author		HEAR Li Heng (100%)
@co-author

----------------------------------------------------------------------------

@course     CSD2401F23
@section    CSD2401F23-A
@project    CSD2401F23 Software Engineering Project 3

@brief      Stores the script name and registers it with MonoManager when the 
			component gets inited and also deregisters it from MonoManager
			when Exit is called.
*//*______________________________________________________________________*/
#include "CSScriptComponent.h"
#include "MonoManager.h"

#if _ENGINE
#include <imgui.h>
#include <imgui_internal.h>
#include "MyImGui.h"
#endif // _ENGINE

void CSScriptComponent::Exit()
{
	MonoManager::GetInstance()->DeregisterScript(mEntID, mScriptName);
}

void CSScriptComponent::Load()
{
	mScriptName = JSONManager::GetInstance()->GetString("ScriptName");

	//Load serialized values
	rapidjson::Value* value = JSONManager::GetInstance()->GetValue("SerializedValues");

	if (value)
	{
		auto memberItr = value->MemberBegin();
		while(memberItr != value->MemberEnd())
		{
			
			SerializedInfo info;
			info.name = memberItr->name.GetString();
			if (memberItr->value.IsInt())
				info.value = memberItr->value.GetInt();
			else if (memberItr->value.IsFloat())
				info.value = memberItr->value.GetFloat();
			else if (memberItr->value.IsBool())
				info.value = memberItr->value.GetBool();
			else if (memberItr->value.IsArray())
			{
				JSONManager::GetInstance()->LoadObject(value);
				if (memberItr->value.Size() == 3)
				{
					info.value = JSONManager::GetInstance()->GetVec3(info.name);
				}
				else if(memberItr->value.Size() == 4)
				{
					info.value = JSONManager::GetInstance()->GetVec4(info.name);
				}
				JSONManager::GetInstance()->CloseObject();
			}

			serializedValues.push_back(info);
			memberItr++;
		}
	}
}

void CSScriptComponent::Save()
{
	JSONManager::GetInstance()->Save("Component", "CSScriptComponent");
	JSONManager::GetInstance()->Save("ScriptName", mScriptName);

	//Save Serialized values
	JSONManager::GetInstance()->SaveObject("SerializedValues", false);
	for (const SerializedInfo& info : serializedValues)
	{
		if (std::get_if<int>(&info.value))
			JSONManager::GetInstance()->Save(info.name, std::get<int>(info.value));
		else if (std::get_if<float>(&info.value))
			JSONManager::GetInstance()->Save(info.name, std::get<float>(info.value));
		else if (std::get_if<bool>(&info.value))
			JSONManager::GetInstance()->Save(info.name, std::get<bool>(info.value));
		else if (std::get_if<Vector3>(&info.value))
			JSONManager::GetInstance()->Save(info.name, std::get<Vector3>(info.value));
		else if (std::get_if<Vector4>(&info.value))
			JSONManager::GetInstance()->Save(info.name, std::get<Vector4>(info.value));
	}
	JSONManager::GetInstance()->CloseObject();
}

//Need to seperate because component is not binded to entity yet when load is called
void CSScriptComponent::SetScript(std::string _scriptName)
{
	mScriptName = _scriptName;
	MonoManager::GetInstance()->RegisterScript(mEntID, mScriptName, this);
}

void CSScriptComponent::DisplayProperties()
{
#if _ENGINE
	ImGui::Text("Script"); ImGui::SameLine();
	{//Box to be able to drag and drop
		char buff[1024];
		strcpy(buff, mScriptName.c_str());
		ImDrawList* draw_list = ImGui::GetForegroundDrawList();
		//Draw uneditable input box
		ImGui::PushStyleVar(ImGuiStyleVar_DisabledAlpha, 1);
		ImGui::BeginDisabled();//we dont want the name to be changed but still want the box
		ImGui::SameLine(ImGui::GetWindowWidth() - 300); ImGui::SetNextItemWidth(250);
		ImGui::InputText((std::string("##scriptName") + mScriptName).c_str(), buff, 1024, ImGuiInputTextFlags_ReadOnly);
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
					mScriptName = std::string(static_cast<const char*>(payload->Data), payload->DataSize);
				}
			}
			ImGui::EndDragDropTarget();
		}
	}
	//Serialized values
	{
		//Update the values

		//Display them and set them if got changes
		for (SerializedInfo& info : serializedValues)
		{
			if (std::holds_alternative<int>(info.value))//int
			{
				if (MyImGui::MyImGuiIntInput(*std::get_if<int>(&info.value), info.name, "ScriptValue"))
				{
					//Update in script side
					MonoManager::GetInstance()->UpdateSerializedValues(serializedValues, this);
				}
			}
			else if (std::holds_alternative<float>(info.value))//float
			{
				if (MyImGui::MyImGuiFloatInput(*std::get_if<float>(&info.value), info.name, "ScriptValue"))
				{
					//Update in script side
					MonoManager::GetInstance()->UpdateSerializedValues(serializedValues, this);
				}
			}
			else if (std::holds_alternative<bool>(info.value))//bool
			{
				if (MyImGui::MyImGuiBoolInput(*std::get_if<bool>(&info.value), info.name, "ScriptValue"))
				{
					//Update in script side
					MonoManager::GetInstance()->UpdateSerializedValues(serializedValues, this);
				}
			}
			else if (std::holds_alternative<Vector3>(info.value))//Vector3
			{
				if (MyImGui::MyImGuiVector3Input(*std::get_if<Vector3>(&info.value), info.name, "ScriptValue"))
				{
					//Update in script side
					MonoManager::GetInstance()->UpdateSerializedValues(serializedValues, this);
				}
			}
			else if (std::holds_alternative<Vector4>(info.value))//Vector4
			{
				if (MyImGui::MyImGuiVector4Input(*std::get_if<Vector4>(&info.value), info.name, "ScriptValue"))
				{
					//Update in script side
					MonoManager::GetInstance()->UpdateSerializedValues(serializedValues, this);
				}
			}
			
		}
	}
#endif // _ENGINE
}

std::ostream& operator<<(std::ostream& _os, const CSScriptComponent& _script)
{
	_os << "Script: " << _script.mScriptName << " in Entity " << _script.mEntID;
	return _os;
}
