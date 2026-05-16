/*!
All content © 2023 DigiPen Institute of Technology Singapore, all rights reserved.
@file       EditorMainScene.cpp
@author		SIM Mong Chuan (60%)
@co-author	TEO Wen Xi (10%)
@co-author	TAN Kang Zheng (10%)
@co-author	HEAR Li Heng (10%)
@co-author	CAI Wenjin (10%)

----------------------------------------------------------------------------

@course     CSD2401F23
@section    CSD2401F23-A
@project    CSD2401F23 Software Engineering Project 3

@brief      Main Scene for the editor program
*//*______________________________________________________________________*/
#if _ENGINE
#define UNREFERENCED_PARAMETER(P) (P)
#define IMGUI_DEFINE_MATH_OPERATORS

#include <limits> //For getting of max value of uint64_t
#include <random> //For randomising of box generation positions
#include <format> //For converting values to string

#include "CoreEngine.h" //Get render fps for debug menu
#include "EditorMainScene.h"

#include "WindowsManager.h"
#include "LibraryManager.h"
#include "TextManager.h"
#include "AssetManager.h"
#include "InputManager.h"
#include "SoundManager.h"
#include "EntityManager.h"
#include "ButtonSelectManager.h"
#include "GraphicsSystem.h"
#include "Collision.h"
#include "Renderer.h"


void EditorMainScene::Init()
{
	mEditorFontScale = WindowsManager::GetInstance()->mHeight / (mNumofLinesOnScreen * mEditorFontInputSize);
	mEditorFontScale = 0.375f;
}

void EditorMainScene::Load()
{
	//Load Imgui settings
	mBasePath = mCurrentOpenPath = AssetManager::GetInstance()->GetCurrentPath();

	ImGuiContext* g = ImGui::GetCurrentContext();
	if (!AssetManager::GetInstance()->DoesFileExist("Assets/imgui.ini"))
	{
		ImGui::LoadIniSettingsFromDisk("Assets/default_imgui.ini");
		ImGui::SaveIniSettingsToDisk("Assets/imgui.ini");
	}
	g->IO.IniFilename = "Assets/imgui.ini";
	ImGui::LoadIniSettingsFromDisk(g->IO.IniFilename);

	g->IO.Fonts->AddFontFromFileTTF(mEditorFont.c_str(), mEditorFontInputSize);
	g->Style.Colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
	g->Style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
	g->Style.Colors[ImGuiCol_WindowBg] = ImVec4(0.04f, 0.07f, 0.12f, 0.94f);
	g->Style.Colors[ImGuiCol_ChildBg] = ImVec4(0.07f, 0.12f, 0.20f, 0.78f);
	g->Style.Colors[ImGuiCol_PopupBg] = ImVec4(0.04f, 0.13f, 0.27f, 0.86f);
	g->Style.Colors[ImGuiCol_Border] = ImVec4(0.43f, 0.43f, 0.50f, 0.50f);
	g->Style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	g->Style.Colors[ImGuiCol_FrameBg] = ImVec4(0.16f, 0.29f, 0.48f, 0.54f);
	g->Style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.40f);
	g->Style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.26f, 0.59f, 0.98f, 0.67f);
	g->Style.Colors[ImGuiCol_TitleBg] = ImVec4(0.04f, 0.04f, 0.04f, 1.00f);
	g->Style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.16f, 0.29f, 0.48f, 1.00f);
	g->Style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.00f, 0.00f, 0.00f, 0.51f);
	g->Style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
	g->Style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.02f, 0.02f, 0.02f, 0.53f);
	g->Style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.31f, 0.31f, 0.31f, 1.00f);
	g->Style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.41f, 0.41f, 0.41f, 1.00f);
	g->Style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.51f, 0.51f, 0.51f, 1.00f);
	g->Style.Colors[ImGuiCol_CheckMark] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
	g->Style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.24f, 0.52f, 0.88f, 1.00f);
	g->Style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
	g->Style.Colors[ImGuiCol_Button] = ImVec4(0.26f, 0.59f, 0.98f, 0.40f);
	g->Style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
	g->Style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.06f, 0.53f, 0.98f, 1.00f);
	g->Style.Colors[ImGuiCol_Header] = ImVec4(0.26f, 0.59f, 0.98f, 0.31f);
	g->Style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.80f);
	g->Style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
	g->Style.Colors[ImGuiCol_Separator] = g->Style.Colors[ImGuiCol_Border];
	g->Style.Colors[ImGuiCol_SeparatorHovered] = ImVec4(0.10f, 0.40f, 0.75f, 0.78f);
	g->Style.Colors[ImGuiCol_SeparatorActive] = ImVec4(0.10f, 0.40f, 0.75f, 1.00f);
	g->Style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.26f, 0.59f, 0.98f, 0.20f);
	g->Style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.67f);
	g->Style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.26f, 0.59f, 0.98f, 0.95f);
	g->Style.Colors[ImGuiCol_Tab] = ImLerp(g->Style.Colors[ImGuiCol_Header], g->Style.Colors[ImGuiCol_TitleBgActive], 0.80f);
	g->Style.Colors[ImGuiCol_TabHovered] = g->Style.Colors[ImGuiCol_HeaderHovered];
	g->Style.Colors[ImGuiCol_TabActive] = ImLerp(g->Style.Colors[ImGuiCol_HeaderActive], g->Style.Colors[ImGuiCol_TitleBgActive], 0.60f);
	g->Style.Colors[ImGuiCol_TabUnfocused] = ImLerp(g->Style.Colors[ImGuiCol_Tab], g->Style.Colors[ImGuiCol_TitleBg], 0.80f);
	g->Style.Colors[ImGuiCol_TabUnfocusedActive] = ImLerp(g->Style.Colors[ImGuiCol_TabActive], g->Style.Colors[ImGuiCol_TitleBg], 0.40f);
	g->Style.Colors[ImGuiCol_DockingPreview] = g->Style.Colors[ImGuiCol_HeaderActive] * ImVec4(1.0f, 1.0f, 1.0f, 0.7f);
	g->Style.Colors[ImGuiCol_DockingEmptyBg] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
	g->Style.Colors[ImGuiCol_PlotLines] = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
	g->Style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
	g->Style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
	g->Style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
	g->Style.Colors[ImGuiCol_TableHeaderBg] = ImVec4(0.19f, 0.19f, 0.20f, 1.00f);
	g->Style.Colors[ImGuiCol_TableBorderStrong] = ImVec4(0.31f, 0.31f, 0.35f, 1.00f);
	g->Style.Colors[ImGuiCol_TableBorderLight] = ImVec4(0.23f, 0.23f, 0.25f, 1.00f);
	g->Style.Colors[ImGuiCol_TableRowBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	g->Style.Colors[ImGuiCol_TableRowBgAlt] = ImVec4(1.00f, 1.00f, 1.00f, 0.06f);
	g->Style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.26f, 0.59f, 0.98f, 0.35f);
	g->Style.Colors[ImGuiCol_DragDropTarget] = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
	g->Style.Colors[ImGuiCol_NavHighlight] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
	g->Style.Colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
	g->Style.Colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
	g->Style.Colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);

	DebugLogger::GetInstance()->Log("Loaded asset %s\n", g->IO.IniFilename);
	DebugLogger::GetInstance()->Log("--------------Flush messages: Start of scene--------------\n");
}

void EditorMainScene::Input(double _dt)
{
	UNREFERENCED_PARAMETER(_dt);

	//Scene dragging
	if (InputManager::GetInstance()->GetMouseButtonDown(0))
	{

		Vector3 mousePos = InputManager::GetInstance()->GetMousePosWorld();
		//DebugLogger::GetInstance()->Log("Mouse clicked at <%f/%f>\n", mousePos.mX, mousePos.mY);
		if (mousePos.mX < -WindowsManager::GetInstance()->mWidth / 2 || mousePos.mX > WindowsManager::GetInstance()->mWidth / 2)
			return;
		if (mousePos.mY < -WindowsManager::GetInstance()->mHeight / 2 || mousePos.mY > WindowsManager::GetInstance()->mHeight / 2)
			return;

		Entity* tgtEnt = SelectObject(InputManager::GetInstance()->GetMousePosWorld());
		if (mSelectedEnt != tgtEnt)
			mJumpToEnt = true;

		mSelectedEnt = tgtEnt;
	}

	if (InputManager::GetInstance()->GetMouseButton(0))
	{
		if (mSelectedEnt != nullptr)
		{
			if (mCurrentlyDraggingEntity) //If currently is dragging an entity
			{
				DragEntity(mCurrentDragEnt);
			}
			else //Set all parameters
			{
				mCurrentDragEnt = SelectObject(InputManager::GetInstance()->GetMousePosWorld());
				if (mSelectedEnt != mCurrentDragEnt)
					mJumpToEnt = true;


				if (mCurrentDragEnt != nullptr)
				{
					mCurrentlyDraggingEntity = true;
					mDragEntStartVec = mCurrentDragEnt->GetComponent<Transform>()->GetPosition();
					mMouseStartVec = InputManager::GetInstance()->GetMousePosWorld();
					mMouseOffset = Vector3{0, 0, 0};
				}
			}
		}
	}

	if (mCurrentlyDraggingEntity && InputManager::GetInstance()->GetMouseButtonUp(0))
	{
		mCurrentlyDraggingEntity = false;
	}

	if ((InputManager::GetInstance()->GetKeyTriggered(KeyCode::DELETE_)) && mSelectedEnt != nullptr)
	{
		mSelectedEnt->SetToDestroy(true);
		mSelectedEnt = nullptr;
	}

	if (InputManager::GetInstance()->GetKeyReleased(KeyCode::F11))
	{
		mSceneFullScreen = !mSceneFullScreen;

		if (mSceneFullScreen)
		{
			mShowDebugInfoWindow = false;
			mShowMenuUI = false;
			mShowApplicationDebugLog = false;
			mShowAudioUI = false;
			mShowSystemTime = false;
			mShowAssetUI = false;
			mShowTestWindow = false;
			mShowPlayerInfo = false;
			mShowHierarchy = false;
			mShowProperties = false;
			mShowComponentList = false;
			mShowUnsupportedLog = false;
			mShowResolutionList = false;
			mShowPlayPause = false;
			mShowSaveLoad = false;
			mShowCameraConfig = false;
			mShowMainMenuBar = false;
			mShowTilePalette = false;
		}
		else
		{
			mShowDebugInfoWindow = true;
			mShowAudioUI = true;
			mShowSystemTime = true;
			mShowAssetUI = true;
			mShowTestWindow = true;
			mShowPlayerInfo = true;
			mShowHierarchy = true;
			mShowProperties = true;
			mShowPlayPause = true;
			mShowSaveLoad = true;
			mShowCameraConfig = true;
			mShowMainMenuBar = true;
			mShowTilePalette = true;
		}
	}

	if ((InputManager::GetInstance()->GetKey(KeyCode::LEFT_ALT) || InputManager::GetInstance()->GetKey(KeyCode::RIGHT_ALT)) && InputManager::GetInstance()->GetKey(KeyCode::M))
	{
		if (mSelectedEnt->GetName() == "Player")
		{
			mSelectedEnt->GetComponent<PhysicsComponent>()->mObjBigScale.mX += 1;
		}
		// scale x value up
		EntityManager::GetInstance()->GetEntity(mSelectedEnt->GetID())->GetComponent<Transform>()->mScale.mX += 1;
	}
	if ((InputManager::GetInstance()->GetKey(KeyCode::LEFT_ALT) || InputManager::GetInstance()->GetKey(KeyCode::RIGHT_ALT)) && InputManager::GetInstance()->GetKey(KeyCode::N))
	{
		if (mSelectedEnt->GetName() == "Player")
		{
			mSelectedEnt->GetComponent<PhysicsComponent>()->mObjBigScale.mX -= 1;
		}
		// scale x value down
		EntityManager::GetInstance()->GetEntity(mSelectedEnt->GetID())->GetComponent<Transform>()->mScale.mX -= 1;
	}
	if ((InputManager::GetInstance()->GetKey(KeyCode::LEFT_ALT) || InputManager::GetInstance()->GetKey(KeyCode::RIGHT_ALT)) && InputManager::GetInstance()->GetKey(KeyCode::L))
	{
		if (mSelectedEnt->GetName() == "Player")
		{
			mSelectedEnt->GetComponent<PhysicsComponent>()->mObjBigScale.mY += 1;
		}
		// scale y value up
		EntityManager::GetInstance()->GetEntity(mSelectedEnt->GetID())->GetComponent<Transform>()->mScale.mY += 1;
	}
	if ((InputManager::GetInstance()->GetKey(KeyCode::LEFT_ALT) || InputManager::GetInstance()->GetKey(KeyCode::RIGHT_ALT)) && InputManager::GetInstance()->GetKey(KeyCode::K))
	{
		if (mSelectedEnt->GetName() == "Player")
		{
			mSelectedEnt->GetComponent<PhysicsComponent>()->mObjBigScale.mY -= 1;
		}
		// scale y value down
		EntityManager::GetInstance()->GetEntity(mSelectedEnt->GetID())->GetComponent<Transform>()->mScale.mY -= 1;
	}
	if ((InputManager::GetInstance()->GetKey(KeyCode::LEFT_ALT) || InputManager::GetInstance()->GetKey(KeyCode::RIGHT_ALT)) && InputManager::GetInstance()->GetKey(KeyCode::COMMA))
	{
		// scale y value up
		EntityManager::GetInstance()->GetEntity(mSelectedEnt->GetID())->GetComponent<Transform>()->mRotation += (10.f * (float)WindowsManager::GetInstance()->mDeltaTime);
	}
	if ((InputManager::GetInstance()->GetKey(KeyCode::LEFT_ALT) || InputManager::GetInstance()->GetKey(KeyCode::RIGHT_ALT)) && InputManager::GetInstance()->GetKey(KeyCode::PERIOD))
	{
		// scale y value down
		EntityManager::GetInstance()->GetEntity(mSelectedEnt->GetID())->GetComponent<Transform>()->mRotation -= (10.f * (float)WindowsManager::GetInstance()->mDeltaTime);
	}

	//Spawning of objects in scene
	if (CoreEngine::GetInstance()->mGameState != CoreEngine::GameState::GS_PLAY)
		return;

	/*if (InputManager::GetInstance()->GetMouseButton(1))
	{
		CreatePrefab();
	}

	if (InputManager::GetInstance()->GetKeyTriggered(KeyCode::Q))
	{
		for (int i = 0; i < 2500; ++i)
			CreateMysteryBox();
	}
	if (InputManager::GetInstance()->GetKeyTriggered(KeyCode::E))
	{
		CreateObject1();
	}*/
}

