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
#pragma once

#include <mono/jit/jit.h>//for MonoString
#include "Vector3.h"
#include "Vector4.h"
#include "ComponentsHeader.h"

namespace Engine
{
	//--------------Time--------------
	/*!
	 * @brief
	 * Gets the delta time elapsed
	 * @return
	 * DeltaTime
	*/
	double Time_GetDeltaTime();

	/*!
	 * @brief 
	 * Gets the total run time since the start
	 * @return 
	 * RunTime
	*/
	double Time_GetRunTime();


	//--------------Debugger--------------
	/*!
	 * @brief 
	 * Passes a string from C# to DebugLogger
	 * @param _message 
	 * Message to print
	*/
	void Console_Log(MonoString* _message);

	//--------------MonoBehaviour--------------
	/*!
	 * @brief 
	 * GetComponent for C#
	 * @param _entityID 
	 * The entity to get component from
	 * @param _type 
	 * The type of component to get
	 * @return 
	 * Pointer to the component
	*/
	void* Engine_GetComponent(EntityID _entityID, MonoString* _type);

	//--------------Collision--------------
	/*!
	 * @brief 
	 * Gets if trigger entered
	 * @param _entityID 
	 * Entity of trigger
	 * @return 
	 * True if entered, false otherwise
	*/
	bool Engine_GetTriggerEntered(EntityID _entityID);

	//--------------Camera--------------
	/*!
	 * @brief 
	 * Gets the position of the camera
	 * @param _pos 
	 * The reference to put the camera position to
	*/
	void Camera_GetPosition(Vector3& _pos);

	/*!
	 * @brief 
	 * Sets the position of the camera
	 * @param _pos 
	 * The value to set
	*/
	void Camera_SetPosition(Vector3& _pos);

	/*!
	 * @brief 
	 * Gets the camera's horizontal clamp range
	 * @param _range 
	 * The reference to put the value to
	*/
	void Camera_GetHorizontalClampRange(Vector3& _range);

	/*!
	 * @brief 
	 * Sets the camera's horizontal clamp range
	 * @param _range 
	 * The value to set
	*/
	void Camera_SetHorizontalClampRange(Vector3& _range);

	/*!
	 * @brief
	 * Gets the camera's horizontal clamp range
	 * @param _range 
	 * The reference to put the value to
	*/
	void Camera_GetVerticalClampRange(Vector3& _range);

	/*!
	 * @brief 
	 * Sets the camera's vertical clamp range
	 * @param _range 
	 * The value to set
	*/
	void Camera_SetVerticalClampRange(Vector3& _range);

	/*!
	 * @brief 
	 * Gets the camera's world size
	 * @param _size 
	 * The reference to put the value to
	*/
	void Camera_GetWorldSize(Vector3& _size);

	/*!
	 * @brief 
	 * Sets the camera's world size
	 * @param _size 
	 * The value to set
	*/
	void Camera_SetWorldSize(Vector3& _size);

	/*!
	 * @brief 
	 * Gets the camera's zoom value
	 * @param _zoom 
	 * The reference to put the value to
	*/
	void Camera_GetZoom(float& _zoom);

	/*!
	 * @brief 
	 * Sets the camera's zoom
	 * @param _zoom 
	 * The value to set
	*/
	void Camera_SetZoom(float& _zoom);

	/*!
	 * @brief 
	 * Call's the camera's shake
	*/
	void Camera_ScreenShake();

	/*!
	 * @brief 
	 * Change the camera's target
	 * @param _entID 
	 * The target's id to change to
	*/
	void UpdateCameraTarget(EntityID _entID);

	
	/*!
	 * @brief 
	 * Calls the DialogueTrigger's PlayTrigger function
	 * @param _entID 
	 * The entity containing the DialogueTrigger component
	*/
	void DialogueTrigger_PlayTrigger(EntityID _entID);

	/*!
	 * @brief 
	 * Gets the current CamWaypoint
	 * @param _pos 
	 * The reference to put the value to
	 * @param _spawn 
	 * The reference to put the value to
	*/
	void CamWaypoint_GetCurrentCamWaypoint(Vector3& _pos, Vector3& _spawn);

