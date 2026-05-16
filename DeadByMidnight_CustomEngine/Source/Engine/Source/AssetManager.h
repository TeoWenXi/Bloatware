/*!
All content © 2023 DigiPen Institute of Technology Singapore, all rights reserved.
@file       AssetManager.h
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

#ifndef ASSET_MANAGER_H
#define ASSET_MANAGER_H

#include "Singleton.h"
#include "Manager.h"
#include <string>
#include <unordered_map>
#include <vector>

#include "SoundManager.h"
#include "Texture.h"
#include "Shader.h"
#pragma warning(push, 0)
#include <document.h>
#pragma warning(pop)
#include <ft2build.h>
#include FT_FREETYPE_H

#if _ENGINE
#include "DebugLogger.h"
#endif // _ENGINE

//Class to store asset info
struct AssetInfo
{
    void* data{ nullptr };//pointer to data (needs to be casted to whatever type is required)
    void(*destructPtr)(void*&) { nullptr };//pointer to destroy function if data is special type
    std::string fullPath{};//full path to the file
};


class AssetManager : public Singleton <AssetManager>, public Manager
{
public:
	/*!
	 * @brief 
	 *	Inits the AssetManager.
	*/
	void Init();

	/*!
	 * @brief 
	 *	Loads the AssetManager.
	*/
	void Load();

	/*!
	 * @brief 
	 *	Updates the AssetManager.
	*/
	void Update();

	/*!
	 * @brief 
	 * Unloads the AssetManager.
	*/
	void Unload();

	/*!
	 * @brief 
	 *	Checks if the file exist at the given file path.
	 * @param _filePath 
	 *	The file path to check.
	 * @return 
	 *	True if it exist false otherwise.
	*/
	bool DoesFileExist(std::string _filePath);

	/*!
	 * @brief 
	 *	Template function to get asset based on the type required.
	 * @tparam T 
	 *	Type of the asset.
	 * @param name 
	 *	Name of the asset.
	 * @param asset 
	 *	Reference pointer to point to the asset.
	 * @return 
	 *	True if asset is found false otherwise.
	*/
	template<typename T>
	bool GetAsset(const std::string& _name, T*& _asset);

    /*!
     * @brief 
	 *	Gets the file path of the asset associated with the name.
     * @tparam T 
	 *	Type of the asset.
     * @param name 
	 *	Name of the asset.
     * @return 
    */
    template<typename T>
    std::string GetFilePath(const std::string& _name);

	/*!
	 * @brief 
	 *	Renames a file.
	 * @param oldpath
	 *	The path of the old file to be renamed.
	 * @param newname 
	 *	The new name of the file.
	 * @return 
	 *	True if rename successful false otherwise.
	*/
	bool RenameMyFile(std::string _oldpath, std::string _newname);

	/*!
	 * @brief
	 *	Deletes a file.
	 * @param filepath
	 *	The path of the file to be deleted.
	 * @return
	 *	True if delete successful false otherwise.
	*/
	bool DeleteMyFile(std::string _filePath);

	/*!
	 * @brief 
	 *	Moves a file from one location to another.
	 * @param oldPath 
	 *	The original path to move from.
	 * @param newPath 
	 *	The new path to move to.
	 * @return 
	 *	True if move successful false otherwise.
	*/
	bool MoveMyFile(std::string _oldPath, std::string _newPath);//oldPath contains file name newPath just contains destination

	/*!
	 * @brief
	 *	Copies a file from one location to another.
	 * @param oldPath
	 *	The original path to copy from.
	 * @param newPath
	 *	The new path to copy to.
	 * @return
	 *	True if copy successful false otherwise.
	*/
	bool CopyMyFile(std::string _oldPath, std::string _newPath);

	/*!
	 * @brief 
	 *	Gets the current path being tracked.(where our assets folder is)
	 * @return 
	 *	The relative path to the folder being tracked.
	*/
	std::string GetCurrentPath();

	std::string mSupportedExtensions{ "txt|prefab|scene|ttf|otf|wav|png|cs|vert" };//List of supported extensions to be checked against

	//Assets types (this will be pointed to by the void* in AssetInfo ie. AssetInfo.data = new JSONAsset
	using SoundAsset = std::pair<FMOD::Sound*, SoundManager::SOUND_TYPE>;
	using JSONAsset = rapidjson::Document;
	using FontAsset = FT_Face;
	using ImageAsset = Texture;
	using CSSCRIPTAsset = std::string;//we only need to keep track of the name
	using ShaderAsset = Shader;

	enum DIRECTORY_ITEM_TYPE//Enums of all the different types displayed
	{
		FOLDER,
		SPRITE,
		AUDIO,
		JSON,
		PREFAB,
		SCENE,
		FONT,
		CSSCRIPT,
		NONE
	};

	//Type to get directory
	struct DirectoryItem
	{
		std::string fullPath;
		std::string fileName;
		std::string extension;
		DIRECTORY_ITEM_TYPE type{ NONE };
	};
	using DirectoryInfo = std::vector<DirectoryItem>;

	//Call this if u want to get info of a folder, if second param is default(NONE) get all files in current level, else get all files recursively of that type
	/*!
	 * @brief 
	 *	Gets the full directory info (vector of items) of a path
	 * @param _fullPath 
	 *	Path to start from
	 * @param _type 
	 *	Type to filter results
	 * @param _isRecursive 
	 *	To get items in subfolder
	 * @return 
	 *	DirectoryInfo
	*/
	DirectoryInfo GetDirectoryInfo(std::string _fullPath, DIRECTORY_ITEM_TYPE _type = NONE, bool _isRecursive = false);

	std::vector<std::string> mUnsupportedExtensionLog;

	std::vector<std::string> mExternalFiles;

	/*!
	 * @brief 
	 *	Gets if external drag is in progress
	 * @return 
	 *	True if dragging | false otherwise
	*/
	bool GetDragDrop();

	/*!
	 * @brief 
	 *	Copies all files saved in mExternalFiles to the new path
	 * @param _newPath 
	 *	Path to copy to
	 * @return 
	 *	True if successful | false otherwise
	*/
	bool CopyFromExternal(std::string _newPath);

	/*!
	 * @brief 
	 *	Saves the full path to an external file so EditorMainScene
	 *	can tell us the final destination path
	 * @param path 
	 *	Full path to external file
	*/
	void SaveExternalFiles(std::string path);

