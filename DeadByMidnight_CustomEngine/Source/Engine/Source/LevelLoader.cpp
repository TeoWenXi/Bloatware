/*!
All content © 2023 DigiPen Institute of Technology Singapore, all rights reserved.
@file       LevelLoader.cpp
@author     HEAR Li Heng (100%)
@co-author

----------------------------------------------------------------------------

@course     CSD2401F23
@section    CSD2401F23-A
@project    CSD2401F23 Software Engineering Project 3

@brief      Manages the loading of levels and also the spawning of gameobjects
*//*______________________________________________________________________*/

#include "LevelLoader.h"

#include "EntityManager.h"
#include "JSONManager.h"
#include "Scene.h"
#include "MonoManager.h"

std::unordered_map<std::string, std::list<EntityID>> prefabsList;

bool LevelLoader::SaveLevel(std::string sceneName, float* _colorArray, std::string saveLocation)
{
	JSONManager* jsonManager = JSONManager::GetInstance();
	jsonManager->CreateNewSave();
	jsonManager->Save("SceneName", sceneName);
	jsonManager->SaveObject("BackgroundColor", true);
	jsonManager->Pushback(_colorArray[0]);
	jsonManager->Pushback(_colorArray[1]);
	jsonManager->Pushback(_colorArray[2]);
	jsonManager->Pushback(_colorArray[3]);
	jsonManager->CloseObject();

	//Camera stuff
	jsonManager->Save("EditorCameraSpeed", GraphicsSystem::GetInstance()->mCamera.mCamMoveSpeed);
	jsonManager->Save("CameraThresholdHorizontal", (Vector3)GraphicsSystem::GetInstance()->mCamera.mHorizontalThreshold);
	jsonManager->Save("CameraThresholdVertical", (Vector3)GraphicsSystem::GetInstance()->mCamera.mVerticalThreshold);

	//Camera Waypoints
	jsonManager->SaveObject("CameraWaypoints", true);
	for (int i = 0; i < SceneManager::GetInstance()->GetActiveScene()->mCamWaypoints.size(); ++i)
	{
		jsonManager->SaveObject("", false);
		SceneManager::GetInstance()->GetActiveScene()->mCamWaypoints[i].Save();
		jsonManager->CloseObject();

	}
	jsonManager->CloseObject();

	//Save all components
	jsonManager->SaveObject("GameObjects", true);
	for (int i = 0; i < EntityManager::GetInstance()->mEntityList.size(); ++i)
	{
		Entity* currEnt = &EntityManager::GetInstance()->mEntityList[i];
		if (currEnt->GetParentID() != 0 || !currEnt->GetSaveInScene()) //We only need save parents and entities that want to be saved
			continue;
		jsonManager->SaveObject("", true);
		SaveEntity(currEnt);
		jsonManager->CloseObject();
	}

	jsonManager->CloseObject();
	//Create new object to store name
	jsonManager->SaveFile(sceneName, "scene", saveLocation);
	jsonManager->DeleteNewSave();

	return true;
}

