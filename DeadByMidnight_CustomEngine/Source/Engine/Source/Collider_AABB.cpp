/*!
All content © 2023 DigiPen Institute of Technology Singapore, all rights reserved.
@file       Collider_AABB.cpp
@author     SIA Xin Tian (40%)
@co-author	TEO Wen Xi (60%)

----------------------------------------------------------------------------

@course     CSD2401F23
@section    CSD2401F23-A
@project    CSD2401F23 Software Engineering Project 3

@brief      Stores essential data like min & max of bounding box, and renderer.
*//*______________________________________________________________________*/

#include "Collider_AABB.h"
#include "Collision.h"

#include "EntityManager.h"
#include "Transform.h"

#if _ENGINE
#include "MyImGui.h"
#include <imgui.h>
#endif // _ENGINE

void Collider_AABB::Init()
{
	Transform* xform = EntityManager::GetInstance()->GetEntity(mEntID)->GetComponent<Transform>();
	mRdr.mModelType = Renderer::MODEL_TYPE::BOX;
	mRdr.mDebugModelType = Renderer::DEBUG_MODEL_TYPE::BOX;
	mFinalpos = xform->GetPosition();
	mScale = xform->mScale;
	mColliderType = ColliderType::AABB;

	for (int i = 0; i < NUM_COLLIDER_TYPES; ++i)
	{
		switch (ColliderType(i))
		{
		case 0: mColliderTypesNames.push_back("AABB");
			break;
		case 1: mColliderTypesNames.push_back("OOBB");
			break;
		}
	}

}

void Collider_AABB::Update()
{
	Transform* xform = EntityManager::GetInstance()->GetEntity(mEntID)->GetComponent<Transform>();
	Vector3 pos = xform->GetPosition();
	Vector3 scale = xform->mScale;

	//mScale = {10, 10};
	mFinalpos = pos + mOffset; //final pos of the collider box

	//Horizontal
	bool horizontal = false;
	if ((int)xform->mRotation % 180 != 0 && (int)xform->mRotation % 90 == 0)
		horizontal = true;

	if (horizontal == false)
	{
		//Min
		mMin.mX = mFinalpos.mX - xform->mScale.mX * 0.5f;
		mMin.mY = mFinalpos.mY - xform->mScale.mY * 0.5f;
		mMin.mZ = 0;

		//Max
		mMax.mX = mFinalpos.mX + xform->mScale.mX * 0.5f;
		mMax.mY = mFinalpos.mY + xform->mScale.mY * 0.5f;
		mMax.mZ = 0;
	}
	else
	{
		//Min
		mMin.mX = mFinalpos.mX - xform->mScale.mY * 0.5f;
		mMin.mY = mFinalpos.mY - xform->mScale.mX * 0.5f;
		mMin.mZ = 0;

		//Max
		mMax.mX = mFinalpos.mX + xform->mScale.mY * 0.5f;
		mMax.mY = mFinalpos.mY + xform->mScale.mX * 0.5f;
		mMax.mZ = 0;
	}
}

void Collider_AABB::Exit()
{
}

void Collider_AABB::Load()
{
	mMin = JSONManager::GetInstance()->GetVec3("Min");
	mMax = JSONManager::GetInstance()->GetVec3("Max");
}

void Collider_AABB::Save()
{
	JSONManager::GetInstance()->Save("Component", "Collider_AABB");
	JSONManager::GetInstance()->Save("Min", mMin);
	JSONManager::GetInstance()->Save("Max", mMax);
}

void Collider_AABB::DisplayProperties()
{
#if _ENGINE
	//Display Collider type selector
	ImGui::Text("Collider Type");
	ImGui::SameLine();
	ImGui::SetNextItemWidth(-1);

	std::string typeString{};
	if (mColliderTypesNames.empty())
		typeString = mColliderType == 0 ? "AABB" : "OOBB";
	else
		typeString = mColliderTypesNames[mColliderType];
	if (ImGui::BeginCombo("##ColliderType", typeString.c_str(), 0))
	{
		for (int n = 0; n < mColliderTypesNames.size(); n++)
		{
			const bool is_selected = (mColliderType == n);
			if (ImGui::Selectable(mColliderTypesNames[n].c_str(), is_selected))
				mColliderType = ColliderType(n);

			if (is_selected)
				ImGui::SetItemDefaultFocus();
		}
		ImGui::EndCombo();
	}

	// offset
	MyImGui::MyImGuiVector3Input(mOffset, "Offset");
	// scale
	MyImGui::MyImGuiVector3Input(mScale, "Size");
	//Min
	MyImGui::MyImGuiVector3Input(mMin, "Min");
	//Max
	MyImGui::MyImGuiVector3Input(mMax, "Max");
#endif // _ENGINE
}

ColliderType Collider_AABB::GetType()
{
	return mColliderType;
}

bool Collider_AABB::GetIsCollided() const
{
	return mIsCollided;
}

bool Collider_AABB::GetIsTriggered() const
{
	return mIsTriggered;
}

bool Collider_AABB::GetIsEnabled() const
{
	return mIsEnabled;
}

void Collider_AABB::SetIsCollided(bool _val)
{
	mIsCollided = _val;
}

void Collider_AABB::SetIsTriggered(bool _val)
{
	mIsTriggered = _val;
}

void Collider_AABB::SetIsEnabled(bool _val)
{
	mIsEnabled = _val;
}

std::ostream& operator<< (std::ostream& _os, const Collider_AABB& _col)
{
	_os << "Min: " << _col.mMin << " Max: " << _col.mMax;
	return _os;
}