void EditorMainScene::Update(double _dt)
{
	UNREFERENCED_PARAMETER(_dt);
	// For graphics render scale rotate
	GraphicsSystem::GetInstance()->mSelectedScaleRotate = mSelectedEnt;

	ImGuiContext* g = ImGui::GetCurrentContext();
	if (!g->SettingsLoaded)
	{
		if (g->IO.IniFilename)
			ImGui::LoadIniSettingsFromDisk(g->IO.IniFilename);
		g->SettingsLoaded = true;
		DebugLogger::GetInstance()->Log("--------------------Loaded ini----------------\n");
	}

	//Update scale based on current screen height
	g->IO.FontGlobalScale = mEditorFontScale;

	LibraryManager::GetInstance()->ImGUIUpdate();


	// Throw error if unsupported file type
	if (AssetManager::GetInstance()->mUnsupportedExtensionLog.size() != 0)
	{
		mShowUnsupportedLog = true;
	}

	if (mShowUnsupportedLog)
	{
		static bool _useWorkArea = true;
		static ImGuiWindowFlags _flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings;

		// We demonstrate using the full viewport area or the work area (without menu-bars, task-bars etc.)
		// Based on your use case you may want one or the other.
		const ImGuiViewport* _viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(_useWorkArea ? _viewport->WorkPos : _viewport->Pos);
		ImGui::SetNextWindowSize(_useWorkArea ? _viewport->WorkSize : _viewport->Size);
		if (ImGui::Begin("###ErrorUnsupportedExtensionLog", &mShowUnsupportedLog, _flags))
		{
			float textWidth = ImGui::CalcTextSize("WARNING").x;
			ImGui::SetCursorPosX((ImGui::GetWindowWidth() * 0.5f) - (textWidth * 0.5f));
			ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "WARNING");

			std::string mSupportedExtensionText = "Here is the list of file extensions that are supported " + AssetManager::GetInstance()->mSupportedExtensions + "|frag.";
			ImGui::SetCursorPosY((ImGui::GetWindowHeight() * 0.04f));
			textWidth = ImGui::CalcTextSize(mSupportedExtensionText.c_str()).x;
			ImGui::SetCursorPosX((ImGui::GetWindowWidth() * 0.5f) - (textWidth * 0.5f));
			ImGui::Text(mSupportedExtensionText.c_str());

			float cursorPositionUnsupportedX = (ImGui::GetWindowWidth() * 0.5f);
			float cursorPositionUnsupportedY = (ImGui::GetWindowHeight() * 0.1f);
			ImGui::SetCursorPosY(cursorPositionUnsupportedY);
			for (int i = 0; i < AssetManager::GetInstance()->mUnsupportedExtensionLog.size(); ++i)
			{
				ImGui::SetCursorPosX(cursorPositionUnsupportedX - (ImGui::CalcTextSize(AssetManager::GetInstance()->mUnsupportedExtensionLog[i].c_str()).x * 0.5f));
				ImGui::Text(AssetManager::GetInstance()->mUnsupportedExtensionLog[i].c_str());
			}

			std::string _closeUnsupportedLogWindowText = "OK";
			ImGui::SetCursorPosY(ImGui::GetWindowHeight() * 0.75f);
			if (mShowUnsupportedLog && LibraryManager::GetInstance()->ImGUICreateAlignedHorizontalButton(_closeUnsupportedLogWindowText, 0.5f, ImGui::GetWindowWidth() * 0.1f, ImGui::GetWindowHeight() * 0.1f))
			{
				for (int i = 0; i < AssetManager::GetInstance()->mUnsupportedExtensionLog.size(); ++i)
				{
					AssetManager::GetInstance()->mUnsupportedExtensionLog[i].erase();
				}
				AssetManager::GetInstance()->mUnsupportedExtensionLog.clear();
				mShowUnsupportedLog = false;
			}
		}
		ImGui::End();
	}


	// Main Menu bar
	if (mShowMainMenuBar)
	{
		ImGui::BeginMainMenuBar();
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("New", "Ctrl+N", false, false)) {}
			if (ImGui::MenuItem("Open", "Ctrl+O", false, false)) {}
			if (ImGui::BeginMenu("Open Recent", false)) {}
			if (ImGui::MenuItem("Save", "Ctrl+S", false, false)) {}
			if (ImGui::MenuItem("Save As..", NULL, false, false)) {}

			ImGui::Separator();
			if (ImGui::BeginMenu("Options", false)) {}

			if (ImGui::MenuItem("Checked", NULL, true)) {}


			ImGui::Separator();

			if (ImGui::MenuItem("Quit", "Alt+F4"))
			{
				glfwSetWindowShouldClose(WindowsManager::GetInstance()->mPtrWindow, true);
			}
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Edit"))
		{
			if (ImGui::MenuItem("Undo", "CTRL+Z")) {}
			if (ImGui::MenuItem("Redo", "CTRL+Y", false, false)) {}  // Disabled item
			ImGui::Separator();
			if (ImGui::MenuItem("Cut", "CTRL+X")) {}
			if (ImGui::MenuItem("Copy", "CTRL+C")) {}
			if (ImGui::MenuItem("Paste", "CTRL+V")) {}
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Debug"))
		{
			ImGui::MenuItem("Debug Info", NULL, &mShowDebugInfoWindow, true);
			ImGui::MenuItem("Debug Log", NULL, &mShowApplicationDebugLog, true);
			ImGui::MenuItem("System Time", NULL, &mShowSystemTime, true);
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("View"))
		{
			ImGui::MenuItem("Asset Explorer", NULL, &mShowAssetUI, true);
			ImGui::MenuItem("Audio UI", NULL, &mShowAudioUI, true);
			ImGui::MenuItem("Window Resolution Tester", NULL, &mShowResolutionList, true);
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}

	// 2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
	if (mShowTestWindow)
	{
		static float f = 0.0f;
		static int counter = 0;

		ImGui::Begin("Default Window", &mShowTestWindow);

		ImGui::Checkbox("Screen Grid", &TileComponent::mEnableScreenGrid);
		ImGui::Checkbox("Collision Grid", &TileComponent::mEnableCollisionGrid);

		if (ImGui::Button("Test Wind Tunnels Up"))
		{
			SpawnWindTunnelUp();
			TileComponent::LoadTileData();
		}
		if (ImGui::Button("Test Wind Tunnels"))
		{
			CreateWindTunnels();
			TileComponent::LoadTileData();
		}
		if (ImGui::Button("Spawn Player"))
		{
			CreatePlayer();
		}
		if (ImGui::Button("Test Laser (after player)"))
		{
			CreateLaser();
			TileComponent::LoadTileData();
		}
		//Clear color
		ImGui::ColorEdit3("Background color", GraphicsSystem::GetInstance()->mBackgroundColor); // Edit 3 floats representing a color

		ImGui::SliderFloat("Editor Font Scale", &mEditorFontScale, 0.1f, 2.f);
		ImGui::SliderInt("TileSize", &TileComponent::mTileSize, 1, 50);
		ImGui::Checkbox("Audio UI", &mShowAudioUI);

		ImGui::Checkbox("Gfx debugging mode", &GraphicsSystem::GetInstance()->mDebugGfxMode);

		ImGui::Spacing();
		/*Camera* currCam = &GraphicsSystem::GetInstance()->mCamera;
		if (ImGui::SliderFloat("Editor Camera Zoom", &currCam->editorZoom, currCam->minZoomVal, currCam->maxZoomVal, "%.2f"))
			GraphicsSystem::GetInstance()->mCamera.UpdateCameraInfo();
		ImGui::SliderFloat("Game Camera Zoom", &currCam->gameZoom, currCam->minZoomVal, currCam->maxZoomVal, "%.2f");*/
		
		// render layers master control
		if (ImGui::Button("Show Render layers"))
		{
			mShowRenderLayers = !mShowRenderLayers;
		}

		if (mShowRenderLayers)
			ShowRenderLayers();
		ImGui::End();
	}

	//Scene Display
	if (mShowScene)
	{
		ImGui::Begin("Scene");
		if (!mSceneFullScreen)
		{
			ImGui::SetWindowSize(ImVec2(800, 600)); //Initial window size, will be adjusted when user adjusts the tab
		}
		else
		{
			ImGui::SetNextWindowSize(ImVec2(ImGui::GetIO().DisplaySize.x, ImGui::GetIO().DisplaySize.y));
			ImGui::SetNextWindowPos(ImVec2(0, 0));
		}


		ImGui::Image((ImTextureID)static_cast<uintptr_t>(GraphicsSystem::GetInstance()->mFrameBufferTextureID), ImGui::GetContentRegionAvail());
		InputManager::GetInstance()->UpdateScreenPos(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y);
		InputManager::GetInstance()->UpdateRegionMax(ImGui::GetWindowContentRegionMax().x, ImGui::GetWindowContentRegionMax().y);
		InputManager::GetInstance()->UpdateRegionMin(ImGui::GetWindowContentRegionMin().x, ImGui::GetWindowContentRegionMin().y);
		if (ImGui::BeginDragDropTarget())
		{
			const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("PrefabDrag");
			if (payload)
			{
				Entity* spawn = LevelLoader::LoadPrefab(std::string(static_cast<const char*>(payload->Data), payload->DataSize));
				if (spawn->GetComponent<Transform>())
					spawn->GetComponent<Transform>()->SetPosition(InputManager::GetInstance()->GetMousePosScreen() + GraphicsSystem::GetInstance()->mCamera.mCurrCamInfo.mPosition);
			}
			ImGui::EndDragDropTarget();
		}

		ImGui::End();
	}
	//Save load button
	if (mShowSaveLoad)
	{
		ImGui::Begin("##Save&Load");
		if (ImGui::Button("Save"))
		{
			LevelLoader::SaveLevel(SceneManager::GetInstance()->mActiveSceneName, GraphicsSystem::GetInstance()->mBackgroundColor, "Assets/Scenes");
		}
		ImGui::SameLine();
		if (ImGui::Button("Load"))
		{
			if (CoreEngine::GetInstance()->mGameState == CoreEngine::GameState::GS_PLAY)
				EntityManager::GetInstance()->ClearSceneBackup();
			SceneManager::GetInstance()->SetSceneToRestart(true);
			mIsEditorPlaying = false;
		}
		ImGui::End();
	}

	if (mShowPlayPause)
	{
		ImGui::Begin("##PlayPauseStop");
		//Save load button
		if (ImGui::Button("Play"))
		{
			if (mIsEditorPlaying == false)
			{
				if (CoreEngine::GetInstance()->mGameState == CoreEngine::GameState::GS_STOP)
				{
					CoreEngine::GetInstance()->mGameState = CoreEngine::GameState::GS_PLAY;
					EventManager::GetInstance()->AddEvent(EventPlayPause(EventPlayPause::PLAY));
					EntityManager::GetInstance()->BackupCurrScene();
					ButtonSelectManager::GetInstance()->CreateBackup();
					GraphicsSystem::GetInstance()->mCamera.UpdateCameraInfo();
				}
				else if (CoreEngine::GetInstance()->mGameState == CoreEngine::GameState::GS_PAUSE)
				{
					CoreEngine::GetInstance()->mGameState = CoreEngine::GameState::GS_PLAY;
					EventManager::GetInstance()->AddEvent(EventPlayPause(EventPlayPause::PLAY));
				}

				mIsEditorPlaying = true;
				mCamWaypointIndex = SceneManager::GetInstance()->GetActiveScene()->mCamWaypointIndex;
			}
		}
		ImGui::SameLine();
		if (ImGui::Button("Pause"))
		{
			if (CoreEngine::GetInstance()->mGameState == CoreEngine::GameState::GS_PLAY)
			{
				CoreEngine::GetInstance()->mGameState = CoreEngine::GameState::GS_PAUSE;
				EventManager::GetInstance()->AddEvent(EventPlayPause(EventPlayPause::PAUSE));
				mIsEditorPlaying = false;
			}
		}
		ImGui::SameLine();
		if (ImGui::Button("Stop"))
		{
			if (CoreEngine::GetInstance()->mGameState != CoreEngine::GameState::GS_STOP)
			{
				for (Entity ent : EntityManager::GetInstance()->mEntityList)
				{
					if (ent.GetComponent<ButtonWrapper>())
					{
						ent.GetComponent<ButtonWrapper>()->Exit();
					}
				}

				CoreEngine::GetInstance()->mGameState = CoreEngine::GameState::GS_STOP;
				EventManager::GetInstance()->AddEvent(EventPlayPause(EventPlayPause::STOP));
				EntityManager::GetInstance()->LoadSceneBackup();
				ButtonSelectManager::GetInstance()->LoadBackup();
				GraphicsSystem::GetInstance()->mCamera.UpdateCameraInfo();

				for (Entity ent : EntityManager::GetInstance()->mEntityList)
				{
					if (ent.GetComponent<ButtonWrapper>())
					{
						ent.GetComponent<ButtonWrapper>()->LoadButtonType();
					}
				}

				TextManager::GetInstance()->ClearAllText();
			}

			mIsEditorPlaying = false;
			SceneManager::GetInstance()->GetActiveScene()->mCamWaypointIndex = mCamWaypointIndex;
		}
		ImGui::End();
	}

	// Debug Stats
	if (mShowDebugInfoWindow)
	{
		DebugInfoWindow(mShowDebugInfoWindow);
	}

	// Temporary menu UI test
	if (mShowMenuUI)
	{
		static bool _useWorkArea = true;
		static ImGuiWindowFlags _flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings;

		// We demonstrate using the full viewport area or the work area (without menu-bars, task-bars etc.)
		// Based on your use case you may want one or the other.
		const ImGuiViewport* _viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(_useWorkArea ? _viewport->WorkPos : _viewport->Pos);
		ImGui::SetNextWindowSize(_useWorkArea ? _viewport->WorkSize : _viewport->Size);
		if (ImGui::Begin("Example: Fullscreen window", &mShowMenuUI, _flags))
		{
			std::string _closeWindowText = "Close this window";

			ImGui::SetCursorPosY(ImGui::GetCursorPosY() + ImGui::GetWindowHeight() * 0.1f);

			if (mShowMenuUI && LibraryManager::GetInstance()->ImGUICreateAlignedHorizontalButton(_closeWindowText, 0.5f, ImGui::GetWindowWidth() * 0.1f, ImGui::GetWindowHeight() * 0.1f))
				mShowMenuUI = false;

			ImGui::SetCursorPosY(ImGui::GetCursorPosY() + ImGui::GetWindowHeight() * 0.1f);

			if (mShowMenuUI && LibraryManager::GetInstance()->ImGUICreateAlignedHorizontalButton("Unga Bunga", 0.5f, ImGui::GetWindowWidth() * 0.1f, ImGui::GetWindowHeight() * 0.1f))
				mShowMenuUI = false;

			ImGui::SetCursorPosY(ImGui::GetCursorPosY() + ImGui::GetWindowHeight() * 0.1f);

			if (mShowMenuUI && LibraryManager::GetInstance()->ImGUICreateAlignedHorizontalButton("Lorem Ipsum", 0.5f, ImGui::GetWindowWidth() * 0.1f, ImGui::GetWindowHeight() * 0.1f))
				mShowMenuUI = false;

			ImGui::SetCursorPosY(ImGui::GetCursorPosY() + ImGui::GetWindowHeight() * 0.1f);

			if (mShowMenuUI && LibraryManager::GetInstance()->ImGUICreateAlignedHorizontalButton("UShell Not Pass", 0.5f, ImGui::GetWindowWidth() * 0.1f, ImGui::GetWindowHeight() * 0.1f))
				mShowMenuUI = false;
		}
		ImGui::End();
	}

	// Debug logger ImGUI
	if (mShowApplicationDebugLog)
	{
		ApplicationDebugLogWindow(g, mShowApplicationDebugLog);
	}

	//Audio
	if (mShowAudioUI)
	{
		ImGui::Begin("Audio Test UI", &mShowAudioUI);
		static float _sfxVol = SoundManager::GetInstance()->GetSFXVolume();
		static float _bgmVol = SoundManager::GetInstance()->GetBGMVolume();

		if (ImGui::BeginTable("##AudioTestUITable", 2, ImGuiTableFlags_Resizable))
		{
			////////////////////////////////
			ImGui::TableNextRow(); ImGui::TableSetColumnIndex(0);
			ImGui::Text("SFX Volume");
			ImGui::TableSetColumnIndex(1); ImGui::SetNextItemWidth(-1);
			ImGui::SliderFloat("##audioSFX Volume", &_sfxVol, 0.0f, 1.0f);
			////////////////////////////////
			ImGui::TableNextRow(); ImGui::TableSetColumnIndex(0);
			ImGui::Text("BGM Volume");
			ImGui::TableSetColumnIndex(1); ImGui::SetNextItemWidth(-1);
			ImGui::SliderFloat("##audioBGM Volume", &_bgmVol, 0.0f, 1.0f);
			////////////////////////////////
			ImGui::EndTable();
		}

		/*----------------------------------SFX----------------------------------*/

		ImGui::Text("\n SFX Tests:");
		if (ImGui::Button("Jump Sound"))
			SoundManager::GetInstance()->PlaySound("SFX_Jump", SoundManager::GetInstance()->GetSFXVolume(), 1.f);
		ImGui::SameLine();
		if (ImGui::Button("Stop SFX"))
			SoundManager::GetInstance()->StopChannel(SoundManager::SoundType_SFX);
		ImGui::SameLine();
		if (ImGui::Button("Toggle SFX Pause"))
			SoundManager::GetInstance()->SetSFXPaused(!SoundManager::GetInstance()->GetSFXPaused());
		ImGui::SameLine();
		if (SoundManager::GetInstance()->GetSFXPaused())
		{
			ImGui::Text("SFX Paused");
		}
		else
		{
			ImGui::Text("SFX Active");
		}

		if (ImGui::Button("Collect Item"))
			SoundManager::GetInstance()->PlaySound("SFX_CollectItem", SoundManager::GetInstance()->GetSFXVolume(), 1.f);
		if (ImGui::Button("Collect Item + Unlock"))
			SoundManager::GetInstance()->PlaySound("SFX_CollectItemAndUnlock", SoundManager::GetInstance()->GetSFXVolume(), 1.f);

		/*----------------------------------BGM----------------------------------*/
		ImGui::Text("\n BGM Tests:");
		if (ImGui::Button("Qubie BGM"))
			SoundManager::GetInstance()->PlaySound("BGM_QubieBGM", SoundManager::GetInstance()->GetBGMVolume(), 1.f);
		ImGui::SameLine();
		if (ImGui::Button("Stop BGM"))
			SoundManager::GetInstance()->StopChannel(SoundManager::SoundType_BGM);
		ImGui::SameLine();
		if (ImGui::Button("Toggle BGM Pause"))
			SoundManager::GetInstance()->SetBGMPaused(!SoundManager::GetInstance()->GetBGMPaused());
		ImGui::SameLine();
		if (SoundManager::GetInstance()->GetBGMPaused())
		{
			ImGui::Text("BGM Paused");
		}
		else
		{
			ImGui::Text("BGM Active");
		}

		ImGui::Text("FMod Active Channels: %i", SoundManager::GetInstance()->GetActiveChannels());
		ImGui::Text("FMod DSP Size: %i", SoundManager::GetInstance()->GetDSPBufferSize());
		ImGui::End();
	}

	//Profiler
	if (mShowSystemTime)
	{
		ProfilerWindow(mShowSystemTime);
	}

	//Asset Browser
	if (mShowAssetUI)
	{
		ImGui::Begin("Asset Explorer", &mShowAssetUI);

		//If need to update drag and drop
		if (AssetManager::GetInstance()->GetDragDrop())
		{
			//Spawn the yellow box
			if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceExtern))	// we use an external source (i.e. not ImGui-created)
			{
				// replace "FILES" with whatever identifier you want - possibly dependant upon what type of files are being dragged
				// you can specify a payload here with parameter 2 and the sizeof(parameter) for parameter 3.
				// I store the payload within a vector of strings within the application itself so don't need it.
				ImGui::SetDragDropPayload("ExternalDrag", nullptr, 0);
				ImGui::BeginTooltip();
				ImGui::Text("Import File");
				ImGui::EndTooltip();
				ImGui::EndDragDropSource();
			}
		}


		float mAssetLeftSpace = 0;
		if (ImGui::GetContentRegionAvail().x * 0.5f > ((ImGui::GetWindowWidth() * 0.20f) + g->Style.FramePadding.x))
		{
			mAssetLeftSpace = (ImGui::GetWindowWidth() * 0.20f) + g->Style.FramePadding.x;
		}
		else
		{
			mAssetLeftSpace = ImGui::GetContentRegionAvail().x - ((ImGui::GetWindowWidth() * 0.20f) + g->Style.FramePadding.x);
		}
		ImGui::BeginChild("AssetLeft", ImVec2(mAssetLeftSpace, 0), true, ImGuiWindowFlags_HorizontalScrollbar);
		ImGuiTreeNodeFlags _treeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_OpenOnArrow;
		if (ImGui::TreeNodeEx((mBasePath + "##folderList").c_str(), _treeFlags))
		{
			MyImGuiDragTargetMove(mBasePath);
			if (ImGui::IsMouseClicked(0) && ImGui::IsItemHovered())
			{
				mCurrentOpenPath = mBasePath;
			}
			RecursiveFolderDisplay(mBasePath, _treeFlags);
			ImGui::TreePop();
		}

		ImGui::EndChild();
		ImGui::SameLine();
		ImGui::BeginChild("AssetRight", ImVec2(0.f, 0.f), true, false);
		//For dropping of prefab into directory
		{
			ImRect inner_rect = ImGui::GetCurrentWindow()->InnerRect;
			if (ImGui::BeginDragDropTargetCustom(inner_rect, ImGui::GetID("##window")))
			{
				const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("EntityDrag", ImGuiDragDropFlags_AcceptBeforeDelivery);//need to check before delivery so we can draw the yellow box
				if (payload)
				{
					if (payload->IsPreview())
					{
						ImDrawList* draw_list = ImGui::GetForegroundDrawList();
						draw_list->AddRectFilled(inner_rect.Min, inner_rect.Max, ImGui::GetColorU32(ImGuiCol_DragDropTarget, 0.05f));//draw inner darker overlay
						draw_list->AddRect(inner_rect.Min, inner_rect.Max, ImGui::GetColorU32(ImGuiCol_DragDropTarget), 0.0f, 0, 2.0f);//draw the yellow outer box
					}
					if (payload->IsDelivery())
						LevelLoader::SavePrefab(std::string(static_cast<const char*>(payload->Data), payload->DataSize), mSelectedEnt, mCurrentOpenPath);
				}
				else //Check for external drag
				{
					payload = ImGui::AcceptDragDropPayload("ExternalDrag", ImGuiDragDropFlags_AcceptBeforeDelivery);
					if(payload)
					{
						if (payload->IsPreview())
						{
							ImDrawList* draw_list = ImGui::GetForegroundDrawList();
							draw_list->AddRectFilled(inner_rect.Min, inner_rect.Max, ImGui::GetColorU32(ImGuiCol_DragDropTarget, 0.05f));//draw inner darker overlay
							draw_list->AddRect(inner_rect.Min, inner_rect.Max, ImGui::GetColorU32(ImGuiCol_DragDropTarget), 0.0f, 0, 2.0f);//draw the yellow outer box
						}
						if (payload->IsDelivery())
						{
							//Copy over
							AssetManager::GetInstance()->CopyFromExternal(mCurrentOpenPath);
						}
					}

				}
				ImGui::EndDragDropTarget();
			}
		}
		AssetManager::DirectoryInfo dirInfo = AssetManager::GetInstance()->GetDirectoryInfo(mCurrentOpenPath);
		ImVec2 size = ImVec2(128.0f, 128.0f);                         // Size of the image we want to make visible
		ImVec2 padding = ImVec2(10.0f, 10.0f);							//size of adding around 
		ImVec2 uv0 = ImVec2(0.0f, 1.0f);                            // UV coordinates for lower-left
		ImVec2 uv1 = ImVec2(1.0f, 0.0f);							// UV coordinates for (32,32) in our texture
		ImVec4 bg_col = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);             // Black background
		ImVec4 tint_col = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);           // No tint
		float mExplorer_width = ImGui::GetContentRegionAvail().x;
		int mNumberofColumns = static_cast<int>(mExplorer_width / (size.x + padding.x * 2));
		if (mNumberofColumns < 1)
		{
			mNumberofColumns = 1;
		}

		if (mCurrentOpenPath != mBasePath)
		{
			mPrintCurrentFolder = mCurrentOpenPath;
			//if (ImGui::Button("<-"))
			//{
			//	size_t splitPos = mCurrentOpenPath.find_last_of('/');
			//	mCurrentOpenPath = mCurrentOpenPath.substr(0, splitPos);
			//}
			ImGui::SameLine();
			mPrintCurrentFolder = mPrintCurrentFolder.substr(mBasePath.size() + 1, mPrintCurrentFolder.size());
			ImGui::TextWrapped("%s", mPrintCurrentFolder.c_str());
		}
		ImGui::Columns(mNumberofColumns, 0, false);
		for (int i = 0; i < dirInfo.size(); ++i)
		{
			AssetManager::ImageAsset* image{nullptr};
			AssetManager::GetInstance()->GetAsset("NONE", image);
			switch (dirInfo[i].type)
			{
				case AssetManager::FOLDER:
					AssetManager::GetInstance()->GetAsset("FOLDER", image);
					break;
				case AssetManager::SPRITE:
					AssetManager::GetInstance()->GetAsset(dirInfo[i].fileName, image);
					break;
				case AssetManager::AUDIO:
					AssetManager::GetInstance()->GetAsset("AUDIO", image);
					break;
				case AssetManager::JSON:
					AssetManager::GetInstance()->GetAsset("JSON", image);
					break;
				case AssetManager::PREFAB:
					{
						AssetManager::GetInstance()->GetAsset("PREFAB", image);
						LevelLoader::PrefabInfo tempPrefabInfo{};
						TileComponent::mLoadMap = false;
						LevelLoader::LoadPrefabInfo(dirInfo[i].fileName, tempPrefabInfo);
						if (tempPrefabInfo.mRenderer.second == true)
							AssetManager::GetInstance()->GetAsset(tempPrefabInfo.mRenderer.first.mTextureName, image);
					}
					break;
				case AssetManager::SCENE:
					AssetManager::GetInstance()->GetAsset("SCENE", image);
					break;
				case AssetManager::FONT:
					AssetManager::GetInstance()->GetAsset("FONT", image);
					break;
				case AssetManager::CSSCRIPT:
					AssetManager::GetInstance()->GetAsset("CSSCRIPT", image);
					break;
				default:
					break;
			}

			GLuint my_tex_id{0};
			if (image)
				my_tex_id = image->mTexID;

			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, padding);
			if (ImGui::ImageButton(std::string("#####" + dirInfo[i].fileName).c_str(), (ImTextureID*)(intptr_t)(my_tex_id), size, uv0, uv1, bg_col, tint_col))
			{
				if (dirInfo[i].type == AssetManager::FOLDER)
				{
					mCurrentOpenPath += "/" + dirInfo[i].fileName;
				}
				if (dirInfo[i].type == AssetManager::SCENE)
				{
					//Open prompt to load
					mSceneToLoadName = dirInfo[i].fileName;
					ImGui::OpenPopup("LoadNewScene");
				}
			}
			ImGui::PopStyleVar();
			if (dirInfo[i].type == AssetManager::PREFAB)
			{
				if (ImGui::BeginDragDropSource())
				{
					ImGui::SetDragDropPayload("PrefabDrag", dirInfo[i].fileName.c_str(), dirInfo[i].fileName.size());
					ImGui::Image((ImTextureID*)(intptr_t)my_tex_id, size / 2, uv0, uv1);
					mCurrentDraggingExt = dirInfo[i].extension;
					ImGui::EndDragDropSource();
				}
				//Edit prefab
				if (ImGui::IsMouseClicked(0) && ImGui::IsItemHovered())
				{
					mSelectedEnt = &mPrefabEnt;
					mPrefabEnt.SetName(dirInfo[i].fileName);
					LevelLoader::LoadPrefabInfo(dirInfo[i].fileName, mPrefabInfo);
				}

			}
			else if (dirInfo[i].type == AssetManager::FOLDER)
			{
				//For dropping of prefab into directory
				MyImGuiDragTargetMove(dirInfo[i].fullPath);
			}

			if (ImGui::BeginDragDropSource())
			{
				std::string payloadName{};
				if (dirInfo[i].type == AssetManager::FOLDER)	 payloadName = "FolderDrag";
				else if (dirInfo[i].type == AssetManager::SPRITE)payloadName = "SpriteDrag";
				else if (dirInfo[i].type == AssetManager::AUDIO) payloadName = "AudioDrag";
				else if (dirInfo[i].type == AssetManager::JSON)	 payloadName = "JSONDrag";
				else if (dirInfo[i].type == AssetManager::PREFAB)payloadName = "PrefabDrag";
				else if (dirInfo[i].type == AssetManager::SCENE) payloadName = "SceneDrag";
				else if (dirInfo[i].type == AssetManager::FONT)  payloadName = "FontDrag";
				else if (dirInfo[i].type == AssetManager::CSSCRIPT)  payloadName = "CSScriptDrag";

				ImGui::SetDragDropPayload(payloadName.c_str(), dirInfo[i].fileName.c_str(), dirInfo[i].fileName.size());
				ImGui::Image((ImTextureID*)(intptr_t)my_tex_id, size / 2, uv0, uv1);
				mCurrentDraggingExt = dirInfo[i].extension;
				ImGui::EndDragDropSource();
			}



			if (mFileActions != NONE && ImGui::GetItemID() == mSelectedID)
			{
				if (mFileActions == RENAME)
				{
					if (mNameBuffer[i] == '\0')//first time
					{
						strcpy(mNameBuffer, dirInfo[i].fileName.c_str());
						ImGui::SetKeyboardFocusHere(0);
					}
					ImGui::InputText("##fileRename", mNameBuffer, 1024, ImGuiInputTextFlags_::ImGuiInputTextFlags_AutoSelectAll);
					if (ImGui::IsItemDeactivatedAfterEdit() || ImGui::IsMouseClicked(0))
					{
						AssetManager::GetInstance()->RenameMyFile(dirInfo[i].fullPath.c_str(), (mCurrentOpenPath + "/" + mNameBuffer).c_str());
						memset(mNameBuffer, 0, 1024);
						mFileActions = NONE;
					}
				}
				else if (mFileActions == DELETE)
				{
					AssetManager::GetInstance()->DeleteMyFile(dirInfo[i].fullPath);
					mFileActions = NONE;
				}

			}
			else
				ImGui::TextWrapped(dirInfo[i].fileName.c_str());

			ImGui::NextColumn();

		}

		{//This needs to be duplicated in the other child also
			if (ImGui::IsWindowHovered())
			{
				if (ImGui::IsMouseClicked(1))//right click 
				{
					if (ImGui::IsAnyItemHovered())
					{
						ImGui::OpenPopup("EditMenu");
						mSelectedID = ImGui::GetHoveredID();
					}
					else
						ImGui::OpenPopup("CreateMenu");

				}
			}

			//For creating new stuffs
			ImVec2 buttonSize(150, 30);
			ImGui::PushStyleVar(ImGuiStyleVar_ButtonTextAlign, ImVec2(0, 0.5f));
			//Create menu
			if (ImGui::BeginPopup("CreateMenu"))
			{
				ImGui::SeparatorText("Create New");
				if (ImGui::Button("Scene", buttonSize))
				{
					LevelLoader::CreateBlankScene(mCurrentOpenPath);
					ImGui::CloseCurrentPopup();
				}
				if (!ImGui::IsAnyItemHovered() && ImGui::IsMouseClicked(0))
				{
					ImGui::CloseCurrentPopup();
				}

				ImGui::EndPopup();
			}
			//File edit menu
			if (ImGui::BeginPopup("EditMenu"))
			{
				if (ImGui::Button("Rename", buttonSize))
				{
					mFileActions = RENAME;
					ImGui::CloseCurrentPopup();
				}
				if (ImGui::Button("Delete", buttonSize))
				{
					mFileActions = DELETE;
					ImGui::CloseCurrentPopup();
				}

				if (!ImGui::IsAnyItemHovered() && ImGui::IsMouseClicked(0))
				{
					mSelectedID = ImGui::GetID("NULL");
					ImGui::CloseCurrentPopup();
				}
				ImGui::EndPopup();
			}
			ImGui::PopStyleVar();
		}

		//popup for switching scene
		if (ImGui::BeginPopupModal("LoadNewScene"))//Do we want to block the back? if not use without modal
		{
			ImGui::Text("Do you want to change scene?");
			if (ImGui::Button("Yes"))
			{
				SceneManager::GetInstance()->SetActiveScene(SceneManager::GetSceneFromName(mSceneToLoadName));
				LevelLoader::LoadLevel(mSceneToLoadName, GraphicsSystem::GetInstance()->mBackgroundColor);

				mIsEditorPlaying = false;
				ImGui::CloseCurrentPopup();
			}
			if (ImGui::Button("No"))
			{
				ImGui::CloseCurrentPopup();
			}
			ImGui::EndPopup();
		}
		ImGui::EndChild();


		ImGui::End();
	}

	//Hierarchical List
	if (mShowHierarchy)
	{
		ImGui::Begin("Hierarchy", &mShowHierarchy);
		if (ImGui::BeginTable("##Hierarchy", 2, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_Resizable))
		{
			//Table Headers
			ImGui::TableSetupColumn("Name");
			ImGui::TableSetupColumn("ID");
			ImGui::TableHeadersRow();

			//Hierarchy table
			for (int i = 0; i < EntityManager::GetInstance()->mEntityList.size(); ++i)
			{
				//Skip objects with parents as recursion will be done by parent object
				if (EntityManager::GetInstance()->mEntityList[i].GetParentID() != 0)
					continue;

				Entity* currEnt = &EntityManager::GetInstance()->mEntityList[i];
				if (currEnt == nullptr)
					continue;

				DisplayHierarchyNode(currEnt);

				if (mJumpToEnt && currEnt == mSelectedEnt)
				{
					ImGui::SetScrollHereY(0.5f);
					--mHiercJumpCounter;
					if (mHiercJumpCounter <= 0)
					{
						mJumpToEnt = false;
						mHiercJumpCounter = 5;
					}
				}
			}

			ImGui::EndTable();
		}

		//If no items in hierarchy table yet
		if (EntityManager::GetInstance()->mEntityList.size() == 0)
		{
			if (ImGui::Button("Create New Entity"))
			{
				Entity* newEnt = EntityManager::GetInstance()->CreateEntity();
				//Add transform to new entity
				newEnt->AddComponent<Transform>();
			}
		}

		ImGui::End();
	}

	//Resolution change tester
	if (mShowResolutionList)
	{
		ImGui::Begin("Resolution", &mShowResolutionList);
		if (ImGui::Button("800 x 600"))
		{
			WindowsManager::GetInstance()->ChangeWindowSize(800, 600);
		}
		if (ImGui::Button("1440 x 900"))
		{
			WindowsManager::GetInstance()->ChangeWindowSize(1440, 900);
		}
		if (ImGui::Button("1920 x 1080"))
		{
			WindowsManager::GetInstance()->ChangeWindowSize(1920, 1080);
		}
		if (ImGui::Button("2560 x 1440"))
		{
			WindowsManager::GetInstance()->ChangeWindowSize(2560, 1440);
		}
		ImGui::End();
	}

	//Camera current info + Waypoints
	if (mShowCameraConfig)
	{
		ImGui::Begin("Camera");

		ImGui::Text("Camera Speed"); ImGui::SameLine();
		ImGui::SliderFloat("##CamSpeed", &GraphicsSystem::GetInstance()->mCamera.mCamMoveSpeed, 10, 1000);
		ImGui::Text("Scene Camera Zoom"); ImGui::SameLine();
		if (ImGui::SliderFloat("##CamZoom", &GraphicsSystem::GetInstance()->mCamera.mCurrCamInfo.mZoom, 0, 10))
			GraphicsSystem::GetInstance()->mCamera.UpdateCameraInfo();
		ImGui::Text("Current Player Checkpoint: "); ImGui::SameLine();
		ImGui::InputInt("##CurrentPlayerCP", &SceneManager::GetInstance()->GetActiveScene()->mCamWaypointIndex);
		if (ImGui::Button("Reset camera to checkpoint ^"))
			GraphicsSystem::GetInstance()->mCamera.mCurrCamInfo = SceneManager::GetInstance()->GetActiveScene()->mCamWaypoints[SceneManager::GetInstance()->GetActiveScene()->mCamWaypointIndex];

		ImGui::Text("Camera Thresholds");
		ImGui::Text("Left");
		ImGui::SameLine(); ImGui::SetNextItemWidth(ImGui::GetWindowWidth() / 4);
		ImGui::InputFloat("##CamThresholdLeft", &GraphicsSystem::GetInstance()->mCamera.mHorizontalThreshold.x);
		ImGui::SameLine();
		ImGui::Text("Right");
		ImGui::SameLine(); ImGui::SetNextItemWidth(ImGui::GetWindowWidth() / 4);
		ImGui::InputFloat("##CamThresholdRight", &GraphicsSystem::GetInstance()->mCamera.mHorizontalThreshold.y);
		ImGui::Spacing();
		ImGui::Text("Bottom");
		ImGui::SameLine(); ImGui::SetNextItemWidth(ImGui::GetWindowWidth() / 4);
		ImGui::InputFloat("##CamThresholdBottom", &GraphicsSystem::GetInstance()->mCamera.mVerticalThreshold.x);
		ImGui::SameLine();
		ImGui::Text("Top");
		ImGui::SameLine(); ImGui::SetNextItemWidth(ImGui::GetWindowWidth() / 4);
		ImGui::InputFloat("##CamThresholdTop", &GraphicsSystem::GetInstance()->mCamera.mVerticalThreshold.y);
		ImGui::Spacing();

		ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing();

		//Display current camera details
		GraphicsSystem::GetInstance()->mCamera.mCurrCamInfo.DisplayProperties(-1);

		ImGui::Spacing();

		if (ImGui::Button("Add as new Waypoint"))
			SceneManager::GetInstance()->GetActiveScene()->mCamWaypoints.push_back(GraphicsSystem::GetInstance()->mCamera.mCurrCamInfo);

		ImGui::Spacing();

		//Display waypoints
		if(ImGui::TreeNode("Scene Waypoints"))
		{
			static ImGuiTreeNodeFlags base_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth;
			static int selection_mask = (1 << 2);
			static int node_clicked = -1;

			for (size_t i = 0; i < SceneManager::GetInstance()->GetActiveScene()->mCamWaypoints.size(); ++i)
			{
				ImGuiTreeNodeFlags node_flags = base_flags;
				const bool is_selected = (selection_mask & (1 << i)) != 0;
				if (is_selected)
					node_flags |= ImGuiTreeNodeFlags_Selected;
				std::string nodeName{ "Waypoint " + std::to_string(i) };

				bool node_open = ImGui::TreeNodeEx((void*)(intptr_t)i, node_flags, nodeName.c_str());
				if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen())
					node_clicked = (int)i;
				if (ImGui::IsItemClicked(ImGuiMouseButton_Right))
				{
					node_clicked = (int)i;
					ImGui::OpenPopup((nodeName + "Popup").c_str());
				}
				if (ImGui::BeginPopup((nodeName + "Popup").c_str()))
				{
					if (ImGui::Selectable("New Waypoint"))
						SceneManager::GetInstance()->GetActiveScene()->mCamWaypoints.push_back(CamWaypoint{});
					if (ImGui::Selectable("Duplicate"))
						SceneManager::GetInstance()->GetActiveScene()->mCamWaypoints.push_back(mCamWaypoints[node_clicked]);
					if (ImGui::Selectable("Delete"))
					{
						mCamWaypointsDelQueue.push(node_clicked);
						node_clicked = -1;
					}
					ImGui::EndPopup();
				}

				if (node_open)
				{
					SceneManager::GetInstance()->GetActiveScene()->mCamWaypoints[i].DisplayProperties((int)i);
					ImGui::TreePop();
				}
			}
			if (node_clicked != -1)
			{
				// Update selection state
				// (process outside of tree loop to avoid visual inconsistencies during the clicking frame)
				if (ImGui::GetIO().KeyCtrl)
					selection_mask ^= (1 << node_clicked);          // CTRL+click to toggle
				else //if (!(selection_mask & (1 << node_clicked))) // Depending on selection behavior you want, may want to preserve selection when clicking on item that is part of the selection
					selection_mask = (1 << node_clicked);           // Click to single-select
			}

			ImGui::TreePop();
		}

		ImGui::End();

		//Deletion of waypoint nodes if any
		while (mCamWaypointsDelQueue.size() > 0)
		{
			SceneManager::GetInstance()->GetActiveScene()->mCamWaypoints.erase(SceneManager::GetInstance()->GetActiveScene()->mCamWaypoints.begin() + mCamWaypointsDelQueue.front());
			mCamWaypointsDelQueue.pop();
		}
	}

	//Properties based on item selected in hierarchial list
	if (mShowProperties)
	{
		ImGui::Begin("Properties", &mShowProperties);
		if (mSelectedEnt)
		{
			bool isPrefab = false;
			if (mSelectedEnt == &mPrefabEnt)//Editing prefab
			{
				isPrefab = true;
				ImGui::SeparatorText("Prefab");
				if (ImGui::Button("SAVE", ImVec2(80, 20)))
				{
					//save
					LevelLoader::SavePrefab(mSelectedEnt->GetName(), mSelectedEnt, mCurrentOpenPath, true, &mPrefabInfo);
				}
			}
			{
				ImGui::Text("Name");
				char nameBuffer[1024]{};
				strcpy(nameBuffer, mSelectedEnt->GetName().c_str());
				ImGui::SameLine(); ImGui::SetNextItemWidth(-1);
				if (ImGui::InputText("##Name", nameBuffer, 1024))
				{
					mSelectedEnt->SetName(nameBuffer);
				}
				//Get all components
				LevelLoader::AllComponents comps(mSelectedEnt, isPrefab, &mPrefabInfo);
				for (int i = 0; i < comps.componentsList.size(); ++i)
				{
					std::string componentName = comps.componentsList[i].second;
					componentName = componentName.substr(6, componentName.size());
					if (!ImGui::CollapsingHeader(componentName.c_str(), ImGuiTreeNodeFlags_DefaultOpen))
						continue;//not open so just skip

					if (ImGui::IsMouseClicked(1) && ImGui::IsItemHovered())
					{
						mSelectedID = ImGui::GetItemID();
						ImGui::OpenPopup("ComponentOptions");
					}

					if (mSelectedID == ImGui::GetItemID())
					{
						if (ImGui::BeginPopup("ComponentOptions"))
						{
							if (ImGui::Button("Delete##component"))
							{
								if (!isPrefab)
								{
									//if (componentName == "Transform") mSelectedEnt->RemoveComponent<Transform>(); //disabled cause need to add check incase PhysicsComponent is attached
									if (componentName == "Renderer") mSelectedEnt->RemoveComponent<Renderer>();
									else if (componentName == "PhysicsComponent") mSelectedEnt->RemoveComponent<PhysicsComponent>();
									else if (componentName == "Collider_AABB") mSelectedEnt->RemoveComponent<Collider_AABB>();
									else if (componentName == "Player") mSelectedEnt->RemoveComponent<Player>();
									else if (componentName == "PlayerCollisionHandler") mSelectedEnt->RemoveComponent<PlayerCollisionHandler>();
									else if (componentName == "EnemyCollisionHandler") mSelectedEnt->RemoveComponent<EnemyCollisionHandler>();
									else if (componentName == "GameUIObjectHandler") mSelectedEnt->RemoveComponent<GameUIObjectHandler>();
									else if (componentName == "GameUITextHandler") mSelectedEnt->RemoveComponent<GameUITextHandler>();
									else if (componentName == "CSScriptComponent") mSelectedEnt->RemoveComponent<CSScriptComponent>();
									else if (componentName == "AIComponent") mSelectedEnt->RemoveComponent<AIComponent>();
									else if (componentName == "SoundComponent") mSelectedEnt->RemoveComponent<SoundComponent>();
									else if (componentName == "ButtonWrapperComponent") mSelectedEnt->RemoveComponent<ButtonWrapper>();
									else if (componentName == "Key") mSelectedEnt->RemoveComponent<Key>();
									else if (componentName == "KeyCollisionHandler") mSelectedEnt->RemoveComponent<KeyCollisionHandler>();
									else if (componentName == "KeyWall") mSelectedEnt->RemoveComponent<KeyWall>();
									else if (componentName == "KeyWallCollisionHandler") mSelectedEnt->RemoveComponent<KeyWallCollisionHandler>();
									else if (componentName == "VFXEntity") mSelectedEnt->RemoveComponent<VFXEntity>();
									else if (componentName == "WindCollisionHandler") mSelectedEnt->RemoveComponent<WindCollisionHandler>();
									else if (componentName == "Wind") mSelectedEnt->RemoveComponent<Wind>();
									else if (componentName == "EndGoal") mSelectedEnt->RemoveComponent<EndGoal>();
									else if (componentName == "PlayOnceSoundComponent") mSelectedEnt->RemoveComponent<PlayOnceSoundComponent>();
									else if (componentName == "DialogueTrigger") mSelectedEnt->RemoveComponent<DialogueTrigger>();
									else if (componentName == "NextCPCollisionHandler") mSelectedEnt->RemoveComponent<NextCPCollisionHandler>();
									ImGui::EndPopup();
									continue;
								}
							}
							ImGui::EndPopup();
						}
					}

					comps.componentsList[i].first->DisplayProperties();
					if (componentName == "Renderer")
					{
						Renderer* rend = static_cast<Renderer*>(comps.componentsList[i].first);
						if (ImGui::BeginPopup("##SpriteList"))//for sprite list
						{
							AssetManager::DirectoryInfo imageList = AssetManager::GetInstance()->GetDirectoryInfo(mBasePath, AssetManager::SPRITE, true);

							for (int imgCount = 0; imgCount < imageList.size(); ++imgCount)
							{
								if (ImGui::Selectable((imageList[imgCount].fileName + "##SpriteList").c_str()))
								{
									rend->mTextureName = imageList[imgCount].fileName;
								}
							}
							ImGui::EndPopup();
						}
					}
				}

				//add new component
				{
					ImGui::Separator();
					if (ImGui::Button("Add Component"))
					{
						ImGui::OpenPopup("ComponentList");
					}

					ImVec2 popupPos = ImGui::GetCurrentContext()->LastItemData.Rect.GetCenter();//just to make it look nice
					if (popupPos.y <= WindowsManager::GetInstance()->mHeight - (12 * g->FontSize))
					{
						//(popupPos.y += (12 * g->FontSize));
					}
					else
					{
						(popupPos.y -= (12 * (g->FontSize + g->Style.WindowPadding.y + g->Style.ItemSpacing.y)));
					}
					ImGui::SetNextWindowPos(popupPos);
					ImGui::SetNextWindowSizeConstraints(ImVec2(0, 0), ImVec2(12 * g->FontSize + g->Style.WindowPadding.x * 2, WindowsManager::GetInstance()->mHeight - popupPos.y - g->FontSize));//Fixed the size
					if (ImGui::BeginPopup("ComponentList"))
					{
						if (ImGui::Button("Transform##Add"))
						{
							mSelectedEnt->AddComponent<Transform>();
							ImGui::CloseCurrentPopup();
						}
						if (ImGui::Button("Renderer##Add"))
						{
							mSelectedEnt->AddComponent<Renderer>();
							ImGui::CloseCurrentPopup();
						}
						if (ImGui::Button("PhysicsComponent##Add"))
						{
							mSelectedEnt->AddComponent<PhysicsComponent>();
							ImGui::CloseCurrentPopup();
						}
						if (ImGui::Button("Collider_AABB##Add"))
						{
							mSelectedEnt->AddComponent<Collider_AABB>();
							ImGui::CloseCurrentPopup();
						}
						if (ImGui::Button("Player##Add"))
						{
							mSelectedEnt->AddComponent<Player>();
							ImGui::CloseCurrentPopup();
						}
						if (ImGui::Button("PlayerCollisionHandler##Add"))
						{
							mSelectedEnt->AddComponent<PlayerCollisionHandler>();
							ImGui::CloseCurrentPopup();
						}
						if (ImGui::Button("EnemyCollisionHandler##Add"))
						{
							mSelectedEnt->AddComponent<EnemyCollisionHandler>();
							ImGui::CloseCurrentPopup();
						}
						if (ImGui::Button("GameUIObjectHandler##Add"))
						{
							mSelectedEnt->AddComponent<GameUIObjectHandler>();
							ImGui::CloseCurrentPopup();
						}
						if (ImGui::Button("GameUITextHandler##Add"))
						{
							mSelectedEnt->AddComponent<GameUITextHandler>();
							if (mSelectedEnt->GetComponent<Transform>() == nullptr)
								mSelectedEnt->AddComponent<Transform>();
							ImGui::CloseCurrentPopup();
						}
						if (ImGui::Button("AIComponent##Add"))
						{
							mSelectedEnt->AddComponent<AIComponent>();
							ImGui::CloseCurrentPopup();
						}
						if (ImGui::Button("SoundComponent##Add"))
						{
							mSelectedEnt->AddComponent<SoundComponent>();
							ImGui::CloseCurrentPopup();
						}
						if (ImGui::Button("ButtonWrapperComponent##Add"))
						{
							mSelectedEnt->AddComponent<ButtonWrapper>();
							ImGui::CloseCurrentPopup();
						}
						if (ImGui::Button("Key##Add"))
						{
							mSelectedEnt->AddComponent<Key>();
							ImGui::CloseCurrentPopup();
						}
						if (ImGui::Button("KeyCollisionHandler##Add"))
						{
							mSelectedEnt->AddComponent<KeyCollisionHandler>();
							ImGui::CloseCurrentPopup();
						}
						if (ImGui::Button("KeyWall##Add"))
						{
							mSelectedEnt->AddComponent<KeyWall>();
							ImGui::CloseCurrentPopup();
						}
						if (ImGui::Button("KeyWallCollisionHandler##Add"))
						{
							mSelectedEnt->AddComponent<KeyWallCollisionHandler>();
							ImGui::CloseCurrentPopup();
						}
						if (ImGui::Button("VFXEntity##Add"))
						{
							mSelectedEnt->AddComponent<VFXEntity>();
							ImGui::CloseCurrentPopup();
						}
						if (ImGui::Button("WindCollisionHandler##Add"))
						{
							mSelectedEnt->AddComponent<WindCollisionHandler>();
							ImGui::CloseCurrentPopup();
						}
						if (ImGui::Button("Wind##Add"))
						{
							mSelectedEnt->AddComponent<Wind>();
							ImGui::CloseCurrentPopup();
						}
						if (ImGui::Button("EndGoal##Add"))
						{
							mSelectedEnt->AddComponent<EndGoal>();
							ImGui::CloseCurrentPopup();
						}
						if (ImGui::Button("PlayOnceSoundComponent##Add"))
						{
							mSelectedEnt->AddComponent<PlayOnceSoundComponent>();
							ImGui::CloseCurrentPopup();
						}
						if (ImGui::Button("DialogueTrigger##Add"))
						{
							mSelectedEnt->AddComponent<DialogueTrigger>();
							ImGui::CloseCurrentPopup();
						}
						if (ImGui::Button("NextCPCollisionHandler##Add"))
						{
							mSelectedEnt->AddComponent<NextCPCollisionHandler>();
							ImGui::CloseCurrentPopup();
						}
						//Get all scripts
						AssetManager::DirectoryInfo scriptList = AssetManager::GetInstance()->GetDirectoryInfo(mBasePath, AssetManager::CSSCRIPT, true);
						for (AssetManager::DirectoryItem& scriptFile : scriptList)
						{
							if (ImGui::Button(("<Script> " + scriptFile.fileName + "##Add").c_str()))
							{
								CSScriptComponent* script = mSelectedEnt->AddComponent<CSScriptComponent>();
								if (script)
									script->SetScript(scriptFile.fileName);
								ImGui::CloseCurrentPopup();
							}
						}
						ImGui::EndPopup();
					}
				}
			}//Editing scene entity
			//Hierarchy table
		}
		ImGui::End();
	}

	//Tile Palette
	if (mShowTilePalette)
	{
		ImGui::Begin("Tile Palette", &mShowTilePalette);

		//TILE SELECTOR
		{
			ImGui::Text("Tile Selector");
			AssetManager::DirectoryInfo dirInfo = AssetManager::GetInstance()->GetDirectoryInfo("Assets/Prefabs/Tiles");
			ImVec2 size = ImVec2(64.0f, 64.0f);                         // Size of the image we want to make visible
			ImVec2 padding = ImVec2(2.5f, 2.5f);						// size of adding around 
			ImVec2 uv0 = ImVec2(0.0f, 1.0f);                            // UV coordinates for lower-left
			ImVec2 uv1 = ImVec2(1.0f, 0.0f);							// UV coordinates for (32,32) in our texture
			ImVec4 bg_col = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);             // Black background
			ImVec4 tint_col = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);           // No tint
			float mExplorer_width = ImGui::GetContentRegionAvail().x;
			int mNumberofColumns = static_cast<int>(mExplorer_width / (size.x + padding.x * 2));
			if (mNumberofColumns < 1)
				mNumberofColumns = 1;

			ImGui::Columns(mNumberofColumns, 0, false);
			for (int i = 0; i < dirInfo.size(); ++i)
			{
				AssetManager::ImageAsset* tilePrefabImage{ nullptr };

				if (dirInfo[i].type != AssetManager::PREFAB)
					continue;

				//Get tile image
				TileComponent::mLoadMap = false;
				LevelLoader::PrefabInfo mTilePrefabInfo;
				LevelLoader::LoadPrefabInfo(dirInfo[i].fileName, mTilePrefabInfo);
				if (mTilePrefabInfo.mRenderer.second == true)
					AssetManager::GetInstance()->GetAsset(mTilePrefabInfo.mRenderer.first.mTextureName, tilePrefabImage);

				//Load the image
				GLuint tileTexID{ 0 };
				if (tilePrefabImage)
					tileTexID = tilePrefabImage->mTexID;

				if (mSelectedTileName == dirInfo[i].fileName)
					bg_col = ImVec4(0.75f, 0.97f, 0.29f, 1.0f);
				else
					bg_col = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);

				ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, padding);
				if (ImGui::ImageButton(std::string("#####" + dirInfo[i].fileName + "PALETTE").c_str(), (ImTextureID*)(intptr_t)(tileTexID), size, uv0, uv1, bg_col, tint_col))
					mSelectedTileName = dirInfo[i].fileName;
				ImGui::PopStyleVar();

				ImGui::NextColumn();
			}
			ImGui::Columns(1);
		}
		ImGui::Spacing();
		ImGui::Spacing();

		//Toggle Buttons
		ImGui::Text("Configs");
		ImGui::Checkbox("Tile Palette Enabled", &mTilePaletteEnabled);
		ImGui::Checkbox("Snap To Grid", &mSnapToGrid);

		if (ImGui::Button("Load Tile Collision Data (Jovan this one)"))
		{
			TileComponent::LoadTileData();
			GraphicsSystem::GetInstance()->mCollisionGrid.GenerateGridModel(true);
		}
		
		//Keybinds
		ImGui::Spacing();
		ImGui::Text("Press 'C' to spawn selected tile");
		if (mTilePaletteEnabled)
		{
			if (InputManager::GetInstance()->GetKeyTriggered(KeyCode::C))
			{
				TileComponent::mLoadMap = false;
				Entity* loadedPrefab = LevelLoader::LoadPrefab(mSelectedTileName);
				loadedPrefab->GetComponent<Renderer>()->mEnabled = true;

				Vector3 pos = GraphicsSystem::GetInstance()->mCamera.mCurrCamInfo.mPosition + InputManager::GetInstance()->GetMousePosScreen();
				if (mSnapToGrid == false)
					loadedPrefab->GetComponent<Transform>()->SetPosition(pos);
				else
				{
					TileComponent::SnapToCell(&pos.mX);
					TileComponent::SnapToCell(&pos.mY);
					loadedPrefab->GetComponent<Transform>()->SetPosition(pos);
				}

				mSelectedEnt = loadedPrefab;
			}
		}

		ImGui::End();
	}
}