	/*!
	 * @brief 
	 * Calls the CamWaypoint's NextCamWaypoint function
	*/
	void CamWaypoint_NextCamWaypoint();

	/*!
	 * @brief 
	 * Spawn particles using ParticleSystem
	 * @param _pos 
	 * The position to spawn at
	 * @param _offset 
	 * The offset to spawn
	*/
	void ParticleSystem_SpawnParticles(Vector3& _pos, Vector3& _offset);

	/*!
	 * @brief 
	 * Spawns an entity
	 * @param _entID 
	 * The reference to put the new entity's id to
	*/
	void GameObject_SpawnEntity(EntityID& _entID);

	/*!
	 * @brief 
	 * Spawns an entity based on a prefab
	 * @param _entID 
	 * The reference to put the new entity's id to
	 * @param _prefabName 
	 * The name of the prefab
	*/
	void GameObject_SpawnEntityPrefab(EntityID& _entID, MonoString*& _prefabName);

	/*!
	 * @brief 
	 * Deletes an entity
	 * @param _entID 
	 * The id of the entity to delete
	*/
	void GameObject_DeleteEntity(EntityID _entID);

	/*!
	 * @brief 
	 * Gets the name of an entity
	 * @param _entID 
	 * The id of the entity
	 * @param _name 
	 * The reference to put the name to
	*/
	void GameObject_GetName(EntityID _entID, MonoString*& _name);

	/*!
	 * @brief 
	 * Sets the name of an entity
	 * @param _entID 
	 * The id of the entity
	 * @param _name 
	 * The name to set to
	*/
	void GameObject_SetName(EntityID _entID, MonoString*& _name);

	/*!
	 * @brief 
	 * Runs an external function of another script
	 * @param _entID 
	 * The id of the entity that contains the script
	 * @param _name 
	 * The name of the function to run
	*/
	void GameObject_RunFuncExternal(EntityID _entID, MonoString* _name);

	/*!
	 * @brief 
	 * Finds an entity based on the name
	 * @param _entID 
	 * The reference to put the id of the entity
	 * @param _name 
	 * The name of the entity to find
	*/
	void GameObject_FindEntity(EntityID& _entID, MonoString* _name);

	
	//--------------Audio--------------

	/*!
	 * @brief 
	 * Plays an external sound
	 * @param _name 
	 * The name of the sound
	 * @param _volume 
	 * Volume of the sound
	 * @param _pitch 
	 * Pitch of the sound
	*/
	void SoundManager_PlaySoundExternal(MonoString* _name, float _volume, float _pitch);

	/*!
	 * @brief 
	 * Gets the channel volume
	 * @param _volume 
	 * The reference to put the volume to
	*/
	void SoundManager_GetChannelVolume(float& _volume);

	/*!
	 * @brief 
	 * Sets the channel volume
	 * @param _volume 
	 * The volume to set to
	*/
	void SoundManager_SetChannelVolume(float& _volume);

	/*!
	 * @brief
	 * Gets the channel volume
	 * @param _pitch
	 * The reference to put the pitch to
	*/
	void SoundManager_GetChannelPitch(float& _pitch);

	/*!
	 * @brief 
	 * Sets the channel pitch
	 * @param _pitch 
	 * The pitch to set to
	*/
	void SoundManager_SetChannelPitch(float& _pitch);
	
	//--------------InputManager--------------
	/*!
	 * @brief
	 * Calls the InputManager.GetKey().
	 * @param _keycode
	 * Keycode param for InputManager.GetKey().
	 * @return
	 * Returns the value returned by InputManager.GetKey().
	*/
	bool InputManager_GetKey(int _keycode);

	/*!
	 * @brief
	 * Calls the InputManager.GetKeyTriggered().
	 * @param _keycode
	 * Keycode param for InputManager.GetKeyTriggered().
	 * @return
	 * Returns the value returned by InputManager.GetKeyTriggered().
	*/
	bool InputManager_GetKeyTriggered(int _keycode);