bool LevelLoader::LoadLevel(std::string sceneName,float* _colorArray)
{
	if (!JSONManager::GetInstance()->LoadFile(sceneName))
		return false;
	//Clear current entities
	for (int i = static_cast<int>(EntityManager::GetInstance()->mEntityList.size()) - 1; i >= 0; --i)
	{
		EntityManager::GetInstance()->DestroyEntity(EntityManager::GetInstance()->mEntityList[i].GetID());
	}
	//Clear prefab list
	prefabsList.clear();

	rapidjson::Value* backgroundColor = JSONManager::GetInstance()->GetValue("BackgroundColor");
	_colorArray[0] = backgroundColor[0][0].GetFloat();
	_colorArray[1] = backgroundColor[0][1].GetFloat();
	_colorArray[2] = backgroundColor[0][2].GetFloat();
	_colorArray[3] = backgroundColor[0][3].GetFloat();

	//Camera
	GraphicsSystem::GetInstance()->mCamera.mCamMoveSpeed = JSONManager::GetInstance()->GetFloat("EditorCameraSpeed");
	GraphicsSystem::GetInstance()->mCamera.mHorizontalThreshold = JSONManager::GetInstance()->GetVec3("CameraThresholdHorizontal");
	GraphicsSystem::GetInstance()->mCamera.mVerticalThreshold = JSONManager::GetInstance()->GetVec3("CameraThresholdVertical");

	//Camera Waypoints
	SceneManager::GetInstance()->GetActiveScene()->mCamWaypoints.clear();
	rapidjson::Value* camWaypoints = JSONManager::GetInstance()->GetValue("CameraWaypoints");//this will return the array of waypoints
	for (unsigned int i = 0; i < camWaypoints->Size(); ++i)
	{
		CamWaypoint newWaypoint{};
		JSONManager::GetInstance()->LoadObject(&camWaypoints[0][i]);
		newWaypoint.Load();
		SceneManager::GetInstance()->GetActiveScene()->mCamWaypoints.push_back(newWaypoint);
		JSONManager::GetInstance()->CloseObject();
	}

	//Load
	rapidjson::Value* gameobjects = JSONManager::GetInstance()->GetValue("GameObjects");//this will return the array of gameobjects

	for (unsigned int i = 0; i < gameobjects->Size(); ++i)
	{
		//spawn gameObjects
		CreateEntityFromJSON(gameobjects[0][i]);
	}

	JSONManager::GetInstance()->CloseFile();

	//Loading specific init stuff
	for (auto ent : EntityManager::GetInstance()->mEntityList)
		if (ent.GetComponent<Transform>() != nullptr)
			ent.GetComponent<Transform>()->LoadPosFromParent();

	GraphicsSystem::GetInstance()->mCamera.SearchParallaxBG();
	GraphicsSystem::GetInstance()->mCamera.EndPermanentScreenshake();

	return true;
}

void LevelLoader::CreateBlankScene(std::string saveLocation)
{
	JSONManager* jsonManager = JSONManager::GetInstance();
	jsonManager->CreateNewSave();
	jsonManager->Save("SceneName", "NewScene");
	//Save all components
	jsonManager->SaveObject("GameObjects", true);
	jsonManager->CloseObject();
	jsonManager->SaveFile("NewScene", "scene", saveLocation);
	jsonManager->DeleteNewSave();
}

Entity* LevelLoader::LoadPrefab(std::string prefabName)
{
	JSONManager::GetInstance()->LoadFile(prefabName);
	rapidjson::Value* prefab = JSONManager::GetInstance()->GetValue("Prefab");

	Entity* retEntity = CreateEntityFromJSON(*prefab);

	JSONManager::GetInstance()->CloseFile();

	if (retEntity->GetComponent<Transform>() != nullptr)
		retEntity->GetComponent<Transform>()->LoadPosFromParent();
	if (std::find(prefabsList[prefabName].begin(), prefabsList[prefabName].end(), retEntity->GetID()) == prefabsList[prefabName].end())
		prefabsList[prefabName].push_back(retEntity->GetID());
	return retEntity;
}