void EditorMainScene::Pause(double _dt)
{
	UNREFERENCED_PARAMETER(_dt);
}

void EditorMainScene::Render(double _dt)
{
	UNREFERENCED_PARAMETER(_dt);

}

void EditorMainScene::Free()
{

}

void EditorMainScene::Exit()
{

}


/*!
 * @brief
 * Creates a player entity with a collider
*/
void EditorMainScene::CreatePlayer()
{
	Entity* newEnt = EntityManager::GetInstance()->CreateEntity();
	newEnt->SetName("Player");

	if (newEnt == nullptr)
		return;

	Transform newXform{};
	newXform.SetPosition(Vector3{-10, 20, 1});
	newXform.mScale = Vector3{10, 10, 1};
	newEnt->AddComponent(newXform);

	Renderer* _renderer = newEnt->AddComponent<Renderer>();
	//_renderer->mModelType = Renderer::MODEL_TYPE::Sprite;
	//_renderer->mTextureName = "placeholder";

	_renderer->mModelType = Renderer::MODEL_TYPE::SPRITE_ANIMATION;
	//_renderer->mDebugModelType = Renderer::DEBUG_MODEL_TYPE::Box;
	_renderer->mTextureName = "SpriteSheet_DeathAnimation";

	//AnimationComponent* anim = newEnt->AddComponent<AnimationComponent>();
	//anim->mAnimation.mTotalFrames = 17;
	//anim->mAnimation.mTimeDelay = 0.05f;
	//anim->mAnimation.mIsLooping = true;

	//Add physics components
	PhysicsComponent* phy = newEnt->AddComponent<PhysicsComponent>();
	phy->mGravity.mY = 80;
	phy->mMass = 5;
	phy->mIsCharacterSmall = false;
	phy->mJumpMultiplier = 1.2f;
	phy->mObjBigScale = Vec3{10, 10, 1};

	TileComponent* map = newEnt->AddComponent<TileComponent>();
	map->mGridCollisionFlag = 0;
	map->mFlag = TileComponent::FLAG_ACTIVE | TileComponent::FLAG_VISIBLE;

	//Add player component for movement
	Player* plr = newEnt->AddComponent<Player>();
	plr->mJumpStrength = 30000;
	plr->mMoveSpeed = 40;

	//Add collision aabb
	newEnt->AddComponent<Collider_AABB>();

	newEnt->AddComponent<PlayerCollisionHandler>();
	newEnt->AddComponent<GfxHandler>();
}