	/*!
	 * @brief
	 * Calls the InputManager.GetKeyReleased().
	 * @param _keycode
	 * Keycode param for InputManager.GetKeyReleased().
	 * @return
	 * Returns the value returned by InputManager.GetKeyReleased().
	*/
	bool InputManager_GetKeyReleased(int _keycode);

	/*!
	 * @brief
	 * Calls the InputManager.GetAction().
	 * @param _keycode
	 * Keycode param for InputManager.GetAction().
	 * @return
	 * Returns the value returned by InputManager.GetAction().
	*/
	bool InputManager_GetAction(MonoString* _action);

	/*!
	 * @brief
	 * Calls the InputManager.GetActionTriggered().
	 * @param _keycode
	 * Keycode param for InputManager.GetActionTriggered().
	 * @return
	 * Returns the value returned by InputManager.GetActionTriggered().
	*/
	bool InputManager_GetActionTriggered(MonoString* _action);

	/*!
	 * @brief
	 * Calls the InputManager.GetActionReleased().
	 * @param _keycode
	 * Keycode param for InputManager.GetActionReleased().
	 * @return
	 * Returns the value returned by InputManager.GetActionReleased().
	*/
	bool InputManager_GetActionReleased(MonoString* _action);

	/*!
	 * @brief 
	 * Gets the transform position
	 * @param _entID 
	 * Entity of component
	 * @param _pos 
	 * The reference to vector3 to set
	*/
	void Transform_GetPosition(EntityID _entID, Vector3& _pos);

	/*!
	 * @brief 
	 * Set the transform position
	 * @param _entID 
	 * Entity of component
	 * @param _pos 
	 * The value to set
	*/
	void Transform_SetPosition(EntityID _entID, Vector3& _pos);

	/*!
	 * @brief 
	 * Gets the transform's local position
	 * @param _entID 
	 * Entity of component
	 * @param _pos 
	 * The reference to vector3 to set
	*/
	void Transform_GetLocalPosition(EntityID _entID, Vector3& _pos);

	/*!
	 * @brief 
	 * Gets the transform's scale
	 * @param _entID 
	 * Entity of component
	 * @param _scale 
	 * The reference to vector3 to set
	*/
	void Transform_GetScale(EntityID _entID, Vector3& _scale);

	/*!
	 * @brief 
	 * Sets the transform's scale
	 * @param _entID 
	 * Entity of component
	 * @param _scale 
	 * The value to set
	*/
	void Transform_SetScale(EntityID _entID, Vector3& _scale);

	/*!
	 * @brief 
	 * Gets the transform's rotation
	 * @param _entID 
	 * Entity of component
	 * @param _rot 
	 * The reference to the float to set
	*/
	void Transform_GetRotation(EntityID _entID, float& _rot);

	/*!
	 * @brief 
	 * Sets the transform's rotation
	 * @param _entID 
	 * Entity of component
	 * @param _rot 
	 * The value to set
	*/
	void Transform_SetRotation(EntityID _entID, float& _rot);

	/*!
	 * @brief 
	 * Gets the entityID of the child
	 * @param _entID 
	 * Entity id of parent
	 * @param childID 
	 * Index of child
	 * @param childEntityID 
	 * Entity id of parent
	*/
	void Transform_GetChild(EntityID _entID, EntityID& _childID, EntityID& _childEntityID);

	/*!
	 * @brief 
	 * Gets the child count of an entity
	 * @param _entID 
	 * The id of the entity
	 * @param _childCount 
	 * The reference to put the childcount to
	*/
	void Transform_GetChildCount(EntityID _entID, int& _childCount);

	/*!
	 * @brief 
	 * Gets the parent of the entity
	 * @param _childID 
	 * The id of the child entity
	 * @param _parentID 
	 * The reference to the id to put the parent's id to
	*/
	void Transform_GetParent(EntityID _childID, EntityID& _parentID);

