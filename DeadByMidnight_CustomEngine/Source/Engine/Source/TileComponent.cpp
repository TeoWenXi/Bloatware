/*!
All content © 2023 DigiPen Institute of Technology Singapore, all rights reserved.
@file       TileComponent.cpp
@author     SIA Xin Tian (100%)
@co-author

----------------------------------------------------------------------------

@course     CSD2401F23
@section    CSD2401F23-A
@project    CSD2401F23 Software Engineering Project 3

@brief      Contains functions to update grid collision
*//*______________________________________________________________________*/

#include "GraphicsSystem.h"

#if _ENGINE
#include <imgui.h>
#endif // _ENGINE

//Components
#include "ComponentsHeader.h"
#include "Transform.h"

Vector3 TileComponent::mMaxBound = { 0,0,0 };
Vector3 TileComponent::mMinBound = { 0,0,0 };
int TileComponent::mTileSize = 10;
int TileComponent::mMapWidth = 0;
int TileComponent::mMapHeight = 0;
std::vector<std::vector<int>> TileComponent::mMapData{};
std::vector<std::vector<int>> TileComponent::mBinaryMap{};
bool TileComponent::mEnableScreenGrid = false;
bool TileComponent::mEnableCollisionGrid = false;
bool TileComponent::mModelCreated = false;
int mGridCollisionFlag = 0;
TileComponent::OBJECT mObjectType = TileComponent::OBJECT::EMPTY;

#if _ENGINE
	bool TileComponent::mLoadMap = false;
#elif _BLOATWARE
bool TileComponent::mLoadMap = true;
#endif

void TileComponent::Init()
{
	for (int i = 0; i < NUM_OBJECTS; ++i)
	{
		switch (OBJECT(i))
		{
		case 0: mObjectEnumNames.push_back("Empty");
			break;
		case 1: mObjectEnumNames.push_back("Wall");
			break;
		case 2: mObjectEnumNames.push_back("Platform");
			break;
		case 3: mObjectEnumNames.push_back("Player");
			break;
		case 4: mObjectEnumNames.push_back("Wind Tunnel Up");
			break;
		case 5: mObjectEnumNames.push_back("Wind Tunnel Down");
			break;
		case 6: mObjectEnumNames.push_back("Wind Tunnel Left");
			break;
		case 7: mObjectEnumNames.push_back("Wind Tunnel Right");
			break;
		}
	}
}

void TileComponent::Update()
{

}

void TileComponent::Exit()
{
	if (mBinaryMap.size() > 0)
		mBinaryMap.clear();
	if (mMapData.size() > 0)
		mMapData.clear();

	mMapHeight = 0;
	mMapWidth = 0;
}

void TileComponent::Load(void)
{
	if (mLoadMap)
	{
		mTileSize = JSONManager::GetInstance()->GetInt("TileSize");
		mMapWidth = JSONManager::GetInstance()->GetInt("MapWidth");
		mMapHeight = JSONManager::GetInstance()->GetInt("MapHeight");
	}
	
	mObjectType = OBJECT(JSONManager::GetInstance()->GetInt("ObjectType"));
	mFlag = JSONManager::GetInstance()->GetInt("Flag");
	mGridCollisionFlag = JSONManager::GetInstance()->GetInt("GridCollisionFlag");
}

void TileComponent::Save(void)
{
	JSONManager::GetInstance()->Save("Component", "TileComponent");
	JSONManager::GetInstance()->Save("TileSize", mTileSize);
	JSONManager::GetInstance()->Save("MapWidth", mMapWidth);
	JSONManager::GetInstance()->Save("MapHeight", mMapHeight);
	JSONManager::GetInstance()->Save("ObjectType", mObjectType);
	JSONManager::GetInstance()->Save("Flag", mFlag);
	JSONManager::GetInstance()->Save("GridCollisionFlag", mGridCollisionFlag);
}

