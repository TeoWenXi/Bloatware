/*!
All content © 2023 DigiPen Institute of Technology Singapore, all rights reserved.
@file       LevelLoader.h
@author     HEAR Li Heng (100%)
@co-author

----------------------------------------------------------------------------

@course     CSD2401F23
@section    CSD2401F23-A
@project    CSD2401F23 Software Engineering Project 3

@brief      Manages the loading of levels and also the spawning of gameobjects
			Currently only can load level and load prefabs from json files
*//*______________________________________________________________________*/

#ifndef LEVEL_LOADER_H
#define LEVEL_LOADER_H

#pragma warning(push, 0)
#include <document.h>
#pragma warning(pop)
#include <vector>

#include "ComponentsHeader.h"
#include <unordered_map>
#include <list>

class LevelLoader
{
public:
	struct PrefabInfo//Used to store info for editing prefabs
	{
		std::pair<Transform, bool> mTransform{ {}, false };
		std::pair<Renderer, bool> mRenderer{ {}, false };
		std::pair<PhysicsComponent, bool> mPhysicsComponent{ {}, false };
		std::pair<Collider_AABB, bool> mCollider_AABB{ {}, false };
		std::pair<Collider_Circle, bool> mCollider_Circle{ {}, false };
		std::pair<Player, bool> mPlayer{ {}, false };
		std::pair<TileComponent, bool> mTileComponent{ {}, false };
		std::pair<AnimationComponent, bool> mAnimationComponent{ {}, false };
		std::pair<PlayerCollisionHandler, bool> mPlayerCollisionHandler{ {}, false };
		std::pair<EnemyCollisionHandler, bool> mEnemyCollisionHandler{ {}, false };
		std::pair<GameUIObjectHandler, bool> mGameUIObjectHandler{ {}, false };
		std::pair<GameUITextHandler, bool> mGameUITextHandler{ {}, false };
		std::pair<CSScriptComponent, bool> mCSScriptComponent{ {}, false };
		std::pair<AIComponent, bool> mAIComponent{ {}, false };
		std::pair<SoundComponent, bool> mSoundComponent{ {}, false };
		std::pair<ButtonWrapper, bool> mButtonWrapper{ {}, false };
		std::pair<Key, bool> mKeyComponent{ {}, false };
		std::pair<KeyCollisionHandler, bool> mKeyCollisionHandler{ {}, false };
		std::pair<KeyWall, bool> mKeyWallComponent{ {}, false };
		std::pair<KeyWallCollisionHandler, bool> mKeyWallCollisionHandler{ {}, false };
		std::pair<VFXEntity, bool> mVFXEntityComponent{ {}, false };
		std::pair<WindCollisionHandler, bool> mWindCollisionHandler{ {}, false };
		std::pair<Wind, bool> mWind{ {}, false };
		std::pair<EndGoal, bool> mEndGoal{ {}, false };
		std::pair<PlayOnceSoundComponent, bool> mPlayOnceSound{ {}, false };
		std::pair<DialogueTrigger, bool> mDialogueTrigger{ {}, false };
		std::pair<NextCPCollisionHandler, bool> mNextCPCollisionHandler{ {}, false };
	};
	struct AllComponents//temp way to get all components of an entity/prefab **See if got better way
	{
		/*!
		 * @brief 
		 *	Constructor of AllComponents
		 * @param ent 
		 *	The pointer of the entity to get all the components from
		 * @param isPrefab 
		 *	Flag if getting info from a prefab
		 * @param prefabEnt 
		 *	Pointer to the PrefabInfo to store the results
		*/
		AllComponents(Entity* ent, bool isPrefab = false, PrefabInfo* prefabEnt = nullptr)
		{
			if (isPrefab)
			{
				if (prefabEnt->mTransform.second)
					componentsList.push_back(std::make_pair(&prefabEnt->mTransform.first, typeid(Transform).name()));
				if (prefabEnt->mRenderer.second)
					componentsList.push_back(std::make_pair(&prefabEnt->mRenderer.first, typeid(Renderer).name()));
				if (prefabEnt->mPhysicsComponent.second)
					componentsList.push_back(std::make_pair(&prefabEnt->mPhysicsComponent.first, typeid(PhysicsComponent).name()));
				if (prefabEnt->mCollider_AABB.second)
					componentsList.push_back(std::make_pair(&prefabEnt->mCollider_AABB.first, typeid(Collider_AABB).name()));
				if (prefabEnt->mCollider_Circle.second)
					componentsList.push_back(std::make_pair(&prefabEnt->mCollider_Circle.first, typeid(Collider_Circle).name()));
				if (prefabEnt->mPlayer.second)
					componentsList.push_back(std::make_pair(&prefabEnt->mPlayer.first, typeid(Player).name()));
				if (prefabEnt->mTileComponent.second)
					componentsList.push_back(std::make_pair(&prefabEnt->mTileComponent.first, typeid(TileComponent).name()));
				if (prefabEnt->mAnimationComponent.second)
					componentsList.push_back(std::make_pair(&prefabEnt->mAnimationComponent.first, typeid(AnimationComponent).name()));
				if (prefabEnt->mPlayerCollisionHandler.second)
					componentsList.push_back(std::make_pair(&prefabEnt->mPlayerCollisionHandler.first, typeid(PlayerCollisionHandler).name()));
				if (prefabEnt->mEnemyCollisionHandler.second)
					componentsList.push_back(std::make_pair(&prefabEnt->mEnemyCollisionHandler.first, typeid(EnemyCollisionHandler).name()));
				if (prefabEnt->mGameUIObjectHandler.second)
					componentsList.push_back(std::make_pair(&prefabEnt->mGameUIObjectHandler.first, typeid(GameUIObjectHandler).name()));
				if (prefabEnt->mGameUITextHandler.second)
					componentsList.push_back(std::make_pair(&prefabEnt->mGameUITextHandler.first, typeid(GameUITextHandler).name()));
				if (prefabEnt->mCSScriptComponent.second)
					componentsList.push_back(std::make_pair(&prefabEnt->mCSScriptComponent.first, typeid(CSScriptComponent).name()));
				if (prefabEnt->mAIComponent.second)
					componentsList.push_back(std::make_pair(&prefabEnt->mAIComponent.first, typeid(AIComponent).name()));
				if (prefabEnt->mSoundComponent.second)
					componentsList.push_back(std::make_pair(&prefabEnt->mSoundComponent.first, typeid(SoundComponent).name()));
				if (prefabEnt->mButtonWrapper.second)
					componentsList.push_back(std::make_pair(&prefabEnt->mButtonWrapper.first, typeid(ButtonWrapper).name()));
				if (prefabEnt->mKeyComponent.second)
					componentsList.push_back(std::make_pair(&prefabEnt->mKeyComponent.first, typeid(Key).name()));
				if (prefabEnt->mKeyCollisionHandler.second)
					componentsList.push_back(std::make_pair(&prefabEnt->mKeyCollisionHandler.first, typeid(KeyCollisionHandler).name()));
				if (prefabEnt->mKeyWallComponent.second)
					componentsList.push_back(std::make_pair(&prefabEnt->mKeyWallComponent.first, typeid(KeyWall).name()));
				if (prefabEnt->mKeyWallCollisionHandler.second)
					componentsList.push_back(std::make_pair(&prefabEnt->mKeyWallCollisionHandler.first, typeid(KeyWallCollisionHandler).name()));
				if (prefabEnt->mVFXEntityComponent.second)
					componentsList.push_back(std::make_pair(&prefabEnt->mVFXEntityComponent.first, typeid(VFXEntity).name()));
				if (prefabEnt->mWindCollisionHandler.second)
					componentsList.push_back(std::make_pair(&prefabEnt->mWindCollisionHandler.first, typeid(WindCollisionHandler).name()));
				if (prefabEnt->mWind.second)
					componentsList.push_back(std::make_pair(&prefabEnt->mWind.first, typeid(Wind).name()));
				if (prefabEnt->mEndGoal.second)
					componentsList.push_back(std::make_pair(&prefabEnt->mEndGoal.first, typeid(EndGoal).name()));
				if (prefabEnt->mPlayOnceSound.second)
					componentsList.push_back(std::make_pair(&prefabEnt->mPlayOnceSound.first, typeid(PlayOnceSoundComponent).name()));
				if (prefabEnt->mDialogueTrigger.second)
					componentsList.push_back(std::make_pair(&prefabEnt->mDialogueTrigger.first, typeid(DialogueTrigger).name()));
				if (prefabEnt->mNextCPCollisionHandler.second)
					componentsList.push_back(std::make_pair(&prefabEnt->mNextCPCollisionHandler.first, typeid(NextCPCollisionHandler).name()));
			}
			else
			{
				if (ComponentBase* comp = ComponentManager::GetInstance()->GetComponent<Transform>(ent->GetID()))
					componentsList.push_back(std::make_pair(comp, typeid(Transform).name()));
				if (ComponentBase* comp = ComponentManager::GetInstance()->GetComponent<Renderer>(ent->GetID()))
					componentsList.push_back(std::make_pair(comp, typeid(Renderer).name()));
				if (ComponentBase* comp = ComponentManager::GetInstance()->GetComponent<PhysicsComponent>(ent->GetID()))
					componentsList.push_back(std::make_pair(comp, typeid(PhysicsComponent).name()));
				if (ComponentBase* comp = ComponentManager::GetInstance()->GetComponent<Collider_AABB>(ent->GetID()))
					componentsList.push_back(std::make_pair(comp, typeid(Collider_AABB).name()));
				if (ComponentBase* comp = ComponentManager::GetInstance()->GetComponent<Collider_Circle>(ent->GetID()))
					componentsList.push_back(std::make_pair(comp, typeid(Collider_Circle).name()));
				if (ComponentBase* comp = ComponentManager::GetInstance()->GetComponent<Player>(ent->GetID()))
					componentsList.push_back(std::make_pair(comp, typeid(Player).name()));
				if (ComponentBase* comp = ComponentManager::GetInstance()->GetComponent<TileComponent>(ent->GetID()))
					componentsList.push_back(std::make_pair(comp, typeid(TileComponent).name()));
				if (ComponentBase* comp = ComponentManager::GetInstance()->GetComponent<AnimationComponent>(ent->GetID()))
					componentsList.push_back(std::make_pair(comp, typeid(AnimationComponent).name()));
				if (ComponentBase* comp = ComponentManager::GetInstance()->GetComponent<PlayerCollisionHandler>(ent->GetID()))
					componentsList.push_back(std::make_pair(comp, typeid(PlayerCollisionHandler).name()));
				if (ComponentBase* comp = ComponentManager::GetInstance()->GetComponent<EnemyCollisionHandler>(ent->GetID()))
					componentsList.push_back(std::make_pair(comp, typeid(EnemyCollisionHandler).name()));
				if (ComponentBase* comp = ComponentManager::GetInstance()->GetComponent<GameUIObjectHandler>(ent->GetID()))
					componentsList.push_back(std::make_pair(comp, typeid(GameUIObjectHandler).name()));
				if (ComponentBase* comp = ComponentManager::GetInstance()->GetComponent<GameUITextHandler>(ent->GetID()))
					componentsList.push_back(std::make_pair(comp, typeid(GameUITextHandler).name()));
				if (ComponentBase* comp = ComponentManager::GetInstance()->GetComponent<CSScriptComponent>(ent->GetID()))
					componentsList.push_back(std::make_pair(comp, typeid(CSScriptComponent).name()));
				if (ComponentBase* comp = ComponentManager::GetInstance()->GetComponent<AIComponent>(ent->GetID()))
					componentsList.push_back(std::make_pair(comp, typeid(AIComponent).name()));
				if (ComponentBase* comp = ComponentManager::GetInstance()->GetComponent<SoundComponent>(ent->GetID()))
					componentsList.push_back(std::make_pair(comp, typeid(SoundComponent).name()));
				if (ComponentBase* comp = ComponentManager::GetInstance()->GetComponent<ButtonWrapper>(ent->GetID()))
					componentsList.push_back(std::make_pair(comp, typeid(ButtonWrapper).name()));
				if (ComponentBase* comp = ComponentManager::GetInstance()->GetComponent<Key>(ent->GetID()))
					componentsList.push_back(std::make_pair(comp, typeid(Key).name()));
				if (ComponentBase* comp = ComponentManager::GetInstance()->GetComponent<KeyCollisionHandler>(ent->GetID()))
					componentsList.push_back(std::make_pair(comp, typeid(KeyCollisionHandler).name()));
				if (ComponentBase* comp = ComponentManager::GetInstance()->GetComponent<KeyWall>(ent->GetID()))
					componentsList.push_back(std::make_pair(comp, typeid(KeyWall).name()));
				if (ComponentBase* comp = ComponentManager::GetInstance()->GetComponent<KeyWallCollisionHandler>(ent->GetID()))
					componentsList.push_back(std::make_pair(comp, typeid(KeyWallCollisionHandler).name()));
				if (ComponentBase* comp = ComponentManager::GetInstance()->GetComponent<VFXEntity>(ent->GetID()))
					componentsList.push_back(std::make_pair(comp, typeid(VFXEntity).name()));
				if (ComponentBase* comp = ComponentManager::GetInstance()->GetComponent<WindCollisionHandler>(ent->GetID()))
					componentsList.push_back(std::make_pair(comp, typeid(WindCollisionHandler).name()));
				if (ComponentBase* comp = ComponentManager::GetInstance()->GetComponent<Wind>(ent->GetID()))
					componentsList.push_back(std::make_pair(comp, typeid(Wind).name()));
				if (ComponentBase* comp = ComponentManager::GetInstance()->GetComponent<EndGoal>(ent->GetID()))
					componentsList.push_back(std::make_pair(comp, typeid(EndGoal).name()));
				if (ComponentBase* comp = ComponentManager::GetInstance()->GetComponent<PlayOnceSoundComponent>(ent->GetID()))
					componentsList.push_back(std::make_pair(comp, typeid(PlayOnceSoundComponent).name()));
				if (ComponentBase* comp = ComponentManager::GetInstance()->GetComponent<DialogueTrigger>(ent->GetID()))
					componentsList.push_back(std::make_pair(comp, typeid(DialogueTrigger).name()));
				if (ComponentBase* comp = ComponentManager::GetInstance()->GetComponent<NextCPCollisionHandler>(ent->GetID()))
					componentsList.push_back(std::make_pair(comp, typeid(NextCPCollisionHandler).name()));
			}

		};
		std::vector<std::pair<ComponentBase*, std::string>> componentsList;
	};

