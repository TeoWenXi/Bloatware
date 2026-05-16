/*!
All content © 2023 DigiPen Institute of Technology Singapore, all rights reserved.
@file       EngineDll.h
@author		HEAR Li Heng (100%)
@co-author

----------------------------------------------------------------------------

@course     CSD2401F23
@section    CSD2401F23-A
@project    CSD2401F23 Software Engineering Project 3

@brief      This file acts as a bridge between c# scripts and our Engine

            All this functions will be registered by MonoManager so that
            it will be linked to EngineScript.cs for our C# scripts to call
            since script will not be able to GetInstance and also to convert
            some parameter types since C# doesnt support pointers

*//*______________________________________________________________________*/
#include "EngineDll.h"

#include "InputManager.h"
#include "ComponentManager.h"
#include "LevelLoader.h"
#include "Scene.h"
#include "MonoManager.h"
#include "ParticleSystem.h"

double Engine::Time_GetDeltaTime()
{
    return WindowsManager::GetInstance()->mDeltaTime;
}

double Engine::Time_GetRunTime()
{
    return glfwGetTime();
}

void Engine::Console_Log(MonoString* _message)
{
    char* s = mono_string_to_utf8(_message);
    std::string msg(s);
    mono_free(s);
    msg += '\n';
#if _ENGINE
    DebugLogger::GetInstance()->Log(msg.c_str());
#endif // _ENGINE
}

void* Engine::Engine_GetComponent(EntityID _entityID, MonoString* _type)
{
    char* s = mono_string_to_utf8(_type);
    std::string type(s);
    mono_free(s);
    if (type == "Transform")
        return (void*)ComponentManager::GetInstance()->GetComponent<Transform>(_entityID);
    else if (type == "PhysicsComponent")
        return (void*)ComponentManager::GetInstance()->GetComponent<PhysicsComponent>(_entityID);
    else if (type == "BoxCollider")
        return (void*)ComponentManager::GetInstance()->GetComponent<Collider_AABB>(_entityID);
    else if (type == "Renderer")
        return (void*)ComponentManager::GetInstance()->GetComponent<Renderer>(_entityID);
    else if (type == "TileComponent")
        return (void*)ComponentManager::GetInstance()->GetComponent<TileComponent>(_entityID);
    else if (type == "DialogueTrigger")
        return (void*)ComponentManager::GetInstance()->GetComponent<DialogueTrigger>(_entityID);
    return 0;
}

bool Engine::Engine_GetTriggerEntered(EntityID _entityID)
{
    Collider_AABB* col = ComponentManager::GetInstance()->GetComponent<Collider_AABB>(_entityID);
    if (col)
    {
        return col->GetIsTriggered();
    }
    return false;
}

void Engine::Camera_GetPosition(Vector3& _pos)
{
    _pos = GraphicsSystem::GetInstance()->mCamera.mCurrCamInfo.mPosition;
}

void Engine::Camera_SetPosition(Vector3& _pos)
{
    GraphicsSystem::GetInstance()->mCamera.mCurrCamInfo.mPosition = _pos;
}

void Engine::Camera_GetHorizontalClampRange(Vector3& _range)
{
    _range = GraphicsSystem::GetInstance()->mCamera.mCurrCamInfo.mClampHoriRange;
}

void Engine::Camera_SetHorizontalClampRange(Vector3& _range)
{
    GraphicsSystem::GetInstance()->mCamera.mCurrCamInfo.mClampHoriRange = _range;
}

void Engine::Camera_GetVerticalClampRange(Vector3& _range)
{
    _range = GraphicsSystem::GetInstance()->mCamera.mCurrCamInfo.mClampVertRange;
}

void Engine::Camera_SetVerticalClampRange(Vector3& _range)
{
    GraphicsSystem::GetInstance()->mCamera.mCurrCamInfo.mClampVertRange = _range;
}

void Engine::Camera_GetWorldSize(Vector3& _size)
{
    _size = GraphicsSystem::GetInstance()->mWorldSize;
}

void Engine::Camera_SetWorldSize(Vector3& _size)
{
     GraphicsSystem::GetInstance()->mWorldSize = _size;
}

void Engine::Camera_GetZoom(float& _zoom)
{
    _zoom = GraphicsSystem::GetInstance()->mCamera.mCurrCamInfo.mZoom;
}

void Engine::Camera_SetZoom(float& _zoom)
{
    GraphicsSystem::GetInstance()->mCamera.mCurrCamInfo.mZoom = _zoom;
}

