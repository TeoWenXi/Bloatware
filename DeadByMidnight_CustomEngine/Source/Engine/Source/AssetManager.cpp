/*!
All content © 2023 DigiPen Institute of Technology Singapore, all rights reserved.
@file       AssetManager.cpp
@author     HEAR Li Heng (100%)
@co-author

----------------------------------------------------------------------------

@course     CSD2401F23
@section    CSD2401F23-A
@project    CSD2401F23 Software Engineering Project 3

@brief      Manages the loading and storing of all assets in the Assets folder
			Loads all files at start up so that throughout the runtime, everywhere
			else only need to call get function since all loaded files are consolidated
			here.
			Also checks the Assets folder and if a file changes in the Assets folder,
			reloads the file so if Get function is called, the changed file will be read
*//*______________________________________________________________________*/

//windows api
#include <filesystem>
#include <Windows.h>
#include <fileapi.h>

#include "AssetManager.h"//need be after windows.h if not redefiniton of macros

//file extensions
#include "JSONManager.h"
#include "SoundManager.h"
#include "TextManager.h"
#include "MonoManager.h"

#include "DragDrop.h"

#define BUFFER_SIZE 1024

//variables that require windows.h/filesystem/fileapi.h
const TCHAR* g_path{ "Assets" };//path to directory to watch (based on where exe is located)
BYTE g_fileNotifyInfo[BUFFER_SIZE];//buffer to store file changes
DWORD g_bytesChanged;//variable to store bytes changed in function ReadDirectoryChangesW/GetOverlappedResult
OVERLAPPED g_ovl;//contains info used in async call of ReadDirectoryChangesW

HANDLE g_directoryHandle;//the handle to the directory to be monitored.
UINT16 g_filters = FILE_NOTIFY_CHANGE_DIR_NAME | FILE_NOTIFY_CHANGE_FILE_NAME | FILE_NOTIFY_CHANGE_LAST_WRITE;//filters of what to monitor Note* think of a way to prevent multiple calls at the same time like file name change might call lastwrite/namechange

DragDrop dragDrop;

void AssetManager::Init()
{
	LoadAssets();//Load assets in directory

	//Event to watch changes
	g_ovl.hEvent = ::CreateEvent(NULL, FALSE, FALSE, NULL);

	//create handle by opening directory
	g_directoryHandle = CreateFile(
		g_path, // pointer to the directory containing the tex files
		FILE_LIST_DIRECTORY | GENERIC_READ,                // access (read-write) mode
		FILE_SHARE_READ | FILE_SHARE_DELETE | FILE_SHARE_WRITE,  // share mode
		NULL, // security descriptor
		OPEN_EXISTING, // how to create
		FILE_FLAG_BACKUP_SEMANTICS | FILE_FLAG_OVERLAPPED, // file attributes
		NULL);

	if (g_directoryHandle == INVALID_HANDLE_VALUE || g_directoryHandle == NULL)//failed to create handle
	{
#if _ENGINE
		DebugLogger::GetInstance()->Log("Failed to create directory watcher\n");
#endif // _ENGINE
		return;
	}

#if _ENGINE
	MonitorDirectory();//Start to monitor directory
#endif // _ENGINE
}

void AssetManager::Load()
{
	dragDrop.Init();
	//This is done here because GraphicsSystem need to be Init first  but it relies on WindowsManager which relies on me to load window size
	//Anyways there shouldnt be anybody getting Texture data before the init of AssetManager anyways
	mInInit = false;
	AssetMap* map = GetAssetMap("png");
	for (auto itr : *map)
	{
		Texture::CreateImageAsset(itr.second->fullPath, itr.second->data);
	}

	map = GetAssetMap("vert");
	for (auto itr : *map)
	{
		std::string estimatedFragPath = itr.second->fullPath;
		size_t splitPos = estimatedFragPath.find_last_of('.');
		estimatedFragPath = estimatedFragPath.substr(0, splitPos);
		estimatedFragPath += ".frag";
		std::string bothFilePath = itr.second->fullPath + "|" + estimatedFragPath;
		if (!mInInit)
			Shader::CreateShaderAsset(bothFilePath, itr.second->data);
	}
}

void AssetManager::LoadAssets()
{
	//loop through directory and load accordingly
	for (const auto& dirEntry : std::filesystem::recursive_directory_iterator(g_path))
	{
		//get file extension
		std::string name;
		std::string extension = "folder";
		if (!ParseFilePath(dirEntry.path().string(), name, extension))
			continue;
		//Save asset into map
		StoreAsset(dirEntry.path().string(), name, extension);
	}
}