	/*!
	 * @brief 
	 * Sets the parent of an entity
	 * @param _childID 
	 * The id of the child entity
	 * @param _parentID 
	 * The id of the parent to be set to
	*/
	void Transform_SetParent(EntityID _childID, EntityID& _parentID);

	
		

	/*!
	 * @brief 
	 * Gets the physics' velocity
	 * @param _entID 
	 * Entity of component
	 * @param _vel 
	 * The reference to the vector3 to set
	*/
	void Physics_GetVelocity(EntityID _entID, Vector3& _vel);

	/*!
	 * @brief 
	 * Sets the physics' velocity
	 * @param _entID 
	 * Entity of component
	 * @param _vel 
	 * The value to set
	*/
	void Physics_SetVelocity(EntityID _entID, Vector3& _vel);

	/*!
	 * @brief 
	 * Gets the physics' gravity
	 * @param _entID 
	 * Entity of component
	 * @param _grav 
	 * The reference to vector3 to set
	*/
	void Physics_GetGravity(EntityID _entID, Vector3& _grav);

	/*!
	 * @brief 
	 * Sets the physics' gravity
	 * @param _entID 
	 * Entity of component
	 * @param _grav 
	 * The value to set
	*/
	void Physics_SetGravity(EntityID _entID, Vector3& _grav);

	/*!
	 * @brief 
	 * Gets the physics' acceleration
	 * @param _entID 
	 * Entity of component
	 * @param _acc 
	 * The reference to vector3 to set
	*/
	void Physics_GetAcceleration(EntityID _entID, Vector3& _acc);

	/*!
	 * @brief 
	 * Sets the physics' acceleration
	 * @param _entID 
	 * Entity of component
	 * @param _acc 
	 * The reference to vector3 to set
	*/
	void Physics_SetAcceleration(EntityID _entID, Vector3& _acc);


	/*!
	 * @brief
	 * Gets the physics' mass
	 * @param _entID
	 * Entity of component
	 * @param _acc
	 * The reference to float to set
	*/
	void Physics_GetMass(EntityID _entID, float& _mass);

	/*!
	 * @brief
	 * Sets the physics' mass
	 * @param _entID
	 * Entity of component
	 * @param _acc
	 * The reference to float to set
	*/
	void Physics_SetMass(EntityID _entID, float& _mass);

	/*!
	 * @brief 
	 * Gets the wind tunnel force
	 * @param _entID 
	 * The entity id 
	 * @param _force 
	 * The reference to put the force to
	*/
	void Physics_GetWindTunnelForce(EntityID _entID, Vector3& _force);

	/*!
	 * @brief 
	 * Sets the wind tunnel force
	 * @param _entID 
	 * The entity id
	 * @param _force
	 * The force to set
	*/
	void Physics_SetWindTunnelForce(EntityID _entID, Vector3& _force);

	/*!
	 * @brief
	 * Gets the jump force
	 * @param _entID
	 * The entity id
	 * @param _force
	 * The reference to put the force to
	*/
	void Physics_GetJumpForce(EntityID _entID, Vector3& _force);

	/*!
	 * @brief
	 * Sets the jump force
	 * @param _entID
	 * The entity id
	 * @param _force
	 * The force to set
	*/
	void Physics_SetJumpForce(EntityID _entID, Vector3& _force);

	/*!
	 * @brief
	 * Gets the objectBigScale value
	 * @param _entID
	 * The entity id
	 * @param _big
	 * The reference to put the objectBigScale to
	*/
	void Physics_GetObjectBig(EntityID _entID, Vector3& _big);

	/*!
	 * @brief
	 * Sets the objectBigScale value
	 * @param _entID
	 * The entity id
	 * @param _big
	 * The objectBigScale to set
	*/
	void Physics_SetObjectBig(EntityID _entID, Vector3& _big);
	
	/*!
	 * @brief
	 * Gets the isCharacterSmall value
	 * @param _entID
	 * The entity id
	 * @param _isSmall
	 * The reference to put the isCharacterSmall to
	*/
	void Physics_GetCharacterSmall(EntityID _entID, bool& _isSmall);

