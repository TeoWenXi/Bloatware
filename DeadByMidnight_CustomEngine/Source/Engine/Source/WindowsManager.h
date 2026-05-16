/*!
All content © 2023 DigiPen Institute of Technology Singapore, all rights reserved.
@file       WindowsManager.h
@author     TEO Wen Xi (100%)
@co-author

----------------------------------------------------------------------------

@course     CSD2401F23
@section    CSD2401F23-A
@project    CSD2401F23 Software Engineering Project 3

@brief      Manages all windows in the program
*//*______________________________________________________________________*/

#ifndef WINDOWS_MANAGER_H
#define WINDOWS_MANAGER_H

#include <vector>
#include <string>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#undef APIENTRY //Remove the APIRENTRY redefinition warning

#include "Singleton.h"
#include "Manager.h"

class WindowsManager : public Singleton<WindowsManager>, public Manager
{
private:
	enum WINDOW_MODE
	{
		WM_FULLSCREEN = 0,
		//WM_BORDERLESS,
		WM_WINDOWED,
		WM_COUNT
	};

public:
	//Manager Functions

	/*!
	 * @brief
	 *	Virtual init function for manager class.
	*/
	void Init();

	/*!
	 * @brief
	 *	Virtual load function for manager class.
	*/
	void Load();

	/*!
	 * @brief
	 *	Virtual update function for manager class.
	*/
	void Update();

	/*!
	 * @brief
	 *	Virtual unload function for the manager class.
	*/
	void Unload();

	//Window creation

	/*!
	 * @brief 
	 * Creates the Window Application
	 * @param _width 
	 * Given width of the window
	 * @param _height 
	 * Given height of the window
	 * @param _winTitle 
	 * Given title of the window
	 * @param _fpsUpdateInterval
	 * Frequency of FPS Update
	 * @return 
	 * Successfully created or not
	*/
	bool CreateWindow(GLint _width, GLint _height, std::string _winTitle, GLdouble _fpsUpdateInterval = 1.0f);

	//Updating Delta time

	/*!
	 * @brief 
	 * Updates the delta time value
	*/
	void UpdateTime();

	//Callback functions

	/*!
	 * @brief 
	 * Error callback for GLFW
	 * @param _error 
	 * Callback error code
	 * @param _description 
	 * Callback description
	*/
	static void ErrorCallback(int _error, const char* _description);

	/*!
	 * @brief 
	 * Framebuffer size callback for windows resizing
	 * @param _winPtr 
	 * Window that was resized
	 * @param _width 
	 * Window width
	 * @param _height
	 * Window height
	*/
	static void FrameBufferSizeCallback(GLFWwindow* _winPtr, GLint _width, GLint _height);

	/*!
	 * @brief 
	 * Focus callback for GLFW
	 * @param _window 
	 * Window that was focused
	 * @param _focused 
	 * Callback value
	*/
	static void FocusCallback(GLFWwindow* _window, int _focused);

	//Switching of window modes

	/*!
	 * @brief 
	 * Switch windows mode
	 * Fullscreen, Borderless & Windowed
	 * @param _nextMode 
	 * Next window mode to be swapped into
	*/
	void SwitchWindowMode(WINDOW_MODE _nextMode);

	//Updating the Window Resolution

	/*!
	 * @brief 
	 * Change Window Size
	 * @param _width 
	 * Given width of the window
	 * @param _height 
	 * Given height of the window
	*/
	void ChangeWindowSize(GLint _width, GLint _height);

	//Window variables
	GLFWwindow* mPtrWindow;
	static GLint mWidth;
	static GLint mHeight;
	std::string mTitle;

	//Time related
	GLdouble mDeltaTime;
	GLdouble mDTThreshold;

	//FPS of Update
	GLdouble mFPSOfUpdate;

	float mSceneOpacity = 1.0f;
	float mSceneTrueOpacity = 1.0f;
	float mSceneRedIntensity = 0.0f;
	float mDeathVignette = 0.0f;

private:
	//Constructor & Destructors

	/*!
	 * @brief 
	 * Constructor for WindowsManager
	*/
	WindowsManager();

	//For singleton
	friend Singleton<WindowsManager>;

	//Time calculation
	GLdouble mElapsedTime;
	GLdouble mPrevTime;

	//FPS calculation
	GLuint mGameLoopCount;
	GLdouble mStartTime;
	GLdouble mFPSUpdateInterval;

	WINDOW_MODE mWindowMode{ WM_FULLSCREEN }; //Defaulted to fullscreen mode
	bool mWindowSwapped{ false };
};

#endif
