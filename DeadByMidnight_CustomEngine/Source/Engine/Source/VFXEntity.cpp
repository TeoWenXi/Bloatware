/*!
All content © 2023 DigiPen Institute of Technology Singapore, all rights reserved.
@file       VFXEntity.cpp
@author     Jovan Zhuo Wen LOW (100%)
@co-author 

----------------------------------------------------------------------------

@course     CSD2401F23
@section    CSD2401F23-A
@project    CSD2401F23 Software Engineering Project 3

@brief      Definitions of the VFXEntity class functions
*//*______________________________________________________________________*/

#include "VFXEntity.h"

#if _ENGINE
#include "MyImGui.h"
#endif // _ENGINE

#include "Transform.h"
#include "Renderer.h"

#include <limits>
#include "AnimationComponent.h"

std::vector<EntityID> VFXEntity::mVFXIDs{};

void VFXEntity::Init()
{
	mVFXID = mEntID;
	mVFXIDs.push_back(mEntID);
}

void VFXEntity::Update()
{
	if (!mSetToBeDestroy)
	{
		//Will not be limited solely to entities operated by AnimationComponent
		if (EntityManager::GetInstance()->GetEntity(mEntID)->GetComponent<AnimationComponent>()
			&& !EntityManager::GetInstance()->GetEntity(mEntID)->GetComponent<AnimationComponent>()->IsPlaying())
		{
			mSetToBeDestroy = true;
			EntityManager::GetInstance()->GetEntity(mEntID)->SetToDestroy(true);
		}
	}
}

void VFXEntity::Exit()
{
	auto iter = std::find(mVFXIDs.begin(), mVFXIDs.end(), mEntID);
	if (iter != mVFXIDs.end())
	{
		mVFXIDs.erase(iter);
	}
}

void VFXEntity::Load()
{

}

void VFXEntity::Save()
{

}

void VFXEntity::DisplayProperties()
{
#if _ENGINE
	int entityID = static_cast<int>(mVFXID);
	MyImGui::MyImGuiIntInput(entityID, "VFXID");
#endif // _ENGINE
}

std::ostream& operator<<(std::ostream& os, const VFXEntity& _vfxentity)
{
	os << EntityManager::GetInstance()->GetEntity(_vfxentity.mVFXID)->GetName();
	return os;
}