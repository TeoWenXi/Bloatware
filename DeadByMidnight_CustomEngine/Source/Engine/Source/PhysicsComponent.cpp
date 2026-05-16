/*!
All content © 2023 DigiPen Institute of Technology Singapore, all rights reserved.
@file       PhysicsComponent.cpp
@author     SIA Xin Tian (40%)
@co-author	TEO Wen Xi (60%)

----------------------------------------------------------------------------

@course     CSD2401F23
@section    CSD2401F23-A
@project    CSD2401F23 Software Engineering Project 3

@brief      Stores essential data like size, position, velocity.
*//*______________________________________________________________________*/

#include "PhysicsComponent.h"

#include "Transform.h"
#include "Player.h"

#include "WindowsManager.h"
#include "EntityManager.h"

#if _ENGINE
#include "MyImGui.h"
#endif // _ENGINE

void PhysicsComponent::Init()
{

}

void PhysicsComponent::Update()
{
	if (!mEnabled) return;

	Transform* xform = EntityManager::GetInstance()->GetEntity(mEntID)->GetComponent<Transform>();
	Vec3 zero{ 0,0,0 };

	mObjCurrentMass = mMass;
	Vec3 objscale = mObjBigScale;
	if ((mObjBigScale != zero) && mCharacterSizeFactor < 1.0f - FLT_EPSILON)
	{
		objscale = mObjBigScale * mCharacterSizeFactor; //0.5 ~ 1.0
	}
	else
	{
		mWindTunnelForce.mX = 0;
		mWindTunnelForce.mY = 0;
		mObjCurrentMass = mMass;
	}

	//update scale
	if(mObjBigScale != zero)
		xform->mScale = objscale;

	//slower falling speed when character is small
	if ((mObjBigScale != zero) && mCharacterSizeFactor < 1.0f - FLT_EPSILON && mCurrentVelocity.mY < 0)
		mCurrentVelocity.mY *= (1 - 0.05f * mCharacterWindFactor);

	mJumpForce.mY = 0;
	
	//Update position
	xform->SetPosition(Vector3( xform->GetPosition().mX,
								mCurrentVelocity.mY * (float)WindowsManager::GetInstance()->mDeltaTime + xform->GetPosition().mY,
								xform->GetPosition().mZ));	

}

void PhysicsComponent::Exit()
{

}

void PhysicsComponent::Load()
{
	mCurrentVelocity = JSONManager::GetInstance()->GetVec3("CurrentVelocity");
	mGravity = JSONManager::GetInstance()->GetVec3("Gravity");
	mCurrentAcceleration = JSONManager::GetInstance()->GetVec3("CurrentAcceleration");
	mForce = JSONManager::GetInstance()->GetFloat("Force");
	mMass = JSONManager::GetInstance()->GetFloat("mMass");
	mObjCurrentMass = JSONManager::GetInstance()->GetFloat("Mass");
	mObjBigScale = JSONManager::GetInstance()->GetVec3("mObjBigScale");
	mIsCharacterSmall = JSONManager::GetInstance()->GetBool("Is_Char_Small");
	mJumpMultiplier = JSONManager::GetInstance()->GetFloat("JumpMultiplier");
	mJumpVelocity = JSONManager::GetInstance()->GetFloat("JumpVelocity");
	mCoefficientOfFriction = JSONManager::GetInstance()->GetFloat("Coeff_Of_Friction");
}

void PhysicsComponent::Save()
{
	JSONManager::GetInstance()->Save("Component", "PhysicsComponent");
	JSONManager::GetInstance()->Save("CurrentVelocity", mCurrentVelocity);
	JSONManager::GetInstance()->Save("CurrentAcceleration", mCurrentAcceleration);
	JSONManager::GetInstance()->Save("Gravity", mGravity);
	JSONManager::GetInstance()->Save("Force", mForce);
	JSONManager::GetInstance()->Save("mMass", mMass);
	JSONManager::GetInstance()->Save("Mass", mObjCurrentMass);
	JSONManager::GetInstance()->Save("mObjBigScale", mObjBigScale);
	JSONManager::GetInstance()->Save("Is_Char_Small", mIsCharacterSmall);
	JSONManager::GetInstance()->Save("JumpMultiplier", mJumpMultiplier);
	JSONManager::GetInstance()->Save("JumpVelocity", mJumpVelocity);
	JSONManager::GetInstance()->Save("Coeff_Of_Friction", mCoefficientOfFriction);
}

void PhysicsComponent::DisplayProperties()
{
#if _ENGINE
	//Velocity
	MyImGui::MyImGuiVector3Input(mCurrentVelocity, "Velocity");
	//Acceleration
	MyImGui::MyImGuiFloatInput(mCurrentA, "Acceleration");
	//Gravity
	MyImGui::MyImGuiVector3Input(mGravity, "Gravity");
	//Force
	MyImGui::MyImGuiFloatInput(mForce, "Force");
	//Mass
	MyImGui::MyImGuiFloatInput(mMass , "Mass");
	//Wind Tunnel Force
	MyImGui::MyImGuiVector3Input(mWindTunnelForce, "Wind Tunnel Force");
#endif // _ENGINE
}

void PhysicsComponent::SetOnGround()
{
	mForce = 0;
	mCurrentVelocity.mY = 0;
}

std::ostream& operator<< (std::ostream& _os, const PhysicsComponent& _phy)
{
	_os << "Velocity: " << _phy.mCurrentVelocity << " Gravity: " << _phy.mGravity << " Mass: " << _phy.mMass;
	return _os;
}

