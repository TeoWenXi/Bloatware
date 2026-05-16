/*!
All content © 2023 DigiPen Institute of Technology Singapore, all rights reserved.
@file       JSONManager.cpp
@author     HEAR Li Heng (100%)
@co-author

----------------------------------------------------------------------------

@course     CSD2401F23
@section    CSD2401F23-A
@project    CSD2401F23 Software Engineering Project 3

@brief      Manages the reading and parsing of json files
			Uses rapidjson --> https://rapidjson.org/

*//*______________________________________________________________________*/

#include "JSONManager.h"
#include <fstream>

#include "EntityManager.h"
#include "Transform.h"
#include "Renderer.h"

#include <prettywriter.h>
#include <ostreamwrapper.h>
#include <istreamwrapper.h>

#include "AssetManager.h"

//temp
#include "LevelLoader.h"
void JSONManager::Init()
{
}

void JSONManager::Load()
{

}

void JSONManager::Update()
{

}

void JSONManager::Unload()
{
	//Destroy instance
	Destroy();
}

//Sets the current loaded file(mCurrentFile) so that member _values can be read
bool JSONManager::LoadFile(std::string _fileName)
{
	return AssetManager::GetInstance()->GetAsset(_fileName, mCurrentFile);
}

//Sets the current loaded _object(mCurrentValue) so that member _values can be read
bool JSONManager::LoadObject(std::string __objName)
{
	if (mCurrentFile == nullptr)//file not opened
		return false;
	mCurrentValue.push(GetValue(__objName));
	return true;
}

//Sets the current loaded _object(mCurrentValue) so that member _values can be read
void JSONManager::LoadObject(rapidjson::Value* _obj)
{
	mCurrentValue.push(_obj);
}

//CLoses the currently loaded _object
void JSONManager::CloseObject()
{
	mCurrentValue.pop();
}

//CLoses the currently loaded file
void JSONManager::CloseFile()
{
	mCurrentFile = nullptr;
}

bool JSONManager::CreateJSONAsset(std::string _filePath, void*& newDoc)
{
	std::ifstream saveFile;
	saveFile.open(_filePath);//open saveFile.txt
	if (!saveFile.good())
	{
#if _ENGINE
		DebugLogger::GetInstance()->Log("Failed to open file (%s)!!\n", _filePath.c_str());
#endif // _ENGINE
		return false;
	}

	rapidjson::IStreamWrapper stream(saveFile);
	newDoc = new AssetManager::JSONAsset;
	static_cast<AssetManager::JSONAsset*>(newDoc)->ParseStream(stream);

	saveFile.close();
	return true;
}

void JSONManager::DeleteJSONAsset(void*& _dataPtr)
{
	delete static_cast<AssetManager::JSONAsset*>(_dataPtr);
}

bool JSONManager::Pushback(std::string _val)
{
	if (!mCurrentValue.empty() && mCurrentValue.top()->IsArray())//currently opened _object
	{
		rapidjson::Value strVal;
		strVal.SetString(_val.c_str(), (rapidjson::SizeType)_val.length(), mCurrentFile->GetAllocator());
		mCurrentValue.top()->PushBack(strVal, mCurrentFile->GetAllocator());
		return true;
	}
	return false;
}

#pragma region GetFunctions
bool JSONManager::GetBool(std::string _name)
{
	if (!mCurrentValue.empty())//currently opened _object
	{
		if (mCurrentValue.top()->HasMember(_name.c_str()))
			return (*mCurrentValue.top())[_name.c_str()].GetBool();
	}
	else
	{
		if (mCurrentFile && mCurrentFile->HasMember(_name.c_str()))//at the top layer of _object
			return (*mCurrentFile)[_name.c_str()].GetBool();
	}
#if _ENGINE
	DebugLogger::GetInstance()->Log("Bool (%s) does not exist\n", _name.c_str());
#endif // _ENGINE
	return false;
}