void TileComponent::DisplayProperties()
{
#if _ENGINE
	//Display object type selector
	ImGui::Text("Grid Object Type");
	ImGui::SameLine();
	ImGui::SetNextItemWidth(-1);

	if (mObjectEnumNames.size() == 0)
		Init();

	if(ImGui::BeginCombo("##GridObjectType", mObjectEnumNames[mObjectType].c_str(), 0))
	{
		for (int n = 0; n < mObjectEnumNames.size(); n++)
		{
			const bool is_selected = (mObjectType == n);
			if (ImGui::Selectable(mObjectEnumNames[n].c_str(), is_selected))
				mObjectType = OBJECT(n);

			if (is_selected)
				ImGui::SetItemDefaultFocus();
		}
		ImGui::EndCombo();
	}
#endif // _ENGINE
}

/*!
 * @brief
 * Returns collision value from mBinaryMap
*/
int TileComponent::GetCellValue(int _X, int _Y)
{
	//checking if coordinates exist within the bounds of the grid
	if (mMinBound.mX <= _X && _X <= mMaxBound.mX && mMinBound.mY <= _Y && _Y <= mMaxBound.mY) {

		//to get binary coordinates
		int Xcell = static_cast<int>((_X - mMinBound.mX) / TileComponent::mTileSize);
		int Ycell = static_cast<int>((_Y - mMinBound.mY) / TileComponent::mTileSize);

		if ((Xcell < mMapWidth) && (Ycell < mMapHeight) && (Xcell >= 0) && (Ycell >= 0)) {
			return mBinaryMap[Ycell][Xcell];
		}
	}
	return 0;
}

std::pair<int, std::pair<int, int>> TileComponent::GetCellValue_YX(int _X, int _Y)
{
	//checking if coordinates exist within the bounds of the grid
	if (mMinBound.mX <= _X && _X <= mMaxBound.mX && mMinBound.mY <= _Y && _Y <= mMaxBound.mY) {

		//to get binary coordinates
		int Xcell = static_cast<int>((_X - mMinBound.mX) / TileComponent::mTileSize);
		int Ycell = static_cast<int>((_Y - mMinBound.mY) / TileComponent::mTileSize);

		if ((Xcell < mMapWidth) && (Ycell < mMapHeight) && (Xcell >= 0) && (Ycell >= 0)) {
			return std::pair<int, std::pair<int, int>>(mBinaryMap[Ycell][Xcell], std::pair<int, int>(Xcell, Ycell));
		}
	}
	return std::pair<int, std::pair<int, int>>(0, std::pair<int, int>(0, 0));
}

/*!
 * @brief
 * Returns map value from mMapData
*/
int TileComponent::GetMapValue(float _X, float _Y)
{
	//checking if coordinates exist within the bounds of the grid
	if (mMinBound.mX <= _X && _X <= mMaxBound.mX && mMinBound.mY <= _Y && _Y <= mMaxBound.mY) {

		//to get map coordinates
		int Xcell = static_cast<int>((_X - mMinBound.mX) / TileComponent::mTileSize);
		int Ycell = static_cast<int>((_Y - mMinBound.mY) / TileComponent::mTileSize);

		if ((Xcell < mMapWidth) && (Ycell < mMapHeight) && (Xcell >= 0) && (Ycell >= 0)) {
			return mMapData[Ycell][Xcell];
		}
	}
	return 0;
}

/*!
 * @brief
 * Snaps entity back to center of cell
*/
void TileComponent::SnapToCell(float* _coordinate)
{
	if (*_coordinate > 0)
		*_coordinate = static_cast<int>(*_coordinate / TileComponent::mTileSize) * TileComponent::mTileSize + TileComponent::mTileSize / 2.f;
	
	else if (*_coordinate < 0)
		*_coordinate = static_cast<int>(*_coordinate / TileComponent::mTileSize) * TileComponent::mTileSize - TileComponent::mTileSize / 2.f;
	
}

