/*!
All content © 2023 DigiPen Institute of Technology Singapore, all rights reserved.
@file       JSONManager.h
@author     HEAR Li Heng (100%)
@co-author

----------------------------------------------------------------------------

@course     CSD2401F23
@section    CSD2401F23-A
@project    CSD2401F23 Software Engineering Project 3

@brief      Manages the reading and parsing of json files
*//*______________________________________________________________________*/

#ifndef DATA_PARSER_H
#define DATA_PARSER_H
//primarily used for reading from json files(level files,prefab files)
#include "Singleton.h"
#include "Manager.h"
#include <string>
#include <map>
#include <vector>
#include <iostream>
#include "Vector3.h"
#include "Vector4.h"
#include <stack>

#pragma warning(push, 0)
#include <document.h>//needed for rapidjson::Value* return type ** try to remove
#pragma warning(pop)

#include "AssetManager.h"

#if _ENGINE
#include "DebugLogger.h"
#endif // _ENGINE

class Entity;//forward declare
class JSONManager : public Singleton<JSONManager>, public Manager
{
public:
	//mostly for unload to clear loaded jsons

	/*!
	 * @brief 
	 *	Inits the JSONManager
	*/
	void Init();

	/*!
	 * @brief 
	 *	Loads the JSONManager
	*/
	void Load();

	/*!
	 * @brief 
	 *	Updates the JSONManager
	*/
	void Update();

	/*!
	 * @brief 
	 *	Unloads the JSONManager
	*/
	void Unload();

	/*!
	 * @brief 
	 *	Sets the current loaded file(mCurrentFile) so that member _values can be read
	 * @param _fileName 
	 *	The _name of the file to be loaded
	 * @return 
	 *	True if file is loaded false otherwise
	*/
	bool LoadFile(std::string _fileName);

	/*!
	 * @brief 
	 *	Sets the current loaded _object(mCurrentValue) so that member _values can be read
	 * @param __objName 
	 *	The _name of the _object to be loaded
	 * @return 
	 *	True if _object is loaded false otherwise
	*/
	bool LoadObject(std::string __objName);

	/*!
	 * @brief 
	 *	Sets the current loaded _object(mCurrentValue) so that member _values can be read
	 *	Takes in a Value pointer instead (If _object is created elsewehere
	 * @param _obj 
	 *	The pointer to the _object to be loaded
	*/
	void LoadObject(rapidjson::Value* _obj);

	/*!
	 * @brief 
	 *	CLoses the currently loaded _object
	*/
	void CloseObject();				
	
	/*!
	 * @brief 
	 *	Closes the currently loaded file
	*/
	void CloseFile();

	//----------Get functions----------

	/*!
	 * @brief 
	 *	Gets the Bool _value from the currently loaded _object/file
	 * @param _name 
	 *	Name of the Bool
	 * @return 
	 *	Value of the bool (returns false if not found)
	*/
	bool GetBool(std::string _name);

	/*!
	 * @brief
	 *	Gets the Int _value from the currently loaded _object/file
	 * @param _name
	 *	Name of the Int
	 * @return
	 *	Value of the Int (returns -1 if not found)
	*/
	int GetInt(std::string _name);

	/*!
	 * @brief
	 *	Gets the Float _value from the currently loaded _object/file
	 * @param _name
	 *	Name of the Float
	 * @return
	 *	Value of the Float (returns -10f if not found)
	*/
	float GetFloat(std::string _name);

	/*!
	 * @brief
	 *	Gets the Double _value from the currently loaded _object/file
	 * @param _name
	 *	Name of the Double
	 * @return
	 *	Value of the Double (returns -10 if not found)
	*/
	double GetDouble(std::string _name);

	/*!
	 * @brief
	 *	Gets the Vector3 value from the currently loaded object/file
	 *	Converts an array of size 3 into a Vector3 since rapidJSON has
	 *	no Vector3
	 * @param _name
	 *	Name of the Vector3
	 * @return
	 *	Value of the Vector3 (returns Vector3(0,0,0) if not found)
	*/
	Vector3 GetVec3(std::string _name);