int JSONManager::GetInt(std::string _name)
{
	if (!mCurrentValue.empty())
	{
		if (mCurrentValue.top()->HasMember(_name.c_str()))
			return (*mCurrentValue.top())[_name.c_str()].GetInt();
	}
	else
	{
		if (mCurrentFile && mCurrentFile->HasMember(_name.c_str()))
			return (*mCurrentFile)[_name.c_str()].GetInt();
	}
#if _ENGINE
	DebugLogger::GetInstance()->Log("Int (%s) does not exist\n", _name.c_str());
#endif // _ENGINE
	return -1;
}

float JSONManager::GetFloat(std::string _name)
{
	if (!mCurrentValue.empty())
	{
		if (mCurrentValue.top()->HasMember(_name.c_str()))
			return (*mCurrentValue.top())[_name.c_str()].GetFloat();
	}
	else
	{
		if (mCurrentFile && mCurrentFile->HasMember(_name.c_str()))
			return (*mCurrentFile)[_name.c_str()].GetFloat();
	}
#if _ENGINE
	DebugLogger::GetInstance()->Log("Float (%s) does not exist\n", _name.c_str());
#endif // _ENGINE
	return -1.0f;
}

double JSONManager::GetDouble(std::string _name)
{
	if (!mCurrentValue.empty())
	{
		if (mCurrentValue.top()->HasMember(_name.c_str()))
			return (*mCurrentValue.top())[_name.c_str()].GetDouble();
	}
	else
	{
		if (mCurrentFile && mCurrentFile->HasMember(_name.c_str()))
			return (*mCurrentFile)[_name.c_str()].GetDouble();
	}
#if _ENGINE
	DebugLogger::GetInstance()->Log("Double (%s) does not exist\n", _name.c_str());
#endif // _ENGINE
	return -1.0;
}

Vector3 JSONManager::GetVec3(std::string _name)
{
	if (!mCurrentValue.empty())
	{
		if (mCurrentValue.top()->HasMember(_name.c_str()))
			return Vector3((*mCurrentValue.top())[_name.c_str()][0].GetFloat(), (*mCurrentValue.top())[_name.c_str()][1].GetFloat(), (*mCurrentValue.top())[_name.c_str()][2].GetFloat());
	}
	else
	{
		if (mCurrentFile && mCurrentFile->HasMember(_name.c_str()))
			return Vector3((*mCurrentFile)[_name.c_str()][0].GetFloat(), (*mCurrentFile)[_name.c_str()][1].GetFloat(), (*mCurrentFile)[_name.c_str()][2].GetFloat());
	}
#if _ENGINE
	DebugLogger::GetInstance()->Log("Vec3 (%s) does not exist\n", _name.c_str());
#endif // _ENGINE
	return Vector3();
}

Vector4 JSONManager::GetVec4(std::string _name)
{
	if (!mCurrentValue.empty())
	{
		if (mCurrentValue.top()->HasMember(_name.c_str()))
			return Vector4((*mCurrentValue.top())[_name.c_str()][0].GetFloat(), (*mCurrentValue.top())[_name.c_str()][1].GetFloat(), (*mCurrentValue.top())[_name.c_str()][2].GetFloat(), (*mCurrentValue.top())[_name.c_str()][3].GetFloat());
	}
	else
	{
		if (mCurrentFile && mCurrentFile->HasMember(_name.c_str()))
			return Vector4((*mCurrentFile)[_name.c_str()][0].GetFloat(), (*mCurrentFile)[_name.c_str()][1].GetFloat(), (*mCurrentFile)[_name.c_str()][2].GetFloat(), (*mCurrentFile)[_name.c_str()][3].GetFloat());
	}
#if _ENGINE
	DebugLogger::GetInstance()->Log("Vec4 (%s) does not exist\n", _name.c_str());
#endif // _ENGINE
	return Vector4();
}


