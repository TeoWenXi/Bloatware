/*!
All content © 2023 DigiPen Institute of Technology Singapore, all rights reserved.
@file       MyImGui.cpp
@author		HEAR Li Heng (100%)
@co-author

----------------------------------------------------------------------------

@course     CSD2401F23
@section    CSD2401F23-A
@project    CSD2401F23 Software Engineering Project 3

@brief      This file contains some frequently used code for displaying
			common types in the Property window. Mostly used in 
			components.DisplayProperties()
*//*______________________________________________________________________*/
#if _ENGINE


#include "MyImGui.h"
#include <imgui.h>


//Fixed the input fields to be all aligned to the right
#define PROPERTY_WIDTH 	ImGui::GetWindowWidth() - 300

//Displays the vector3
bool MyImGui::MyImGuiVector3Input(Vector3& _vec, std::string _name, std::string _extraTag)
{
	float buffer[3]{ _vec.mX,_vec.mY,_vec.mZ };
	ImGui::PushTextWrapPos(PROPERTY_WIDTH);
	ImGui::TextWrapped(_name.c_str());
	ImGui::PopTextWrapPos();
	std::string tag = "##" + _name + _extraTag;
	ImGui::SameLine(PROPERTY_WIDTH); ImGui::SetNextItemWidth(-1);
	if (ImGui::InputFloat3(tag.c_str(), buffer))
	{
		_vec.mX = buffer[0];
		_vec.mY = buffer[1];
		_vec.mZ = buffer[2];
		return true;
	}
	return false;
}
bool MyImGui::MyImGuiVector3Input(glm::vec3& _vec, std::string _name, std::string _extraTag)
{
	float buffer[3]{ _vec.x,_vec.y,_vec.z };
	ImGui::PushTextWrapPos(PROPERTY_WIDTH);
	ImGui::TextWrapped(_name.c_str());
	ImGui::PopTextWrapPos();
	std::string tag = "##" + _name + _extraTag;
	ImGui::SameLine(PROPERTY_WIDTH); ImGui::SetNextItemWidth(-1);
	if (ImGui::InputFloat3(tag.c_str(), buffer))
	{
		_vec.x = buffer[0];
		_vec.y = buffer[1];
		_vec.z = buffer[2];
		return true;
	}
	return false;
}

bool MyImGui::MyImGuiVector4Input(Vector4& _vec, std::string _name, std::string _extraTag)
{
	float buffer[4]{ _vec.mX,_vec.mY,_vec.mZ,_vec.mW };
	ImGui::PushTextWrapPos(PROPERTY_WIDTH);
	ImGui::TextWrapped(_name.c_str());
	ImGui::PopTextWrapPos();
	std::string tag = "##" + _name + _extraTag;
	ImGui::SameLine(PROPERTY_WIDTH); ImGui::SetNextItemWidth(-1);
	if (ImGui::InputFloat4(tag.c_str(), buffer))
	{
		_vec.mX = buffer[0];
		_vec.mY = buffer[1];
		_vec.mZ = buffer[2];
		_vec.mW = buffer[3];

		return true;
	}
	return false;
}


//Displays the vector2
bool MyImGui::MyImGuiVector2Input(glm::vec2& _vec, std::string _name, std::string _extraTag)
{
	float buffer[2]{ _vec.x,_vec.y};
	ImGui::PushTextWrapPos(PROPERTY_WIDTH);
	ImGui::TextWrapped(_name.c_str());
	ImGui::PopTextWrapPos();
	std::string tag = "##" + _name + _extraTag;
	ImGui::SameLine(PROPERTY_WIDTH); ImGui::SetNextItemWidth(-1);
	if (ImGui::InputFloat2(tag.c_str(), buffer))
	{
		_vec.x = buffer[0];
		_vec.y = buffer[1];
		return true;
	}
	return false;
}

//Displays the float
bool MyImGui::MyImGuiFloatInput(float& _val, std::string _name, std::string _extraTag)
{
	float buffer{ _val };
	ImGui::PushTextWrapPos(PROPERTY_WIDTH);
	ImGui::TextWrapped(_name.c_str());
	ImGui::PopTextWrapPos();
	std::string tag = "##" + _name + _extraTag;
	ImGui::SameLine(PROPERTY_WIDTH); ImGui::SetNextItemWidth(-1);
	if (ImGui::InputFloat(tag.c_str(), &buffer))
	{
		_val = buffer;
		return true;
	}
	return false;
}

//Displays the double
bool MyImGui::MyImGuiDoubleInput(double& _val, std::string _name, std::string _extraTag)
{
	double buffer{ _val};
	ImGui::PushTextWrapPos(PROPERTY_WIDTH);
	ImGui::TextWrapped(_name.c_str());
	ImGui::PopTextWrapPos();
	std::string tag = "##" + _name + _extraTag;
	ImGui::SameLine(PROPERTY_WIDTH); ImGui::SetNextItemWidth(-1);
	if (ImGui::InputDouble(tag.c_str(), &buffer))
	{
		_val = buffer;
		return true;
	}
	return false;
}

//Displays the int
bool MyImGui::MyImGuiIntInput(int& _val, std::string _name, std::string _extraTag)
{
	int buffer{ _val };
	ImGui::PushTextWrapPos(PROPERTY_WIDTH);
	ImGui::TextWrapped(_name.c_str());
	ImGui::PopTextWrapPos();
	std::string tag = "##" + _name + _extraTag;

	ImGui::SameLine(PROPERTY_WIDTH); ImGui::SetNextItemWidth(-1);
	if (ImGui::InputInt(tag.c_str(), &buffer))
	{
		_val = buffer;
		return true;
	}
	return false;
}

//Displays the string
bool MyImGui::MyImGuiStringInput(std::string& _val, std::string _name, std::string _extraTag)
{
	if (_name[0] != '#')
		ImGui::PushTextWrapPos(PROPERTY_WIDTH);
		ImGui::TextWrapped(_name.c_str());
		ImGui::PopTextWrapPos();
	char nameBuffer[1024]{};
	strcpy(nameBuffer, _val.c_str());

	ImGui::SameLine(PROPERTY_WIDTH); ImGui::SetNextItemWidth(-1);
	if (ImGui::InputText(_name.c_str(), nameBuffer, 1024))
	{
		_val = nameBuffer;
		return true;
	}
	return false;
}

//Displays wrapped text
void MyImGui::MyImGuiTextOutput(std::string _text)
{
	ImGui::PushTextWrapPos(PROPERTY_WIDTH);
	ImGui::TextWrapped(_text.c_str());
	ImGui::PopTextWrapPos();
	ImGui::SameLine(PROPERTY_WIDTH); ImGui::SetNextItemWidth(-1);
}

bool MyImGui::MyImGuiBoolInput(bool& _val, std::string _name, std::string _extraTag)
{
	bool buffer{ _val };
	ImGui::PushTextWrapPos(PROPERTY_WIDTH);
	ImGui::TextWrapped(_name.c_str());
	ImGui::PopTextWrapPos();
	std::string tag = "##" + _name + _extraTag;

	ImGui::SameLine(PROPERTY_WIDTH); ImGui::SetNextItemWidth(-1);
	if (ImGui::Checkbox(tag.c_str(), &buffer))
	{
		_val = buffer;
		return true;
	}
	return false;
}
#endif // _ENGINE
