/*!
All content © 2023 DigiPen Institute of Technology Singapore, all rights reserved.
@file       InputManager.cpp
@author     HEAR Li Heng (100%)
@co-author

----------------------------------------------------------------------------

@course     CSD2401F23
@section    CSD2401F23-A
@project    CSD2401F23 Software Engineering Project 3

@brief      InputManager is an instance that can be ascessed from anywhere ny calling Input->
			This allows the input to be read from anywhere

			mTriggerEventsRead contains the keyboard events since the previous update while
			mTriggerEvents will contain the keyboard events that have been pressed in this loop
*//*______________________________________________________________________*/

#define UNREFERENCED_PARAMETER(P) (P)
#define GAMESTICK_DEADZONE 0.01f

#include "InputManager.h"
#include "WindowsManager.h"
#include "GraphicsSystem.h"
#include <string> 

#if _ENGINE
#include <imgui_impl_glfw.h>
#endif // _ENGINE

#include "JSONManager.h"
#include "LevelLoader.h"

InputManager::InputManager()
	: mCurrentWindow(nullptr)
{
}

InputManager::~InputManager()
{
}

InputManager::TRIGGER_EVENTS& InputManager::GetTriggerEvents()
{
	return mTriggerEvents;
}

void key_callback(GLFWwindow* _window, int _key, int _scancode, int _action, int _mods)
{
	UNREFERENCED_PARAMETER(_mods);
	UNREFERENCED_PARAMETER(_window);
	UNREFERENCED_PARAMETER(_scancode);
#if _ENGINE
	ImGui_ImplGlfw_KeyCallback(_window, _key, _scancode, _action, _mods);
#endif // _ENGINE
	if (_action == GLFW_REPEAT)
		return;
	if (InputManager::GetInstance()->lockKeyboard)
		return;
	InputManager::GetInstance()->GetTriggerEvents().emplace_back(std::make_pair(_key, _action));

	if (_action == GLFW_RELEASE)//Check rebind
		InputManager::GetInstance()->CheckRebind(_key);
}

void mouse_button_callback(GLFWwindow* _window, int _button, int _action, int _mods)
{
	UNREFERENCED_PARAMETER(_mods);
	UNREFERENCED_PARAMETER(_window);
#if _ENGINE
	ImGui_ImplGlfw_MouseButtonCallback(_window, _button, _action, _mods);
#endif // _ENGINE
	if (_action == GLFW_REPEAT)
		return;

	if (InputManager::GetInstance()->GetMouseInScene())
		InputManager::GetInstance()->GetTriggerEvents().emplace_back(std::make_pair(_button, _action));
}

void joystick_callback(int jid, int event)
{
	if (event == GLFW_CONNECTED)
	{
		// The joystick was connected
		InputManager::GetInstance()->AddGamepad(jid);
	}
	else if (event == GLFW_DISCONNECTED)
	{
		// The joystick was disconnected
		InputManager::GetInstance()->RemoveGamepad(jid);
	}
}

void InputManager::AddGamepad(int id)
{
	Gamepad newGamepad;
	newGamepad.id = id;
	glfwGetGamepadState(id, &newGamepad.curr);
	for (int i = 0; i < 6; ++i)
	{
		newGamepad.defaultDeadZonePositive[i] = newGamepad.curr.axes[i] + GAMESTICK_DEADZONE;
		newGamepad.defaultDeadZoneNegative[i] = newGamepad.curr.axes[i] - GAMESTICK_DEADZONE;

	}
	gamepads.push_back(newGamepad);

	//Spawn controller connected prefab only if its not alr up
	if (!EntityManager::GetInstance()->GetEntity("ControllerConnected Prefab"))
	{
		LevelLoader::LoadPrefab("ControllerConnected Prefab");
	}
}

void InputManager::RemoveGamepad(int id)
{
	UNREFERENCED_PARAMETER(id);
	gamepads.clear();

	//Spawn controller connected prefab only if its not alr up
	if (!EntityManager::GetInstance()->GetEntity("ControllerDisconnected Prefab"))
	{
		LevelLoader::LoadPrefab("ControllerDisconnected Prefab");
	}
}

