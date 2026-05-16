/*!
All content © 2023 DigiPen Institute of Technology Singapore, all rights reserved.
@file       Collider_Circle.cpp
@author     SIA Xin Tian (100%)
@co-author

----------------------------------------------------------------------------

@course     CSD2401F23
@section    CSD2401F23-A
@project    CSD2401F23 Software Engineering Project 3

@brief      Stores essential data like center and radius of bounding circle, and renderer.
*//*______________________________________________________________________*/

#include "Collider_CIRCLE.h"

#include "EntityManager.h"
#include "Transform.h"

void Collider_Circle::Init()
{
	mRdr.mModelType = Renderer::MODEL_TYPE::CIRCLE;
}

void Collider_Circle::Update()
{
	Transform* xform = EntityManager::GetInstance()->GetEntity(mEntID)->GetComponent<Transform>();
	Vector3 pos = xform->GetPosition();
	Vector3 scale = xform->mScale;

	mCenter.mX = pos.mX;
	mCenter.mY = pos.mY;

	//change later
	scale.mX = 10;
	scale.mY = 10;
	
}

void Collider_Circle::Exit()
{
}

void Collider_Circle::Load()
{
	mCenter = JSONManager::GetInstance()->GetVec3("Center");
	mRadius = JSONManager::GetInstance()->GetFloat("Radius");
}

void Collider_Circle::Save()
{
	JSONManager::GetInstance()->Save("Center", mCenter);
	JSONManager::GetInstance()->Save("Radius", mRadius);
}

void Collider_Circle::DisplayProperties()
{
#if _ENGINE
#endif // _ENGINE
}

std::ostream& operator<< (std::ostream& _os, const Collider_Circle& _col)
{
	_os << "Center: " << _col.mCenter << " Radius: " << _col.mRadius;
	return _os;
}