Entity* LevelLoader::CreateEntityFromJSON(rapidjson::Value& object)
{
	Entity* retEntity = EntityManager::GetInstance()->CreateEntity();
	if (object.Size() < 1 || !object[0].HasMember("Name"))//make sure prefab file is in correct format
	{
#if _ENGINE
		DebugLogger::GetInstance()->Log("Error with Entity (Name Error)\n");
#endif // _ENGINE
		return nullptr;
	}
	retEntity->SetName(object[0]["Name"].GetString());//first value will always be name
	if (std::string(object[0]["Prefab"].GetString()) != "")
	{
		prefabsList[object[0]["Prefab"].GetString()].push_back(retEntity->GetID());
	}
	for (rapidjson::SizeType i = 1; i < object.Size(); ++i)
	{
		if (object[i].HasMember("Child"))//We need spawn a child for this object
		{
			retEntity->AddChild( CreateEntityFromJSON(object[i]["Child"])->GetID());
			continue;
		}
		else if (!object[i].HasMember("Component"))//prefab file has corrupted components
		{
#if _ENGINE
			DebugLogger::GetInstance()->Log("Error with prefab (Component Error)\n");
#endif // _ENGINE
			return retEntity;
		}
		std::string componentName = object[i]["Component"].GetString();
		JSONManager::GetInstance()->LoadObject(&object[i]);

		if (componentName == "Transform")
			AddComponentFromJSON<Transform>(retEntity);
		else if (componentName == "Renderer")
			AddComponentFromJSON<Renderer>(retEntity);
		else if (componentName == "PhysicsComponent")
			AddComponentFromJSON<PhysicsComponent>(retEntity);
		else if (componentName == "Collider_AABB")
			AddComponentFromJSON<Collider_AABB>(retEntity);
		else if (componentName == "Collider_Circle")
			AddComponentFromJSON<Collider_Circle>(retEntity);
		else if (componentName == "Player")
			AddComponentFromJSON<Player>(retEntity);
		else if (componentName == "TileComponent")
			AddComponentFromJSON<TileComponent>(retEntity);
		else if (componentName == "AnimationComponent")
			AddComponentFromJSON<AnimationComponent>(retEntity);
		else if (componentName == "PlayerCollisionHandler")
			AddComponentFromJSON<PlayerCollisionHandler>(retEntity);
		else if (componentName == "EnemyCollisionHandler")
			AddComponentFromJSON<EnemyCollisionHandler>(retEntity);
		else if (componentName == "GameUIObjectHandler")
			AddComponentFromJSON<GameUIObjectHandler>(retEntity);
		else if (componentName == "GameUITextHandler")
			AddComponentFromJSON<GameUITextHandler>(retEntity);
		else if (componentName == "CSScriptComponent")
		{
			AddComponentFromJSON<CSScriptComponent>(retEntity);
			retEntity->GetComponent<CSScriptComponent>()->SetScript(retEntity->GetComponent<CSScriptComponent>()->mScriptName);
		}
		else if (componentName == "AIComponent")
			AddComponentFromJSON<AIComponent>(retEntity);
		else if (componentName == "SoundComponent")
			AddComponentFromJSON<SoundComponent>(retEntity);
		else if (componentName == "ButtonWrapper")
			AddComponentFromJSON<ButtonWrapper>(retEntity);
		else if (componentName == "Key")
			AddComponentFromJSON<Key>(retEntity);
		else if (componentName == "KeyCollisionHandler")
			AddComponentFromJSON<KeyCollisionHandler>(retEntity);
		else if (componentName == "KeyWall")
			AddComponentFromJSON<KeyWall>(retEntity);
		else if (componentName == "KeyWallCollisionHandler")
			AddComponentFromJSON<KeyWallCollisionHandler>(retEntity);
		else if (componentName == "VFXEntity")
			AddComponentFromJSON<VFXEntity>(retEntity);
		else if (componentName == "WindCollisionHandler")
			AddComponentFromJSON<WindCollisionHandler>(retEntity);
		else if (componentName == "Wind")
			AddComponentFromJSON<Wind>(retEntity);
		else if (componentName == "EndGoal")
			AddComponentFromJSON<EndGoal>(retEntity);
		else if (componentName == "PlayOnceSoundComponent")
			AddComponentFromJSON<PlayOnceSoundComponent>(retEntity);
		else if (componentName == "DialogueTrigger")
			AddComponentFromJSON<DialogueTrigger>(retEntity);
		else if (componentName == "NextCPCollisionHandler")
			AddComponentFromJSON<NextCPCollisionHandler>(retEntity);

		JSONManager::GetInstance()->CloseObject();
	}

	if (retEntity->GetComponent<Player>() != nullptr)
		retEntity->GetComponent<Player>()->SetChildObjects();

	return retEntity;
}


bool LevelLoader::SaveEntity(Entity* ent, bool isPrefab, PrefabInfo* prefabInfo)
{
	JSONManager* jsonManager = JSONManager::GetInstance();

	//Create new object to store name
	jsonManager->SaveObject("", false);
	jsonManager->Save("Name", ent->GetName());

	EntityID id = ent->GetID();
	std::string prefabName = "";
	for (auto pairs : prefabsList)
	{
		if (std::find(pairs.second.begin(), pairs.second.end(), id) != pairs.second.end())
		{
			prefabName = pairs.first;
			break;
		}
	}
	jsonManager->Save("Prefab", prefabName);
	jsonManager->CloseObject();

	//Create new object to store components
	AllComponents comps(ent, isPrefab, prefabInfo);
	for (int i = 0; i < comps.componentsList.size(); ++i)
	{
		jsonManager->SaveObject("", false);
		comps.componentsList[i].first->Save();
		jsonManager->CloseObject();
	}


	//Now save children
	for (EntityID childID : ent->GetChildEntList())
	{
		SaveChildRecursive(childID, isPrefab, prefabInfo);
	}
	return true;
}