void InputManager::Init()
{
	mCurrentWindow = WindowsManager::GetInstance()->mPtrWindow;
	glfwSetKeyCallback(mCurrentWindow, key_callback);
	glfwSetMouseButtonCallback(mCurrentWindow, mouse_button_callback);
	//Setup bindings of keys to controls
	//this can be serialized to read from file later
	LoadControls();
	UpdateRegionMax(static_cast<float>(WindowsManager::GetInstance()->mWidth), static_cast<float>(WindowsManager::GetInstance()->mHeight));
	UpdateRegionMin(0, 0);

	//Prevent these keys from being rebinded
	lockedKeys.push_back(KeyCode::O);
	lockedKeys.push_back(KeyCode::ENTER);
	lockedKeys.push_back(KeyCode::ALPHA_1);
	lockedKeys.push_back(KeyCode::G);
	lockedKeys.push_back(KeyCode::H);
}

void InputManager::Load()
{
	glfwSetJoystickCallback(joystick_callback);

	if (glfwJoystickPresent(GLFW_JOYSTICK_1))//joystick connected
		AddGamepad(GLFW_JOYSTICK_1);
}

void InputManager::LoadControls()
{
#if _ENGINE
	JSONManager::GetInstance()->LoadFile("controls_Engine");
#else
	JSONManager::GetInstance()->LoadFile("controls_Bloatware");
#endif // _ENGINE
	//check keyboard
	{
		std::vector<std::string> bindings = JSONManager::GetInstance()->GetArray<std::string>("controlsBinding");
		for (std::string buffer : bindings)
		{
			size_t split = buffer.find(':');
			std::string _action = buffer.substr(0, split);
			std::string _keyInt = buffer.substr(split + 1, buffer.length() - split);
			mActionsMap.insert(std::pair<std::string, KeyType>(_action, KeyType(KeyType::Keyboard, std::stoi(_keyInt))));
		}
	}
	//check for gamepad

	std::vector<std::string> bindings = JSONManager::GetInstance()->GetArray<std::string>("gamepadBinding");
	for (std::string buffer : bindings)
	{
		size_t split = buffer.find(':');
		std::string _action = buffer.substr(0, split);
		std::string _keyInt = buffer.substr(split + 1, buffer.length() - split);
		mActionsMap.insert(std::pair<std::string, KeyType>(_action, KeyType(KeyType::Gamepad, std::stoi(_keyInt))));
	}

}

void InputManager::SaveControls()
{
	JSONManager* jsonManager = JSONManager::GetInstance();
	jsonManager->CreateNewSave();
	//check keyboard
	{
		jsonManager->SaveObject("controlsBinding", true);
		for (auto action : mActionsMap)
		{
			if (action.second.inputDevice == KeyType::Keyboard)
			{
				std::string save = action.first + ":" + std::to_string(action.second.key);
				jsonManager->Pushback(save);
			}
		}
		jsonManager->CloseObject();
	}
	//check for gamepad
	{
		jsonManager->SaveObject("gamepadBinding", true);
		for (auto action : mActionsMap)
		{
			if (action.second.inputDevice == KeyType::Gamepad)
			{
				std::string save = action.first + ":" + std::to_string(action.second.key);
				jsonManager->Pushback(save);
			}
		}
		jsonManager->CloseObject();
	}
#if _ENGINE
	JSONManager::GetInstance()->SaveFile("controls_Engine", "txt", "Assets");
#else
	JSONManager::GetInstance()->SaveFile("controls_Bloatware", "txt", "Assets");
#endif // _ENGINE
}

bool InputManager::ChangeControls(const char* _action, int _key, KeyType::DeviceType _type)
{
	//This one not really being used anymore so can either change it or smth
	//Dunno why but its using char* instead of string??

	std::pair<std::multimap<std::string, KeyType>::iterator, std::multimap<std::string, KeyType>::iterator> result = mActionsMap.equal_range(_action);
	if (result.first == result.second)//_action not found
	{
#if _ENGINE
		//print out error message
		DebugLogger::GetInstance()->Log("Action <%s> not found in map\n", _action);
#endif // _ENGINE
		return false;
	}

	for (std::multimap<std::string, KeyType>::iterator itr = result.first; itr != result.second; ++itr)
	{
		if ((*itr).second.inputDevice == _type)
		{
			(*itr).second.key = _key;
			return true;
		}
	}
	return false;
}

