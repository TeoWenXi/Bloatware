/*!
All content © 2023 DigiPen Institute of Technology Singapore, all rights reserved.
@file       InputManager.h
@author     HEAR Li Heng (100%)
@co-author

----------------------------------------------------------------------------

@course     CSD2401F23
@section    CSD2401F23-A
@project    CSD2401F23 Software Engineering Project 3

@brief      InputManager is an instance that can be ascessed from anywhere ny calling Input->
			This allows the input to be read from anywhere

			//Example usage
			if (Input->GetActionTriggered("jump"))
				std::cout << "TRIGGERED!!!\n";
			if (Input->GetActionReleased("jump"))
				std::cout << "RELEASED!!!\n";
			if (Input->GetAction("jump"))
				std::cout << "JUMP!!\n";
			std::cout << Input->GetMousePos().x << "|" << Input->GetMousePos().y << std::endl;

*//*______________________________________________________________________*/

#ifndef INPUT_MANAGER_H
#define INPUT_MANAGER_H

#include "Singleton.h"
#include "Manager.h"
#include "Vector3.h"

#include <GLFW/glfw3.h>
#include <deque>
#include <vector>
#include <string>
#include <map>

class InputManager : public Singleton <InputManager>, public Manager
{
	struct KeyType
	{
		enum DeviceType
		{
			Keyboard,
			Gamepad,
			Stick
		}inputDevice;//0 = keyboard | 1 = gamepad
		int key;
	};
	using TRIGGER_EVENTS = std::deque<std::pair<int, int>>;
public:
	/*!
	 * @brief 
	 * Inits the InputManager.
	*/
	void Init();

	/*!
	 * @brief 
	 * Loads the InputManager.
	*/
	void Load();

	/*!
	 * @brief 
	 * Updates the InputManager.
	*/
	void Update();

	/*!
	 * @brief 
	 * Unloads the InputManager.
	*/
	void Unload();

	/*!
	 * @brief 
	 * Loads the controls by reading the "controls.txt" file
	*/
	void LoadControls();

	/*!
	 * @brief 
	 * Saves the current controls into "controls.txt" file
	*/
	void SaveControls();

	/*!
	 * @brief 
	 * Changes the bound key to another
	 * @param _action 
	 * Action to change
	 * @param _key 
	 * The key to change to
	 * @param _type 
	 * The type of device(keyboard,controller)
	 * @return 
	 * True if successful | False otherwise
	*/
	bool ChangeControls(const char* _action, int _key, KeyType::DeviceType _type);

	//----------keys----------

	/*!
	 * @brief 
	 * Checks a KeyCode and returns true if the key is currently held down.
	 * @param _keycode 
	 * Key to check.
	 * @return 
	 * True if key is held down false otherwise.
	*/
	bool GetKey(int _keycode);

	/*!
	 * @brief 
	 * Checks a KeyCode and returns true if key was pressed in this frame.
	 * @param _keycode 
	 * Key to check.
	 * @return 
	 * True if key was triggered this frame false otherwise.
	*/
	bool GetKeyTriggered(int _keycode);

	/*!
	 * @brief 
	 * Checks a KeyCode and returns true if key was released in this frame
	 * @param _keycode 
	 * Key to check.
	 * @return
	 * True if key was released this frame false otherwise.
	*/
	bool GetKeyReleased(int _keycode);

	//----------_actions - used to associate strings with keys ie. binding spacebar and 'W' to jump ----------

	/*!
	 * @brief 
	 * Checks a _action name and returns true if the bound key is currently held down.
	 * @param _action 
	 * Action to check.
	 * @return 
	 * True if _action is held down false otherwise.
	*/
	bool GetAction(const char* _action);

	/*!
	 * @brief
	 * Checks a _action name and returns true if the bound key was pressed in this frame.
	 * @param _action
	 * Action to check.
	 * @return
	 * True if _action was triggered this frame false otherwise.
	*/
	bool GetActionTriggered(const char* _action);

	/*!
	 * @brief 
	 * Checks a _action name and returns true if the bound key was released in this frame.
	 * @param _action 
	 * Action to check.
	 * @return
	 * True if _action was released this frame false otherwise.
	*/
	bool GetActionReleased(const char* _action);