/*!
 * @brief
 * Creates an entity with a collider
*/
void EditorMainScene::CreateObject1()
{
	Entity* newEnt = EntityManager::GetInstance()->CreateEntity();

	if (newEnt == nullptr)
		return;

	newEnt->SetName("CollidableObj");
	//Random number generator
	std::default_random_engine randomGen(std::random_device{}());
	std::uniform_real_distribution<float> distributionX(-1.f, std::nextafter(1.f, std::numeric_limits<float>::max()));
	std::uniform_real_distribution<float> distributionY(0.f, std::nextafter(1.f, std::numeric_limits<float>::max()));

	//Translate based on world range
	Transform newXform{};
	float x_pos = distributionX(randomGen) * (GraphicsSystem::GetInstance()->mWorldSize.x / 2);
	float y_pos = distributionY(randomGen) * (GraphicsSystem::GetInstance()->mWorldSize.y / 2);
	newXform.SetPosition(Vector3{x_pos, y_pos, 1});

	//Scale
	newXform.mScale = Vector3{30, 30, 1};

	newEnt->AddComponent(newXform);
	Renderer* _renderer = newEnt->AddComponent<Renderer>();
	_renderer->mModelType = Renderer::MODEL_TYPE::SPRITE;
	_renderer->mTextureName = "collidable";

	//Add physics components
	PhysicsComponent* phy = newEnt->AddComponent<PhysicsComponent>();
	phy->mGravity.mY = 0;

	//Add collision aabb
	newEnt->AddComponent<Collider_AABB>();

	newEnt->AddComponent<EnemyCollisionHandler>();
}