	/*!
	 * @brief 
	 *	Gets the Vector4 value from the currently loaded object/file
	 *	Converts an array of size 4 into a Vector4 since rapidJSON has
	 *	no Vector4
	 * @param _name 
	 *	Name of the Vector4
	 * @return 
	 *	Value of the Vector4 (returns Vector4(0,0,0,0) if not found)
	*/
	Vector4 GetVec4(std::string _name);

	/*!
	 * @brief
	 *	Gets the String _value from the currently loaded _object/file
	 * @param _name
	 *	Name of the String
	 * @return
	 *	Value of the String (returns "" if not found)
	*/
	std::string GetString(std::string _name);

	/*!
	 * @brief
	 *	Gets a rapidjson::Value from the currently loaded _object/file
	 * @param _name
	 *	Name of the Value
	 * @return
	 *	Value of the Value (returns nullptr if not found)
	*/
	rapidjson::Value* GetValue(std::string _name);

	//----------Save functions----------

	/*!
	 * @brief 
	 *	Creates a new save file
	 *	Call this before saving to create a new save file
	*/
	void CreateNewSave();

	/*!
	 * @brief 
	 *	Deletes the created save file
	 *	Call this after SaveFile is called to unloads current
	 *	loaded file
	*/
	void DeleteNewSave();

	/*!
	 * @brief 
	 *	Creates a new rapidjson::Value to be able to store new data
	 * @param _name 
	 *	Name of the _object
	 * @param isArray 
	 *	Bool to set if this _object should be an array
	 * @return 
	 *	True if save was successful | false otherwise
	*/
	bool SaveObject(std::string _name,bool isArray);

	/*!
	 * @brief 
	 *	Template function to save values (bool/int/float/double)
	 * @tparam T 
	 *	Type of the _value to save
	 * @param _name 
	 *	Name of the _value to save
	 * @param _val 
	 *	The _value to save
	 * @return 
	 *	True if save was successful | false otherwise
	*/
	template<typename T>
	bool Save(std::string _name,T _val);

	/*!
	 * @brief
	 *	Template function to pushback values into array (bool/int/float/double)
	 * @tparam T
	 *	Type of the value to pushback
	 * @param _val
	 *	The _value to save
	 * @return
	 *	True if save was successful | false otherwise
	*/
	template<typename T>
	bool Pushback( T _val);

	/*!
	 * @brief
	 *	Overload of Pushback for string
	 * @param _val
	 *	The value to pushback
	 * @return
	 *	True if pushback was successful | false otherwise
	*/
	bool Pushback(std::string _val);

	/*!
	 * @brief 
	 *	Overloaded Save() for Vector3
	 * @param _name 
	 *	Name of the value to save
	 * @param _val 
	 *	The value to save
	 * @return 
	 *	True if save was successful | false otherwise
	*/
	bool Save(std::string _name, Vector3 _val);

	/*!
	 * @brief 
		Overloaded Save() for Vector4
	 * @param _name 
	 *	Name of the value to save
	 * @param _val 
	 *	the value to save
	 * @return 
	 *	True if save was successful | false otherwise
	*/
	bool Save(std::string _name, Vector4 _val);

	/*!
	 * @brief
	 *	Overloaded Save() for string
	 * @param _name
	 *	Name of the _value to save
	 * @param _val
	 *	The _value to save
	 * @return
	 *	True if save was successful | false otherwise
	*/
	bool Save(std::string _name, std::string _val);

	/*!
	 * @brief
	 *	Overloaded Save() for const char*
	 * @param _name
	 *	Name of the _value to save
	 * @param _val
	 *	The _value to save
	 * @return
	 *	True if save was successful | false otherwise
	*/
	bool Save(std::string _name, const char* _val);

	/*!
	 * @brief 
	 *	Saves the file into an actual file
	 * @param _fileName 
	 *	The _name of the file to save
	 * @param _extension 
	 *	The _extension of the file that you want
	 * @param _saveLocation 
	 *	The location to save the file
	 * @return 
	 *	True if save was successful | false otherwise
	*/
	bool SaveFile(std::string _fileName, std::string _extension, std::string _saveLocation);

	/*!
	 * @brief 
	 *	Gets an array of unspecified size
	 * @tparam T 
	 *	Template type of the array
	 * @param _name 
	 *	Name of the array
	 * @return 
	 *	The vector of _values (return empty vector if not found)
	*/
	template <typename T>
	std::vector<T> GetArray(std::string _name);


	//----------Asset creation/deletion----------

