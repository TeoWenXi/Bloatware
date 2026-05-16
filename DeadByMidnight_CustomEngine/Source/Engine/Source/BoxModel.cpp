/*!
All content © 2023 DigiPen Institute of Technology Singapore, all rights reserved.
@file       BoxModel.cpp
@author     CAI Wenjin (100%)
@co-author

----------------------------------------------------------------------------

@course     CSD2401F23
@section    CSD2401F23-A
@project    CSD2401F23 Software Engineering Project 3

@brief      BoxModel class declarations to create and render a box.
*//*______________________________________________________________________*/

#include "BoxModel.h"

void BoxModel::Init()
{
	glCreateBuffers(1, &mVboid);
	glNamedBufferStorage(mVboid, sizeof(Pts) * mPts.size(), &mPts, GL_DYNAMIC_STORAGE_BIT);

	glCreateVertexArrays(1, &mVaoid);
	// for vertex position array, we use vertex attribute index 0
	// and vertex buffer binding point 3
	glEnableVertexArrayAttrib(mVaoid, 0);
	glVertexArrayVertexBuffer(mVaoid, 3, mVboid, 0, sizeof(Pts));
	glVertexArrayAttribFormat(mVaoid, 0, 2, GL_FLOAT, GL_FALSE, offsetof(Pts, Pts::mPos));
	glVertexArrayAttribBinding(mVaoid, 0, 3);
	// for vertex color array, we use vertex attribute index 1
	// and vertex buffer binding point 3
	glEnableVertexArrayAttrib(mVaoid, 1);
	glVertexArrayAttribFormat(mVaoid, 1, 4, GL_FLOAT, GL_FALSE, offsetof(Pts, Pts::mclr));
	glVertexArrayAttribBinding(mVaoid, 1, 3);
	// for texture coordinate array, we use vertex attribute index 2
	// and vertex buffer binding point 3
	glEnableVertexArrayAttrib(mVaoid, 2);
	glVertexArrayAttribFormat(mVaoid, 2, 2, GL_FLOAT, GL_FALSE, offsetof(Pts, Pts::mTexCoord));
	glVertexArrayAttribBinding(mVaoid, 2, 3);

	// using LINE_LOOPS to draw the rectangle instead of TRIANGLE_STRIPS
	std::vector<GLushort> idx{ 0, 1, 2, 3 };

	// transfer index data to EBO (element buffer object)
	GLuint ebo_hdl;
	glCreateBuffers(1, &ebo_hdl);
	glNamedBufferStorage(ebo_hdl, sizeof(GLushort) * idx.size(), reinterpret_cast<GLvoid*>(idx.data()), GL_DYNAMIC_STORAGE_BIT);
	glVertexArrayElementBuffer(mVaoid, ebo_hdl);
	glBindVertexArray(0);
}

void BoxModel::Render()
{
	glBindVertexArray(mVaoid);
	glLineWidth(mWidth);
	glDrawArrays(GL_LINE_LOOP, 0, 4);
	glLineWidth(1.f);
	glBindVertexArray(0);
}

void BoxModel::Exit()
{
}

BoxModel::BoxModel()
	:mWidth{ 5.f }, mPts{}
{
	// defining the 4 points of the box
	Pts A =
	{
		{-1, -1},
		{0, 1, 0, 1},
		{0, 0}
	};

	Pts B =
	{
		{1, -1},
		{0, 1, 0, 1},
		{1, 0}
	};

	Pts C =
	{
		{1, 1},
		{0, 1, 0, 1},
		{1, 1}
	};

	Pts D =
	{
		{-1, 1},
		{0, 1, 0, 1},
		{0, 1}
	};

	mPts[0] = A;
	mPts[1] = B;
	mPts[2] = C;
	mPts[3] = D;
}

BoxModel::BoxModel(const GLfloat& _mWidth, const std::array<Pts, 4>& _pts)
	: mWidth(_mWidth), mPts(_pts)
{
	mPts[0] = _pts[0];
	mPts[1] = _pts[1];
	mPts[2] = _pts[2];
	mPts[3] = _pts[3];
}