/*!
 * @brief
 * Spawns in wall
*/
void::EditorMainScene::SpawnWall(int _height, int _width)
{
	Entity* Wall = EntityManager::GetInstance()->CreateEntity();

	if (Wall == nullptr)
		return;

	Transform newXform{};
	newXform.SetPosition(Vector3{
		TileComponent::mMinBound.mX + _width * static_cast<float>(TileComponent::mTileSize) + TileComponent::mTileSize / 2,
		TileComponent::mMinBound.mY + _height * static_cast<float>(TileComponent::mTileSize) + TileComponent::mTileSize / 2,
		1});

	newXform.mScale = Vector3{10, 10, 1};
	Wall->AddComponent(newXform);

	Renderer* _renderer = Wall->AddComponent<Renderer>();
	_renderer->mModelType = Renderer::MODEL_TYPE::QUAD;

	//adding collision flags
	TileComponent* map = Wall->AddComponent<TileComponent>();
	map->mFlag = TileComponent::FLAG_ACTIVE | TileComponent::FLAG_VISIBLE;
	map->mFlag ^= TileComponent::FLAG_VISIBLE;
	map->mFlag |= TileComponent::FLAG_NON_COLLIDABLE;
}

/*!
 * @brief
 * Spawns in platform
*/
void EditorMainScene::SpawnPlatform(int _height, int _width)
{
	Entity* Platform = EntityManager::GetInstance()->CreateEntity();

	if (Platform == nullptr)
		return;

	Transform newXform{};
	newXform.SetPosition(Vector3{
		TileComponent::mMinBound.mX + _width * static_cast<float>(TileComponent::mTileSize) + TileComponent::mTileSize / 2,
		TileComponent::mMinBound.mY + _height * static_cast<float>(TileComponent::mTileSize) + TileComponent::mTileSize / 2,
		1});

	newXform.mScale = Vector3{10, 10, 1};
	Platform->AddComponent(newXform);

	Renderer* _renderer = Platform->AddComponent<Renderer>();
	_renderer->mModelType = Renderer::MODEL_TYPE::SPRITE;

	//adding collision flags
	TileComponent* map = Platform->AddComponent<TileComponent>();
	map->mFlag = TileComponent::FLAG_ACTIVE | TileComponent::FLAG_VISIBLE;
	map->mFlag ^= TileComponent::FLAG_VISIBLE;
	map->mFlag |= TileComponent::FLAG_NON_COLLIDABLE;
}