void Engine::Camera_ScreenShake()
{
    GraphicsSystem::GetInstance()->mCamera.DoScreenshake();
}

void Engine::UpdateCameraTarget(EntityID _entID)
{
    UNREFERENCED_PARAMETER(_entID);
    //GraphicsSystem::GetInstance()->mCamera.targetObject = _entID;
}

void Engine::DialogueTrigger_PlayTrigger(EntityID _entID)
{
    ComponentManager::GetInstance()->GetComponent<DialogueTrigger>(_entID)->PlayTrigger();
}

void Engine::CamWaypoint_GetCurrentCamWaypoint(Vector3& _pos, Vector3& _spawn)
{
    _pos = SceneManager::GetInstance()->GetActiveScene()->mCamWaypoints[SceneManager::GetInstance()->GetActiveScene()->mCamWaypointIndex].mPosition;
    _spawn = SceneManager::GetInstance()->GetActiveScene()->mCamWaypoints[SceneManager::GetInstance()->GetActiveScene()->mCamWaypointIndex].mPlayerSpawnPos;
}

void Engine::CamWaypoint_NextCamWaypoint()
{
    SceneManager::GetInstance()->GetActiveScene()->MoveNextCamWaypoint();
}

void Engine::ParticleSystem_SpawnParticles(Vector3& _pos, Vector3& _offset)
{
    ParticleSystem::GetInstance()->SpawnParticles(_pos, _offset);
}

void Engine::GameObject_SpawnEntity(EntityID& _entID)
{
    _entID = EntityManager::GetInstance()->CreateEntity()->GetID();
}

void Engine::GameObject_SpawnEntityPrefab(EntityID& _entID, MonoString*& _prefabName)
{
    char* s = mono_string_to_utf8(_prefabName);
    std::string pName(s);
    mono_free(s);
    _entID = LevelLoader::LoadPrefab(pName)->GetID();
}

void Engine::GameObject_DeleteEntity(EntityID _entID)
{
    EntityManager::GetInstance()->DestroyEntityAll(_entID);
}

void Engine::GameObject_GetName(EntityID _entID, MonoString*& _name)
{
    std::string name = EntityManager::GetInstance()->GetEntity(_entID)->GetName();
    std::wstring data(name.begin(), name.end());
    _name = mono_string_from_utf16(const_cast<mono_unichar2*>(data.c_str()));
}

void Engine::GameObject_SetName(EntityID _entID, MonoString*& _name)
{
    char* s = mono_string_to_utf8(_name);
    std::string newName(s);
    mono_free(s);
    EntityManager::GetInstance()->GetEntity(_entID)->SetName(newName);
}

void Engine::GameObject_RunFuncExternal(EntityID _entID, MonoString* _name)
{
    char* s = mono_string_to_utf8(_name);
    std::string funcName(s);
    mono_free(s);
    MonoManager::GetInstance()->RunFunction(_entID, funcName);
}

void Engine::GameObject_FindEntity(EntityID& _entID, MonoString* _name)
{
    char* s = mono_string_to_utf8(_name);
    std::string entName(s);
    mono_free(s);
    Entity* ent = EntityManager::GetInstance()->GetEntity(entName);
    if (ent)
        _entID = ent->GetID();
    else
        _entID = 0;
}

void Engine::SoundManager_PlaySoundExternal(MonoString* _name, float _volume, float _pitch)
{
    char* s = mono_string_to_utf8(_name);
    std::string soundName(s);
    mono_free(s);
    SoundManager::GetInstance()->PlaySound(soundName, _volume, _pitch);
}

void Engine::SoundManager_GetChannelVolume(float& _volume)
{
    _volume = SoundManager::GetInstance()->GetChannelVolume();
}

void Engine::SoundManager_SetChannelVolume(float& _volume)
{
    SoundManager::GetInstance()->SetChannelVolume(_volume);
}

void Engine::SoundManager_GetChannelPitch(float& _pitch)
{
    _pitch = SoundManager::GetInstance()->GetChannelPitch();
}

void Engine::SoundManager_SetChannelPitch(float& _pitch)
{
    SoundManager::GetInstance()->SetChannelPitch(_pitch);
}

bool Engine::InputManager_GetKey(int _keycode)
{
    return InputManager::GetInstance()->GetKey(_keycode);
}

bool Engine::InputManager_GetKeyTriggered(int _keycode)
{
    return InputManager::GetInstance()->GetKeyTriggered(_keycode);
}

