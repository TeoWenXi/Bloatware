/*!
All content © 2023 DigiPen Institute of Technology Singapore, all rights reserved.
@file       LibraryManager.h
@author     TEO Wen Xi (100%)
@co-author

----------------------------------------------------------------------------

@course     CSD2401F23
@section    CSD2401F23-A
@project    CSD2401F23 Software Engineering Project 3

@brief      Manages all external libraries basic functions (Init, Update, Unload)
*//*______________________________________________________________________*/

#ifndef LIBRARY_MANAGER_H
#define LIBRARY_MANAGER_H

#include "Singleton.h"
#include "Manager.h"

class LibraryManager : public Singleton<LibraryManager>, public Manager
{
public:
	//Manager

	/*!
	 * @brief 
	 * Init function
	*/
	void Init();

	/*!
	 * @brief 
	 * Load function
	*/
	void Load();

	/*!
	 * @brief 
	 * Update function
	*/
	void Update();

	/*!
	 * @brief 
	 * Unload function
	*/
	void Unload();

#if _ENGINE
	//ImGUI
	
	/*!
	 * @brief 
	 * Imgui system update functions
	*/
	void ImGUIUpdate();

	/*!
	 * @brief 
	 * Imgui system rendering functions
	*/
	void ImGUIRender();

	/*!
	 * @brief 
	 * Imgui horizontal button helper function
	 * @param _label 
	 * Imgui button label
	 * @param _alignment 
	 * Alignment value
	 * @param _buttonSizex 
	 * Button horizontal scale
	 * @param _buttonSizeY 
	 * Button vertical scale
	 * @return 
	 * True if button is pressed
	 * False if otherwise
	*/
	bool ImGUICreateAlignedHorizontalButton(std::string _label, float _alignment, float _buttonSizex, float _buttonSizeY);

	//It is automatically called by render(). If you don't need to render data (skipping rendering) you may call endFrame() without render()... but you'll have wasted CPU already! If you don't need to render, better to not create any windows and not call newFrame() at all!
	//https://wiki.giderosmobile.com/index.php/ImGui.Core:endFrame
	void ImGUIEndFrame();
#endif // _ENGINE

private:
	friend Singleton<LibraryManager>;

	//GLAD

	/*!
	 * @brief 
	 * Init function for the GLAD external library
	*/
	void GLADInit();

#if _ENGINE
	//ImGUI

	/*!
	 * @brief 
	 * Init function for Imgui stuff
	*/
	void ImGUIInit();

	/*!
	 * @brief 
	 * Unloads the Imgui stuff
	*/
	void ImGUIUnload();
#endif // BLOATWARE
};

#endif