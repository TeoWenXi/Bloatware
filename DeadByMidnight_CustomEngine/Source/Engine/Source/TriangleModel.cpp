/*!
All content © 2023 DigiPen Institute of Technology Singapore, all rights reserved.
@file       TriangleModel.cpp
@author     CAI Wenjin (100%)
@co-author

----------------------------------------------------------------------------

@course     CSD2401F23
@section    CSD2401F23-A
@project    CSD2401F23 Software Engineering Project 3

@brief      TriangleModel class definitions to create and render a triangle.
*//*______________________________________________________________________*/

#include "TriangleModel.h"

TriangleModel::TriangleModel()
{
	mPos.fill(glm::vec2{});
	mClr = glm::vec4{};
}

TriangleModel::TriangleModel(const glm::vec2& _v0, const glm::vec2& _v1, const glm::vec2& _v2, const glm::vec4& _clr)
{
	mPos[0] = _v0;
	mPos[1] = _v1;
	mPos[2] = _v2;

	mClr = _clr;
}

TriangleModel::~TriangleModel()
{
}

void TriangleModel::Init()
{
	glCreateBuffers(1, &mVboid);
	glNamedBufferStorage(mVboid, sizeof(glm::vec2) * mPos.size() + sizeof(glm::vec4), nullptr, GL_DYNAMIC_STORAGE_BIT);
	glNamedBufferSubData(mVboid, 0, sizeof(glm::vec2) * mPos.size(), mPos.data());
	glNamedBufferSubData(mVboid, sizeof(glm::vec2) * mPos.size(), sizeof(glm::vec4), &mClr);
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
	glVertexArrayVertexBuffer(mVaoid, 4, mVboid, sizeof(glm::vec2) * mPos.size(), sizeof(glm::vec4));
	glVertexArrayAttribFormat(mVaoid, 1, 4, GL_FLOAT, GL_FALSE, 0);
	glVertexArrayAttribBinding(mVaoid, 1, 4);

	glBindVertexArray(0);
}

void TriangleModel::Render()
{
	glBindVertexArray(mVaoid);
	glDrawArrays(GL_TRIANGLES, 0, 3);
	glBindVertexArray(0);
}

void TriangleModel::Exit()
{
}