void AssetManager::StoreAsset(const std::string& _fullPath, const std::string& _name, const std::string& _extension)
{

	if (!std::filesystem::exists(_fullPath))
	{
#if _ENGINE
		DebugLogger::GetInstance()->Log("File doesnt exist: %s\n", _fullPath.c_str());
#endif // _ENGINE
		return;
	}
	AssetMap* map = GetAssetMap(_extension);

	if (map->contains(_name))
	{
#if _ENGINE
		DebugLogger::GetInstance()->Log("Asset with same name: %s already exist!\n", _name.c_str());
#endif // _ENGINE
		return;
	}

	if (_extension == "txt" || _extension == "prefab" || _extension == "scene")
	{
		AssetInfo* newInfo = new AssetInfo;
		if (!JSONManager::GetInstance()->CreateJSONAsset(_fullPath, newInfo->data))
		{
#if _ENGINE
			DebugLogger::GetInstance()->Log("Error creating new Document\n");
#endif // _ENGINE
		}
		newInfo->destructPtr = JSONManager::DeleteJSONAsset;
		newInfo->fullPath = _fullPath;
		map->emplace(_name, newInfo);
	}
    else if (_extension == "wav")
    {
        AssetInfo* newInfo = new AssetInfo;
        SoundManager::GetInstance()->CreateSoundAsset(_fullPath, newInfo->data);
        newInfo->fullPath = _fullPath;
        map->emplace(_name, newInfo);
    }
    else if (_extension == "ttf" || _extension == "otf")
    {
        AssetInfo* newInfo = new AssetInfo;
        TextManager::GetInstance()->CreateFontAsset(_fullPath, _name);
        newInfo->fullPath = _fullPath;
        map->emplace(_name, newInfo);
    }
	else if (_extension == "png" || _extension == "jpg")//we only store the path for now cause graphics manager needs to be init first
	{
		AssetInfo* newInfo = new AssetInfo;
		newInfo->fullPath = _fullPath;
		if (!mInInit)
			Texture::CreateImageAsset(_fullPath, newInfo->data);
		newInfo->destructPtr = Texture::DeleteImageAsset;
		map->emplace(_name, newInfo);
	}
	else if (_extension == "cs")
	{
		AssetInfo* newInfo = new AssetInfo;
		MonoManager::GetInstance()->CreateScriptAsset(_fullPath, newInfo->data);
		newInfo->destructPtr = MonoManager::DeleteScriptAsset;
		newInfo->data = new std::string(_name);
		newInfo->fullPath = _fullPath;
		map->emplace(_name, newInfo);
	}
	else if (_extension == "vert")
	{
		//find the paired frag file too
		std::string estimatedFragPath = _fullPath;
		size_t splitPos = estimatedFragPath.find_last_of('.');
		estimatedFragPath = estimatedFragPath.substr(0, splitPos);
		estimatedFragPath += ".frag";
		std::string bothFilePath = _fullPath + "|" + estimatedFragPath;
		AssetInfo* newInfo = new AssetInfo;
		if (!mInInit)
			Shader::CreateShaderAsset(bothFilePath, newInfo->data);
		newInfo->destructPtr = Shader::DeleteShaderAsset;
		newInfo->fullPath = _fullPath;
		map->emplace(_name, newInfo);
	}
#if _ENGINE
	DebugLogger::GetInstance()->Log("Added asset: %s with extension: <%s>\n", _name.c_str(), _extension.c_str());
#endif // _ENGINE
}

