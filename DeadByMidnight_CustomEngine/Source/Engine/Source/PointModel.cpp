/*!
All content © 2023 DigiPen Institute of Technology Singapore, all rights reserved.
@file       PointModel.cpp
@author     CAI Wenjin (100%)
@co-author

----------------------------------------------------------------------------

@course     CSD2401F23
@section    CSD2401F23-A
@project    CSD2401F23 Software Engineering Project 3

@brief      PointModel class definitions to create and render a point.
*//*______________________________________________________________________*/

#include "PointModel.h"

PointModel::PointModel()
	:mSize{}, mPos{}, mClr{}
{
}

PointModel::PointModel(const GLfloat& _mSize, const glm::vec2& _mPos, const glm::vec4& _mClr)
	: mSize{ _mSize }, mPos{ _mPos }, mClr{ _mClr }
{
}

PointModel::~PointModel()
{
}


void PointModel::Init()
{
	glCreateBuffers(1, &mVboid);
	glNamedBufferStorage(mVboid, sizeof(glm::vec2) + sizeof(glm::vec4), nullptr, GL_DYNAMIC_STORAGE_BIT);
	glNamedBufferSubData(mVboid, 0, sizeof(glm::vec2), &mPos);
	glNamedBufferSubData(mVboid, sizeof(glm::vec2), sizeof(glm::vec4), &mClr);

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
	glVertexArrayVertexBuffer(mVaoid, 4, mVboid, sizeof(glm::vec2), sizeof(glm::vec4));
	glVertexArrayAttribFormat(mVaoid, 1, 4, GL_FLOAT, GL_FALSE, 0);
	glVertexArrayAttribBinding(mVaoid, 1, 4);

	glBindVertexArray(0);
}

void PointModel::Render()
{
	glBindVertexArray(mVaoid);
	glPointSize(mSize);
	glDrawArrays(GL_POINTS, 0, 1);
	glPointSize(1.f);
	glBindVertexArray(0);
}

void PointModel::Exit()
{
}