bool LevelLoader::SaveChildRecursive(EntityID childID, bool isPrefab, PrefabInfo* prefabInfo)
{
	JSONManager* jsonManager = JSONManager::GetInstance();
	Entity* ent = EntityManager::GetInstance()->GetEntity(childID);

	jsonManager->SaveObject("", false);
	jsonManager->SaveObject("Child", true);

	SaveEntity(ent, isPrefab, prefabInfo);

	jsonManager->CloseObject();
	jsonManager->CloseObject();

	return true;
}

bool LevelLoader::SavePrefab(std::string prefabName, Entity* ent, std::string saveLocation, bool isPrefab, PrefabInfo* prefabInfo)
{
	JSONManager* jsonManager = JSONManager::GetInstance();
	jsonManager->CreateNewSave();
	//Save all components
	jsonManager->SaveObject("Prefab", true);
	SaveEntity(ent, isPrefab, prefabInfo);

	jsonManager->CloseObject();

	jsonManager->SaveFile(prefabName, "prefab", saveLocation);
	jsonManager->DeleteNewSave();

	AssetManager::GetInstance()->Update();//Force update to get the updated prefab file
	//Update existing prefabs
	auto tempList = prefabsList[prefabName];
	for (auto id : tempList)
	{
		Transform temp;
		bool validTemp = false;
		//we want to not change pos and rot so we save first
		if (EntityManager::GetInstance()->GetEntity(id)->GetComponent<Transform>())
		{
			temp = *EntityManager::GetInstance()->GetEntity(id)->GetComponent<Transform>();
			validTemp = true;
		}
		EntityManager::GetInstance()->DestroyEntity(id);
		prefabsList[prefabName].remove(id);
		Entity* reloadedEntity = LoadPrefab(prefabName);
		//We reset pos and rot
		if (validTemp && reloadedEntity->GetComponent<Transform>())
		{
			reloadedEntity->GetComponent<Transform>()->SetPosition(temp.GetPosition());
			reloadedEntity->GetComponent<Transform>()->mRotation = temp.mRotation;
		}
	}
	//Save the current scene too

	return true;
}