void InputManager::Update()
{
	//stores the previous inputs into mTriggerEventsRead
	mTriggerEventsRead.clear();
	mTriggerEventsRead = mTriggerEvents;
	mTriggerEvents.clear();

	for (Gamepad& pad : gamepads)
	{
		pad.prev = pad.curr;
		glfwGetGamepadState(pad.id, &pad.curr);
		//DebugLogger::GetInstance()->Log("%f\n", gamepads[gamepads.size() - 1].curr.axes[0]);
	}
}

void InputManager::Unload()
{
	//Destroy instance
	Destroy();
}

//Checks a KeyCode and returns true if the key is currently held down.
bool InputManager::GetKey(int _keycode)
{
	return glfwGetKey(mCurrentWindow, _keycode);
}

//Checks a KeyCode and returns true if key was pressed in this frame
bool InputManager::GetKeyTriggered(int _keycode)
{
	std::pair<int, int> code{ _keycode, GLFW_PRESS };
	std::deque<std::pair<int, int>>::iterator itr = find(mTriggerEventsRead.begin(), mTriggerEventsRead.end(), code);
	return itr != mTriggerEventsRead.end();
}

//Checks a KeyCode and returns true if key was released in this frame
bool InputManager::GetKeyReleased(int _keycode)
{
	std::pair<int, int> code{ _keycode, GLFW_RELEASE };
	std::deque<std::pair<int, int>>::iterator itr = find(mTriggerEventsRead.begin(), mTriggerEventsRead.end(), code);
	return itr != mTriggerEventsRead.end();
}

bool InputManager::GetPad(int _keycode)
{
	if (gamepads.size() == 0) return false;
	if (_keycode > 99)//axes
	{
		if (_keycode % 100 == 0)
			return GetAxis((_keycode / 100) - 1) < gamepads[gamepads.size() - 1].defaultDeadZoneNegative[(_keycode / 100) - 1];
		else
			return GetAxis((_keycode / 100) - 1) > gamepads[gamepads.size() - 1].defaultDeadZonePositive[(_keycode / 100) - 1];
	}
	//normal buttons
	return gamepads[gamepads.size()-1].curr.buttons[_keycode];
}

bool InputManager::GetPadTriggered(int _keycode)
{
	if (gamepads.size() == 0) return false;
	if (_keycode > 99)//axes
	{
		if (_keycode % 100 == 0)
			return GetAxis((_keycode / 100) - 1) <  gamepads[gamepads.size() - 1].defaultDeadZoneNegative[(_keycode / 100) - 1] && GetAxisPrev((_keycode / 100) - 1) >  gamepads[gamepads.size() - 1].defaultDeadZoneNegative[(_keycode / 100) - 1];
		else
			return GetAxis((_keycode / 100) - 1) > gamepads[gamepads.size() - 1].defaultDeadZonePositive[(_keycode / 100) - 1] && GetAxisPrev((_keycode / 100) - 1) < gamepads[gamepads.size() - 1].defaultDeadZonePositive[(_keycode / 100) - 1];
	}
	//normal buttons
	return gamepads[gamepads.size() - 1].curr.buttons[_keycode] && !gamepads[gamepads.size() - 1].prev.buttons[_keycode];
}