	//----------mouse----------

	/*!
	 * @brief 
	 * Gets mouse pos in window.
	 * 0,0 is top left | Note* can go into negative if out of window (To get for entire Engine window).
	 * @return 
	 * Position in window.
	*/
	Vector3 GetMousePos();

	/*!
	 * @brief 
	 * Gets mouse pos relative to Screen
	 * 0,0 is middle and based of the 'Scene' window's position
	 * @return 
	 * Position with respect to Screen.
	*/
	Vector3 GetMousePosScreen();

	/*!
	 * @brief 
	 * Gets mouse pos converted into world pos.
	 * @return 
	 * Position in the world.
	*/
	Vector3 GetMousePosWorld();

	/*!
	 * @brief 
	 * Checks the id of the mouse button and returns true if is currently held down.
	 * @param _buttonID 
	 * The button id to check.
	 * @return 
	 * True if button is held down false otherwise.
	*/
	bool GetMouseButton(int _buttonID);

	/*!
	 * @brief 
	 * Checks the id of the mouse button and returns true if it was pressed in this frame.
	 * @param _buttonID 
	 * The button id to check.
	 * @return
	 * True if button is pressed down this frame false otherwise.
	*/
	bool GetMouseButtonDown(int _buttonID);

	/*!
	 * @brief
	 * Checks the id of the mouse button and returns true if it was released in this frame.
	 * @param _buttonID
	 * The button id to check.
	 * @return
	 * True if button is released down this frame false otherwise.
	*/
	bool GetMouseButtonUp(int _buttonID);

	/*!
	 * @brief 
	 * Checks if mouse is within 'Scene' window.
	 * @return 
	 * True if mouse is within false otherwise
	*/
	bool GetMouseInScene();

	//----------Gamepad----------
	void AddGamepad(int id);
	void RemoveGamepad(int id);
	bool GetPad(int _keycode);
	bool GetPadTriggered(int _keycode);
	bool GetPadReleased(int _keycode);
	float GetAxis(int _keycode);
	float GetAxisPrev(int _keycode);


	//----------screen----------
	
	/*!
	 * @brief 
	 * Updates the 'Scene' window's position for GetMousePosScreen.
	 * @param x 
	 * X position of 'Scene' window.
	 * @param y 
	 * Y position of 'Scene' window.
	*/
	void UpdateScreenPos(float _x, float _y);

	/*!
	 * @brief 
	 * Updates the 'Scene' window's max display region for GetMousePosScreen.
	 * @param x 
	 * X value of 'Scene' window region max.
	 * @param y 
	 * Y value of 'Scene' window region max.
	*/
	void UpdateRegionMax(float _x, float _y);

	/*!
	 * @brief
	 * Updates the 'Scene' window's min display region for GetMousePosScreen.
	 * @param x
	 * X value of 'Scene' window region min.
	 * @param y
	 * Y value of 'Scene' window region min.
	*/
	void UpdateRegionMin(float _x, float _y);

	/*!
	 * @brief 
	 * Gets the latest keypress
	 * @return 
	 * returns 0 if none
	*/
	int GetLatestKeyPress();



	/*!
	 * @brief 
	 * Sets the action that we want to rebind
	 * @param _actionToChange 
	 * The name of the action to rebind
	*/
	void ChangeActionKeybind(std::string _actionToChange);

	//Gets current Keycode attached to Action

	/*!
	 * @brief 
	 * Gets the current keycode bound to the action
	 * @param _action 
	 * The action to check
	 * @return 
	 * The bound keycode
	*/
	int GetCurrentActionKeybind(std::string _action);

	/*!
	 * @brief 
	 * Checks to see if currently have any key to rebind
	 * Gets called by keycallback
	 * @param _keycode 
	 * Keycode to rebind if required
	*/
	void CheckRebind(int _keycode);

	/*!
	 * @brief 
	 * Converts keycode to string
	 * @param _keycode 
	 * Keycode to convert
	 * @return 
	 * Keycode's name
	*/
	std::string KeycodeToString(int _keycode);