/*!
 * @brief
 * Creates grid
*/
void EditorMainScene::CreateGrid()
{
	for (int i = 0; i < 10; ++i)
	{
		Entity* obj = EntityManager::GetInstance()->CreateEntity();
		obj->SetName("FloorObj" + std::to_string(i));
		if (obj == nullptr)
			return;

		Transform newXform{};
		newXform.SetPosition(Vector3{-50 + i * (float)TileComponent::mTileSize + 5, -55, 1});
		newXform.mScale = Vector3{10, 10, 1};
		obj->AddComponent(newXform);
		TileComponent* tile = obj->AddComponent<TileComponent>();
		tile->mObjectType = TileComponent::OBJECT::TILE_PLATFORM;
		Renderer* rdr = obj->AddComponent<Renderer>();
		rdr->mModelType = Renderer::MODEL_TYPE::SPRITE;
		if (i == 9) rdr->mTextureName = "TileSprite_Connector_BottomRight";
		else if (i == 0) rdr->mTextureName = "TileSprite_Connector_BottomLeft";
		else rdr->mTextureName = "TileSprite_Tileset_Bottom";
	}

	for (int i = 0; i < 10; ++i)
	{
		Entity* obj = EntityManager::GetInstance()->CreateEntity();
		obj->SetName("FloorObj" + std::to_string(i));
		if (obj == nullptr)
			return;

		Transform newXform{};
		newXform.SetPosition(Vector3{-50 + i * (float)TileComponent::mTileSize + 5, 55, 1});
		newXform.mScale = Vector3{10, 10, 1};
		obj->AddComponent(newXform);
		TileComponent* tile = obj->AddComponent<TileComponent>();
		tile->mObjectType = TileComponent::OBJECT::TILE_PLATFORM;
		Renderer* rdr = obj->AddComponent<Renderer>();
		rdr->mModelType = Renderer::MODEL_TYPE::SPRITE;
		if (i == 9) rdr->mTextureName = "TileSprite_Connector_TopRight";
		else if (i == 0) rdr->mTextureName = "TileSprite_Connector_TopLeft";
		else rdr->mTextureName = "TileSprite_Tileset_Top";
	}

	for (int i = 0; i < 10; ++i)
	{
		Entity* obj = EntityManager::GetInstance()->CreateEntity();
		obj->SetName("WallObj" + std::to_string(i));
		if (obj == nullptr)
			return;

		Transform newXform{};
		newXform.SetPosition(Vector3{-45, -45 + i * (float)TileComponent::mTileSize, 1});
		newXform.mScale = Vector3{10, 10, 1};
		obj->AddComponent(newXform);
		TileComponent* tile = obj->AddComponent<TileComponent>();
		tile->mObjectType = TileComponent::OBJECT::TILE_WALL;
		Renderer* rdr = obj->AddComponent<Renderer>();
		rdr->mModelType = Renderer::MODEL_TYPE::SPRITE;
		if (i == 3) rdr->mTextureName = "TileSprite_Connector_TopLeft";
		else if (i == 4) rdr->mTextureName = "TileSprite_Connector_BottomLeft";
		else rdr->mTextureName = "TileSprite_Tileset_Left";
	}

	for (int i = 0; i < 10; ++i)
	{
		Entity* obj = EntityManager::GetInstance()->CreateEntity();
		obj->SetName("Wall" + std::to_string(i));
		if (obj == nullptr)
			return;

		Transform newXform{};
		newXform.SetPosition(Vector3{45, -45 + i * (float)TileComponent::mTileSize, 1});
		newXform.mScale = Vector3{10, 10, 1};
		obj->AddComponent(newXform);
		TileComponent* tile = obj->AddComponent<TileComponent>();
		tile->mObjectType = TileComponent::OBJECT::TILE_WALL;
		Renderer* rdr = obj->AddComponent<Renderer>();
		rdr->mModelType = Renderer::MODEL_TYPE::SPRITE;
		rdr->mTextureName = "TileSprite_Tileset_Right";
	}

	for (int i = 0; i < 2; ++i)
	{
		Entity* obj = EntityManager::GetInstance()->CreateEntity();
		obj->SetName("Wall" + std::to_string(i));
		if (obj == nullptr)
			return;

		Transform newXform{};
		newXform.SetPosition(Vector3{-45.f + 1.0f * (float)TileComponent::mTileSize, -15.f + i * (float)TileComponent::mTileSize, 1});
		newXform.mScale = Vector3{10, 10, 1};
		obj->AddComponent(newXform);
		TileComponent* tile = obj->AddComponent<TileComponent>();
		tile->mObjectType = TileComponent::OBJECT::TILE_WALL;
		Renderer* rdr = obj->AddComponent<Renderer>();
		rdr->mModelType = Renderer::MODEL_TYPE::SPRITE;
		if (i == 1) rdr->mTextureName = "TileSprite_Tileset_BottomRight";
		else rdr->mTextureName = "TileSprite_Tileset_TopRight";
	}
}

void EditorMainScene::CreateLaser()
{
	Renderer* rdr;
	Entity* LaserVisual;
	Transform newXformVisual{};

	for (int i = 0; i < 5; ++i)
	{
		LaserVisual = EntityManager::GetInstance()->CreateEntity();
		if (LaserVisual != nullptr)
		{
			LaserVisual->SetName("LaserHazard");
			newXformVisual.SetPosition(Vector3{28.f, -80.f + (float)i * 40.0f, 1.f});
			newXformVisual.mScale = Vector3{10.f, 40.f, 1.f};
			newXformVisual.mRotation = 0;
			LaserVisual->AddComponent(newXformVisual);
			rdr = LaserVisual->AddComponent<Renderer>();
			rdr->mShaderName = "LaserShader";
			rdr->mModelType = Renderer::MODEL_TYPE::SPRITE;
			rdr->mTextureName = "Misc_StackableLaser";
			LaserVisual->AddComponent<PhysicsComponent>()->mMass = 1;
			LaserVisual->AddComponent<Collider_AABB>();
			LaserVisual->AddComponent<EnemyCollisionHandler>();
		}
	}
}

void EditorMainScene::ShowRenderLayers()
{
	if (ImGui::Checkbox("Background ", &mBackGroundLayerEnabled))
		ToggleRenderLayer(Renderer::RENDER_LAYER::BACKGROUND, mBackGroundLayerEnabled);
	if (ImGui::Checkbox("Midground ", &mMidGroundLayerEnabled))
		ToggleRenderLayer(Renderer::RENDER_LAYER::MIDGROUND, mMidGroundLayerEnabled);
	if (ImGui::Checkbox("Foreground ", &mForeGroundLayerEnabled))
		ToggleRenderLayer(Renderer::RENDER_LAYER::FOREGROUND, mForeGroundLayerEnabled);
}

void EditorMainScene::ToggleRenderLayer(Renderer::RENDER_LAYER _layer, bool _enabled)
{
	Renderer* rdr;
	for (auto& entity : EntityManager::GetInstance()->mEntityList)
	{
		rdr = entity.GetComponent<Renderer>();

		if (rdr == nullptr)
			continue;

		if (rdr->mRenderLayer == _layer)
		{
			rdr->mEnabled = _enabled;
			entity.SetActive(_enabled);
		}
	}
}

