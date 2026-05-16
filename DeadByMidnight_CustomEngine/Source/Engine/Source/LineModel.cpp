/*!
All content © 2023 DigiPen Institute of Technology Singapore, all rights reserved.
@file       LineModel.cpp
@author     CAI Wenjin (100%)
@co-author

----------------------------------------------------------------------------

@course     CSD2401F23
@section    CSD2401F23-A
@project    CSD2401F23 Software Engineering Project 3

@brief      LineModel class definitions to create and render a line.
*//*______________________________________________________________________*/

#include "LineModel.h"

/*!
 * @brief
 * 
*/
LineModel::LineModel()
	: mWidth{}, mStartPos{}, mEndPos{}, mClr{}
{
}

LineModel::LineModel(const GLfloat& _mWidth, const glm::vec2& _mStart, const glm::vec2& _mEnd, const glm::vec4& _mClr)
	: mWidth{ _mWidth }, mStartPos{ _mStart }, mEndPos{ _mEnd }, mClr{ _mClr }
{
}

LineModel::~LineModel()
{
}

void LineModel::Init()
{
	std::vector<glm::vec2> pos_vtx;
	pos_vtx.push_back(mStartPos);
	pos_vtx.push_back(mEndPos);

	// transfer vertex position to VBO
	glCreateBuffers(1, &mVboid);
	glNamedBufferStorage(mVboid, sizeof(glm::vec2) * pos_vtx.size() + sizeof(glm::vec4), nullptr, GL_DYNAMIC_STORAGE_BIT);
	glNamedBufferSubData(mVboid, 0, sizeof(glm::vec2) * pos_vtx.size(), pos_vtx.data());
	glNamedBufferSubData(mVboid, sizeof(glm::vec2) * pos_vtx.size(), sizeof(glm::vec4), &mClr);
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
}


void LineModel::Render()
{
	glBindVertexArray(mVaoid);
	glLineWidth(mWidth);
	glDrawArrays(GL_LINES, 0, 2);
	glLineWidth(1.f);
	glBindVertexArray(0);
}

void LineModel::Exit()
{
}
