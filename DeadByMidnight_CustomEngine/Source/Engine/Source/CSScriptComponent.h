/*!
All content © 2023 DigiPen Institute of Technology Singapore, all rights reserved.
@file       CSScriptComponent.h
@author		HEAR Li Heng (100%)
@co-author	

----------------------------------------------------------------------------

@course     CSD2401F23
@section    CSD2401F23-A
@project    CSD2401F23 Software Engineering Project 3

@brief      Stores the script name and registers it with MonoManager when the 
			component gets Inited
*//*______________________________________________________________________*/
#pragma once

#include "ComponentBase.h"
#include <variant>

class CSScriptComponent : public ComponentBase
{
public:
	std::string mScriptName;
	struct SerializedInfo
	{
		std::string name;
		std::variant<int, float, bool, Vector3, Vector4> value;
	};
	std::vector<SerializedInfo> serializedValues;//Current serialized values

	/*!
	 * @brief 
	 *	Init function for script component
	*/
	void Init() {};

	/*!
	 * @brief 
	 *	Update function for script component
	*/
	void Update() {}

	/*!
	 * @brief 
	 *	Exit function for script component
	 *	Deregister script is called here
	*/
	void Exit();

	/*!
	 * @brief 
	 *	Load function for script component
	*/
	void Load();

	/*!
	 * @brief 
	 *	Save function for script component
	*/
	void Save();

	/*!
	 * @brief 
	 *	Sets the script for the component
	 * @param _scriptName 
	 *	The script name to set
	*/
	void SetScript(std::string _scriptName);

	/*!
	 * @brief 
	 *	Displays the component in the properties window
	*/
	void DisplayProperties();

	/*!
	 * @brief
	 * operator<<
	 * @param os
	 * Output stream object
	 * @param _col
	 * Variable to print out
	 * @return
	 * Output stream object
	*/
	friend std::ostream& operator<< (std::ostream& _os, const CSScriptComponent& _script);

};