bool InputManager::GetPadReleased(int _keycode)
{
	if (gamepads.size() == 0) return false;
	if (_keycode > 99)//axes
	{
		if (_keycode % 100 == 0)
			return GetAxis((_keycode / 100) - 1) >  gamepads[gamepads.size() - 1].defaultDeadZoneNegative[(_keycode / 100) - 1] && GetAxisPrev((_keycode / 100) - 1) <  gamepads[gamepads.size() - 1].defaultDeadZoneNegative[(_keycode / 100) - 1];
		else
			return GetAxis((_keycode / 100) - 1) < gamepads[gamepads.size() - 1].defaultDeadZonePositive[(_keycode / 100) - 1] && GetAxisPrev((_keycode / 100) - 1) > gamepads[gamepads.size() - 1].defaultDeadZonePositive[(_keycode / 100) - 1];
	}
	//normal buttons
	return !gamepads[gamepads.size() - 1].curr.buttons[_keycode] && gamepads[gamepads.size() - 1].prev.buttons[_keycode];
}

float InputManager::GetAxis(int _keycode)
{
	if (gamepads.size() == 0) return false;
	if (_keycode == 0)
		return gamepads[gamepads.size() - 1].curr.axes[_keycode];
	return gamepads[gamepads.size() - 1].curr.axes[_keycode];
}

float InputManager::GetAxisPrev(int _keycode)
{
	if (gamepads.size() == 0) return false;
	if (_keycode == 0)
		return gamepads[gamepads.size() - 1].prev.axes[_keycode];
	return gamepads[gamepads.size() - 1].prev.axes[_keycode];
}

//Checks a _action name and returns true if the bound key is currently held down
bool InputManager::GetAction(const char* _action)
{
	std::pair<std::multimap<std::string, KeyType>::iterator, std::multimap<std::string, KeyType>::iterator> result = mActionsMap.equal_range(_action);
	if (result.first == result.second)//_action not found
	{
#if _ENGINE
		//print out error message
		DebugLogger::GetInstance()->Log("Action <%s> not found in map\n", _action);
#endif // _ENGINE
		return false;
	}

	for (std::multimap<std::string, KeyType>::iterator itr = result.first; itr != result.second; ++itr)
	{
		if ((*itr).second.inputDevice == KeyType::Keyboard && GetKey((*itr).second.key))
			return true;
		else if ((*itr).second.inputDevice == KeyType::Gamepad && GetPad((*itr).second.key))
			return true;
	}
	return false;
}

//hecks a _action name and returns true if the bound key was pressed in this frame.
bool InputManager::GetActionTriggered(const char* _action)
{
	std::pair<std::multimap<std::string, KeyType>::iterator, std::multimap<std::string, KeyType>::iterator> result = mActionsMap.equal_range(_action);
	if (result.first == result.second)//_action not found
	{
#if _ENGINE
		//print out error message
		DebugLogger::GetInstance()->Log("Action <%s> not found in map\n", _action);
#endif // _ENGINE
		return false;
	}

	for (std::multimap<std::string, KeyType>::iterator itr = result.first; itr != result.second; ++itr)
	{
		if ((*itr).second.inputDevice == KeyType::Keyboard && GetKeyTriggered((*itr).second.key))
			return true;
		else if ((*itr).second.inputDevice == KeyType::Gamepad && GetPadTriggered((*itr).second.key))
			return true;
	}
	return false;
}

bool InputManager::GetActionReleased(const char* _action)
{
	std::pair<std::multimap<std::string, KeyType>::iterator, std::multimap<std::string, KeyType>::iterator> result = mActionsMap.equal_range(_action);
	if (result.first == result.second)//_action not found
	{
#if _ENGINE
		//print out error message
		DebugLogger::GetInstance()->Log("Action <%s> not found in map\n", _action);
#endif // _ENGINE
		return false;
	}

	for (std::multimap<std::string, KeyType>::iterator itr = result.first; itr != result.second; ++itr)
	{
		if ((*itr).second.inputDevice == KeyType::Keyboard && GetKeyReleased((*itr).second.key))
			return true;
		else if ((*itr).second.inputDevice == KeyType::Gamepad && GetPadReleased((*itr).second.key))
			return true;
	}
	return false;
}

Vector3 InputManager::GetMousePos()
{
	double x, y;
	glfwGetCursorPos(mCurrentWindow, &x, &y);
	return  Vector3(static_cast<float>(x), static_cast<float>(y), 0) - mSceneWindowPos;
}