std::string JSONManager::GetString(std::string _name)
{
	if (!mCurrentValue.empty())
	{
		if (mCurrentValue.top()->IsObject() && mCurrentValue.top()->HasMember(_name.c_str()))
			return (*mCurrentValue.top())[_name.c_str()].GetString();
	}
	else
	{
		if (mCurrentFile && mCurrentFile->IsObject() && mCurrentFile->HasMember(_name.c_str()))
			return (*mCurrentFile)[_name.c_str()].GetString();
	}
#if _ENGINE
	DebugLogger::GetInstance()->Log("String (%s) doesnt exist\n", _name.c_str());
#endif // _ENGINE
	return "";
}

rapidjson::Value* JSONManager::GetValue(std::string _name)
{
	if (mCurrentFile->IsObject())
	{
		if (mCurrentFile && mCurrentFile->IsObject() && mCurrentFile->HasMember(_name.c_str()))
			return &(*mCurrentFile)[_name];
		else if (!mCurrentValue.empty())//currently opened _object
		{
			if (mCurrentValue.top()->HasMember(_name.c_str()))
				return &(*mCurrentValue.top())[_name];
		}
	}
	return nullptr;
}
#pragma endregion

#pragma region SetFunctions
void JSONManager::CreateNewSave()
{
	mCurrentFile = new rapidjson::Document;
	mCurrentFile->SetObject();
}

void JSONManager::DeleteNewSave()
{
	delete mCurrentFile;
}

bool JSONManager::SaveObject(std::string _name, bool isArray)//adds a new _object and sets the current _value to it
{
	rapidjson::Value _nameObj(_name.c_str(), mCurrentFile->GetAllocator());
	rapidjson::Value newObj;
	if (isArray)
		newObj.SetArray();
	else
		newObj.SetObject();

	if (!mCurrentValue.empty())//currently opened _object
	{
		if (mCurrentValue.top()->IsObject())
		{
			mCurrentValue.top()->AddMember(_nameObj.Move(), newObj.Move(), mCurrentFile->GetAllocator());
			mCurrentValue.push(&(mCurrentValue.top()->MemberEnd() - 1)->value);
			return true;
		}
		else
		{
			mCurrentValue.top()->PushBack(newObj.Move(), mCurrentFile->GetAllocator());
			mCurrentValue.push(&(*mCurrentValue.top()).GetArray()[mCurrentValue.top()->Size()-1]);
			return true;
		}
	}
	else
	{
		if (mCurrentFile)//at the top layer of _object
		{
			mCurrentFile->AddMember(_nameObj.Move(), newObj.Move(), mCurrentFile->GetAllocator());
			mCurrentValue.push(&(*mCurrentFile)[_name]);
			return true;
		}
	}
	
	return false;
}

bool JSONManager::Save(std::string _name, Vector3 _val)
{
	rapidjson::Value _nameObj(_name.c_str(), mCurrentFile->GetAllocator());
	//double check for nan (set to 0?)
	if (std::isnan(_val.mX))
		_val.mX = 0;
	if (std::isnan(_val.mY))
		_val.mY = 0;
	if (std::isnan(_val.mZ))
		_val.mZ = 0;
	//create array
	rapidjson::Value saveArr;
	saveArr.SetArray();
	saveArr.PushBack(_val.mX, mCurrentFile->GetAllocator());
	saveArr.PushBack(_val.mY, mCurrentFile->GetAllocator());
	saveArr.PushBack(_val.mZ, mCurrentFile->GetAllocator());
	if (!mCurrentValue.empty())//currently opened _object
	{
		mCurrentValue.top()->AddMember(_nameObj.Move(), saveArr.Move(), mCurrentFile->GetAllocator());
		return true;
	}
	else
	{
		if (mCurrentFile)//at the top layer of _object
		{
			mCurrentFile->AddMember(_nameObj.Move(), saveArr.Move(), mCurrentFile->GetAllocator());
			return true;
		}
	}
#if _ENGINE
	DebugLogger::GetInstance()->Log("Vec3 (%s) can not be saved\n", _name.c_str());
#endif // _ENGINE
	return false;
}