	/*!
	 * @brief 
	 *	Saves a level into a .scene file.
	 * @param levelName 
	 *	Name of the level to save as.
	 * @param saveLocation 
	 *	Path to the location to save to.
	 * @return 
	 *	True is save successful false otherwise.
	*/
	static bool SaveLevel(std::string levelName, float* _colorArray, std::string saveLocation);

	/*!
	 * @brief 
	 *	Loads the level based on the loaded .scene file using AssetManager.
	 * @param levelName 
	 *	Name of the level to load.
	 * @return 
	 *	True of loaded false otherwise.
	*/
	static bool LoadLevel(std::string levelName, float* _colorArray);

	/*!
	 * @brief 
	 *	Creates a blank scene file.
	 *	Defaults the scene name to "NewScene".
	 * @param saveLocation 
	 *	Path to save the scene file to.
	*/
	static void CreateBlankScene(std::string saveLocation);

	/*!
	 * @brief 
	 *	Creates a new entity by loading a prefab.
	 * @param prefabName 
	 *	Name of the prefab.
	 * @return 
	 *	The pointer to the created entity.
	*/
	static Entity* LoadPrefab(std::string prefabName);

	/*!
	 * @brief 
	 *	Create an entity from a rapidjson::value.
	 * @param object 
	 *	The rapidjson::value to load.
	 * @return 
	 *	The pointer to the created entity.
	*/
	static Entity* CreateEntityFromJSON(rapidjson::Value& object);

