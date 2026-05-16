/*!
All content © 2023 DigiPen Institute of Technology Singapore, all rights reserved.
@file       MyImGui.h
@author		HEAR Li Heng (100%)
@co-author	

----------------------------------------------------------------------------

@course     CSD2401F23
@section    CSD2401F23-A
@project    CSD2401F23 Software Engineering Project 3

@brief      This file contains some frequently used code for displaying
			common types in the Property window. Mostly used in 
			components.DisplayProperties()
*//*______________________________________________________________________*/
#if _ENGINE

#include "Vector3.h"
#include "Vector4.h"
#include <glm/glm.hpp>
#include <string>

#ifndef MYIMGUI_H
#define MYIMGUI_H

class MyImGui
{
public:

	/*!
	 * @brief 
	 *	Displays a input field for a Vector3
	 * @param _vec 
	 *	The Vector3 to be edited/displayed
	 * @param _name 
	 *	The name of this field
	 * @param _extraTag 
	 *	Extra tag in case multiple fields of the same name exists
	 * @return 
	 *	True if value was edited | False otherwise
	*/
	static bool MyImGuiVector3Input(Vector3& _vec, std::string _name, std::string _extraTag = "");

	/*!
	 * @brief
	 *	Displays a input field for a Vector3
	 * @param _vec
	 *	The Vector3 to be edited/displayed
	 * @param _name
	 *	The name of this field
	 * @param _extraTag
	 *	Extra tag in case multiple fields of the same name exists
	 * @return
	 *	True if value was edited | False otherwise
	*/
	static bool MyImGuiVector3Input(glm::vec3& _vec, std::string _name, std::string _extraTag = "");

	/*!
	 * @brief 
	 *	Displays a input field for a Vector4
	 * @param _vec 
	 *	The Vector4 to be edited/displayed
	 * @param _name 
	 *	The name of this field
	 * @param _extraTag 
	 *	Extra tag in case multiple fields of the same name exists
	 * @return 
	 *	True if value was edited | False otherwise
	*/
	static bool MyImGuiVector4Input(Vector4& _vec, std::string _name, std::string _extraTag = "");

	/*!
	 * @brief
	 *	Displays a input field for a Vector2
	 * @param _vec
	 *	The Vector2 to be edited/displayed
	 * @param _name
	 *	The name of this field
	 * @param _extraTag
	 *	Extra tag in case multiple fields of the same name exists
	 * @return 
	 *	True if value was edited | False otherwise
	*/
	static bool MyImGuiVector2Input(glm::vec2& _vec, std::string _name, std::string _extraTag = "");

	/*!
	 * @brief 
	 *	Displays a input field for an individual Float
	 * @param _val 
	 *	The Float to be edited/displayed
	 * @param _name 
	 *	The name of this field
	 * @param _extraTag
	 *	Extra tag in case multiple fields of the same name exists
	 * @return 
	 *	True if value was edited | False otherwise
	*/
	static bool MyImGuiFloatInput(float& _val, std::string _name, std::string _extraTag = "");

	/*!
	 * @brief
	 *	Displays a input field for an individual Double
	 * @param _val
	 *	The Double to be edited/displayed
	 * @param _name
	 *	The name of this field
	 * @param _extraTag
	 *	Extra tag in case multiple fields of the same name exists
	 * @return 
	 *	True if value was edited | False otherwise
	*/
	static bool MyImGuiDoubleInput(double& _val, std::string _name, std::string _extraTag = "");

	/*!
	 * @brief 
	 *	Displays a input field for an Int
	 * @param _val 
	 *	The Int to be edited/displayed
	 * @param _name 
	 *	The name of this field
	 * @param _extraTag
	 *	Extra tag in case multiple fields of the same name exists
	 * @return 
	 *	True if value was edited | False otherwise
	*/
	static bool MyImGuiIntInput(int& _val, std::string _name, std::string _extraTag = "");

	/*!
	 * @brief
	 *	Displays a input field for a String
	 * @param _val
	 *	The String to be edited/displayed
	 * @param _name
	 *	The name of this field
	 * @param _extraTag
	 *	Extra tag in case multiple fields of the same name exists
	 * @return 
	 *	True if value was edited | False otherwise
	*/
	static bool MyImGuiStringInput(std::string& _val, std::string _name, std::string _extraTag = "");

	/*!
	 * @brief 
	 *	Displays Text Wrapped
	 * @param _text 
	 *	The text to display
	*/
	static void MyImGuiTextOutput(std::string _text);

	/*!
	 * @brief 
	 *	Displays a checkbox for a bool value
	 * @param _val 
	 *	The bool to display
	 * @param _name 
	 *	The name of this field
	 * @param _extraTag 
	 *	Extra tag in case multiple fields of the same name exists
	 * @return 
	 *	True if value was edited | False otherwise
	*/
	static bool MyImGuiBoolInput(bool& _val, std::string _name, std::string _extraTag = "");
};

#endif
#endif // _ENGINE