void ParseDirectoryEntry(const std::filesystem::directory_entry& _entry, AssetManager::DirectoryInfo& _folderInfo, AssetManager::DirectoryInfo& _fileInfo, AssetManager::DIRECTORY_ITEM_TYPE _type)
{
	AssetManager::DirectoryItem newItem;
	newItem.fullPath = _entry.path().string();

	size_t splitPos;
	std::string file;
	splitPos = newItem.fullPath.find_last_of('\\');
	if (splitPos != std::string::npos)
		file = newItem.fullPath.substr(splitPos + 1, newItem.fullPath.size());

	splitPos = file.find_last_of('.');
	newItem.fileName = file.substr(0, splitPos);

	newItem.type = AssetManager::NONE;
	splitPos = file.find_last_of('.');
	if (splitPos == std::string::npos)//no type so is folder
	{
		if (_type == AssetManager::NONE || _type == AssetManager::FOLDER)
		{
			newItem.type = AssetManager::FOLDER;
			_folderInfo.emplace_back(newItem);
		}
	}
	else
	{
		std::string fileType = file.substr(splitPos + 1, file.size());
		newItem.extension = fileType;
		if ((_type == AssetManager::NONE || _type == AssetManager::AUDIO) && (fileType == "wav"))
		{
			newItem.type = AssetManager::AUDIO;
		}
		else if ((_type == AssetManager::NONE || _type == AssetManager::JSON) && (fileType == "txt" || fileType == "json"))
		{
			newItem.type = AssetManager::JSON;
		}
		else if ((_type == AssetManager::NONE || _type == AssetManager::PREFAB) && (fileType == "prefab"))
		{
			newItem.type = AssetManager::PREFAB;
		}
		else if ((_type == AssetManager::NONE || _type == AssetManager::SCENE) && (fileType == "scene"))
		{
			newItem.type = AssetManager::SCENE;
		}
		else if ((_type == AssetManager::NONE || _type == AssetManager::FONT) && (fileType == "ttf" || fileType == "otf"))
		{
			newItem.type = AssetManager::FONT;
		}
		else if ((_type == AssetManager::NONE || _type == AssetManager::SPRITE) && (fileType == "png" || fileType == "jpg"))
		{
			newItem.type = AssetManager::SPRITE;
		}
		else if ((_type == AssetManager::NONE || _type == AssetManager::CSSCRIPT) && (fileType == "cs"))
		{
			newItem.type = AssetManager::CSSCRIPT;
		}
		else
		{
			return;//Unsupported type so we skip
		}
		_fileInfo.emplace_back(newItem);
	}

}

AssetManager::DirectoryInfo AssetManager::GetDirectoryInfo(std::string _fullPath, DIRECTORY_ITEM_TYPE _type, bool _isRecursive)
{
	DirectoryInfo folderInfo, fileInfo;
	size_t splitPos = _fullPath.find_last_of('.');
	if(splitPos != std::string::npos)//not a directory since have extension
		return folderInfo;

	if (_isRecursive)//Default only get current layer
	{
		for (auto const& dir_entry : std::filesystem::recursive_directory_iterator{ _fullPath })
		{
			ParseDirectoryEntry(dir_entry, folderInfo, fileInfo, _type);
		}
	}
	else
	{
		for (auto const& dir_entry : std::filesystem::directory_iterator{ _fullPath })
		{
			ParseDirectoryEntry(dir_entry, folderInfo, fileInfo, _type);
		}
	}
	folderInfo.insert(folderInfo.end(), fileInfo.begin(), fileInfo.end());
	return folderInfo;
}


std::string AssetManager::GetCurrentPath()
{
	return std::string(g_path);
}

bool AssetManager::RenameMyFile(std::string _oldPath, std::string _newName)
{
	SHFILEOPSTRUCT op{ 0 };
	op.wFunc = FO_RENAME;
	std::string fileName, extension;
	ParseFilePath(_oldPath, fileName, extension);
	_newName += "." + extension;
	_oldPath.push_back('\0');//double null terminated
	_newName.push_back('\0');//double null terminated
	op.pFrom = _oldPath.c_str();
	op.pTo = _newName.c_str();
	op.fFlags = FOF_ALLOWUNDO;// | FOF_SILENT | FOF_NOCONFIRMATION;
	return !SHFileOperation(&op);
}

bool AssetManager::DeleteMyFile(std::string _filePath)
{
	SHFILEOPSTRUCT op{ 0 };
	op.wFunc = FO_DELETE;
	_filePath = std::filesystem::current_path().string() + "\\" + _filePath;
	_filePath.push_back('\0');//double null terminated
	op.pFrom = _filePath.c_str();
	op.fFlags = FOF_ALLOWUNDO;// | FOF_SILENT | FOF_NOCONFIRMATION;//move to recycleBin(FOF_ALLOWUNDO)
	return !SHFileOperation(&op);
}

bool AssetManager::MoveMyFile(std::string _oldPath, std::string _newPath)
{
	SHFILEOPSTRUCT op{ 0 };
	op.wFunc = FO_MOVE;
	_oldPath.push_back('\0');//double null terminated
	_newPath.push_back('\0');//double null terminated
	op.pFrom = _oldPath.c_str();
	op.pTo = _newPath.c_str();
	op.fFlags = FOF_ALLOWUNDO;// | FOF_SILENT | FOF_NOCONFIRMATION;//move to recycleBin(FOF_ALLOWUNDO)
	return !SHFileOperation(&op);
}

bool AssetManager::CopyMyFile(std::string _oldPath, std::string _newPath)
{
	SHFILEOPSTRUCT op{ 0 };
	op.wFunc = FO_COPY;
	_oldPath.push_back('\0');//double null terminated
	_newPath.push_back('\0');//double null terminated
	op.pFrom = _oldPath.c_str();
	op.pTo = _newPath.c_str();
	op.fFlags = FOF_ALLOWUNDO;// | FOF_SILENT | FOF_NOCONFIRMATION;//move to recycleBin(FOF_ALLOWUNDO)
	return !SHFileOperation(&op);
}