private:
	using AssetMap = std::unordered_map <std::string, AssetInfo*>;
	using AllAssetMap = std::unordered_map < std::string, AssetMap>;
	using AllAssetMapIterator = AllAssetMap::iterator;

	//map of all assets --> name of extension : ptr to map of asset(name of asset : ptr to asset)
	AllAssetMap mAllAssetsContainer;

	/*!
	 * @brief 
	 *	Checks and see if there were any file changes in the monitored directory.
	*/
	void MonitorDirectory();

	/*!
	 * @brief 
	 *	Loops through the directory and load all relevant assets.
	*/
	void LoadAssets();

	/*!
	 * @brief 
	 *	Stores the asset into the AssetContainer.
	 * @param fullPath 
	 *	Full path of the asset.
	 * @param name 
	 *	Name of the asset.
	 * @param extension 
	 *	Extension of the asset.
	*/
	void StoreAsset(const std::string& _fullPath, const std::string& _name, const std::string& _extension);
	
	/*!
	 * @brief 
	 *	Converts an extension name into the matching asset type name.
	 * @param extension 
	 *	Extension to convert.
	 * @return 
	 *	The name of the matching asset.
	*/
	std::string ExtensionToTypeName(const std::string& _extension);

	/*!
	 * @brief 
	 *	Gets the matching asset based on the extension.
	 * @param extension 
	 *	Extension to find.
	 * @return 
	 *	The pointer to the map that stores the extension.
	*/
	AssetMap* GetAssetMap(const std::string& _extension);

	/*!
	 * @brief 
	 *	Updates the asset according to the type of change.
	 * @param file 
	 *	Name of the file.
	 * @param action 
	 *	Action according to FILE_NOTIFY_INFORMATION.
	*/
	void UpdateAsset(const std::string& _file, unsigned long _action);

	/*!
	 * @brief 
	 *	Removes an asset from the AssetMap.
	 * @param name 
	 *	Name of the asset.
	 * @param extension 
	 *	Extension of the asset.
	*/
	void RemoveAsset(const std::string& _name, const std::string& _extension);

	/*!
	 * @brief 
	 *	Splits the full path into filename and extension.
	 * @param fullPath 
	 *	The string to split.
	 * @param fileName 
	 *	The reference to the string to store the name of the file.
	 * @param extension 
	 *	The reference to the string to store the extension of the file.
	 * @return 
	 *	True if successful false otherwise.
	*/
	bool ParseFilePath(const std::string& _fullPath, std::string& _fileName, std::string& _extension);

    /*!
     * @brief 
	 *	Converts a wstring to a string.
     * @param wstr 
	 *	The wstring to convert.
     * @return 
	 *	The converted string.
    */
    std::string ToString(std::wstring _wstr);

	bool mInInit{true};//used for assets that loads in Load() instead;
	std::string prevFileChange;//Keep track of recent file changes

	/*!
	 * @brief
	 *	Default constructor for AssetManager.
	*/
	AssetManager();

	/*!
	 * @brief
	 *	Default destructor for AssetManager.
	*/
	~AssetManager();

	friend Singleton <AssetManager>;
};

/*!
 * @brief 
 *	Template function to get a particular asset stored in the AssetManager
 * @tparam T 
 *	Type of asset
 * @param _name 
 *	Name of asset file
 * @param _asset 
 *	Reference to a pointer to point to the asset
 * @return 
 *	True if found | false otherwise
*/
template<typename T>
inline bool AssetManager::GetAsset(const std::string& _name, T*& _asset)
{
	std::string type = typeid(T).name();
	if (!mAllAssetsContainer[type].contains(_name))
	{
#if _ENGINE
		//DebugLogger::GetInstance()->Log("Cannot find file: %s\n", _name.c_str());
#endif // _ENGINE
		return false;
	}
	_asset = static_cast<T*>(mAllAssetsContainer[type][_name]->data);
	return true;
}

/*!
 * @brief 
 *	Gets the filepath where the asset is stored
 * @tparam T 
 *	Type of the asset
 * @param _name 
 *	Name of the asset
 * @return 
 *	Filepath of the asset | "" if not found
*/
template<typename T>
inline std::string AssetManager::GetFilePath(const std::string& _name)
{
    std::string type = typeid(T).name();
    if (!mAllAssetsContainer[type].contains(_name))
    {
#if _ENGINE
		DebugLogger::GetInstance()->Log("Cannot find file: %s\n", _name.c_str());
#endif // _ENGINE
        return std::string();
    }
    return mAllAssetsContainer[type][_name]->fullPath;
}

#endif