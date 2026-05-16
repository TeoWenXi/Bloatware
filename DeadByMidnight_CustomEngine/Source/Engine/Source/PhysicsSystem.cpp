/*!
All content © 2023 DigiPen Institute of Technology Singapore, all rights reserved.
@file       PhysicsSystem.cpp
@author     SIA Xin Tian (70%)
@co-author	TEO Wen Xi (20%)
@co-author	Jovan Zhuo Wen LOW (10%)

----------------------------------------------------------------------------

@course     CSD2401F23
@section    CSD2401F23-A
@project    CSD2401F23 Software Engineering Project 3

@brief      Update physics for all physics based entities
*/
/*______________________________________________________________________*/

#include "PhysicsSystem.h"
#include "EntityManager.h"
#include "WindowsManager.h"
#include "EventManager.h"
#include "CoreEngine.h"

// Components
#include "PhysicsComponent.h"
#include "Transform.h"
#include "Collision.h"
#include "Player.h"
#include "TileComponent.h"

#include "MonoManager.h"

std::unordered_map<EntityID,PhysicsComponent> *physicsComponentArr;

void PhysicsSystem::Init()
{

}

void PhysicsSystem::Load()
{
}

void PhysicsSystem::Update()
{
	if (CoreEngine::GetInstance()->mGameState != CoreEngine::GameState::GS_PLAY)
	{
		return;
	}
	physicsComponentArr = NULL;
	if (ComponentManager::GetInstance()->GetComponentArray<PhysicsComponent>())
		physicsComponentArr = &ComponentManager::GetInstance()->GetComponentArray<PhysicsComponent>()->mComponentArray;
	if (physicsComponentArr == NULL)
		return;

	// Update all entities with physics
	{
		for (auto& phyObj : *physicsComponentArr)
		{
			Collider_AABB* col_aabb = EntityManager::GetInstance()->GetEntity(phyObj.first)->GetComponent<Collider_AABB>();
			if (col_aabb == nullptr)
				continue;

			// Update all physics related stuff here
			phyObj.second.Update();

			// Update all colliders here
			col_aabb->Update();
		}
	}

#pragma region gridCollision
	// check for grid collision
	if (TileComponent::mBinaryMap.empty() == false)
	{
		if (ComponentManager::GetInstance()->GetComponentArray<Player>() == nullptr)
			return;

		auto& playerComponeentArr = ComponentManager::GetInstance()->GetComponentArray<Player>()->mComponentArray;
		if (playerComponeentArr.size() < 1)
			return;

		for (auto& playerObj : playerComponeentArr)
		{
			// Getting player's variables
			Entity* playerEnt = EntityManager::GetInstance()->GetEntity(playerObj.first);
			PhysicsComponent* phy = playerEnt->GetComponent<PhysicsComponent>();
			Transform* xform = playerEnt->GetComponent<Transform>();
			TileComponent* map = playerEnt->GetComponent<TileComponent>();

			if (map == nullptr || 0 == (map->mFlag & TileComponent::FLAG_ACTIVE))
				continue;

			//checking for grid collision
			map->mGridCollisionFlag = Collision::CheckInstanceBinaryMapCollision(xform->GetPosition().mX, xform->GetPosition().mY ,xform, playerObj.second.ShouldExtendBottom()); //to get collision flag
			if (map->mGridCollisionFlag != 0)
			{
				if (map->mGridCollisionFlag & TileComponent::COLLISION_BOTTOM)
				{
					float coord = TileComponent::GetCoord(xform->GetPosition().mY, xform,TileComponent::COLLIDED_DIRECTION::BOTTOM);

					TileComponent::SnapPlayerToCell(&(xform->GetPosition().mY), xform, coord,TileComponent::COLLIDED_DIRECTION::BOTTOM, TileComponent::mTileSize / 3.5f);
					phy->SetOnGround();
					playerObj.second.mIsOnGround = true;
				}
				if (map->mGridCollisionFlag & TileComponent::COLLISION_TOP)
				{
					float coord = TileComponent::GetCoord(xform->GetPosition().mY, xform, TileComponent::COLLIDED_DIRECTION::TOP);

					TileComponent::SnapPlayerToCell(&(xform->GetPosition().mY), xform, coord,TileComponent::COLLIDED_DIRECTION::TOP);
					phy->mCurrentVelocity.mY = 0;
				}
				if (map->mGridCollisionFlag & TileComponent::COLLISION_LEFT)
				{
					float coord = TileComponent::GetCoord(xform->GetPosition().mX, xform,TileComponent::COLLIDED_DIRECTION::LEFT);

					TileComponent::SnapPlayerToCell(&(xform->GetPosition().mX), xform, coord,TileComponent::COLLIDED_DIRECTION::LEFT);
					phy->mCurrentVelocity.mX = 0;
				}
				if (map->mGridCollisionFlag & TileComponent::COLLISION_RIGHT)
				{
					float coord = TileComponent::GetCoord(xform->GetPosition().mX, xform,TileComponent::COLLIDED_DIRECTION::RIGHT);

					TileComponent::SnapPlayerToCell(&(xform->GetPosition().mX), xform, coord,TileComponent::COLLIDED_DIRECTION::RIGHT);
					phy->mCurrentVelocity.mX = 0;
				}

				//Update position along with child objects
				xform->SetPosition(xform->GetPosition());
			}
		}
	}
#pragma endregion

	// Collision Checking & Response
	{
		if (physicsComponentArr->size() < 2)
			return;

		auto phyObj = physicsComponentArr->begin();
		while(phyObj != physicsComponentArr->end())
		{
			Entity* firstEnt = EntityManager::GetInstance()->GetEntity(phyObj->first);

			Collider_AABB* aabb1 = firstEnt->GetComponent<Collider_AABB>();
			ColliderType colliderType1 = aabb1->GetType();
			PhysicsComponent* phy1 = &phyObj->second;
			Transform* xform1 = firstEnt->GetComponent<Transform>();
			if (!phy1->mEnabled)
			{
				++phyObj;
				continue;
			}
			auto phyObj2 = std::next(phyObj);
			while (phyObj2 != physicsComponentArr->end())
			{
				Entity* secondEnt = EntityManager::GetInstance()->GetEntity(phyObj2->first);
				if (secondEnt->GetName() != "Player" && firstEnt->GetName() != "Player")//only check for player
				{
					++phyObj2;
					continue;
				}
				//Component Check
				PhysicsComponent* phy2 = &phyObj2->second;
				Collider_AABB* aabb2 = secondEnt->GetComponent<Collider_AABB>();
				Transform* xform2 = secondEnt->GetComponent<Transform>();

				if (phy2 == nullptr || aabb2 == nullptr || xform2 == nullptr || !phy2->mEnabled)
				{
					++phyObj2;
					continue;
				}

				ColliderType colliderType2 = aabb2->GetType();

				if (colliderType1 != colliderType2)
				{
					++phyObj2;
					continue;
				}

				switch (colliderType1)
				{
				case ColliderType::AABB:
					phy1->mCollisionDir = Collision::SeparatingAxisTheoremAABB(aabb1, aabb2,phy1, phy2, xform1, xform2);
					if (phy1->mCollisionDir != Collision::CollisionDir::CD_NONE)
					{
						// Add event
						EventManager::GetInstance()->AddEvent(EventPlayer{ EventPlayer::PLAYER_COLLIDE_ENEMY });
						EventManager::GetInstance()->AddEvent(EventCollision{ phyObj->first, phyObj2->first, phy1->mCollisionDir });
						// for gfx event
						EventManager::GetInstance()->AddEvent(EventGfx{ phyObj->first, EventGfx::GFX_COLLIDED });

						//Update on C# side - collided
						switch (currentCollisionStates[phy1][phy2])
						{
						case COLLISION_STATES::NO_COLLISION:
							currentCollisionStates[phy1][phy2] = COLLISION_STATES::FIRST_COLLIDED;
							MonoManager::GetInstance()->OnTriggerEnter(phy1->mEntID, phy2->mEntID);
							break;
						case COLLISION_STATES::FIRST_COLLIDED:
						case COLLISION_STATES::COLLIDING:
							currentCollisionStates[phy1][phy2] = COLLISION_STATES::COLLIDING;
							MonoManager::GetInstance()->OnTriggerStay(phy1->mEntID, phy2->mEntID);
							break;
						default:
							break;
						}
					}
					else
					{
						//Update on C# side - no collision
						switch (currentCollisionStates[phy1][phy2])
						{
						case COLLISION_STATES::FIRST_COLLIDED:
						case COLLISION_STATES::COLLIDING:
							currentCollisionStates[phy1][phy2] = COLLISION_STATES::END_COLLISION;
							MonoManager::GetInstance()->OnTriggerExit(phy1->mEntID, phy2->mEntID);
							break;
						case COLLISION_STATES::END_COLLISION:
							currentCollisionStates[phy1][phy2] = COLLISION_STATES::NO_COLLISION;
							break;
						default:
							break;
						}
					}
					break;

				case ColliderType::OOBB:
					//if (Collision::IntersectionPolygons(*xform1, *xform2))
					//{
					//	//  for gfx event
					//	EventManager::GetInstance()->AddEvent(EventGfx{ phyObj1->first, phyObj2->first, EventGfx::GFX_COLLIDED });

					//	//Update on C# side - collided
					//	switch (currentCollisionStates[phy1][phy2])
					//	{
					//	case COLLISION_STATES::NO_COLLISION:
					//		currentCollisionStates[phy1][phy2] = COLLISION_STATES::FIRST_COLLIDED;
					//		MonoManager::GetInstance()->OnTriggerEnter(phyObj1->first, phyObj2->first);
					//		break;
					//	case COLLISION_STATES::FIRST_COLLIDED:
					//		currentCollisionStates[phy1][phy2] = COLLISION_STATES::COLLIDING;
					//		MonoManager::GetInstance()->OnTriggerStay(phyObj1->first, phyObj2->first);
					//		break;
					//	default:
					//		break;
					//	}
					//}
					//else
					//{
					//	//Update on C# side - no collision
					//	switch (currentCollisionStates[phy1][phy2])
					//	{
					//	case COLLISION_STATES::COLLIDING:
					//	case COLLISION_STATES::FIRST_COLLIDED:
					//		currentCollisionStates[phy1][phy2] = COLLISION_STATES::END_COLLISION;
					//		MonoManager::GetInstance()->OnTriggerExit(phyObj1->first, phyObj2->first);
					//		break;
					//	case COLLISION_STATES::END_COLLISION:
					//		currentCollisionStates[phy1][phy2] = COLLISION_STATES::NO_COLLISION;
					//		break;
					//	default:
					//		break;
					//	}
					//}
					break;

				default:
					// Handle other collider types or errors here.
					break;
				}
				++phyObj2;
			}
			++phyObj;
		}
	}
}

void PhysicsSystem::Unload()
{
	// Destroy instance
	Destroy();
}