	/*!
	 * @brief
	 * Sets the isCharacterSmall value
	 * @param _entID
	 * The entity id
	 * @param _big
	 * The isCharacterSmall to set
	*/
	void Physics_SetCharacterSmall(EntityID _entID, bool& _isSmall);

	/*!
	 * @brief
	 * Gets the mCharacterSizeFactor value
	 * @param _entID
	 * The entity id
	 * @param _size
	 * The reference to put the size to
	*/
	void Physics_GetCharacterSize(EntityID _entID, float& _size);


	/*!
	 * @brief
	 * Sets the mCharacterSizeFactor value
	 * @param _entID
	 * The entity id
	 * @param _size
	 * The size to set
	*/
	void Physics_SetCharacterSize(EntityID _entID, float& _size);

	/*!
	 * @brief
	 * Gets the character's wind value
	 * @param _entID
	 * The entity id
	 * @param _wind
	 * The reference to put the wind to
	*/
	void Physics_GetCharacterWind(EntityID _entID, float& _wind);

	/*!
	 * @brief
	 * Sets the character's wind value
	 * @param _entID
	 * The entity id
	 * @param _wind
	 * The wind to set
	*/
	void Physics_SetCharacterWind(EntityID _entID, float& _wind);

	/*!
	 * @brief 
	 * Sets the enable flag
	 * @param _entID 
	 * The entity id
	 * @param _enable 
	 * True or false
	*/
	void Physics_SetEnable(EntityID _entID, bool _enable);

	
	
	//--------------TileComponent--------------

	/*!
	 * @brief 
	 * Gets the minbound of the tilecomponent
	 * @param _min 
	 * The reference to put the min to
	*/
	void TileComponent_GetMinBound(Vector3& _min);

	/*!
	 * @brief 
	 * Sets the minbound of the tilecomponent
	 * @param _min 
	 * The min to set
	*/
	void TileComponent_SetMinBound(Vector3& _min);

	/*!
	 * @brief
	 * Gets the tilesize of the tilecomponent
	 * @param _tileSize
	 * The reference to put the tilesize to
	*/
	void TileComponent_GetTileSize(int& _tileSize);

	/*!
	 * @brief
	 * Sets the tilesize of the tilecomponent
	 * @param _tileSize
	 * The tilesize to set
	*/
	void TileComponent_SetTileSize(int& _tileSize);

	/*!
	 * @brief
	 * Gets the mapwidth of the tilecomponent
	 * @param _width
	 * The reference to put the mapwidth to
	*/
	void TileComponent_GetMapWidth(int& _width);

	/*!
	 * @brief
	 * Sets the mapwidth of the tilecomponent
	 * @param _width
	 * The width to set
	*/
	void TileComponent_SetMapWidth(int& _width);

	/*!
	 * @brief
	 * Gets the mapheight of the tilecomponent
	 * @param _height
	 * The reference to put the mapheight to
	*/
	void TileComponent_GetMapHeight(int& _height);

	/*!
	 * @brief
	 * Sets the mapheight of the tilecomponent
	 * @param _height
	 * The height to set
	*/
	void TileComponent_SetMapHeight(int& _height);

	/*!
	 * @brief
	 * Gets the collisionflag of the tilecomponent
	 * @param _entID
	 * The entity id 
	 * @param _flag
	 * The reference to put the collisionflag to
	*/
	void TileComponent_GetCollisionFlag(EntityID _entID, int& _flag);

	/*!
	 * @brief
	 * Sets the collisionflag of the tilecomponent
	 * @param _entID
	 * The entity id
	 * @param _flag
	 * The collisionflag to set
	*/
	void TileComponent_SetCollisionFlag(EntityID _entID, int& _flag);

	/*!
	 * @brief 
	 * Gets the value of the binary map of the tilecomponent
	 * @param _x 
	 * The x position
	 * @param _y 
	 * The y position
	 * @param _data 
	 * The reference to put the data to
	*/
	void TileComponent_GetBinaryMap(int _x,int _y,int& _data);