bool LevelLoader::LoadPrefabInfo(std::string prefabName, PrefabInfo& info)
{
	//Clear previous loaded stuff
	info = PrefabInfo();
	JSONManager::GetInstance()->LoadFile(prefabName);

	rapidjson::Value& prefab = *(JSONManager::GetInstance()->GetValue("Prefab"));


	if (prefab.Size() < 1 || !prefab[0].HasMember("Name"))//make sure prefab file is in correct format
	{
#if _ENGINE
		DebugLogger::GetInstance()->Log("Error with Entity (Name Error)\n");
#endif // _ENGINE
		return false;
	}

	for (rapidjson::SizeType i = 1; i < prefab.Size(); ++i)
	{
		if (!prefab[i].HasMember("Component"))//prefab file has corrupted components
		{
#if _ENGINE
			DebugLogger::GetInstance()->Log("Error with prefab (Component Error)\n");
#endif // _ENGINE
			return false;
		}

		std::string componentName = prefab[i]["Component"].GetString();
		JSONManager::GetInstance()->LoadObject(&prefab[i]);
		if (componentName == "Transform")
		{
			info.mTransform.first.Load();
			info.mTransform.second = true;
		}
		else if (componentName == "Renderer")
		{
			info.mRenderer.first.Load();
			info.mRenderer.second = true;
		}
		else if (componentName == "PhysicsComponent")
		{
			info.mPhysicsComponent.first.Load();
			info.mPhysicsComponent.second = true;
		}
		else if (componentName == "Collider_AABB")
		{
			info.mCollider_AABB.first.Load();
			info.mCollider_AABB.second = true;
		}
		else if (componentName == "Collider_Circle")
		{
			info.mCollider_Circle.first.Load();
			info.mCollider_Circle.second = true;
		}
		else if (componentName == "Player")
		{
			info.mPlayer.first.Load();
			info.mPlayer.second = true;
		}
		else if (componentName == "TileComponent")
		{
			info.mTileComponent.first.Load();
			info.mTileComponent.second = true;
		}
		else if (componentName == "AnimationComponent")
		{
			info.mAnimationComponent.first.Load();
			info.mAnimationComponent.second = true;
		}
		else if (componentName == "PlayerCollisionHandler")
		{
			info.mPlayerCollisionHandler.first.Load();
			info.mPlayerCollisionHandler.second = true;
		}
		else if (componentName == "EnemyCollisionHandler")
		{
			info.mEnemyCollisionHandler.first.Load();
			info.mEnemyCollisionHandler.second = true;
		}
		else if (componentName == "GameUIObjectHandler")
		{
			info.mGameUIObjectHandler.first.Load();
			info.mGameUIObjectHandler.second = true;
		}
		else if (componentName == "GameUITextHandler")
		{
			info.mGameUITextHandler.first.Load();
			info.mGameUITextHandler.second = true;
		}
		else if (componentName == "AIComponent")
		{
			info.mAIComponent.first.Load();
			info.mAIComponent.second = true;
		}
		else if (componentName == "SoundComponent")
		{
			info.mSoundComponent.first.Load();
			info.mSoundComponent.second = true;
		}
		else if (componentName == "ButtonWrapper")
		{
			info.mButtonWrapper.first.Load();
			info.mButtonWrapper.second = true;
		}
		else if (componentName == "Key")
		{
			info.mKeyComponent.first.Load();
			info.mKeyComponent.second = true;
		}
		else if (componentName == "KeyCollisionHandler")
		{
			info.mKeyCollisionHandler.first.Load();
			info.mKeyCollisionHandler.second = true;
		}
		else if (componentName == "KeyWall")
		{
			info.mKeyWallComponent.first.Load();
			info.mKeyWallComponent.second = true;
		}
		else if (componentName == "KeyWallCollisionHandler")
		{
			info.mKeyWallCollisionHandler.first.Load();
			info.mKeyWallCollisionHandler.second = true;
		}
		else if (componentName == "VFXEntity")
		{
			info.mVFXEntityComponent.first.Load();
			info.mVFXEntityComponent.second = true;
		}
		else if (componentName == "WindCollisionHandler")
		{
			info.mWindCollisionHandler.first.Load();
			info.mWindCollisionHandler.second = true;
		}
		else if (componentName == "Wind")
		{
			info.mWind.first.Load();
			info.mWind.second = true;
		}
		else if (componentName == "EndGoal")
		{
			info.mEndGoal.first.Load();
			info.mEndGoal.second = true;
		}
		else if (componentName == "PlayOnceSoundComponent")
		{
			info.mPlayOnceSound.first.Load();
			info.mPlayOnceSound.second = true;
		}
		else if (componentName == "NextCPCollisionHandler")
		{
			info.mNextCPCollisionHandler.first.Load();
			info.mNextCPCollisionHandler.second = true;
		}
		JSONManager::GetInstance()->CloseObject();
	}

	JSONManager::GetInstance()->CloseFile();

	TileComponent::mLoadMap = true;
	return true;
}

void LevelLoader::UpdateAllPrefabs()
{
	for (auto tempList : prefabsList)
	{
		for (auto id : tempList.second)
		{
			Transform temp;
			bool validTemp = false;
			//we want to not change pos and rot so we save first
			if (EntityManager::GetInstance()->GetEntity(id)->GetComponent<Transform>())
			{
				temp = *EntityManager::GetInstance()->GetEntity(id)->GetComponent<Transform>();
				validTemp = true;
			}
			EntityManager::GetInstance()->DestroyEntity(id);
			Entity* reloadedEntity = LoadPrefab(tempList.first);
			//We reset pos and rot
			if (validTemp && reloadedEntity->GetComponent<Transform>())
			{
				reloadedEntity->GetComponent<Transform>()->SetPosition(temp.GetPosition());
				reloadedEntity->GetComponent<Transform>()->mRotation = temp.mRotation;
			}
		}
	}
}


template<typename T>
static void LevelLoader::AddComponentFromJSON(Entity* ent)
{
	T newComponent{};
	newComponent.Load();
	ent->AddComponent(newComponent);
}