void EditorMainScene::CreateWindTunnels()
{
	Renderer* rdr;
	Entity* WindTunnelVisual;
	Transform newXformVisual{};

	WindTunnelVisual = EntityManager::GetInstance()->CreateEntity();
	if (WindTunnelVisual != nullptr)
	{
		WindTunnelVisual->SetName("WindTunnelVisualUp");
		newXformVisual.SetPosition(Vector3{25.f, -45.f + (1.5f) * (float)TileComponent::mTileSize, 1.f});
		newXformVisual.mScale = Vector3{10.f, 4.0f * 10.f, 1.f};
		newXformVisual.mRotation = 0;
		WindTunnelVisual->AddComponent(newXformVisual);
		rdr = WindTunnelVisual->AddComponent<Renderer>();
		rdr->mShaderName = "WindShader";
		rdr->mModelType = Renderer::MODEL_TYPE::SPRITE;
		rdr->mTextureName = "placeholder";
	}
	for (int i = 0; i < 4; ++i)
	{
		Entity* WindUp = EntityManager::GetInstance()->CreateEntity();
		WindUp->SetName("WindTunnelUp" + std::to_string(i));
		if (WindUp == nullptr)
			return;

		Transform newXform{};
		newXform.SetPosition(Vector3{25, -45 + i * (float)TileComponent::mTileSize, 1});
		newXform.mScale = Vector3{10, 10, 1};
		WindUp->AddComponent(newXform);
		TileComponent* tunnel = WindUp->AddComponent<TileComponent>();
		tunnel->mObjectType = TileComponent::OBJECT::WIND_TUNNEL_UP;
		//Renderer* rdr = WindUp->AddComponent<Renderer>();
		//rdr->mModelType = Renderer::MODEL_TYPE::SPRITE;
		//rdr->mTextureName = "Wall";
	}

	WindTunnelVisual = EntityManager::GetInstance()->CreateEntity();
	if (WindTunnelVisual != nullptr)
	{
		WindTunnelVisual->SetName("WindTunnelVisualRight");
		newXformVisual.SetPosition(Vector3{-25.f + (2.f) * (float)TileComponent::mTileSize, -44.f, 1.f});
		newXformVisual.mScale = Vector3{10.f, 5.0f * 10.f, 1.f};
		newXformVisual.mRotation = -90;
		WindTunnelVisual->AddComponent(newXformVisual);
		rdr = WindTunnelVisual->AddComponent<Renderer>();
		rdr->mShaderName = "WindShader";
		rdr->mModelType = Renderer::MODEL_TYPE::SPRITE;
		rdr->mTextureName = "placeholder";
	}
	for (int i = 0; i < 5; ++i)
	{
		Entity* WindUp = EntityManager::GetInstance()->CreateEntity();
		WindUp->SetName("WindTunnelRight" + std::to_string(i));
		if (WindUp == nullptr)
			return;

		Transform newXform{};
		newXform.SetPosition(Vector3{-25 + i * (float)TileComponent::mTileSize, -44 , 1});
		newXform.mScale = Vector3{10, 10, 1};
		WindUp->AddComponent(newXform);
		TileComponent* tunnel = WindUp->AddComponent<TileComponent>();
		tunnel->mObjectType = TileComponent::OBJECT::WIND_TUNNEL_RIGHT;
		//Renderer* rdr = WindUp->AddComponent<Renderer>();
		//rdr->mModelType = Renderer::MODEL_TYPE::SPRITE;
		//rdr->mTextureName = "Floor";
	}

	WindTunnelVisual = EntityManager::GetInstance()->CreateEntity();
	if (WindTunnelVisual != nullptr)
	{
		WindTunnelVisual->SetName("WindTunnelVisualLeft");
		newXformVisual.SetPosition(Vector3{-15.f + (2.f) * (float)TileComponent::mTileSize, -4.f, 1.f});
		newXformVisual.mScale = Vector3{10.f, 5.0f * 10.f, 1.f};
		newXformVisual.mRotation = 90;
		WindTunnelVisual->AddComponent(newXformVisual);
		rdr = WindTunnelVisual->AddComponent<Renderer>();
		rdr->mShaderName = "WindShader";
		rdr->mModelType = Renderer::MODEL_TYPE::SPRITE;
		rdr->mTextureName = "placeholder";
	}
	for (int i = 0; i < 5; ++i)
	{
		Entity* WindUp = EntityManager::GetInstance()->CreateEntity();
		WindUp->SetName("WindTunnelLeft" + std::to_string(i));
		if (WindUp == nullptr)
			return;

		Transform newXform{};
		newXform.SetPosition(Vector3{-15 + i * (float)TileComponent::mTileSize, -4 , 1});
		newXform.mScale = Vector3{10, 10, 1};
		WindUp->AddComponent(newXform);
		TileComponent* tunnel = WindUp->AddComponent<TileComponent>();
		tunnel->mObjectType = TileComponent::OBJECT::WIND_TUNNEL_LEFT;
		//Renderer* rdr = WindUp->AddComponent<Renderer>();
		//rdr->mModelType = Renderer::MODEL_TYPE::SPRITE;
		//rdr->mTextureName = "Floor";
	}
}

void EditorMainScene::SpawnWindTunnelUp()
{
	Renderer* rdr;
	Entity* WindTunnelVisual;
	Transform newXformVisual{};

	WindTunnelVisual = EntityManager::GetInstance()->CreateEntity();
	if (WindTunnelVisual != nullptr)
	{
		WindTunnelVisual->SetName("WindTunnelVisualUp");
		newXformVisual.SetPosition(Vector3{25.f, -45.f + (1.5f) * (float)TileComponent::mTileSize, 1.f});
		newXformVisual.mScale = Vector3{10.f, 4.0f * 10.f, 1.f};
		newXformVisual.mRotation = 0;
		WindTunnelVisual->AddComponent(newXformVisual);
		rdr = WindTunnelVisual->AddComponent<Renderer>();
		rdr->mShaderName = "WindShader";
		rdr->mModelType = Renderer::MODEL_TYPE::SPRITE;
		rdr->mTextureName = "placeholder";
	}
	for (int i = 0; i < 4; ++i)
	{
		Entity* WindUp = EntityManager::GetInstance()->CreateEntity();
		WindUp->SetName("WindTunnelUp" + std::to_string(i));
		if (WindUp == nullptr)
			return;

		Transform newXform{};
		newXform.SetPosition(Vector3{-25, -45 + i * (float)TileComponent::mTileSize, 1});
		newXform.mScale = Vector3{10, 10, 1};
		WindUp->AddComponent(newXform);
		TileComponent* tunnel = WindUp->AddComponent<TileComponent>();
		tunnel->mObjectType = TileComponent::OBJECT::WIND_TUNNEL_UP;
		//Renderer* rdr = WindUp->AddComponent<Renderer>();
		//rdr->mModelType = Renderer::MODEL_TYPE::SPRITE;
		//rdr->mTextureName = "Wall";
	}

	WindTunnelVisual = EntityManager::GetInstance()->CreateEntity();
	if (WindTunnelVisual != nullptr)
	{
		WindTunnelVisual->SetName("WindTunnelVisualUp");
		newXformVisual.SetPosition(Vector3{15.f, -45.f + (1.5f) * (float)TileComponent::mTileSize, 1.f});
		newXformVisual.mScale = Vector3{10.f, 4.0f * 10.f, 1.f};
		newXformVisual.mRotation = 0;
		WindTunnelVisual->AddComponent(newXformVisual);
		rdr = WindTunnelVisual->AddComponent<Renderer>();
		rdr->mShaderName = "WindShader";
		rdr->mModelType = Renderer::MODEL_TYPE::SPRITE;
		rdr->mTextureName = "placeholder";
	}
	for (int i = 0; i < 4; ++i)
	{
		Entity* WindUp = EntityManager::GetInstance()->CreateEntity();
		WindUp->SetName("WindTunnelUp" + std::to_string(i));
		if (WindUp == nullptr)
			return;

		Transform newXform{};
		newXform.SetPosition(Vector3{15, -45 + i * (float)TileComponent::mTileSize, 1});
		newXform.mScale = Vector3{10, 10, 1};
		WindUp->AddComponent(newXform);
		TileComponent* tunnel = WindUp->AddComponent<TileComponent>();
		tunnel->mObjectType = TileComponent::OBJECT::WIND_TUNNEL_UP;
		//Renderer* rdr = WindUp->AddComponent<Renderer>();
		//rdr->mModelType = Renderer::MODEL_TYPE::SPRITE;
		//rdr->mTextureName = "Wall";
	}

}


/*!
 * @brief
 * Creates an entity without a collider. For 2500 objects with 60fps
*/
void EditorMainScene::CreateMysteryBox()
{
	Entity* newEnt = EntityManager::GetInstance()->CreateEntity();

	if (newEnt == nullptr)
		return;

	newEnt->SetName("MysteryBox");
	//Random number generator
	std::default_random_engine randomGen(std::random_device{}());
	std::uniform_real_distribution<float> distribution(-1.f, std::nextafter(1.f, std::numeric_limits<float>::max()));

	//Translate, range from world size
	Transform newXform{};
	newXform.SetPosition(Vector3(distribution(randomGen) * (GraphicsSystem::GetInstance()->mWorldSize.x / 2),
											 distribution(randomGen) * (GraphicsSystem::GetInstance()->mWorldSize.y / 2),
											 1));

	//Scale, range from 5 to 10
	float const rangeMin = 5.f;
	float const rangeMax = 10.f;
	float const rangeMiddle = rangeMax - rangeMin;
	newXform.mScale = Vector3{distribution(randomGen) * (rangeMiddle / 2) + (rangeMiddle + rangeMin),
								distribution(randomGen) * (rangeMiddle / 2) + (rangeMiddle + rangeMin), 1};

	newEnt->AddComponent(newXform);
	Renderer* renderer = newEnt->AddComponent<Renderer>();
	renderer->mModelType = Renderer::MODEL_TYPE::QUAD;
}

/*!
 * @brief
 * Creates a prefab
*/
void EditorMainScene::CreatePrefab()
{
	Entity* loadedPrefab = LevelLoader::LoadPrefab("square");
	loadedPrefab->GetComponent<Transform>()->SetPosition(InputManager::GetInstance()->GetMousePosScreen());
}

/*!
 * @brief
 * Used when dragging an item from one place to another
 * @param _finalPath
*/
void EditorMainScene::MyImGuiDragTargetMove(std::string _finalPath)
{
	if (ImGui::BeginDragDropTarget())
	{
		//First for saving of prefabs
		const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("EntityDrag");//need to check before delivery so we can draw the yellow box
		if (payload)
		{
			LevelLoader::SavePrefab(std::string(static_cast<const char*>(payload->Data), payload->DataSize), mSelectedEnt, _finalPath);
		}

		//Moving of items around
		if (!payload) payload = ImGui::AcceptDragDropPayload("FolderDrag");
		if (!payload) payload = ImGui::AcceptDragDropPayload("SpriteDrag");
		if (!payload) payload = ImGui::AcceptDragDropPayload("AudioDrag");
		if (!payload) payload = ImGui::AcceptDragDropPayload("JSONDrag");
		if (!payload) payload = ImGui::AcceptDragDropPayload("PrefabDrag");
		if (!payload) payload = ImGui::AcceptDragDropPayload("SceneDrag");
		if (!payload) payload = ImGui::AcceptDragDropPayload("FontDrag");

		if (payload)
		{
			std::string fileName = std::string(static_cast<const char*>(payload->Data), payload->DataSize);
			fileName += "." + mCurrentDraggingExt;
			AssetManager::GetInstance()->MoveMyFile(mCurrentOpenPath + "/" + fileName, _finalPath + "/" + fileName);
		}
		else
		{
			payload = ImGui::AcceptDragDropPayload("ExternalDrag");
			if (payload)
			{
				AssetManager::GetInstance()->CopyFromExternal(_finalPath);
			}
		}

		ImGui::EndDragDropTarget();
	}
}

/*!
 * @brief
 * Used in Asset Browser to check if the current folder has a sub folder inside
 * @param _folderPath
 * @param _treeFlags
 * @return
 * True if the current folder has a sub folder inside
*/
bool EditorMainScene::RecursiveFolderDisplay(std::string _folderPath, ImGuiTreeNodeFlags _treeFlags)
{
	AssetManager::DirectoryInfo folderInfo = AssetManager::GetInstance()->GetDirectoryInfo(_folderPath, AssetManager::FOLDER);
	if (folderInfo.size() == 0)
		return false;
	for (int i = 0; i < folderInfo.size(); ++i)
	{
		if (AssetManager::GetInstance()->GetDirectoryInfo(folderInfo[i].fullPath, AssetManager::FOLDER).size() > 0)
		{
			if (ImGui::TreeNodeEx((folderInfo[i].fileName + "##folderList" + std::to_string(i)).c_str(), _treeFlags))
			{
				MyImGuiDragTargetMove(folderInfo[i].fullPath);
				if (ImGui::IsMouseClicked(0) && ImGui::IsItemHovered())
				{
					mCurrentOpenPath = folderInfo[i].fullPath;
				}
				RecursiveFolderDisplay(folderInfo[i].fullPath, _treeFlags);
				ImGui::TreePop();
			}
		}
		else
		{
			_treeFlags |= ImGuiTreeNodeFlags_Leaf;
			if (ImGui::TreeNodeEx((folderInfo[i].fileName + "##folderList" + std::to_string(i)).c_str(), _treeFlags))
			{
				MyImGuiDragTargetMove(folderInfo[i].fullPath);
				if (ImGui::IsMouseClicked(0) && ImGui::IsItemHovered())
				{
					mCurrentOpenPath = folderInfo[i].fullPath;
				}
				ImGui::TreePop();
			}
		}
	}

	return true;
}

/*!
 * @brief
 * Searches for an entity that the mousePos is in
*/
Entity* EditorMainScene::SelectObject(Vector3 _mousePos)
{
	//DebugLogger::GetInstance()->Log("Attempting to get entity at <%f/%f/%f>", _mousePos.mX, _mousePos.mY, _mousePos.mX);
	Entity* retEnt = nullptr;
	for (size_t i = 0; i < EntityManager::GetInstance()->mEntityList.size(); ++i)
	{
		Entity* currEntity = &(EntityManager::GetInstance()->mEntityList.at(i));
		if (EntityManager::GetInstance()->mEntityList[i].GetParentID() != 0)
		{
			currEntity = EntityManager::GetInstance()->GetEntity(EntityManager::GetInstance()->mEntityList[i].GetParentID());
		}
		if (currEntity->GetName() == "ParallaxBG_Z" || currEntity->GetName() == "ParallaxBG_A" || currEntity->GetName() == "ParallaxBG_B" || currEntity->GetName() == "ParallaxBG_C")
			continue;

		Transform* xform = currEntity->GetComponent<Transform>();

		if (xform == nullptr)
			continue;

		if (Collision::CollisionStaticAABB(_mousePos, *xform))
			retEnt = currEntity;
	}

	return retEnt;
}

