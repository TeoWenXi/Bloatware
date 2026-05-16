/*!
All content © 2023 DigiPen Institute of Technology Singapore, all rights reserved.
@file       QuadModel.cpp
@author     CAI Wenjin (100%)
@co-author

----------------------------------------------------------------------------

@course     CSD2401F23
@section    CSD2401F23-A
@project    CSD2401F23 Software Engineering Project 3

@brief      QuadModel class definitions to create and render a rectangle.
*//*______________________________________________________________________*/

#include "QuadModel.h"

void QuadModel::Init()
{
	std::vector<glm::vec2> pos_vtx
	{
		glm::vec2{1.f, -1.f}, glm::vec2{1.f, 1.f},
		glm::vec2{-1.f, 1.f}, glm::vec2{-1.f, -1.f}
	};

	std::vector<glm::vec4> clr_vtx
	{
		glm::vec4{0.f, 0.f, 0.f, 1.f}, glm::vec4{0.f, 0.f, 0.f, 1.f},
		glm::vec4{0.f, 0.f, 0.f, 1.f}, glm::vec4{0.f, 0.f, 0.f, 1.f}
	};

	std::vector<glm::vec2> texPos_vtx
	{
		glm::vec2{ 1.f, 0.f }, glm::vec2{ 1.f, 1.f },
		glm::vec2{ 0.f, 1.f }, glm::vec2{ 0.f, 0.f }
	};

	//Transfer vertex details to VBO
	glCreateBuffers(1, &mVboid);
	size_t arrSize = sizeof(glm::vec2) * pos_vtx.size() + sizeof(glm::vec4) * clr_vtx.size() + sizeof(glm::vec2) * texPos_vtx.size();
	glNamedBufferStorage(mVboid, arrSize, nullptr, GL_DYNAMIC_STORAGE_BIT);
	glNamedBufferSubData(mVboid, 0, sizeof(glm::vec2) * pos_vtx.size(), pos_vtx.data());
	glNamedBufferSubData(mVboid, sizeof(glm::vec2) * pos_vtx.size(), sizeof(glm::vec4) * clr_vtx.size(), clr_vtx.data());
	glNamedBufferSubData(mVboid, sizeof(glm::vec2) * pos_vtx.size() + sizeof(glm::vec4) * clr_vtx.size(), sizeof(glm::vec2) * texPos_vtx.size(), texPos_vtx.data());

	//Transfer VBO data to VAO
	//Encapsulate information of VBO and VBO handle to VAO
	glCreateVertexArrays(1, &mVaoid);
	//Vertex position array, vertex attrib 0, binding point 0
	glEnableVertexArrayAttrib(mVaoid, 0);
	glVertexArrayVertexBuffer(mVaoid, 0, mVboid, 0, sizeof(glm::vec2));
	glVertexArrayAttribFormat(mVaoid, 0, 2, GL_FLOAT, GL_FALSE, 0);
	glVertexArrayAttribBinding(mVaoid, 0, 0);

	//Vertex color array, vertex attrib 1, binding point 1
	glEnableVertexArrayAttrib(mVaoid, 1);
	glVertexArrayVertexBuffer(mVaoid, 1, mVboid, sizeof(glm::vec2) * pos_vtx.size(), sizeof(glm::vec4));
	glVertexArrayAttribFormat(mVaoid, 1, 4, GL_FLOAT, GL_FALSE, 0);
	glVertexArrayAttribBinding(mVaoid, 1, 1);

	//Vertex texture coords array, vertex attrib 2, binding point 2
	glEnableVertexArrayAttrib(mVaoid, 2);
	glVertexArrayVertexBuffer(mVaoid, 2, mVboid, sizeof(glm::vec2) * pos_vtx.size() + sizeof(glm::vec4) * clr_vtx.size(), sizeof(glm::vec2));
	glVertexArrayAttribFormat(mVaoid, 2, 2, GL_FLOAT, GL_FALSE, 0);
	glVertexArrayAttribBinding(mVaoid, 2, 2);

	//Transfer indices vertex array to VAO
	std::vector<GLuint> idx_vtx
	{
		1, 0, 2, 3
	};

	GLuint ebo_hdl;
	glCreateBuffers(1, &ebo_hdl);
	glNamedBufferStorage(ebo_hdl, sizeof(GLuint) * idx_vtx.size(), reinterpret_cast<GLvoid*>(idx_vtx.data()), GL_DYNAMIC_STORAGE_BIT);
	glVertexArrayElementBuffer(mVaoid, ebo_hdl);
	glBindVertexArray(0);
}

void QuadModel::Render()
{
	glBindVertexArray(mVaoid);
	glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_INT, NULL);
	glBindVertexArray(0);
}

void QuadModel::Exit()
{
}