Vector3 InputManager::GetMousePosScreen()
{
	Vector3 screenPos = GetMousePos();
	screenPos.mX -= (mScreenWindowMax.mX - mScreenWindowMin.mX) / 2 + mScreenWindowMin.mX;
	screenPos.mY -= (mScreenWindowMax.mY - mScreenWindowMin.mY) / 2 + mScreenWindowMin.mY;
	screenPos.mY = -screenPos.mY;

	screenPos.mX *= InputManager::GetInstance()->GetScreenToWorldScaling().mX;
	screenPos.mY *= InputManager::GetInstance()->GetScreenToWorldScaling().mY;

	return screenPos;
}

Vector3 InputManager::GetMousePosWorld()
{
	return GetMousePosScreen() + GraphicsSystem::GetInstance()->mCamera.mCurrCamInfo.mPosition;
}

bool InputManager::GetMouseButton(int _buttonID)
{
	if (_buttonID < 0 || _buttonID > GLFW_MOUSE_BUTTON_LAST || !InputManager::GetInstance()->GetMouseInScene())//not valid button or out of scene
		return false;
	return glfwGetMouseButton(mCurrentWindow, _buttonID);
}

bool InputManager::GetMouseButtonDown(int _buttonID)
{
	std::pair<int, int> code{ _buttonID, GLFW_PRESS };
	std::deque<std::pair<int, int>>::iterator itr = find(mTriggerEventsRead.begin(), mTriggerEventsRead.end(), code);
	return itr != mTriggerEventsRead.end();
}

bool InputManager::GetMouseButtonUp(int _buttonID)
{
	std::pair<int, int> code{ _buttonID, GLFW_RELEASE };
	std::deque<std::pair<int, int>>::iterator itr = find(mTriggerEventsRead.begin(), mTriggerEventsRead.end(), code);
	return itr != mTriggerEventsRead.end();
}

bool InputManager::GetMouseInScene()
{
	Vector3 pos = GetMousePos();
	if (pos.mX < 0 || pos.mY < 0 || pos.mX > mScreenWindowMax.mX || pos.mY > mScreenWindowMax.mY)
		return false;
	else
		return true;
}

void InputManager::UpdateScreenPos(float _x, float _y)
{
	mSceneWindowPos.mX = _x;
	mSceneWindowPos.mY = _y;
}

void InputManager::UpdateRegionMax(float _x, float _y)
{
	mScreenWindowMax.mX = _x;
	mScreenWindowMax.mY = _y;
}

void InputManager::UpdateRegionMin(float _x, float _y)
{
	mScreenWindowMin.mX = _x;
	mScreenWindowMin.mY = _y;
}

int InputManager::GetLatestKeyPress()
{
	if (mTriggerEventsRead.size() == 0)
		return 0;

	return mTriggerEventsRead.front().first;
}

Vector3 InputManager::GetScreenToWorldScaling()
{
	Vector3 worldSize = GraphicsSystem::GetInstance()->mWorldSize;
	GLfloat camZoomVal = 1 / GraphicsSystem::GetInstance()->mCamera.mCurrCamInfo.mZoom;
	return Vector3((worldSize.mX / (mScreenWindowMax.mX - mScreenWindowMin.mX)) * camZoomVal, (worldSize.mY / (mScreenWindowMax.mY - mScreenWindowMin.mY)) * camZoomVal, 0);
}

void InputManager::ChangeActionKeybind(std::string _actionToChange)
{
	std::pair<std::multimap<std::string, KeyType>::iterator, std::multimap<std::string, KeyType>::iterator> result = mActionsMap.equal_range(_actionToChange);
	if (result.first == result.second)//_action not found
	{
#if _ENGINE
		//print out error message
		DebugLogger::GetInstance()->Log("Action <%s> not found in map\n", _actionToChange.c_str());
#endif // _ENGINE
		return;
	}

	for (std::multimap<std::string, KeyType>::iterator itr = result.first; itr != result.second; ++itr)
	{
		if ((*itr).second.inputDevice == KeyType::Keyboard)
		{
			(*itr).second.key = -1;
			mKeyBindToChange = _actionToChange;
			return;
		}
	}

}