/*!
 * @brief
 * Updates relevant values related to mouse position, as well as the dragged entity
*/
void EditorMainScene::DragEntity(Entity* _ent)
{
	if (_ent == nullptr || _ent->GetComponent<Transform>() == nullptr)
		return;

	mSelectedEnt = _ent;
	mMouseOffset = InputManager::GetInstance()->GetMousePosWorld() - mMouseStartVec;
	if (mSnapToGrid == false)
		_ent->GetComponent<Transform>()->SetPosition(mDragEntStartVec + mMouseOffset);
	else
	{
		Vector3 pos = mDragEntStartVec + mMouseOffset;
		TileComponent::SnapToCell(&pos.mX);
		TileComponent::SnapToCell(&pos.mY);
		_ent->GetComponent<Transform>()->SetPosition(pos);
	}
}

/*!
 * @brief Display system stats like fps
 * @param _debugInfoCloseButton
 * boolean of the current window if open or closed
*/
void EditorMainScene::DebugInfoWindow(bool _debugInfoCloseButton)
{
	ImGui::Begin("Debug Window", &_debugInfoCloseButton);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
	ImGui::SeparatorText("Window Stuff");
	ImGui::Text("OpenGL version: \t%s", glGetString(GL_VERSION));
	ImGui::Text("OpenGL renderer:\t%s", glGetString(GL_RENDERER));
	ImGui::Text("Window Size:\t\t%d x %d", WindowsManager::GetInstance()->mWidth, WindowsManager::GetInstance()->mHeight);

	ImGui::SeparatorText("Framerate Stuff");
	ImGui::Checkbox("FPS Cap", &CoreEngine::GetInstance()->mEnableFPSTarget);
	ImGui::SameLine();
	ImGui::Checkbox("Update Loop Cap", &CoreEngine::GetInstance()->mEnableUpdateLoopTarget);
	ImGui::Text("Processing rate:\t%.1f", WindowsManager::GetInstance()->mFPSOfUpdate);
	ImGui::Text("FPS:\t\t\t\t%.1f", CoreEngine::GetInstance()->mRenderFPS);


	if (ImGui::BeginTable("##AudioTestUITable", 2, ImGuiTableFlags_Resizable))
	{
		////////////////////////////////
		if (CoreEngine::GetInstance()->mEnableUpdateLoopTarget)
		{
			ImGui::TableNextRow(); ImGui::TableSetColumnIndex(0);
			ImGui::Text("Target Update Per Second:");
			ImGui::TableSetColumnIndex(1); ImGui::SetNextItemWidth(-1);
			ImGui::SliderFloat("##targetupdateloop", &CoreEngine::GetInstance()->mTargetUpdateFrequency, 60.f, 360.f, "%.0f");
		}
		////////////////////////////////
		if (CoreEngine::GetInstance()->mEnableFPSTarget)
		{
			ImGui::TableNextRow(); ImGui::TableSetColumnIndex(0);
			ImGui::Text("Target Framerate:\t\t");
			ImGui::TableSetColumnIndex(1); ImGui::SetNextItemWidth(-1);
			ImGui::SliderFloat("##targetfps", &CoreEngine::GetInstance()->mTargetRenderFPS, 60.f, 360.f, "%.0f");
		}
		ImGui::EndTable();
	}

	ImGui::SeparatorText("Entity Stuff");
	ImGui::Text("Current Entity Count: %d", EntityManager::GetInstance()->mEntityList.size());
	ImGui::Text("Max Entities Count:"); ImGui::SameLine();
	ImGui::SetNextItemWidth(-1);
	ImGui::InputScalar("###", ImGuiDataType_::ImGuiDataType_U64, &EntityManager::GetInstance()->mTargetMaxEntities, 0, NULL, 0,
										 ImGuiInputTextFlags_::ImGuiInputTextFlags_EnterReturnsTrue);
	ImGui::End();
}

/*!
 * @brief Display the in-system console
 * @param g
 * ImGuiCurrentContext
 * @param _applicationDebugLogCloseButton
 * boolean of the current window if open or closed
*/
void EditorMainScene::ApplicationDebugLogWindow(ImGuiContext* g, bool _applicationDebugLogCloseButton)
{
	if (!(g->NextWindowData.Flags & ImGuiNextWindowDataFlags_HasSize))
		ImGui::SetNextWindowSize(ImVec2(0.0f, ImGui::GetFontSize() * 12.0f), ImGuiCond_FirstUseEver);
	if (!ImGui::Begin("Dear ImGui Debug Log", &_applicationDebugLogCloseButton) || ImGui::GetCurrentWindow()->BeginCount > 1)
	{
		ImGui::End();
		return;
	}

	ImGui::CheckboxFlags("All", &g->DebugLogFlags, ImGuiDebugLogFlags_EventMask_);
	ImGui::SameLine(); ImGui::CheckboxFlags("ActiveId", &g->DebugLogFlags, ImGuiDebugLogFlags_EventActiveId);
	ImGui::SameLine(); ImGui::CheckboxFlags("Focus", &g->DebugLogFlags, ImGuiDebugLogFlags_EventFocus);
	ImGui::SameLine(); ImGui::CheckboxFlags("Popup", &g->DebugLogFlags, ImGuiDebugLogFlags_EventPopup);
	ImGui::SameLine(); ImGui::CheckboxFlags("Nav", &g->DebugLogFlags, ImGuiDebugLogFlags_EventNav);
	ImGui::SameLine(); if (ImGui::CheckboxFlags("Clipper", &g->DebugLogFlags, ImGuiDebugLogFlags_EventClipper)) { g->DebugLogClipperAutoDisableFrames = 2; } if (ImGui::IsItemHovered()) ImGui::SetTooltip("Clipper log auto-disabled after 2 frames");
	//ImGui::SameLine(); ImGui::CheckboxFlags("Selection", &g->DebugLogFlags, ImGuiDebugLogFlags_EventSelection);
	ImGui::SameLine(); ImGui::CheckboxFlags("IO", &g->DebugLogFlags, ImGuiDebugLogFlags_EventIO);
	ImGui::SameLine(); ImGui::CheckboxFlags("Docking", &g->DebugLogFlags, ImGuiDebugLogFlags_EventDocking);
	ImGui::SameLine(); ImGui::CheckboxFlags("Viewport", &g->DebugLogFlags, ImGuiDebugLogFlags_EventViewport);
	ImGui::SameLine(); ImGui::CheckboxFlags("Engine", &g->DebugLogFlags, ImGuiDebugLogFlags_EventEngine);		//To Remove

	if (ImGui::SmallButton("Clear"))
	{
		g->DebugLogBuf.clear();
		g->DebugLogIndex.clear();
	}
	ImGui::SameLine();
	if (ImGui::SmallButton("Copy"))
		ImGui::SetClipboardText(g->DebugLogBuf.c_str());
	ImGui::BeginChild("##log", ImVec2(0.0f, 0.0f), true, ImGuiWindowFlags_AlwaysVerticalScrollbar | ImGuiWindowFlags_AlwaysHorizontalScrollbar);

	ImGuiListClipper clipper;
	clipper.Begin(g->DebugLogIndex.size());
	while (clipper.Step())
		for (int line_no = clipper.DisplayStart; line_no < clipper.DisplayEnd; line_no++)
		{
			const char* line_begin = g->DebugLogIndex.get_line_begin(g->DebugLogBuf.c_str(), line_no);
			const char* line_end = g->DebugLogIndex.get_line_end(g->DebugLogBuf.c_str(), line_no);
			ImGui::TextUnformatted(line_begin, line_end);
			ImRect text_rect = g->LastItemData.Rect;
			if (ImGui::IsItemHovered())
				for (const char* p = line_begin; p <= line_end - 10; p++)
				{
					ImGuiID id = 0;
					if (p[0] != '0' || (p[1] != 'x' && p[1] != 'X') || sscanf(p + 2, "%X", &id) != 1)
						continue;
					ImVec2 p0 = ImGui::CalcTextSize(line_begin, p);
					ImVec2 p1 = ImGui::CalcTextSize(p, p + 10);
					g->LastItemData.Rect = ImRect(text_rect.Min + ImVec2(p0.x, 0.0f), text_rect.Min + ImVec2(p0.x + p1.x, p1.y));
					if (ImGui::IsMouseHoveringRect(g->LastItemData.Rect.Min, g->LastItemData.Rect.Max, true))
						ImGui::DebugLocateItemOnHover(id);
					p += 10;
				}
		}
	if (ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
		ImGui::SetScrollHereY(1.0f);
	ImGui::EndChild();

	ImGui::End();
}

/*!
 * @brief Display the time taken for each system of the engine
 * @param _profilerCloseButton
 * boolean of the current window if open or closed
*/
void EditorMainScene::ProfilerWindow(bool _profilerCloseButton)
{
	ImGui::Begin("System Time", &_profilerCloseButton);
	if (ImGui::BeginTable("##SystemTimeTable", 2, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_Resizable))
	{
		ImGui::TableSetupColumn("System");
		ImGui::TableSetupColumn("Time Taken");
		ImGui::TableHeadersRow();

		for (int j = 0; j < CoreEngine::GetInstance()->mSystemsDeltaTime.size(); ++j)
		{
			ImGui::TableNextRow();
			for (int i = 0; i < 2; i++)
			{
				ImGui::TableSetColumnIndex(i);
				if (i == 0)
				{
					std::string name = CoreEngine::GetInstance()->mSystemsDeltaTime[j].first;
					name.erase(0, 6); // remove word "class "
					ImGui::Text(name.c_str());
				}
				else
				{
					double utilisationPercentage = (CoreEngine::GetInstance()->mSystemsDeltaTime[j].second / CoreEngine::GetInstance()->mTotalSystemsTime) * 100;
					std::string time = std::format("{:.2f}", CoreEngine::GetInstance()->mSystemsDeltaTime[j].second) + " ms (" + std::format("{:.2f}", utilisationPercentage) + "%%)";
					ImGui::Text(time.c_str());
				}
			}
		}
		ImGui::EndTable();
	}
	ImGui::End();
}


void EditorMainScene::DisplayHierarchyNode(Entity* _currNode)
{
	if (_currNode == nullptr)
		return;

	ImGui::TableNextRow();
	ImGui::TableNextColumn();
	const bool hasChild = (_currNode->GetChildEntList().size() > 0);

	//For popup menu
	std::string popupName{"EntityPopup" + std::to_string(_currNode->GetID())};

	//Parent Object
	if (hasChild)
	{
		//Set flags
		ImGuiTreeNodeFlags treeParentFlag = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_SpanAllColumns;
		if (mSelectedEnt == _currNode)
			treeParentFlag |= ImGuiTreeNodeFlags_Selected;

		bool open = ImGui::TreeNodeEx((_currNode->GetName() + "##" + std::to_string(_currNode->GetID())).c_str(), treeParentFlag);//need add id if not clicking same name will open both
		if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen())
			mSelectedEnt = _currNode;
		if (ImGui::IsItemClicked(ImGuiMouseButton_Right))
		{
			mSelectedEnt = _currNode;
			ImGui::OpenPopup(popupName.c_str());
		}
		EntityCreationPopup(popupName, _currNode);

		if (ImGui::BeginDragDropSource())//TO save entity as prefab
		{
			mSelectedEnt = _currNode;
			std::string name = _currNode->GetName();
			if (name == "")//need to ensure have name
				name += "(Prefab)";
			ImGui::SetDragDropPayload("EntityDrag", name.c_str(), name.size());
			AssetManager::ImageAsset* image;
			AssetManager::GetInstance()->GetAsset("PREFAB", image);
			ImGui::Image((ImTextureID*)(intptr_t)image->mTexID, ImVec2(60, 60), ImVec2(0.0f, 1.0f), ImVec2(1.0f, 0.0f));
			ImGui::EndDragDropSource();
		}

		ImGui::TableNextColumn();
		ImGui::Text("%d", _currNode->GetID());
		if (open)
		{
			for (size_t childCount = 0; childCount < _currNode->GetChildEntList().size(); childCount++)
				DisplayHierarchyNode(EntityManager::GetInstance()->GetEntity(_currNode->GetChildEntList()[childCount]));
			ImGui::TreePop();
		}
	}
	//Child Object
	else
	{
		//Set flags
		ImGuiTreeNodeFlags treeChildFlag = ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_Bullet | ImGuiTreeNodeFlags_NoTreePushOnOpen | ImGuiTreeNodeFlags_SpanFullWidth | ImGuiTreeNodeFlags_SpanAllColumns;
		if (mSelectedEnt == _currNode)
			treeChildFlag |= ImGuiTreeNodeFlags_Selected;

		ImGui::TreeNodeEx((_currNode->GetName() + "##" + std::to_string(_currNode->GetID())).c_str(), treeChildFlag);
		if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen())
			mSelectedEnt = _currNode;
		if (ImGui::IsItemClicked(ImGuiMouseButton_Right))
		{
			mSelectedEnt = _currNode;
			ImGui::OpenPopup(popupName.c_str());
		}
		EntityCreationPopup(popupName, _currNode);

		ImGui::TableNextColumn();
		ImGui::Text("%d", _currNode->GetID());
	}
}

void EditorMainScene::EntityCreationPopup(std::string _popupName, Entity* _currNode)
{
	if (_currNode != mSelectedEnt)
		return;

	//Popup for entity creation
	if (ImGui::BeginPopup(_popupName.c_str()))
	{
		if (ImGui::Selectable("New Entity"))
		{
			Entity* newEnt = EntityManager::GetInstance()->CreateEntity();
			//Add transform to new entity
			newEnt->AddComponent<Transform>();
		}
		if (ImGui::Selectable("New Child Entity"))
		{
			Entity* newEnt = EntityManager::GetInstance()->CreateChildEntity(mSelectedEnt->GetID());
			//Add transform to new entity
			Transform* newXform = newEnt->AddComponent<Transform>();
			newXform->SetPosition(mSelectedEnt->GetComponent<Transform>()->GetPosition());
		}
		if (ImGui::Selectable("Delete"))
		{
			_currNode->SetToDestroy(true);
			mSelectedEnt = nullptr;
		}
		if (ImGui::Selectable("Duplicate"))
			EntityManager::GetInstance()->DuplicateEntity(_currNode->GetID());
		if (ImGui::Selectable("Save as Prefab"))
			LevelLoader::SavePrefab(mSelectedEnt->GetName(), mSelectedEnt, mCurrentOpenPath);
		ImGui::EndPopup();
	}
}
#endif // _ENGINE
