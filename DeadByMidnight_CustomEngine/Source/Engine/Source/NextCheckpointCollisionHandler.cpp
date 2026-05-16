/*!
All content © 2023 DigiPen Institute of Technology Singapore, all rights reserved.
@file       NextCheckpointCollisionHandler.cpp
@author     TEO Wen Xi (100%)
@co-author

----------------------------------------------------------------------------

@course     CSD2401F23
@section    CSD2401F23-A
@project    CSD2401F23 Software Engineering Project 3

@brief		Collision handler observer for the next checkpoint collider.
*//*______________________________________________________________________*/

#include "NextCheckpointCollisionHandler.h"
#include "Scene.h"
#include "SceneManager.h"
#include "GraphicsSystem.h"

#if _ENGINE
#include "MyImGui.h"
#endif // _ENGINE

void NextCPCollisionHandler::DisplayProperties()
{
#if _ENGINE
	ImGui::Checkbox("HasCollided", &mHasCollided);
	ImGui::Checkbox("NextStageCollider", &mNextStageCollider);
	MyImGui::MyImGuiStringInput(mNextSceneName, "Next Scene Name");
#endif // _ENGINE
}


void NextCPCollisionHandler::OnNotify(EventObject* _eventObj)
{
	EventCollision* colEvent = dynamic_cast<EventCollision*>(_eventObj);
	if (colEvent)
	{
		//Ensures that this object is in mEntA
		if (colEvent->SameEntCheck())
			return;
		if (colEvent->mEntB == mEntID)
			std::swap(colEvent->mEntA, colEvent->mEntB);

		//Check if this entity is the item that collided
		if (colEvent->mEntA != mEntID)
			return;

		//Check if other collided object is player
		if (EntityManager::GetInstance()->GetEntity(colEvent->mEntB)->GetComponent<Player>() == nullptr)
			return;

		//Player has already collided with this next CP obj
		if (mHasCollided == true)
			return;

		if (mNextStageCollider)
		{
			SceneManager::GetInstance()->SetSceneToTransitionTo(mNextSceneName);
		}

		//Go to next checkpoint
		SceneManager::GetInstance()->GetActiveScene()->MoveNextCamWaypoint();
		mHasCollided = true;
	}
}