//do we want to check against journal instead?
void AssetManager::MonitorDirectory()
{
	if (!ReadDirectoryChangesW(g_directoryHandle, g_fileNotifyInfo, BUFFER_SIZE, true, g_filters, &g_bytesChanged, &g_ovl, NULL))
	{
#if _ENGINE
		DWORD error = GetLastError();
		//std::wcout << "Directory monitor error : " << error << std::endl;
		DebugLogger::GetInstance()->Log("Directory monitor error : %s\n", std::to_string(error).c_str());
#endif // _ENGINE
	}
}

void AssetManager::Update()
{
	if (GetOverlappedResult(g_directoryHandle, &g_ovl, &g_bytesChanged, false))//get if changes made
	{
#if _ENGINE
		//DebugLogger::GetInstance()->Log("---------FILE UPDATE!----------\n");
#endif // _ENGINE
		BYTE* fileEvent = g_fileNotifyInfo;

		while (true)
		{
			FILE_NOTIFY_INFORMATION* info = reinterpret_cast<FILE_NOTIFY_INFORMATION*>(fileEvent);
			std::wstring ws(info->FileName);
			DWORD length = info->FileNameLength;

			//Convert wstring to string
			std::string str = ToString(ws.substr(0, length / 2));

			//std::wcout << "FILEINFO: " << info->FileName << " with size: " << (length / 2) << std::endl;
#if _ENGINE
			//DebugLogger::GetInstance()->Log("FILEINFO: %ls with size: %s\n", info->FileName, std::to_string(length / 2).c_str());
#endif // _ENGINE
			UpdateAsset(str, info->Action);
			if (info->NextEntryOffset != 0)
			{
				fileEvent += info->NextEntryOffset;
			}
			else
				break;
		}
		MonitorDirectory();
	}
}

std::string AssetManager::ExtensionToTypeName(const std::string& _ext)
{
	if (_ext == "txt" || _ext == "json" || _ext == "prefab" || _ext == "scene")
	{
		return typeid(JSONAsset).name();
	}
	else if (_ext == "wav")
	{
		return typeid(SoundAsset).name();
	}
	else if (_ext == "ttf" || _ext == "otf")
	{
		return typeid(FontAsset).name();
	}
	else if (_ext == "png")
	{
		return typeid(ImageAsset).name();
	}
	else if (_ext == "cs")
	{
		return typeid(CSSCRIPTAsset).name();
	}
	else if (_ext == "vert")
	{
		return typeid(ShaderAsset).name();
	}

	return "";
}

AssetManager::AssetMap* AssetManager::GetAssetMap(const std::string& _ext)
{
	std::string mapName = ExtensionToTypeName(_ext);

	//Check if already created inside mAllAssetsContainer
	AllAssetMapIterator map = mAllAssetsContainer.find(mapName);
	if (map == mAllAssetsContainer.end())
	{
		//Create map
		mAllAssetsContainer.emplace(mapName, AssetMap{});
		map = mAllAssetsContainer.find(mapName);
	}

	return &(*map).second;
}

void AssetManager::UpdateAsset(const std::string& _file, unsigned long _action)
{
	if (prevFileChange == _file && _action == FILE_ACTION_MODIFIED)//To prevent double triggering when new file created (new file = add + modified)
		return;
	prevFileChange = _file;

	std::string name, extension;
	std::string filePath = std::string{ g_path } + '\\' + _file;
	static std::string previousFilePath;
	ParseFilePath(filePath, name, extension);
	if (name == "")
		return;
	switch (_action) {
	case FILE_ACTION_ADDED: {
		StoreAsset(filePath, name, extension);
	} break;

	case FILE_ACTION_REMOVED: {
		RemoveAsset(name, extension);
	} break;

	case FILE_ACTION_MODIFIED: {
		if (previousFilePath == filePath)
		{
			previousFilePath = "";
			break;
		}
#if _ENGINE
		DebugLogger::GetInstance()->Log("MODIFIED!! %s\n", filePath.c_str());
#endif // _ENGINE
		RemoveAsset(name, extension);
		StoreAsset(filePath, name, extension);
		previousFilePath = filePath;
	} break;

	case FILE_ACTION_RENAMED_OLD_NAME: {
#if _ENGINE
		DebugLogger::GetInstance()->Log("OLD NAME!! %s\n", filePath.c_str());
#endif // _ENGINE
		RemoveAsset(name, extension);
	} break;

	case FILE_ACTION_RENAMED_NEW_NAME: {
#if _ENGINE
		DebugLogger::GetInstance()->Log("RENAMED!! %s\n", filePath.c_str());
#endif // _ENGINE
		StoreAsset(filePath, name, extension);
	} break;

	default: {
#if _ENGINE
		DebugLogger::GetInstance()->Log("UNKNOWN FILE ACTION!! %s\n", filePath.c_str());
#endif // _ENGINE
	} break;
	}

}

