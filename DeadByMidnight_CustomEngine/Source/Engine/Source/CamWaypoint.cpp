/*!
All content © 2023 DigiPen Institute of Technology Singapore, all rights reserved.
@file       CamWaypoint.cpp
@author     TEO Wen Xi (100%)
@co-author

----------------------------------------------------------------------------

@course     CSD2401F23
@section    CSD2401F23-A
@project    CSD2401F23 Software Engineering Project 3

@brief      Camera waypoint class definitions.
*//*______________________________________________________________________*/

#include "CamWaypoint.h"
#include "JSONManager.h"
#include "GraphicsSystem.h"
#include "EntityManager.h"
#include "Transform.h"
#include <string>

#if _ENGINE
#include "MyImGui.h"
#include <imgui.h>
#define PROPERTY_WIDTH 	ImGui::GetWindowWidth() - 300
#endif // _ENGINE

#define UNREFERENCED_PARAMETER(P) (P)

void CamWaypoint::Load()
{
	mPosition = JSONManager::GetInstance()->GetVec3("Position");
	mZoom = JSONManager::GetInstance()->GetFloat("Zoom");

	mTrackPlayer = JSONManager::GetInstance()->GetBool("TrackPlayer");
	mPlayerSpawnPos = JSONManager::GetInstance()->GetVec3("PlayerSpawnPos");

	mHoldVertical = JSONManager::GetInstance()->GetBool("HoldVertical");
	mHoldHorizontal = JSONManager::GetInstance()->GetBool("HoldHorizontal");

	mClampEnabled = JSONManager::GetInstance()->GetBool("ClampEnabled");
	mClampVertRange = JSONManager::GetInstance()->GetVec3("ClampVertRange");
	mClampHoriRange = JSONManager::GetInstance()->GetVec3("ClampHoriRange");
}

void CamWaypoint::Save()
{
	JSONManager::GetInstance()->Save("Position", static_cast<Vector3>(mPosition));
	JSONManager::GetInstance()->Save("Zoom", mZoom);

	JSONManager::GetInstance()->Save("TrackPlayer", mTrackPlayer);
	JSONManager::GetInstance()->Save("PlayerSpawnPos", static_cast<Vector3>(mPlayerSpawnPos));

	JSONManager::GetInstance()->Save("HoldVertical", mHoldVertical);
	JSONManager::GetInstance()->Save("HoldHorizontal", mHoldHorizontal);

	JSONManager::GetInstance()->Save("ClampEnabled", mClampEnabled);
	JSONManager::GetInstance()->Save("ClampVertRange", static_cast<Vector3>(mClampVertRange));
	JSONManager::GetInstance()->Save("ClampHoriRange", static_cast<Vector3>(mClampHoriRange));
}

void CamWaypoint::DisplayProperties(int _waypointIndex)
{

#if _BLOATWARE
	UNREFERENCED_PARAMETER(_waypointIndex);
#endif // BLOATWARE

#if _ENGINE
	MyImGui::MyImGuiVector3Input(mPosition, "Camera Start Position", "##" + _waypointIndex);
	MyImGui::MyImGuiFloatInput(mZoom, "Zoom Level", "##" + _waypointIndex);
	MyImGui::MyImGuiVector3Input(mPlayerSpawnPos, "Player Spawn Location", "##" + _waypointIndex);
	
	ImGui::Checkbox(std::string("##Follow player" + std::to_string(_waypointIndex)).c_str(), &mTrackPlayer);
	ImGui::SameLine(); ImGui::Text("Follow player");
	ImGui::Checkbox(std::string("##Stop Vertical Movement" + std::to_string(_waypointIndex)).c_str(), &mHoldVertical);
	ImGui::SameLine();  ImGui::Text("Stop Vertical Movement");
	ImGui::Checkbox(std::string("##Stop Horizontal Movement" + std::to_string(_waypointIndex)).c_str(), &mHoldHorizontal);
	ImGui::SameLine(); ImGui::Text("Stop Horizontal Movement");

	ImGui::Checkbox(std::string("##Enable Clamping" + std::to_string(_waypointIndex)).c_str(), &mClampEnabled);
	ImGui::SameLine(); ImGui::Text("Enable Clamping");

	MyImGui::MyImGuiVector2Input(mClampVertRange, "Vertical Clamp Range",  "##" + _waypointIndex);
	MyImGui::MyImGuiVector2Input(mClampHoriRange, "Horizontal Clamp Range", "##" + _waypointIndex);

	ImGui::Spacing();

	if (ImGui::Button((std::string("Set Left Clamp ") + std::to_string(_waypointIndex)).c_str()))
		mClampHoriRange.x = mPosition.x - (GraphicsSystem::GetInstance()->mWorldSize.x / 2);
	ImGui::SameLine();
	if (ImGui::Button((std::string("Set Right Clamp ") + std::to_string(_waypointIndex)).c_str()))
		mClampHoriRange.y = mPosition.x + (GraphicsSystem::GetInstance()->mWorldSize.x / 2);
	if (ImGui::Button((std::string("Set Bottom Clamp ") + std::to_string(_waypointIndex)).c_str()))
		mClampVertRange.x = mPosition.y - (GraphicsSystem::GetInstance()->mWorldSize.y / 2);
	ImGui::SameLine();
	if (ImGui::Button((std::string("Set Top Clamp ") + std::to_string(_waypointIndex)).c_str()))
		mClampVertRange.y = mPosition.y + (GraphicsSystem::GetInstance()->mWorldSize.y / 2);
	if (ImGui::Button((std::string("Set Player SpawnPoint ") + std::to_string(_waypointIndex)).c_str()))
	{
		if (ComponentManager::GetInstance()->GetComponentArray<Player>() != nullptr)
			mPlayerSpawnPos = EntityManager::GetInstance()->GetEntity(Player::mPlayerID)->GetComponent<Transform>()->GetPosition();
	}

	ImGui::Spacing();
	if (ImGui::Button((std::string("Reset Waypoint ") + std::to_string(_waypointIndex)).c_str()))
		Reset();

#endif // _ENGINE
}

void CamWaypoint::Reset()
{
	mPosition = { 0, 0, 0 };	//Starting position of the waypoint
	mZoom = 1.f;

	//Tracking toggle
	mTrackPlayer = false;
	mPlayerSpawnPos = { 0, 0, 0 };	//Player spawn position for the checkpoint

	//Horizontal/Vertical movement only
	mHoldVertical = false;
	mHoldHorizontal = false;

	//Clamping
	mClampEnabled = false;
	mClampVertRange = {0, 0};	//x = bottom, y = top
	mClampHoriRange = {0, 0};	//x = left, y = right
}