	/*!
	 * @brief 
	 *	Creates a JSONAsset to be stored in AssetManager
	 * @param _filePath 
	 *	The file path to the JSON file
	 * @param _dataPtr 
	 *	The pointer to where the JSONAsset will be stored
	 * @return 
	*/
	bool CreateJSONAsset(std::string _filePath, void*& _dataPtr);

	/*!
	 * @brief 
	 *	Deletes the JSONAsset
	 * @param _dataPtr 
	 *	The pointer to the JSONAsset
	*/
	static void DeleteJSONAsset(void*& _dataPtr);

private:
	//The stack of Values* (FILO container)
	//points to current loaded _value so outside functions can just
	//read from currentValue in the get functions
	std::stack<rapidjson::Value*> mCurrentValue;

	//The pointer to the JSONAsset which is loaded
	AssetManager::JSONAsset* mCurrentFile{ nullptr };

	/*!
	 * @brief 
	 *	Default constructor
	*/
	JSONManager();

	/*!
	 * @brief
	 *	Default destructor
	*/
	~JSONManager();

	//For singleton
	friend Singleton<JSONManager>;

};

//Template function definitions

/*!
 * @brief 
 *	Template function to save basic types
 * @tparam T 
 *	Basic type to save
 * @param _name 
 *	Name of value to save as
 * @param _val 
 *	Value to save
 * @return 
 *	True if save was successful | false otherwise
*/
template<typename T>
inline bool JSONManager::Save(std::string _name, T _val)
{
	rapidjson::Value _nameObj(_name.c_str(), mCurrentFile->GetAllocator());
	if (!mCurrentValue.empty())//currently opened _object
	{
		mCurrentValue.top()->AddMember(_nameObj.Move(), _val, mCurrentFile->GetAllocator());
		return true;
	}
	else
	{
		if (mCurrentFile)//at the top layer of _object
		{
			mCurrentFile->AddMember(_nameObj.Move(), _val, mCurrentFile->GetAllocator());
			return true;
		}
	}
#if _ENGINE
	DebugLogger::GetInstance()->Log("(%s) can not be saved\n", _name.c_str());
#endif // _ENGINE
	return false;
}

/*!
 * @brief 
 *	Pushes back a value into an array that has been set by SaveObject(true)
 *	Use this for specific types of array that are not vectors
 * @tparam T 
 *	The type of the value
 * @param _val 
 *	The value to pushback
 * @return 
 *	True if pushback was successful | false otherwise
*/
template<typename T>
inline bool JSONManager::Pushback(T _val)
{
	if (!mCurrentValue.empty() && mCurrentValue.top()->IsArray())//currently opened _object
	{
		mCurrentValue.top()->PushBack(_val, mCurrentFile->GetAllocator());
		return true;
	}
	return false;
}

/*!
 * @brief 
 *	Template function to get an array of values and store as a vector
 * @tparam T 
 *	The type of the value
 * @param _name 
 *	The name of the array
 * @return 
 *	A vector filled with the values if successful | An empty value otherwise
*/
template<typename T>
inline std::vector<T> JSONManager::GetArray(std::string _name)
{
	std::vector<T> returnVec;
	if (!mCurrentValue.empty())
	{
		if (mCurrentValue.top()->HasMember(_name.c_str()) && mCurrentValue.top()->IsArray())
		{
			rapidjson::GenericArray<false, rapidjson::Value> arr = mCurrentValue.top()->GetArray();
			for (rapidjson::SizeType i = 0; i < arr.Size(); ++i)
			{
				returnVec.push_back(arr[i].Get<T>());
			}
		}
	}
	else
	{
		if (mCurrentFile && mCurrentFile->HasMember(_name.c_str()) && (*mCurrentFile)[_name].IsArray())
		{
			rapidjson::GenericArray<false, rapidjson::Value> arr = (*mCurrentFile)[_name].GetArray();
			for (rapidjson::SizeType i = 0; i < arr.Size(); ++i)
			{
				returnVec.push_back(arr[i].Get<T>());
			}
		}
	}
	if (returnVec.size() == 0)
	{
#if _ENGINE
		DebugLogger::GetInstance()->Log("Array (%s) does not exist\n", _name.c_str());
#endif // _ENGINE
	}
	return returnVec;
}

#endif