	/*!
	 * @brief 
	 *	Save a prefab based on the Entity*.
	 * @param prefabName 
	 *	Name to save the prefab as.
	 * @param ent 
	 *	The pointer to the Entity.
	 * @param saveLocation 
	 *	The location to save to.
	 * @param isPrefab 
	 *	If saving from a prefab or entity.
	 * @param prefabInfo 
	 *	The prefabInfo to save from if its a prefab.
	 * @return 
	 *	True if save successful false otherwise.
	*/
	static bool SavePrefab(std::string prefabName, Entity* ent, std::string saveLocation, bool isPrefab = false, PrefabInfo* prefabInfo = nullptr);

	/*!
	 * @brief 
	 *	Template function to add a component to an entity.
	 * @tparam T 
	 *	The type of the component.
	 * @param ent 
	 *	The pointer to the entity to add to.
	*/
	template<typename T>
	static void AddComponentFromJSON(Entity* ent);

	/*!
	 * @brief 
	 *	Loads and stores the info based on the prefab name.
	 * @param prefabName 
	 *	The name of the prefab to load.
	 * @param info 
	 *	The reference to the info to save to.
	 * @return 
	 *	True if successful false otherwise.
	*/
	static bool LoadPrefabInfo(std::string prefabName, PrefabInfo& info);

	/*!
	 * @brief 
	 *	Updates all prefabs in the current scene
	*/
	static void UpdateAllPrefabs();

private:
	//To be used by SavePrefab

	/*!
	 * @brief 
	 *	Saves an entity as a prefab
	 * @param ent 
	 *	The pointer to the entity to save
	 * @param isPrefab 
	 *	If the entity is already prefab
	 * @param prefabInfo 
	 *	The info to save
	 * @return 
	 *	True if successful | false otherwise
	*/
	static bool SaveEntity(Entity* ent, bool isPrefab = false, PrefabInfo* prefabInfo = nullptr);

	//Recurive saving of childs

	/*!
	 * @brief 
	 *	Recursive function to save a prefab with its childs
	 * @param childID 
	 *	Entity id of the child
	 * @param isPrefab 
	 *	If the entity is already a prefab
	 * @param prefabInfo 
	 *	The info to save
	 * @return 
	 *	True if successful | false otherwise
	*/
	static bool SaveChildRecursive(EntityID childID, bool isPrefab = false, PrefabInfo* prefabInfo = nullptr);

};

#endif