void TileComponent::SnapPlayerToCell(float* _coordinate, Transform* _xform, float _tile, COLLIDED_DIRECTION _side, float _denialThreshold)
{
	float objScale = _xform->mScale.mX;
	float newCoord = *_coordinate;
	switch (_side)
	{
	case BOTTOM:
		newCoord = _tile + mTileSize / 2.f + objScale / 2.f;
		break;
	case TOP:
		newCoord = _tile - mTileSize / 2.f - objScale / 2.f;
		break;
	case LEFT:
		newCoord = _tile + mTileSize / 2.f + objScale / 2.f;
		break;
	case RIGHT:
		newCoord = _tile - mTileSize / 2.f - objScale / 2.f;
		break;
	}
	if (fabsf(*_coordinate - newCoord) < _denialThreshold)
		*_coordinate = newCoord;
}

float TileComponent::GetCoord(float _coord, Transform* _xform, COLLIDED_DIRECTION _side)
{
	float coord = 0;

	switch (_side)
	{
		case BOTTOM://y
		{
			float objscale = _xform->mScale.mY;
			coord = TileComponent::mMinBound.mY + (float)TileComponent::mTileSize * 0.5f;
			coord += (float)TileComponent::mTileSize * static_cast<int>(((int)(_coord - objscale / 2 - 1) - mMinBound.mY) / TileComponent::mTileSize);
			break;
		}case TOP://y
		{
			float objscale = _xform->mScale.mY;
			coord = TileComponent::mMinBound.mY + (float)TileComponent::mTileSize * 0.5f;
			coord += (float)TileComponent::mTileSize * static_cast<int>(((int)(_coord + objscale / 2 - 1) - mMinBound.mY) / TileComponent::mTileSize);
			break;
		}case LEFT://x
		{
			float objscale = _xform->mScale.mX;
			coord = TileComponent::mMinBound.mX + (float)TileComponent::mTileSize * 0.5f;
			coord += (float)TileComponent::mTileSize * static_cast<int>(((int)(_coord - objscale / 2) - mMinBound.mX) / TileComponent::mTileSize);
			break;
		}case RIGHT://x
		{
			float objscale = _xform->mScale.mX;
			coord = TileComponent::mMinBound.mX + (float)TileComponent::mTileSize * 0.5f;
			coord += (float)TileComponent::mTileSize * static_cast<int>(((int)(_coord + objscale / 2) - mMinBound.mX) / TileComponent::mTileSize);
			break;
		}

	}

	return coord;
}

/*!
 * @brief
 * Saves the maximum value of vector
*/
void TileComponent::MaxValue(Vector3& _lhs, Vector3 _rhs)
{
	if (_lhs.mX < _rhs.mX)
		_lhs.mX = _rhs.mX;
	if (_lhs.mY < _rhs.mY)
		_lhs.mY = _rhs.mY;
}

/*!
 * @brief
 * Saves the minimum value of vector
*/
void TileComponent::MinValue(Vector3& _lhs, Vector3 _rhs)
{
	if (_lhs.mX > _rhs.mX)
		_lhs.mX = _rhs.mX;
	if (_lhs.mY > _rhs.mY)
		_lhs.mY = _rhs.mY;
}