	/*!
	 * @brief 
	 * Gets the trigger events so that glfw keycallback can use
	 * @return 
	 * Reference to our triggerevents list
	*/
	TRIGGER_EVENTS& GetTriggerEvents();//Gets the list of trigger events

	//if lockedkeyboard, then no keyboard inputs will be used
	bool lockKeyboard = false;
private:
	/*!
	 * @brief
	 * Gets the world scaling for GetMousePosScreen.
	 * @return
	 * The world scaling.
	*/
	Vector3 GetScreenToWorldScaling();

	/*!
	 * @brief 
	 * Default constructor for InputManager.
	*/
	InputManager();

	/*!
	 * @brief 
	 * Default destructor for InputManager.
	*/
	~InputManager();

	//Singleton
	friend Singleton<InputManager>;

	GLFWwindow* mCurrentWindow{ nullptr };//ptr to current window for GLFW
	
	std::multimap<std::string, KeyType> mActionsMap;//similar to unity's _action map for both keyboard/controller support
	TRIGGER_EVENTS mTriggerEvents;//used to store recent triggers/release and will be cleaned at the start of a newupdate loop
	TRIGGER_EVENTS mTriggerEventsRead;//used to store recent triggers/release and will be cleaned at the start of a newupdate loop
	Vector3 mScreenWindowMin{}, mScreenWindowMax{};//stores 'Scene' window min and max boundary to be used for GetMousePosScreen calculations
	Vector3 mSceneWindowPos{};//pos of scene window to offset mouse pos

	std::string mKeyBindToChange;//keys track if there is a keybinding we want to change
	std::vector<int> lockedKeys;//keeps track of all the keys we dont want to be rebinded to another key

	//Contains all the data we need to store info for the gamepad
	struct Gamepad
	{
		int id;
		GLFWgamepadstate prev;
		GLFWgamepadstate curr;
		float defaultDeadZonePositive[6];
		float defaultDeadZoneNegative[6];

	};
	std::vector<Gamepad> gamepads;
};

