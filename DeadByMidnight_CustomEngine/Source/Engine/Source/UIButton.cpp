/*!
All content © 2023 DigiPen Institute of Technology Singapore, all rights reserved.
@file       UIButton.cpp
@author     TAN Kang Zheng (100%)
@co-author

----------------------------------------------------------------------------

@course     CSD2401F23
@section    CSD2401F23-A
@project    CSD2401F23 Software Engineering Project 3

@brief      Base Component for buttons in the UI
*//*______________________________________________________________________*/

#include "UIButton.h"
#include "GameUIManager.h"
#include "InputManager.h"
#include "Collision.h"

#if _ENGINE
#include "MyImGui.h"
#endif // _ENGINE

UIButton::~UIButton()
{
	std::vector<EntityID> _childEnt = EntityManager::GetInstance()->GetEntity(mEntID)->GetChildEntList();
	for (auto i : _childEnt)
	{
		EntityManager::GetInstance()->GetEntity(i)->SetToDestroy(true);
	}
}

void UIButton::Update()
{
	//Transform* _transform = EntityManager::GetInstance()->GetEntity(mEntID)->GetComponent<Transform>();
	//Vec3 _mousePos = InputManager::GetInstance()->GetMousePosWorld();
	//
	//if (_transform == nullptr)
	//	return;
	//if (Collision::CollisionStaticAABB(_mousePos, *_transform))
	//{
	//	//Mouse is hovering over button
	//	if (!lastFrameHover)
	//	{
	//		OnHoverEnter();
	//		lastFrameHover = true;
	//	}
	//	
	//	//Check for hold
	//	if (InputManager::GetInstance()->GetMouseButton(0))
	//	{
	//		if (lastFramePress)
	//			OnHold();
	//		else
	//		{
	//			OnClick();
	//			lastFramePress = true;
	//		}
	//	}
	//
	//	//Check for release
	//	if (InputManager::GetInstance()->GetMouseButtonUp(0))
	//	{
	//		lastFramePress = false;
	//	}
	//}
	//else
	//{
	//	//Mosue not hovering over button
	//	if (lastFrameHover)
	//	{
	//		OnHoverExit();
	//		lastFrameHover = false;
	//	}
	//}
}