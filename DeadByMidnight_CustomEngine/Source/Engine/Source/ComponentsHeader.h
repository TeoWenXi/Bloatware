/*!
All content © 2023 DigiPen Institute of Technology Singapore, all rights reserved.
@file       ComponentsHeader.h
@author     HEAR Li Heng (100%)
@co-author

----------------------------------------------------------------------------

@course     CSD2401F23
@section    CSD2401F23-A
@project    CSD2401F23 Software Engineering Project 3

@brief      Contains all components headers for easy referencing
*//*______________________________________________________________________*/
#ifndef COMPONENTS_HEADER_H
#define COMPONENTS_HEADER_H

//Basic
#include "Transform.h"

//Graphics
#include "Renderer.h"

//Physics
#include "PhysicsComponent.h"

//Colliders
#include "Collider_AABB.h"
#include "Collider_Circle.h"

//Player
#include "Player.h"

//Game Objects
#include "Key.h"
#include "KeyWall.h"
#include "Wind.h"

//Tile
#include "TileComponent.h"
//Animation
#include "AnimationComponent.h"
#include "VFXEntity.h"

//Handlers for observers
#include "PlayerCollisionHandler.h"
#include "EnemyCollisionHandler.h"
#include "KeyCollisionHandler.h"
#include "KeyWallCollisionHandler.h"
#include "WindCollisionHandler.h"
#include "GfxHandler.h"
#include "NextCheckpointCollisionHandler.h"

//For texts
#include "GameUIObjectHandler.h"
#include "GameUITextHandler.h"

//Script'
#include "CSScriptComponent.h"

//For AI
#include "AIComponent.h"

//Sound
#include "SoundComponent.h"
#include "PlayOnceSoundComponent.h"

//Buttons
#include "ButtonWrapper.h"

//Entry Triggers
#include "EndGoal.h"
#include "DialogueTrigger.h"
#endif

//Cinematic
#include "CinematicBars.h"