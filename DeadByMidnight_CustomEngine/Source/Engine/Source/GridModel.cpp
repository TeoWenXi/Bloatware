/*!
All content © 2023 DigiPen Institute of Technology Singapore, all rights reserved.
@file       GridModel.h
@author     TEO Wen Xi (100%)
@co-author

----------------------------------------------------------------------------

@course     CSD2401F23
@section    CSD2401F23-A
@project    CSD2401F23 Software Engineering Project 3

@brief      GridModel class declarations to create and rendering a grid.
*//*______________________________________________________________________*/

#include "GridModel.h"
#include "TileComponent.h"
#include "GraphicsSystem.h"

/*!
 * @brief
 * Constructor for LineModel class.
*/
GridModel::GridModel()
	: mWidth{ 1 }, mDrawCount{ 0 }, mClr{1, 0, 0, 1}
{
}

/*!
 * @brief
 * Constructor for the LineModel class with user defined line width, start and end points and the
 * line color.
 * @param mWidth
 * Width of the line to render with.
 * @param mStart
 * Start position of the line to render from.
 * @param mEnd
 * End position of the line to render to.
 * @param mClr
 * The color of the line to render with.
*/
GridModel::GridModel(const GLfloat& _lineWidth, const Vector3& _lineColor)
	: mWidth{ _lineWidth }, mDrawCount{ 0 }, mClr{ _lineColor.mX, _lineColor.mY, _lineColor.mZ, 1 }
{
}

/*!
 * @brief
 * Initializes the line model.
*/
void GridModel::Init()
{
}

/*!
 * @brief
 * Renders the line model.
*/
void GridModel::Render()
{
	Shader* currShader;
	AssetManager::GetInstance()->GetAsset("GridShader", currShader);
	currShader->Use();
	glBindVertexArray(mVaoid);
	glLineWidth(mWidth);
	glDrawArrays(GL_LINES, 0, mDrawCount);
	glLineWidth(1.f);
	glBindVertexArray(0);
	currShader->UnUse();
}

/*!
 * @brief
 * Exit function for the line model.
*/
void GridModel::Exit()
{
}

void GridModel::GenerateGridModel(bool _isCollisionGrid)
{
	std::vector<glm::vec2> pos_vtx;
	std::vector<glm::vec4> clr_vtx;
	Vector3 worldSize = GraphicsSystem::GetInstance()->mWorldSize;
	Vector3 halfWorldSize = worldSize / 2;
	if (_isCollisionGrid)
	{
		mClr = glm::vec4(0, 1, 0, 1);	//Green color
		mWidth = 3;

		//Vertical lines
		for (int i = 0; i <= TileComponent::mMapWidth; ++i)
		{
			float xPos = TileComponent::mMinBound.mX + i * TileComponent::mTileSize;
			pos_vtx.push_back(glm::vec2(xPos / halfWorldSize.mX, -TileComponent::mMinBound.mY / halfWorldSize.mY));
			pos_vtx.push_back(glm::vec2(xPos / halfWorldSize.mX, -TileComponent::mMaxBound.mY / halfWorldSize.mY));
		}

		//Horizontal Lines
		for (int i = 0; i <= TileComponent::mMapHeight; ++i)
		{
			float yPos = TileComponent::mMinBound.mY + i * TileComponent::mTileSize;
			pos_vtx.push_back(glm::vec2(TileComponent::mMinBound.mX / halfWorldSize.mX, -yPos / halfWorldSize.mY));
			pos_vtx.push_back(glm::vec2(TileComponent::mMaxBound.mX / halfWorldSize.mX, -yPos / halfWorldSize.mY));
		}
		mDrawCount = (GLuint)pos_vtx.size();
	}
	else
	{
		mClr = glm::vec4(0.3, 0.3, 0.3, 1);	//Gray color
		mWidth = 2;

		int xCount = (int)(worldSize.mX / TileComponent::mTileSize) + 1;
		int yCount = (int)(worldSize.mY / TileComponent::mTileSize) + 1;

		//Vertical lines
		for (int i = 0; i < xCount; ++i)
		{
			float xPos = (-worldSize.mX / 2) + i * TileComponent::mTileSize;
			pos_vtx.push_back(glm::vec2(xPos / halfWorldSize.mX, -(-worldSize.mY / 2) / halfWorldSize.mY));
			pos_vtx.push_back(glm::vec2(xPos / halfWorldSize.mX, -(worldSize.mY /2) / halfWorldSize.mY));
		}

		//Horizontal Lines
		for (int i = 0; i < yCount; ++i)
		{
			float yPos = (-(worldSize.mY / 2)) + i * TileComponent::mTileSize;
			pos_vtx.push_back(glm::vec2((-worldSize.mX / 2) / halfWorldSize.mX, -yPos / halfWorldSize.mY));
			pos_vtx.push_back(glm::vec2((worldSize.mX / 2) / halfWorldSize.mX, -yPos / halfWorldSize.mY));
		}

		mDrawCount = (GLuint)pos_vtx.size();
	}
	for (GLuint i = 0; i < mDrawCount; ++i)
		clr_vtx.push_back(mClr);

	//Delete previous buffers for collision grid re-generation
	if (_isCollisionGrid && TileComponent::mModelCreated == true)
	{
		glDeleteVertexArrays(1, &mVaoid);
		glDeleteBuffers(1, &mVboid);
	}

	// transfer vertex position to VBO
	glCreateBuffers(1, &mVboid);
	glNamedBufferStorage(mVboid, sizeof(glm::vec2) * pos_vtx.size() + sizeof(glm::vec4) * clr_vtx.size(), nullptr, GL_DYNAMIC_STORAGE_BIT);
	glNamedBufferSubData(mVboid, 0, sizeof(glm::vec2) * pos_vtx.size(), pos_vtx.data());
	glNamedBufferSubData(mVboid, sizeof(glm::vec2) * pos_vtx.size(), sizeof(glm::vec4) * clr_vtx.size(), clr_vtx.data());

	// encapsulate information about contents of VBO and VBO handle
	// to another object called VAO
	glCreateVertexArrays(1, &mVaoid);

	// for vertex position array, we use vertex attribute index 0
	// and vertex buffer binding point 0
	glEnableVertexArrayAttrib(mVaoid, 0);
	glVertexArrayVertexBuffer(mVaoid, 0, mVboid, 0, sizeof(glm::vec2));
	glVertexArrayAttribFormat(mVaoid, 0, 2, GL_FLOAT, GL_FALSE, 0);
	glVertexArrayAttribBinding(mVaoid, 0, 0);

	// for vertex color array, we use vertex attribute index 1
	// and vertex buffer binding point 4
	glEnableVertexArrayAttrib(mVaoid, 1);
	glVertexArrayVertexBuffer(mVaoid, 4, mVboid, sizeof(glm::vec2) * pos_vtx.size(), sizeof(glm::vec4));
	glVertexArrayAttribFormat(mVaoid, 1, 4, GL_FLOAT, GL_FALSE, 0);
	glVertexArrayAttribBinding(mVaoid, 1, 4);
	
	glBindVertexArray(0);

	if(_isCollisionGrid)
		TileComponent::mModelCreated = true;
}