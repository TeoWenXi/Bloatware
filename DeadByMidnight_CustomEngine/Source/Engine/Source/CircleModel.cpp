/*!
All content © 2023 DigiPen Institute of Technology Singapore, all rights reserved.
@file       CircleModel.cpp
@author     CAI Wenjin (100%)
@co-author

----------------------------------------------------------------------------

@course     CSD2401F23
@section    CSD2401F23-A
@project    CSD2401F23 Software Engineering Project 3

@brief      CircleModel class definitions to create and render a circle.
*//*______________________________________________________________________*/

#include "CircleModel.h"

CircleModel::CircleModel()
	: mTriCount{}, mSlices{}, mClr{}
{
}

CircleModel::CircleModel(const GLint& _slices, const glm::vec4& _clr)
{
	mSlices = _slices;
	mTriCount = mSlices + 2;
	mClr = _clr;
}

CircleModel::~CircleModel()
{
}

void CircleModel::Init()
{
#if 0
	GLfloat theta = glm::two_pi<GLfloat>() / mSlices;
	mTriCount = mSlices + 2;

	std::vector<glm::vec2> pos_vtx(mTriCount);
	std::vector<glm::vec4> clr_vtx(mTriCount);

	// setting the pivot to be at (0, 0)
	pos_vtx[0] = { 0, 0 };
	for (GLint i{ 1 }; i < mTriCount; ++i)
	{
		GLfloat x{ glm::cos((i - 1) * theta) };
		GLfloat y{ glm::sin((i - 1) * theta) };

		pos_vtx[i] = { x , y };
	}

	for (GLint i{}; i < mTriCount; ++i)
	{
		clr_vtx[i] = mClr;
	}
#else
	GLfloat theta = glm::two_pi<GLfloat>() / mSlices;
	std::vector<glm::vec2> pos_vtx(mSlices);
	std::vector<glm::vec4> clr_vtx(mSlices);

	for (GLint i{}; i < mSlices; ++i)
	{
		GLfloat x{ glm::cos(i * theta) };
		GLfloat y{ glm::sin(i * theta) };

		pos_vtx[i] = { x, y };
	}

	for (GLint i{}; i < mSlices; ++i)
	{
		clr_vtx[i] = mClr;
	}

#endif

	// transfer vertex position and color attributes to VBO
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
}

void CircleModel::Render()
{
#if 0
	glBindVertexArray(mVaoid);
	glDrawArrays(GL_TRIANGLE_FAN, 0, mTriCount);
	glBindVertexArray(0);
#else
	glBindVertexArray(mVaoid);
	glLineWidth(5.f);
	glDrawArrays(GL_LINE_LOOP, 0, mSlices);
	glLineWidth(1.f);
	glBindVertexArray(0);
#endif	
}

void CircleModel::Exit()
{
}
