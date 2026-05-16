/*!
All content © 2023 DigiPen Institute of Technology Singapore, all rights reserved.
@file       LibraryManager.cpp
@author     TEO Wen Xi (100%)
@co-author

----------------------------------------------------------------------------

@course     CSD2401F23
@section    CSD2401F23-A
@project    CSD2401F23 Software Engineering Project 3

@brief      Manages all external libraries basic functions (Init, Update, Unload)
*//*______________________________________________________________________*/

#include "WindowsManager.h"
#include "LibraryManager.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#if _ENGINE
#pragma warning(push, 0)
#include <imgui.h>
#include <imgui_internal.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#pragma warning(pop)
#include "DebugLogger.h"
#endif // _ENGINE

#include <iostream>

//Manager Functions
void LibraryManager::Init()
{
	GLADInit();
#if _ENGINE
	ImGUIInit();
#endif // _ENGINE
}

void LibraryManager::Load()
{

}

void LibraryManager::Update()
{
}

void LibraryManager::Unload()
{
#if _ENGINE
	ImGUIUnload();
#endif // _ENGINE

	//Destroy instance
	Destroy();
}

//GLAD
void LibraryManager::GLADInit()
{
	//Initialize GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
#if _ENGINE
		DebugLogger::GetInstance()->Log("Failed to initialize GLAD\n");
#endif // _ENGINE
		exit(0);
	}
}
#if _ENGINE
//IMGUI
void LibraryManager::ImGUIInit()
{
	IMGUI_CHECKVERSION();
	ImGuiContext* g = ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;			// Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;			// Enable Gamepad Controls

	//Enable Docking
	ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_DockingEnable;

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsLight();

	// Setup Platform/Renderer backends
	ImGui_ImplGlfw_InitForOpenGL(WindowsManager::GetInstance()->mPtrWindow, true);
	const char* glsl_version = "#version 450";
	ImGui_ImplOpenGL3_Init(glsl_version);


	g->DebugLogFlags |= ImGuiDebugLogFlags_EventEngine; //To Remove


	// Load Fonts
	// - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them.
	// - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among multiple.
	// - If the file cannot be loaded, the function will return a nullptr. Please handle those errors in your application (e.g. use an assertion, or display an error and quit).
	// - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below will call.
	// - Use '#define IMGUI_ENABLE_FREETYPE' in your imconfig file to use Freetype for higher quality font rendering.
	// - Read 'docs/FONTS.md' for more instructions and details.
	// - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
	// - Our Emscripten build process allows embedding fonts to be accessible at runtime from the "fonts/" folder. See Makefile.emscripten for details.
	//io.Fonts->AddFontDefault();
	//io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\segoeui.ttf", 18.0f);
	//io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
	//io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
	//io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
	//ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, nullptr, io.Fonts->GetGlyphRangesJapanese());
	//IM_ASSERT(font != nullptr);
}

void LibraryManager::ImGUIUpdate()
{
	// Start the Dear ImGui frame
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
	ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());
}

void LibraryManager::ImGUIEndFrame()
{
	ImGui::EndFrame();
}

void LibraryManager::ImGUIRender()
{
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void LibraryManager::ImGUIUnload()
{
	ImGuiContext* g = ImGui::GetCurrentContext();
	g->IO.IniFilename = "Assets/imgui.ini";
	if (g->SettingsLoaded && g->IO.IniFilename != NULL)
		ImGui::SaveIniSettingsToDisk(g->IO.IniFilename);
	//Cleanup
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

bool LibraryManager::ImGUICreateAlignedHorizontalButton(std::string _label, float _alignment, float _buttonSizeX = -1.0f, float _buttonSizeY = -1.0f)
{
	ImGuiStyle& style = ImGui::GetStyle();

	if (_buttonSizeX != -1) //Button width was given
	{
		float avail = ImGui::GetContentRegionAvail().x;
		float off = (avail - _buttonSizeX) * _alignment;
		if (off > 0.0f)
			ImGui::SetCursorPosX(ImGui::GetCursorPosX() + off);
	}
	else //Button width not given
	{
		float size = ImGui::CalcTextSize(_label.c_str()).x + style.FramePadding.x * 2.0f;
		if (_buttonSizeX != -1 && _buttonSizeX > size) //Check if the given button size is larger than the text size
			size = _buttonSizeX;

		float avail = ImGui::GetContentRegionAvail().x;

		float off = (avail - size) * _alignment;
		if (off > 0.0f)
			ImGui::SetCursorPosX(ImGui::GetCursorPosX() + off);
	}

	if (_buttonSizeX != -1 && _buttonSizeY != -1) //an input was given
		return ImGui::Button(_label.c_str(), ImVec2{ static_cast<float>(_buttonSizeX), static_cast<float>(_buttonSizeY) });
	else //no input for the size of the button was given
		return ImGui::Button(_label.c_str());

}
#endif // _ENGINE