/*!
 * @brief
 * Loads tile data and saves into variables
*/
int TileComponent::LoadTileData()
{
	//Delete previous collision grid
	if (mBinaryMap.size() > 0)
		mBinaryMap.clear();
	if (mMapData.size() > 0)
		mMapData.clear();

	mMapHeight = 0;
	mMapWidth = 0;

	//Generate grid
	mMaxBound = Vector3(-FLT_MAX, -FLT_MAX, -FLT_MAX);
	mMinBound = Vector3(FLT_MAX, FLT_MAX, FLT_MAX);

	//looping through all entities to find their positions
	for (size_t i = 0; i < EntityManager::GetInstance()->mEntityList.size(); ++i)
	{
		Transform* xform2 = EntityManager::GetInstance()->mEntityList[i].GetComponent< Transform>();
 		Player* player = EntityManager::GetInstance()->mEntityList[i].GetComponent<Player>();
		TileComponent* tileComp = EntityManager::GetInstance()->mEntityList[i].GetComponent<TileComponent>();

		if (tileComp == nullptr || player != nullptr)
			continue;

		Vector3 pos = { xform2->GetPosition().mX,
						xform2->GetPosition().mY,
						1 };

		//finding most extreme positions of entities for max and min bounds
		MinValue(mMinBound, pos);
		MaxValue(mMaxBound, pos);
	}

	//find max and min bounds
	mMinBound.mX -= static_cast<float>(TileComponent::mTileSize / 2);
	mMinBound.mY -= static_cast<float>(TileComponent::mTileSize / 2);
	mMaxBound.mX += static_cast<float>(TileComponent::mTileSize / 2);
	mMaxBound.mY += static_cast<float>(TileComponent::mTileSize / 2);
	
	//store width and height
	mMapWidth = static_cast<int>((mMaxBound.mX - mMinBound.mX) / mTileSize);
	mMapHeight = static_cast<int>((mMaxBound.mY - mMinBound.mY) / mTileSize);

	//allocating memory
	for (int i = 0; i < mMapHeight; ++i)
	{
		mMapData.push_back(std::vector<int>());
		mBinaryMap.push_back(std::vector<int>());
		for (int j = 0; j < mMapWidth; ++j)
		{
			mMapData[i].push_back(0);
			mBinaryMap[i].push_back(0);
		}
	}

	//Read Map Data
	for (size_t i = 0; i < EntityManager::GetInstance()->mEntityList.size(); ++i)
	{
		Transform* xform2 = EntityManager::GetInstance()->mEntityList[i].GetComponent<Transform>();
		Player* player = EntityManager::GetInstance()->mEntityList[i].GetComponent<Player>();
		TileComponent* tileComp = EntityManager::GetInstance()->mEntityList[i].GetComponent<TileComponent>();

		if (tileComp == nullptr || player != nullptr)
			continue;

		int posX = (int)((xform2->GetPosition().mX - mMinBound.mX - (static_cast<float>(TileComponent::mTileSize) / 2)) / mTileSize);
		int posY = (int)((xform2->GetPosition().mY - mMinBound.mY - (static_cast<float>(TileComponent::mTileSize) / 2)) / mTileSize);
		mMapData[posY][posX] = tileComp->mObjectType;

		if (EntityManager::GetInstance()->mEntityList[i].GetComponent<Renderer>())
		{
			EntityManager::GetInstance()->mEntityList[i].GetComponent<Renderer>()->mShaderName = "TileShader";
		}
	};

	if (mMapData.size() == 0)
		return 0;

	//store collision into mBinaryMap
	for (size_t i = 0; i < mMapHeight; i++)
	{
		for (size_t j = 0; j < mMapWidth; j++)
		{
			/*if (mMapData[i][j] == EMPTY) 
				mBinaryMap[i][j] = 0;
			else
				mBinaryMap[i][j] = 1;*/
			if (mMapData[i][j] == TILE_WALL || mMapData[i][j] == TILE_PLATFORM)
				mBinaryMap[i][j] = 1;
			else
				mBinaryMap[i][j] = 0;
		}
	}

	return 1;
}

std::ostream& operator<<(std::ostream& _os, const TileComponent& _col)
{
	_os << "mMaxBound: " << _col.mMaxBound << "\n";
	_os << "mMinBound: " << _col.mMinBound << "\n";
	_os << "MapWidth: " << _col.mMapWidth << "\n";
	_os << "MapHeight: " << _col.mMapHeight << "\n";

	_os << "MapData: \n";
	for (int i = 0; i < _col.mMapHeight; i++) 
	{
		for (int j = 0; j < _col.mMapWidth; j++) 
		{
			_os << _col.mMapData[i][j] << " ";
		}
		_os << "\n";
	}

	_os << "BinaryMap: \n";
	for (int i = 0; i < _col.mMapHeight; i++)
	{
		for (int j = 0; j < _col.mMapWidth; j++)
		{
			_os << _col.mBinaryMap[i][j] << " ";
		}
		_os << '\n';
	}

	return _os;
}
