/*!
All content © 2023 DigiPen Institute of Technology Singapore, all rights reserved.
@file       EditorMainScene.h
@author		SIM Mong Chuan (60%)
@co-author	TEO Wen Xi (10%)
@co-author	TAN Kang Zheng (10%)
@co-author	HEAR Li Heng (10%)
@co-author	CAI Wenjin (10%)

----------------------------------------------------------------------------

@course     CSD2401F23
@section    CSD2401F23-A
@project    CSD2401F23 Software Engineering Project 3

@brief      Header file for the Editor Main Scene
*//*______________________________________________________________________*/
#if _ENGINE
#ifndef EDITOR_MAIN_SCENE_H
#define EDITOR_MAIN_SCENE_H

#include "Scene.h"
#include "SceneManager.h"
#include "EntityManager.h"
#include "DebugLogger.h"

//For editing components
#include "ComponentsHeader.h"
#include "LevelLoader.h"

//IMGUI
#pragma warning(push, 0)
#include <imgui.h>
#include <imgui_internal.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#pragma warning(pop)


class Vector3;//forward declaration for ImguiVector3/ImguiFloat
class EditorMainScene : public Scene
{
public:

	//Scene functions
	/*!
	 * @brief
	 * Load function of the scene
	*/
	void Load();

	/*!
	 * @brief
	 * Init function of the scene
	*/
	void Init();

	/*!
	 * @brief
	 * Handles input for the scene
	 * @param dt
	 * System delta time
	*/
	void Input(double _dt);

	/*!
	 * @brief 
	 * Handles update for the scene
	 * @param dt 
	 * System delta time
	*/
	void Update(double _dt);

	/*!
	 * @brief 
	 * Handles the scene when it is paused
	 * @param dt 
	 * System delta time
	*/
	void Pause(double _dt);

	/*!
	 * @brief 
	 * Handles Render for the scene
	 * @param dt 
	 * System delta time
	*/
	void Render(double _dt);

	/*!
	 * @brief 
	 * Handles freeing of memory for the scene
	*/
	void Free();

	/*!
	 * @brief 
	 * Handles the unloading of assets in the scene
	*/
	void Exit();

	bool mIsEditorPlaying = false;

private:
	bool mShowDebugInfoWindow = true;
	bool mShowMenuUI = false;
	bool mShowApplicationDebugLog = false;
	bool mShowAudioUI = true;
	bool mShowSystemTime = true;
	bool mShowAssetUI = true;
	bool mShowTestWindow = true;
	bool mShowPlayerInfo = true;
	bool mShowHierarchy = true;
	bool mShowProperties = true;
	bool mShowComponentList = false;
	bool mShowUnsupportedLog = false;
	bool mShowResolutionList = false;
	bool mShowPlayPause = true;
	bool mShowSaveLoad = true;
	bool mShowCameraConfig = true;
	bool mShowMainMenuBar = true;
	bool mShowScene = true;
	bool mShowTilePalette = true;

	// render layers toggle
	bool mShowRenderLayers = false;
	bool mBackGroundLayerEnabled = true;
	bool mMidGroundLayerEnabled = true;
	bool mForeGroundLayerEnabled = true;
	
	// For editor font type, font scale
	std::string mEditorFont = "Assets/Fonts/Montserrat-Regular.ttf";
	float mEditorFontScale{};
	float mEditorFontInputSize = 50.f;
	float mNumofLinesOnScreen = 50.f;

	bool mSceneFullScreen = false;

	//For Hierarchy & Editor
	Entity* mSelectedEnt = nullptr;
	bool mJumpToEnt = false;
	int mHiercJumpCounter = 0;

	//For prefab editor
	std::string mCurrentOpenPath{};
	std::string mPrintCurrentFolder{};
	std::string mBasePath{};

	std::string mSceneToLoadName{};
	char mNameBuffer[1024]{};
	ImGuiID mSelectedID;
	enum FileActions
	{
		RENAME,
		DELETE,
		NONE
	}mFileActions{ NONE };//use to track what actions should be performed to file
	std::string mCurrentDraggingExt{};

	bool mCurrentlyDraggingEntity = false;
	Entity* mCurrentDragEnt;
	Vector3 mDragEntStartVec, mMouseStartVec, mMouseOffset;