int InputManager::GetCurrentActionKeybind(std::string _action)
{
	std::pair<std::multimap<std::string, KeyType>::iterator, std::multimap<std::string, KeyType>::iterator> result = mActionsMap.equal_range(_action);
	if (result.first == result.second)//_action not found
	{
#if _ENGINE
		//print out error message
		DebugLogger::GetInstance()->Log("Action <%s> not found in map\n", _action.c_str());
#endif // _ENGINE
		return -1;
	}

	for (std::multimap<std::string, KeyType>::iterator itr = result.first; itr != result.second; ++itr)
	{
		if ((*itr).second.inputDevice == KeyType::Keyboard)
			return (*itr).second.key;
	}

	return -1;
}

std::string InputManager::KeycodeToString(int keycode)
{
	switch (keycode)
	{
		case KeyCode::KEY_UNKNOWN: return "Press Any Key";
		case KeyCode::SPACE: return "SPACE";
		case KeyCode::APOSTROPHE: return "'";
		case KeyCode::COMMA: return ",";
		case KeyCode::MINUS: return "-";
		case KeyCode::PERIOD: return ".";
		case KeyCode::SLASH: return "/";
		case KeyCode::ALPHA_0 : return "0";
		case KeyCode::ALPHA_1 : return "1";
		case KeyCode::ALPHA_2 : return "2";
		case KeyCode::ALPHA_3 : return "3";
		case KeyCode::ALPHA_4 : return "4";
		case KeyCode::ALPHA_5 : return "5";
		case KeyCode::ALPHA_6 : return "6";
		case KeyCode::ALPHA_7 : return "7";
		case KeyCode::ALPHA_8 : return "8";
		case KeyCode::ALPHA_9 : return "9";
		case KeyCode::SEMICOLON: return ";";
		case KeyCode::EQUAL: return "=";
		case KeyCode::A: return "A";
		case KeyCode::B: return "B";
		case KeyCode::C: return "C";
		case KeyCode::D: return "D";
		case KeyCode::E: return "E";
		case KeyCode::F: return "F";
		case KeyCode::G: return "G";
		case KeyCode::H: return "H";
		case KeyCode::I: return "I";
		case KeyCode::J: return "J";
		case KeyCode::K: return "K";
		case KeyCode::L: return "L";
		case KeyCode::M: return "M";
		case KeyCode::N: return "N";
		case KeyCode::O: return "O";
		case KeyCode::P: return "P";
		case KeyCode::Q: return "Q";
		case KeyCode::R: return "R";
		case KeyCode::S: return "S";
		case KeyCode::T: return "T";
		case KeyCode::U: return "U";
		case KeyCode::V: return "V";
		case KeyCode::W: return "W";
		case KeyCode::X: return "X";
		case KeyCode::Y: return "Y";
		case KeyCode::Z: return "Z";
		case KeyCode::LEFT_BRACKET: return "[";
		case KeyCode::BACKSLASH: return "\\";
		case KeyCode::RIGHT_BRACKET: return "]";
		case KeyCode::GRAVE_ACCENT: return "`";
		case KeyCode::ESCAPE: return "ESC";
		case KeyCode::ENTER: return "ENTER";
		case KeyCode::TAB: return "TAB";
		case KeyCode::BACKSPACE: return "BACKSPACE";
		case KeyCode::INSERT: return "INSERT";
		case KeyCode::DELETE_: return "DELETE";
		case KeyCode::RIGHT: return "RIGHT";
		case KeyCode::LEFT: return "LEFT";
		case KeyCode::DOWN: return "DOWN";
		case KeyCode::UP: return "UP";
		case KeyCode::PAGE_UP: return "PAGE UP";
		case KeyCode::PAGE_DOWN: return "PAGE DOWN";
		case KeyCode::HOME: return "HOME";
		case KeyCode::END: return "END";
		case KeyCode::CAPS_LOCK: return "CAPS";
		case KeyCode::SCROLL_LOCK: return "SCROLL";
		case KeyCode::NUM_LOCK: return "NUM LOCK";
		case KeyCode::PRINT_SCREEN: return "PRINT SCREEN";
		case KeyCode::PAUSE: return "PAUSE";
		case KeyCode::F1: return "F1";
		case KeyCode::F2: return "F2";
		case KeyCode::F3: return "F3";
		case KeyCode::F4: return "F4";
		case KeyCode::F5: return "F5";
		case KeyCode::F6: return "F6";
		case KeyCode::F7: return "F7";
		case KeyCode::F8: return "F8";
		case KeyCode::F9: return "F9";
		case KeyCode::F10: return "F10";
		case KeyCode::F11: return "F11";
		case KeyCode::F12: return "F12";
		case KeyCode::F13: return "F13";
		case KeyCode::F14: return "F14";
		case KeyCode::F15: return "F15";
		case KeyCode::F16: return "F16";
		case KeyCode::F17: return "F17";
		case KeyCode::F18: return "F18";
		case KeyCode::F19: return "F19";
		case KeyCode::F20: return "F20";
		case KeyCode::F21: return "F21";
		case KeyCode::F22: return "F22";
		case KeyCode::F23: return "F23";
		case KeyCode::F24: return "F24";
		case KeyCode::F25: return "F25";
		case KeyCode::KEYPAD_0: return "KEYPAD 0";
		case KeyCode::KEYPAD_1: return "KEYPAD 1";
		case KeyCode::KEYPAD_2: return "KEYPAD 2";
		case KeyCode::KEYPAD_3: return "KEYPAD 3";
		case KeyCode::KEYPAD_4: return "KEYPAD 4";
		case KeyCode::KEYPAD_5: return "KEYPAD 5";
		case KeyCode::KEYPAD_6: return "KEYPAD 6";
		case KeyCode::KEYPAD_7: return "KEYPAD 7";
		case KeyCode::KEYPAD_8: return "KEYPAD 8";
		case KeyCode::KEYPAD_9: return "KEYPAD 9";
		case KeyCode::KEYPAD_DECIMAL: return "KEYPAD .";
		case KeyCode::KEYPAD_DIVIDE: return "KEYPAD /";
		case KeyCode::KEYPAD_MULTIPLY: return "KEYPAD *";
		case KeyCode::KEYPAD_SUBTRACT: return "KEYPAD -";
		case KeyCode::KEYPAD_ADD: return "KEYPAD +";
		case KeyCode::KEYPAD_ENTER : return "KEYPAD ENTER";
		case KeyCode::KEYPAD_EQUAL : return "KEYPAD =";
		case KeyCode::LEFT_SHIFT: return "LEFT SHIFT";
		case KeyCode::LEFT_CONTROL: return "LEFT CONTROL";
		case KeyCode::LEFT_ALT: return "LEFT ALT";
		case KeyCode::LEFT_SUPER: return "LEFT SUPER";
		case KeyCode::RIGHT_SHIFT: return "RIGHT SHIFT";
		case KeyCode::RIGHT_CONTROL: return "RIGHT CONTROL";
		case KeyCode::RIGHT_ALT: return "RIGHT ALT";
		case KeyCode::RIGHT_SUPER: return "RIGHT SUPER";
		case KeyCode::MENU: return "MENU";
	default:
		return "UNKNOWN";
	}

	return "UNKNOWN";
}

void InputManager::CheckRebind(int _keycode)
{
	//We skip locked keys
	if (std::find(lockedKeys.begin(), lockedKeys.end(), _keycode) != lockedKeys.end())
		return;

	if (mKeyBindToChange.length() > 0)
	{
		std::pair<std::multimap<std::string, KeyType>::iterator, std::multimap<std::string, KeyType>::iterator> result = mActionsMap.equal_range(mKeyBindToChange);
		for (std::multimap<std::string, KeyType>::iterator itr = result.first; itr != result.second; ++itr)
		{
			if ((*itr).second.inputDevice == KeyType::Keyboard)
			{
				 (*itr).second.key = _keycode;
				 SaveControls();
				 mKeyBindToChange = "";
				 return;
			}
		}
	}
}
