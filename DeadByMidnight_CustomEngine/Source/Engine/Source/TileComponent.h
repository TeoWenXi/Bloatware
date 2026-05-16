/*!
All content © 2023 DigiPen Institute of Technology Singapore, all rights reserved.
@file       TileComponent.h
@author     SIA Xin Tian (100%)
@co-author

----------------------------------------------------------------------------

@course     CSD2401F23
@section    CSD2401F23-A
@project    CSD2401F23 Software Engineering Project 3

@brief      Contains functions to update grid collision
*//*______________________________________________________________________*/

#ifndef MAP_H
#define MAP_H

#include "ComponentBase.h"
#include "Transform.h"
#include <vector>

class TileComponent : public ComponentBase
{
public:
	enum OBJECT
	{
		EMPTY,				//0
		TILE_WALL,			//1
		TILE_PLATFORM,		//2
		PLAYER,				//3
		WIND_TUNNEL_UP,		//4
		WIND_TUNNEL_DOWN,	//5
		WIND_TUNNEL_LEFT,	//6
		WIND_TUNNEL_RIGHT,	//7
		NUM_OBJECTS
	};

	enum COLLIDED_DIRECTION
	{
		BOTTOM,
		TOP,
		LEFT,
		RIGHT,
		NONE
	};

private:
	std::vector<std::string> mObjectEnumNames;

public:
	//Flags
	static const unsigned int	FLAG_ACTIVE = 0x00000001;
	static const unsigned int	FLAG_VISIBLE = 0x00000002;
	static const unsigned int	FLAG_NON_COLLIDABLE = 0x00000004;

	//Collision flags
	static const unsigned int	COLLISION_LEFT = 0x00000001;	//0001
	static const unsigned int	COLLISION_RIGHT = 0x00000002;	//0010
	static const unsigned int	COLLISION_TOP = 0x00000004;		//0100
	static const unsigned int	COLLISION_BOTTOM = 0x00000008;	//1000

	static Vector3 mMaxBound;
	static Vector3 mMinBound;
	static int mTileSize;
	static int mMapWidth;
	static int mMapHeight;
	static const int mWindTunnelForceX = 120;
	static const int mWindTunnelForceY = 120;
	static std::vector<std::vector<int>> mMapData;
	static std::vector<std::vector<int>> mBinaryMap;
	static bool mEnableScreenGrid;
	static bool mEnableCollisionGrid;
	static bool mModelCreated;
	static bool mLoadMap;

	//Per component stuff
	int mFlag;
	int mGridCollisionFlag;
	OBJECT mObjectType;

public:

	/*!
	 * @brief 
	 * Init function for Tile Component
	*/
	void Init();

	/*!
	 * @brief 
	 * Update function for Tile Component
	*/
	void Update();

	/*!
	 * @brief 
	 * Exit function for Tile Component
	*/
	void Exit();

	/*!
	 * @brief 
	 * Load function for Tile Component
	*/
	void Load();

	/*!
	 * @brief 
	 * Save function for Tile Component
	*/
	void Save();

	/*!
	 * @brief 
	 * Function to display properites
	*/
	void DisplayProperties();

	/*!
	 * @brief 
	 * Gets the cell value where the position is at
	 * @param X 
	 * X position
	 * @param Y 
	 * Y position
	 * @return 
	 * Cell value
	*/
	static int GetCellValue(int _X, int _Y);
	
	/*!
	 * @brief
	 * Check for collision value of cell
	 * @param _X 
	 * X value of cell
	 * @param _Y 
	 * Y value of cell
	 * @return 
	 * Collision value and coordinates as a pair
	*/
	static std::pair<int, std::pair<int, int>> GetCellValue_YX(int _X, int _Y);

	/*!
	 * @brief Find map value from mMapData
	 * @param _X 
	 * X coordinate
	 * @param _Y 
	 * Y coordinate
	 * @return 
	 * Map value from mMapData
	*/
	static int GetMapValue(float _X, float _Y);

	/*!
	 * @brief 
	 * Snaps Coordinate back to cell
	 * @param Coordinate 
	 * Position of the object
	*/
	static void SnapToCell(float* _coordinate);
	
	/*!
	 * @brief 
	 * Snaps the player back to cell
	 * @param _coordinate 
	 * Coordinate of player
	 * @param xform 
	 * Transform of player
	 * @param _tile 
	 * Size of tile
	 * @param side 
	 * Direction of collided
	 * @param denialThreshold 
	 * Checker to see if the snapped position of the player is too far
	*/
	static void SnapPlayerToCell(float* _coordinate, Transform* _xform, float _tile, COLLIDED_DIRECTION _side, float _denialThreshold = FLT_MAX);

	/*!
	 * @brief 
	 * Get the X or Y value of player based on the direction it collided in
	 * @param _coord 
	 * Coordinate of player
	 * @param xform 
	 * Transform of player
	 * @param side 
	 * Collided side
	 * @return 
	 * X or Y value of player
	*/
	static float GetCoord(float _coord, Transform* _xform, COLLIDED_DIRECTION _side);

	/*!
	 * @brief 
	 * Find the larger of the vectors and store it in lhs
	 * @param lhs 
	 * First vector to compare values with
	 * @param rhs 
	 * Second vector to compare values with
	*/
	static void MaxValue(Vector3& _lhs, Vector3 _rhs);

	/*!
	 * @brief 
	 * Find the smaller of the vectors and store it in lhs
	 * @param lhs 
	 * First vector to compare values with
	 * @param rhs 
	 * Second vector to compare values with
	*/
	static void MinValue(Vector3& _lhs, Vector3 _rhs);

	/*!
	 * @brief 
	 * Load tile data
	 * @return 
	 * To return 1 if successful and 0 if not successful
	*/
	static int LoadTileData();

	/*!
	 * @brief 
	 * oeprator<<
	 * @param os 
	 * Output stream object
	 * @param _col 
	 * Variable to print out
	 * @return 
	 * Output stream object
	*/
	friend std::ostream& operator<< (std::ostream& _os, const TileComponent& _col);
};


#endif