bool Engine::InputManager_GetKeyReleased(int _keycode)
{
    return InputManager::GetInstance()->GetKeyReleased(_keycode);
}

bool Engine::InputManager_GetAction(MonoString* _action)
{
    char* s = mono_string_to_utf8(_action);
    std::string action(s);
    mono_free(s);
    return InputManager::GetInstance()->GetAction(action.c_str());
}

bool Engine::InputManager_GetActionTriggered(MonoString* _action)
{
    char* s = mono_string_to_utf8(_action);
    std::string action(s);
    mono_free(s);
    return InputManager::GetInstance()->GetActionTriggered(action.c_str());
}

bool Engine::InputManager_GetActionReleased(MonoString* _action)
{
    char* s = mono_string_to_utf8(_action);
    std::string action(s);
    mono_free(s);
    return InputManager::GetInstance()->GetActionReleased(action.c_str());
}

void Engine::Transform_GetPosition(EntityID _entID, Vector3& _pos)
{
   _pos = ComponentManager::GetInstance()->GetComponent<Transform>(_entID)->GetPosition();
}

void Engine::Transform_SetPosition(EntityID _entID, Vector3& _pos)
{
    ComponentManager::GetInstance()->GetComponent<Transform>(_entID)->SetPosition(_pos);
}

void Engine::Transform_GetLocalPosition(EntityID _entID, Vector3& _pos)
{
    _pos = ComponentManager::GetInstance()->GetComponent<Transform>(_entID)->GetLocalPosition();
}

void Engine::Transform_GetScale(EntityID _entID, Vector3& _scale)
{
    _scale = ComponentManager::GetInstance()->GetComponent<Transform>(_entID)->mScale;
}

void Engine::Transform_SetScale(EntityID _entID, Vector3& _scale)
{
    ComponentManager::GetInstance()->GetComponent<Transform>(_entID)->mScale = _scale;
}

void Engine::Transform_GetRotation(EntityID _entID, float& _rot)
{
    _rot = ComponentManager::GetInstance()->GetComponent<Transform>(_entID)->mRotation;
}

void Engine::Transform_SetRotation(EntityID _entID, float& _rot)
{
    ComponentManager::GetInstance()->GetComponent<Transform>(_entID)->mRotation = _rot;
}

void Engine::Transform_GetChild(EntityID _entID, EntityID& childID, EntityID& childEntityID)
{
    childEntityID = EntityManager::GetInstance()->GetEntity(_entID)->GetChildEntList()[childID];
}

void Engine::Transform_GetChildCount(EntityID _entID, int& _childCount)
{
    _childCount = static_cast<int>(EntityManager::GetInstance()->GetEntity(_entID)->GetChildEntList().size());
}

void Engine::Transform_GetParent(EntityID _childID, EntityID& _parentID)
{
    _parentID = EntityManager::GetInstance()->GetEntity(_childID)->GetParentID();
}

void Engine::Transform_SetParent(EntityID _childID, EntityID& _parentID)
{
    if (_parentID != 0)
    {
        EntityManager::GetInstance()->GetEntity(_parentID)->AddChild(_childID);
        EntityManager::GetInstance()->GetEntity(_childID)->SetParentID(_parentID);
    }
    else//Set to null
    {
        EntityManager::GetInstance()->GetEntity(EntityManager::GetInstance()->GetEntity(_childID)->GetParentID())->RemoveChild(_childID);
        EntityManager::GetInstance()->GetEntity(_childID)->RemoveParenting();
    }

 
}

void Engine::Physics_GetVelocity(EntityID _entID, Vector3& _vel)
{
    _vel = ComponentManager::GetInstance()->GetComponent<PhysicsComponent>(_entID)->mCurrentVelocity;
}

void Engine::Physics_SetVelocity(EntityID _entID, Vector3& _vel)
{
    ComponentManager::GetInstance()->GetComponent<PhysicsComponent>(_entID)->mCurrentVelocity = _vel;
}

void Engine::Physics_GetGravity(EntityID _entID, Vector3& _grav)
{
    _grav = ComponentManager::GetInstance()->GetComponent<PhysicsComponent>(_entID)->mGravity;
}

void Engine::Physics_SetGravity(EntityID _entID, Vector3& _grav)
{
    ComponentManager::GetInstance()->GetComponent<PhysicsComponent>(_entID)->mGravity = _grav;
}

void Engine::Physics_GetAcceleration(EntityID _entID, Vector3& _acc)
{
    _acc = ComponentManager::GetInstance()->GetComponent<PhysicsComponent>(_entID)->mCurrentAcceleration;
}