bool JSONManager::Save(std::string _name, Vector4 _val)
{
	rapidjson::Value _nameObj(_name.c_str(), mCurrentFile->GetAllocator());
	//double check for nan (set to 0?)
	if (std::isnan(_val.mX))
		_val.mX = 0;
	if (std::isnan(_val.mY))
		_val.mY = 0;
	if (std::isnan(_val.mZ))
		_val.mZ = 0;
	if (std::isnan(_val.mW))
		_val.mW = 0;

	//create array
	rapidjson::Value saveArr;
	saveArr.SetArray();
	saveArr.PushBack(_val.mX, mCurrentFile->GetAllocator());
	saveArr.PushBack(_val.mY, mCurrentFile->GetAllocator());
	saveArr.PushBack(_val.mZ, mCurrentFile->GetAllocator());
	saveArr.PushBack(_val.mW, mCurrentFile->GetAllocator());

	if (!mCurrentValue.empty())//currently opened _object
	{
		mCurrentValue.top()->AddMember(_nameObj.Move(), saveArr.Move(), mCurrentFile->GetAllocator());
		return true;
	}
	else
	{
		if (mCurrentFile)//at the top layer of _object
		{
			mCurrentFile->AddMember(_nameObj.Move(), saveArr.Move(), mCurrentFile->GetAllocator());
			return true;
		}
	}
#if _ENGINE
	DebugLogger::GetInstance()->Log("Vec4 (%s) can not be saved\n", _name.c_str());
#endif // _ENGINE
	return false;
}


bool JSONManager::Save(std::string _name, std::string _val)
{
	return Save(_name, _val.c_str());
}

bool JSONManager::Save(std::string _name, const char* _val)
{
	rapidjson::Value _nameObj(_name.c_str(), mCurrentFile->GetAllocator());
	rapidjson::Value _valObj(_val, static_cast<rapidjson::SizeType>(strlen(_val)), mCurrentFile->GetAllocator());

	if (!mCurrentValue.empty())//currently opened _object
	{
		if (mCurrentValue.top()->IsObject())
			mCurrentValue.top()->AddMember(_nameObj.Move(), _valObj.Move(), mCurrentFile->GetAllocator());
		else
			mCurrentValue.top()->PushBack(_valObj.Move(), mCurrentFile->GetAllocator());
		return true;
	}
	else
	{
		if (mCurrentFile)//at the top layer of _object
		{
			mCurrentFile->AddMember(_nameObj.Move(), _valObj.Move(), mCurrentFile->GetAllocator());
			return true;
		}
	}
#if _ENGINE
	DebugLogger::GetInstance()->Log("String (%s) can not be saved\n", _name.c_str());
#endif // _ENGINE
	return false;
}

bool JSONManager::SaveFile(std::string _fileName,std::string _extension,std::string _saveLocation)
{
	if (!mCurrentFile)//no open file
		return false;

#if _ENGINE
	DebugLogger::GetInstance()->Log("---------- Save Prefab: %s ----------\n", _fileName.c_str());
#endif // _ENGINE

	std::string fullPath = _saveLocation + "/" + _fileName + "." + _extension;// AssetManager::GetInstance()->GetFilePath<AssetManager::JSONAsset>(_fileName);
	std::ofstream outputFile;
	outputFile.open(fullPath);
	if (!outputFile.good())
	{
#if _ENGINE
		DebugLogger::GetInstance()->Log("Failed to open file!!\n");
#endif // _ENGINE
		return false;
	}
	rapidjson::OStreamWrapper osw(outputFile);
	rapidjson::PrettyWriter<rapidjson::OStreamWrapper> writer(osw);
	mCurrentFile->Accept(writer);
	outputFile.close();
	return true;
}

#pragma endregion

JSONManager::JSONManager()
{
}

JSONManager::~JSONManager()
{

}