	bool mBackupCreated = false;
	bool mSnapToGrid = false;

	std::queue<int> mCamWaypointsDelQueue;

	//Tile Palette
	std::string mSelectedTileName = "";
	bool mTilePaletteEnabled = false;

	/*!
	 * @brief
	 * Creates an entity without a collider. For 2500 objects with 60fps
	*/
	void CreateMysteryBox();
	/*!
	 * @brief
	 * Creates a prefab
	*/
	void CreatePrefab();

	/*!
	 * @brief
	 * Creates a player entity with a collider
	*/
	void CreatePlayer();
	/*!
	 * @brief
	 * Creates an entity with a collider
	*/
	void CreateObject1();

	/*!
	 * @brief
	 * Searches for an entity that the mousePos is in
	*/
	Entity* SelectObject(Vector3 _mousePos);

	/*!
	 * @brief
	 * Updates relevant values related to mouse position, as well as the dragged entity
	*/
	void DragEntity(Entity* _ent);

	/*!
	 * @brief Display system stats like fps
	 * @param _debugInfoCloseButton 
	 * boolean of the current window if open or closed
	*/
	void DebugInfoWindow(bool _debugInfoCloseButton);
	
	/*!
	 * @brief Display the in-system console
	 * @param g 
	 * ImGuiCurrentContext
	 * @param _applicationDebugLogCloseButton 
	 * boolean of the current window if open or closed
	*/
	void ApplicationDebugLogWindow(ImGuiContext* g, bool _applicationDebugLogCloseButton);

	/*!
	 * @brief Display the time taken for each system of the engine
	 * @param _profilerCloseButton 
	 * boolean of the current window if open or closed
	*/
	void ProfilerWindow(bool _profilerCloseButton);

	/*!
	 * @brief
	 * Used when dragging an item from one place to another
	 * @param _finalPath
	*/
	void MyImGuiDragTargetMove(std::string _finalPath);

	/*!
	 * @brief
	 * Used in Asset Browser to check if the current folder has a sub folder inside
	 * @param _folderPath
	 * @param _treeFlags
	 * @return
	 * True if the current folder has a sub folder inside
	*/
	bool RecursiveFolderDisplay(std::string _folderPath, ImGuiTreeNodeFlags _treeFlags);

	//To store prefab infos
	Entity mPrefabEnt;
	LevelLoader::PrefabInfo mPrefabInfo;
	
	/*!
	 * @brief
	 * Spawns wall
	*/
	void SpawnWall(int _height, int _width);
	/*!
	 * @brief
	 * Spawns platform
	*/
	void SpawnPlatform(int _height, int _width);
	/*!
	 * @brief
	 * Creates grid
	*/
	void CreateGrid();

	/*!
	 * @brief 
	 * Spawns wind tunnels
	*/
	void SpawnWindTunnelUp();

	/*!
	 * @brief
	 * Spawns wind tunnels
	*/
	void CreateWindTunnels();

	/*!
	 * @brief 
	 * Spawns laser
	*/
	void CreateLaser();

	/*!
	 * @brief 
	 * Shows the list of render layers in ImGui.
	*/
	void ShowRenderLayers();

	/*!
	 * @brief 
	 * Toggles the visibility of all game objects in the scene with the specified
	 * render layer based on whether the ImGui checkbox is checked or not.
	 * @param _layer 
	 * The render layer being checked.
	 * @param _enabled 
	 * Whether to enable or disable the visibility of all game objects in that render
	 * layer.
	*/
	void ToggleRenderLayer(Renderer::RENDER_LAYER _layer, bool _enabled);

	/*!
	 * @brief 
	 * Recursive function to display the entity list int he hierarchy imgui
	 * @param _currNode 
	 * The current node to be displayed
	*/
	void DisplayHierarchyNode(Entity* _currNode);

	/*!
	 * @brief 
	 * Function containing the imgui popup menu for entity creation
	 * @param _popupName 
	 * Name of the popup menu
	 * @param _currNode 
	 * Used for error checking
	*/
	void EntityCreationPopup(std::string _popupName, Entity* _currNode);
};

#endif
#endif // _ENGINE