//Basically same as GLFW but copying unity's enum naming
namespace KeyCode
{
	enum
	{
		/*The unknown key*/
		KEY_UNKNOWN = -1,
		/* mouse buttons
		* GLFW_MOUSE_BUTTON_1   0
		  GLFW_MOUSE_BUTTON_2   1
		  GLFW_MOUSE_BUTTON_3   2
		  GLFW_MOUSE_BUTTON_4   3
		  GLFW_MOUSE_BUTTON_5   4
		  GLFW_MOUSE_BUTTON_6   5
		  GLFW_MOUSE_BUTTON_7   6
		  GLFW_MOUSE_BUTTON_8   7
		*/
		/* Printable keys */
		SPACE = 32,
		APOSTROPHE = 39, /* ' */
		COMMA = 44, /* , */
		MINUS = 45, /* - */
		PERIOD = 46, /* . */
		SLASH = 47, /* / */
		ALPHA_0 = 48,
		ALPHA_1 = 49,
		ALPHA_2 = 50,
		ALPHA_3 = 51,
		ALPHA_4 = 52,
		ALPHA_5 = 53,
		ALPHA_6 = 54,
		ALPHA_7 = 55,
		ALPHA_8 = 56,
		ALPHA_9 = 57,
		SEMICOLON = 59, /* ; */
		EQUAL = 61, /* = */
		A = 65,
		B = 66,
		C = 67,
		D = 68,
		E = 69,
		F = 70,
		G = 71,
		H = 72,
		I = 73,
		J = 74,
		K = 75,
		L = 76,
		M = 77,
		N = 78,
		O = 79,
		P = 80,
		Q = 81,
		R = 82,
		S = 83,
		T = 84,
		U = 85,
		V = 86,
		W = 87,
		X = 88,
		Y = 89,
		Z = 90,
		LEFT_BRACKET = 91,  /* [ */
		BACKSLASH = 92,  /* \ */
		RIGHT_BRACKET = 93,  /* ] */
		GRAVE_ACCENT = 96,  /* ` */
		/* Function keys */
		ESCAPE = 256,
		ENTER = 257,
		TAB = 258,
		BACKSPACE = 259,
		INSERT = 260,
		DELETE_ = 261,
		RIGHT = 262,
		LEFT = 263,
		DOWN = 264,
		UP = 265,
		PAGE_UP = 266,
		PAGE_DOWN = 267,
		HOME = 268,
		END = 269,
		CAPS_LOCK = 280,
		SCROLL_LOCK = 281,
		NUM_LOCK = 282,
		PRINT_SCREEN = 283,
		PAUSE = 284,
		F1 = 290,
		F2 = 291,
		F3 = 292,
		F4 = 293,
		F5 = 294,
		F6 = 295,
		F7 = 296,
		F8 = 297,
		F9 = 298,
		F10 = 299,
		F11 = 300,
		F12 = 301,
		F13 = 302,
		F14 = 303,
		F15 = 304,
		F16 = 305,
		F17 = 306,
		F18 = 307,
		F19 = 308,
		F20 = 309,
		F21 = 310,
		F22 = 311,
		F23 = 312,
		F24 = 313,
		F25 = 314,
		KEYPAD_0 = 320,
		KEYPAD_1 = 321,
		KEYPAD_2 = 322,
		KEYPAD_3 = 323,
		KEYPAD_4 = 324,
		KEYPAD_5 = 325,
		KEYPAD_6 = 326,
		KEYPAD_7 = 327,
		KEYPAD_8 = 328,
		KEYPAD_9 = 329,
		KEYPAD_DECIMAL = 330,
		KEYPAD_DIVIDE = 331,
		KEYPAD_MULTIPLY = 332,
		KEYPAD_SUBTRACT = 333,
		KEYPAD_ADD = 334,
		KEYPAD_ENTER = 335,
		KEYPAD_EQUAL = 336,
		LEFT_SHIFT = 340,
		LEFT_CONTROL = 341,
		LEFT_ALT = 342,
		LEFT_SUPER = 343,
		RIGHT_SHIFT = 344,
		RIGHT_CONTROL = 345,
		RIGHT_ALT = 346,
		RIGHT_SUPER = 347,
		MENU = 348
	};
};
//For controllers
namespace GamepadCode
{
	enum
	{
		GAMEPAD_BUTTON_A = 0,
		GAMEPAD_BUTTON_B = 1,
		GAMEPAD_BUTTON_X = 2,
		GAMEPAD_BUTTON_Y = 3,
		GAMEPAD_BUTTON_LEFT_BUMPER = 4,
		GAMEPAD_BUTTON_RIGHT_BUMPER = 5,
		GAMEPAD_BUTTON_BACK = 6,
		GAMEPAD_BUTTON_START = 7,
		GAMEPAD_BUTTON_GUIDE = 8,
		GAMEPAD_BUTTON_LEFT_THUMB = 9,
		GAMEPAD_BUTTON_RIGHT_THUMB = 10,
		GAMEPAD_BUTTON_DPAD_UP = 11,
		GAMEPAD_BUTTON_DPAD_RIGHT = 12,
		GAMEPAD_BUTTON_DPAD_DOWN = 13,
		GAMEPAD_BUTTON_DPAD_LEFT = 14,

		GAMEPAD_BUTTON_LEFT_THUMB_LEFT = 100,
		GAMEPAD_BUTTON_LEFT_THUMB_RIGHT = 101,
		GAMEPAD_BUTTON_LEFT_THUMB_UP = 200,
		GAMEPAD_BUTTON_LEFT_THUMB_DOWN = 201,
		GAMEPAD_BUTTON_RIGHT_THUMB_LEFT = 300,
		GAMEPAD_BUTTON_RIGHT_THUMB_RIGHT = 301,
		GAMEPAD_BUTTON_RIGHT_THUMB_UP = 400,
		GAMEPAD_BUTTON_RIGHT_THUMB_DOWN = 401,
		GAMEPAD_LEFT_TRIGGER = 501,
		GAMEPAD_RIGHT_TRIGGER = 601,
		GAMEPAD_BUTTON_LAST
	};
};

#endif