	/*!
	 * @brief
	 * Sets the value of the binary map of the tilecomponent
	 * @param _x
	 * The x position
	 * @param _y
	 * The y position
	 * @param _data
	 * The value to set to
	*/
	void TileComponent_SetBinaryMap(int _x, int _y, int& _data);

	/*!
	 * @brief 
	 * Gets the cell value
	 * @param _x 
	 * The x position
	 * @param _y 
	 * The y position
	 * @param _valueX 
	 * The x value
	 * @param _valueY 
	 * The y value
	*/
	void GetCellValue_YX_Second(int _x,int _y,int& _valueX, int& _valueY);
	
	
	//--------------BoxCollider--------------
	/*!
	 * @brief
	 * Gets the offset of a boxCollider
	 * @param _entID 
	 * Entity of component
	 * @param _offset 
	 * The reference to vector3 to update to the offset
	*/
	void BoxCollider_GetOffset(EntityID _entID, Vector3& _offset);

	/*!
	 * @brief
	 * Sets the offset of a boxCollider
	 * @param _entID 
	 * Entity of component
	 * @param _offset 
	 * The offset to set
	*/
	void BoxCollider_SetOffset(EntityID _entID, Vector3& _offset);

	/*!
	 * @brief 
	 * Gets the scale of the box collider
	 * @param _entID 
	 * Entity of component
	 * @param _scale 
	 * The reference to vector3 to update to the scale
	*/
	void BoxCollider_GetScale(EntityID _entID, Vector3& _scale);

	/*!
	 * @brief 
	 * Sets the scale of the boxCollider
	 * @param _entID 
	 * Entity of component
	 * @param _scale 
	 * The scale to set
	*/
	void BoxCollider_SetScale(EntityID _entID, Vector3& _scale);


	//--------------Renderer--------------
	/*!
	 * @brief 
	 * Gets the shader name of the renderer
	 * @param _entID 
	 * Entity of component
	 * @param _shaderName 
	 * The reference to the MonoString to update to the name
	*/
	void Renderer_GetShaderName(EntityID _entID, MonoString*& _shaderName);

	/*!
	 * @brief 
	 * Sets the shader name of the renderer
	 * @param _entID 
	 * Entity of component
	 * @param _shaderName 
	 * The shader name to set
	*/
	void Renderer_SetShaderName(EntityID _entID, MonoString*& _shaderName);

	/*!
	 * @brief 
	 * Gets the texture name attached to the renderer
	 * @param _entID 
	 * Entity of component
	 * @param _texName 
	 * The reference to the MonoString to update to the name
	*/
	void Renderer_GetTextureName(EntityID _entID, MonoString*& _texName);

	/*!
	 * @brief 
	 * Sets the texture name attached to the renderer
	 * Which also sets the texture
	 * @param _entID 
	 * Entity of component
	 * @param _texName 
	 * There texture name to set
	*/
	void Renderer_SetTextureName(EntityID _entID, MonoString*& _texName);

	/*!
	 * @brief 
	 * Sets the uniform float in a shader
	 * @param _entID 
	 * Entity of component
	 * @param _uniName 
	 * Name of the uniform
	 * @param val 
	 * Value to set to
	*/
	void Renderer_SetUniformFloat(EntityID _entID, MonoString* _uniName, float val);

	/*!
	 * @brief 
	 * Sets the uniform Vec4 in a shader
	 * @param _entID 
	 * Entity of component
	 * @param _uniName 
	 * Name of the uniform
	 * @param val 
	 * Value to set to
	*/
	void Renderer_SetUniformVec4(EntityID _entID, MonoString* _uniName, Vector4& val);

	/*!
	 * @brief 
	 * Gets the texture id 
	 * @param _texID 
	 * The reference to set the textureID to
	 * @param _texName 
	 * The name of the texture
	*/
	void Renderer_GetTextureIDExternal(unsigned int& _texID, MonoString* _texName);

	/*!
	 * @brief 
	 * Sets the player id
	 * @param _entID 
	 * The entity id to set
	*/
	void Player_SetPlayerID(EntityID _entID);
}