void Engine::Physics_SetAcceleration(EntityID _entID, Vector3& _acc)
{
    ComponentManager::GetInstance()->GetComponent<PhysicsComponent>(_entID)->mCurrentAcceleration = _acc;
}

void Engine::Physics_GetMass(EntityID _entID, float& _mass)
{
    _mass = ComponentManager::GetInstance()->GetComponent<PhysicsComponent>(_entID)->mMass;
}

void Engine::Physics_SetMass(EntityID _entID, float& _mass)
{
    ComponentManager::GetInstance()->GetComponent<PhysicsComponent>(_entID)->mMass = _mass;
}

void Engine::Physics_GetWindTunnelForce(EntityID _entID, Vector3& _force)
{
    _force = ComponentManager::GetInstance()->GetComponent<PhysicsComponent>(_entID)->mWindTunnelForce;
}

void Engine::Physics_SetWindTunnelForce(EntityID _entID, Vector3& _force)
{
    ComponentManager::GetInstance()->GetComponent<PhysicsComponent>(_entID)->mWindTunnelForce = _force;
}

void Engine::Physics_GetJumpForce(EntityID _entID, Vector3& _force)
{
    _force = ComponentManager::GetInstance()->GetComponent<PhysicsComponent>(_entID)->mJumpForce;
}

void Engine::Physics_SetJumpForce(EntityID _entID, Vector3& _force)
{
    ComponentManager::GetInstance()->GetComponent<PhysicsComponent>(_entID)->mJumpForce = _force;
}

void Engine::Physics_GetObjectBig(EntityID _entID, Vector3& _big)
{
    _big = ComponentManager::GetInstance()->GetComponent<PhysicsComponent>(_entID)->mObjBigScale;
}

void Engine::Physics_SetObjectBig(EntityID _entID, Vector3& _big)
{
    ComponentManager::GetInstance()->GetComponent<PhysicsComponent>(_entID)->mObjBigScale = _big;
}

void Engine::Physics_GetCharacterSmall(EntityID _entID, bool& _isSmall)
{
    _isSmall = ComponentManager::GetInstance()->GetComponent<PhysicsComponent>(_entID)->mIsCharacterSmall;
}

void Engine::Physics_SetCharacterSmall(EntityID _entID, bool& _isSmall)
{
    ComponentManager::GetInstance()->GetComponent<PhysicsComponent>(_entID)->mIsCharacterSmall = _isSmall;
}

void Engine::Physics_GetCharacterSize(EntityID _entID, float& _size)
{
    _size = ComponentManager::GetInstance()->GetComponent<PhysicsComponent>(_entID)->mCharacterSizeFactor;
}

void Engine::Physics_SetCharacterSize(EntityID _entID, float& _size)
{
    ComponentManager::GetInstance()->GetComponent<PhysicsComponent>(_entID)->mCharacterSizeFactor = _size;
}

void Engine::Physics_GetCharacterWind(EntityID _entID, float& _wind)
{
    _wind = ComponentManager::GetInstance()->GetComponent<PhysicsComponent>(_entID)->mCharacterWindFactor;
}

void Engine::Physics_SetCharacterWind(EntityID _entID, float& _wind)
{
    ComponentManager::GetInstance()->GetComponent<PhysicsComponent>(_entID)->mCharacterWindFactor = _wind;
}

void Engine::Physics_SetEnable(EntityID _entID, bool _enable)
{
    ComponentManager::GetInstance()->GetComponent<PhysicsComponent>(_entID)->mEnabled = _enable;
}

void Engine::TileComponent_GetMinBound(Vector3& _min)
{
    _min = TileComponent::mMinBound;
}

void Engine::TileComponent_SetMinBound(Vector3& _min)
{
   TileComponent::mMinBound = _min;
}

void Engine::TileComponent_GetTileSize(int& _tileSize)
{
    _tileSize = TileComponent::mTileSize;
}

void Engine::TileComponent_SetTileSize(int& _tileSize)
{
    TileComponent::mTileSize = _tileSize;
}

void Engine::TileComponent_GetMapWidth(int& _width)
{
    _width = TileComponent::mMapWidth;
}

void Engine::TileComponent_SetMapWidth(int& _width)
{
    TileComponent::mMapWidth = _width;
}

void Engine::TileComponent_GetMapHeight(int& _height)
{
    _height = TileComponent::mMapHeight;
}

void Engine::TileComponent_SetMapHeight(int& _height)
{
    TileComponent::mMapHeight = _height;
}

