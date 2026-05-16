/*!
All content © 2023 DigiPen Institute of Technology Singapore, all rights reserved.
@file       Transform.cpp
@author     TEO Wen Xi (30%)
@co-author	HEAR Li Heng (70%)

----------------------------------------------------------------------------

@course     CSD2401F23
@section    CSD2401F23-A
@project    CSD2401F23 Software Engineering Project 3

@brief      Contains functions for the Transform component
*//*______________________________________________________________________*/

#include "Transform.h"
#include "EntityManager.h"

#if _ENGINE
#include "MyImGui.h"
#endif // _ENGINE

void Transform::Init()
{
	//Transform* xform = EntityManager::GetInstance()->GetEntity(mEntID)->GetComponent<Transform>();
	mRdr.mModelType = Renderer::MODEL_TYPE::BOX;
	mRdr.mDebugModelType = Renderer::DEBUG_MODEL_TYPE::BOX;

	//Child has parent
	if (EntityManager::GetInstance()->GetEntity(mEntID)->GetParentID() != 0)
	{
		Transform* pXform = EntityManager::GetInstance()->GetEntity(EntityManager::GetInstance()->GetEntity(mEntID)->GetParentID())->GetComponent<Transform>();
		if (pXform != nullptr)
			mPosition = pXform->GetPosition() - mLocalPos;
	}

	//Item has child
	LoadPosFromParent();
}

void Transform::Load()
{
	mPosition = JSONManager::GetInstance()->GetVec3("Position");
	mLocalPos = JSONManager::GetInstance()->GetVec3("LocalPosition");
	mScale = JSONManager::GetInstance()->GetVec3("Scale");
	mRotation = JSONManager::GetInstance()->GetFloat("Rotation");

	//Update child positions
	LoadPosFromParent();
}

void Transform::Save()
{
	JSONManager::GetInstance()->Save("Component", "Transform");
	JSONManager::GetInstance()->Save("Position", mPosition);
	JSONManager::GetInstance()->Save("LocalPosition", mLocalPos);
	JSONManager::GetInstance()->Save("Scale", mScale);
	JSONManager::GetInstance()->Save("Rotation", mRotation);
}

void Transform::DisplayProperties()
{
#if _ENGINE
	//Position
	MyImGui::MyImGuiVector3Input(mPosition, "Position");

	//Local Position
	if (mEntID != 0)
	{
		if (EntityManager::GetInstance()->GetEntity(mEntID)->GetParentID() != 0)
		{
			mPrevPos = mLocalPos;
			MyImGui::MyImGuiVector3Input(mLocalPos, "Local Position");

			if (mPrevPos != mLocalPos)
				UpdateLocalPosition();
		}
	}
	//Scale
	MyImGui::MyImGuiVector3Input(mScale, "Scale");
	//Rotation
	MyImGui::MyImGuiFloatInput(mRotation, "Rotation");
#endif // _ENGINE
}

std::ostream& operator<<(std::ostream& os, const Transform& _transform)
{
	os << "X: " << _transform.mPosition.mX << " Y: " << _transform.mPosition.mY << " Z: " << _transform.mPosition.mZ << " Rotation:" << _transform.mRotation;
	return os;
}

Vector3& Transform::GetPosition()
{
	return mPosition;
}

const Vector3& Transform::GetPosition() const
{
	return mPosition;
}

void Transform::SetPosition(const Vector3& _newPosition)
{
	//Update world position
	Vector3 offset = _newPosition - mPosition;
	mPosition = _newPosition;

	//Do not touch parenting if this component is created temporary for assigning later
	if (mEntID == 0 && mEntID != std::numeric_limits<EntityID>::max())
		return;

	//Update child members if any
	auto childEntList = EntityManager::GetInstance()->GetEntity(mEntID)->GetChildEntList();
	if (childEntList.size() > 0)
	{
		//Parent will calculate new position for child members using the offset
		for (int i = 0; i < childEntList.size(); ++i)
		{
			//Get child current position
			Entity* currEnt = EntityManager::GetInstance()->GetEntity(childEntList[i]);
			if (currEnt->GetComponent<Transform>() == nullptr)
				continue;

			Vector3 childPos = currEnt->GetComponent<Transform>()->GetPosition();
			currEnt->GetComponent<Transform>()->SetPosition(childPos + offset);
		}
	}
}

void Transform::SetLocalPosition(const Vector3& _newPosition)
{
	mLocalPos = _newPosition;

	//Update child objects if any
	LoadPosFromParent();
}

Vector3 Transform::GetLocalPosition()
{
	return mLocalPos;
}

const Vector3& Transform::GetParentPosition() const
{
	return EntityManager::GetInstance()->GetEntity(EntityManager::GetInstance()->GetEntity(mEntID)->GetParentID())->GetComponent<Transform>()->GetPosition();
}

void Transform::UpdateLocalPosition()
{
	mPosition = GetParentPosition() + mLocalPos;
}

void Transform::LoadPosFromParent()
{
	if (mEntID == 0 || EntityManager::GetInstance()->GetEntity(mEntID) == nullptr)
		return;

	//Update position based on parent
	if (EntityManager::GetInstance()->GetEntity(mEntID)->GetParentID() != 0)
	{
		Entity* parentEnt = EntityManager::GetInstance()->GetEntity(EntityManager::GetInstance()->GetEntity(mEntID)->GetParentID());
		if (parentEnt == nullptr || parentEnt->GetComponent<Transform>() == nullptr)
			return;
		mPosition = parentEnt->GetComponent<Transform>()->GetPosition() + mLocalPos;
	}

	//Update child members if any
	auto childEntList = EntityManager::GetInstance()->GetEntity(mEntID)->GetChildEntList();
	if (childEntList.size() > 0)
	{
		//Parent will calculate new position for child members using the offset
		for (int i = 0; i < childEntList.size(); ++i)
		{
			//Get child current position
			Entity* currEnt = EntityManager::GetInstance()->GetEntity(childEntList[i]);
			if (currEnt->GetComponent<Transform>() == nullptr)
				continue;

			//Child local pos
			Vector3 childLocalPos = currEnt->GetComponent<Transform>()->GetLocalPosition();
			currEnt->GetComponent<Transform>()->SetPosition(mPosition + childLocalPos);
		}
	}
}