void AssetManager::RemoveAsset(const std::string& _name, const std::string& _extension)
{
	if (!mAllAssetsContainer[ExtensionToTypeName(_extension)].contains(_name))//asset doesnt exist in map
		return;
	if (mAllAssetsContainer[ExtensionToTypeName(_extension)][_name]->destructPtr)//if is special _type need to be destroyed
	{
		mAllAssetsContainer[ExtensionToTypeName(_extension)][_name]->destructPtr(mAllAssetsContainer[ExtensionToTypeName(_extension)][_name]->data);
	}
	else
		delete mAllAssetsContainer[ExtensionToTypeName(_extension)][_name]->data;//generic types

	delete (mAllAssetsContainer[ExtensionToTypeName(_extension)][_name]);
	mAllAssetsContainer[ExtensionToTypeName(_extension)].erase(_name);
#if _ENGINE
	DebugLogger::GetInstance()->Log("Removed asset: <%s> with extension: <%s>\n", _name.c_str(), _extension.c_str());
#endif // _ENGINE
}

bool AssetManager::ParseFilePath(const std::string& _fullPath, std::string& fileName, std::string& extension)
{
	size_t offset = _fullPath.find_last_of('.');
	if (offset == std::string::npos)//no file extension
	{
#if _ENGINE
		DebugLogger::GetInstance()->Log("Not a valid extension <%s>\n", _fullPath.c_str());
#endif // _ENGINE
		return false;
	}

	extension = _fullPath.substr(offset + 1, _fullPath.size() - offset);
	if (mSupportedExtensions.find(extension) == std::string::npos)
	{
		if (extension != "ini" && extension != "frag")
		{
#if _ENGINE
			DebugLogger::GetInstance()->Log("Extension <%s> not supported\n", extension.c_str());
#endif // _ENGINE
			std::string mUnspottedString = "The file " + _fullPath + " is not supported";
			mUnsupportedExtensionLog.emplace_back(mUnspottedString);
		}
		return false;
	}

	fileName = _fullPath.substr(0, _fullPath.find_last_of('.'));
	size_t nameStart = fileName.find_last_of('\\');
	if (nameStart == std::string::npos)//file path has no name
	{
#if _ENGINE
		DebugLogger::GetInstance()->Log("Not a valid name <%s>\n", _fullPath.c_str());
#endif // _ENGINE
		return false;
	}
	fileName = fileName.substr(nameStart + 1);

	return true;
}



void AssetManager::Unload()
{
	//clear assets
	for (AllAssetMap::value_type it : mAllAssetsContainer)
	{
		for (AssetMap::value_type it2 : it.second)
		{
			if (it2.second->destructPtr)//if is special _type need to be destroyed
			{
				it2.second->destructPtr((it2.second)->data);
			}
			else
				delete (it2.second)->data;//generic types

			delete (it2.second);
		}
	}

	dragDrop.Unload();
	Destroy();
}

bool AssetManager::DoesFileExist(std::string _filePath)
{
	DWORD attrib = GetFileAttributes(_filePath.c_str());

	return (attrib != INVALID_FILE_ATTRIBUTES && !(attrib & FILE_ATTRIBUTE_DIRECTORY));
}


AssetManager::AssetManager()
{
}

AssetManager::~AssetManager()
{
}

std::string AssetManager::ToString(std::wstring _wstr)
{
	std::vector<char> buf(_wstr.size());
	std::use_facet<std::ctype<wchar_t>>(std::locale{}).narrow(_wstr.data(), _wstr.data() + _wstr.size(), '?', buf.data());

    return std::string(buf.data(), buf.size());
}

bool AssetManager::GetDragDrop()
{
	return dragDrop.IsDragDrop();
}

bool AssetManager::CopyFromExternal(std::string _newPath)
{
	for (std::string& file : mExternalFiles)
	{
		std::string name, extension;
		if (!ParseFilePath(file, name, extension))
			continue;
		CopyMyFile(file, _newPath);
	}
	mExternalFiles.clear();
	return true;
}

void AssetManager::SaveExternalFiles(std::string path)
{
	mExternalFiles.push_back(path);
}