void Engine::TileComponent_GetCollisionFlag(EntityID _entID, int& _flag)
{
    _flag = ComponentManager::GetInstance()->GetComponent<TileComponent>(_entID)->mGridCollisionFlag;
}

void Engine::TileComponent_SetCollisionFlag(EntityID _entID, int& _flag)
{
    ComponentManager::GetInstance()->GetComponent<TileComponent>(_entID)->mGridCollisionFlag = _flag;
}

void Engine::TileComponent_GetBinaryMap(int _x, int _y, int& _data)
{
    _data = TileComponent::mBinaryMap[_x][_y];
}

void Engine::TileComponent_SetBinaryMap(int _x, int _y, int& _data)
{
    TileComponent::mBinaryMap[_x][_y] = _data;
}

void Engine::GetCellValue_YX_Second(int _x, int _y, int& _valueX, int& _valueY)
{
    _valueX = TileComponent::GetCellValue_YX(_x, _y).second.first;
    _valueY = TileComponent::GetCellValue_YX(_x, _y).second.second;
}

void Engine::BoxCollider_GetOffset(EntityID _entID, Vector3& _offset)
{
    _offset = ComponentManager::GetInstance()->GetComponent<Collider_AABB>(_entID)->mOffset;
}

void Engine::BoxCollider_SetOffset(EntityID _entID, Vector3& _offset)
{
    ComponentManager::GetInstance()->GetComponent<Collider_AABB>(_entID)->mOffset = _offset;
}

void Engine::BoxCollider_GetScale(EntityID _entID, Vector3& _scale)
{
    _scale = ComponentManager::GetInstance()->GetComponent<Collider_AABB>(_entID)->mScale;
}

void Engine::BoxCollider_SetScale(EntityID _entID, Vector3& _scale)
{
    ComponentManager::GetInstance()->GetComponent<Collider_AABB>(_entID)->mScale = _scale;
}

void Engine::Renderer_GetShaderName(EntityID _entID, MonoString*& _shaderName)
{
    std::wstring data(ComponentManager::GetInstance()->GetComponent<Renderer>(_entID)->mShaderName.begin(), ComponentManager::GetInstance()->GetComponent<Renderer>(_entID)->mShaderName.end());
    _shaderName = mono_string_from_utf16(const_cast<mono_unichar2*>(data.c_str()));
}

void Engine::Renderer_SetShaderName(EntityID _entID, MonoString*& _shaderName)
{
    char* s = mono_string_to_utf8(_shaderName);
    std::string shdName(s);
    mono_free(s);
    ComponentManager::GetInstance()->GetComponent<Renderer>(_entID)->mShaderName = shdName;
}

void Engine::Renderer_GetTextureName(EntityID _entID, MonoString*& _texName)
{
    std::wstring data(ComponentManager::GetInstance()->GetComponent<Renderer>(_entID)->mTextureName.begin(), ComponentManager::GetInstance()->GetComponent<Renderer>(_entID)->mTextureName.end());
    _texName = mono_string_from_utf16(const_cast<mono_unichar2*>(data.c_str()));
}

void Engine::Renderer_SetTextureName(EntityID _entID, MonoString*& _texName)
{
    ComponentManager::GetInstance()->GetComponent<Renderer>(_entID)->mTextureName = std::string(mono_string_to_utf8(_texName));
}

void Engine::Renderer_SetUniformFloat(EntityID _entID, MonoString* _uniName, float val)
{
    char* s = mono_string_to_utf8(_uniName);
    std::string uniformName(s);
    mono_free(s);
    GraphicsSystem::GetInstance()->AddUniformToSet(_entID, uniformName, val);
}

void Engine::Renderer_SetUniformVec4(EntityID _entID, MonoString* _uniName, Vector4& val)
{
    char* s = mono_string_to_utf8(_uniName);
    std::string uniformName(s);
    mono_free(s);
    GraphicsSystem::GetInstance()->AddUniformToSet(_entID, uniformName, val);
}

void Engine::Renderer_GetTextureIDExternal(unsigned int& _texID, MonoString* _texName)
{
    char* s = mono_string_to_utf8(_texName);
    std::string texName(s);
    mono_free(s);
    AssetManager::ImageAsset* texture{ nullptr };
    AssetManager::GetInstance()->GetAsset(texName, texture);
    _texID = texture->mTexID;
}

void Engine::Player_SetPlayerID(EntityID _entID)
{
    Player::mPlayerID = _entID;
}
