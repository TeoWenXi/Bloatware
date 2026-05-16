/*!
All content © 2023 DigiPen Institute of Technology Singapore, all rights reserved.
@file       TriangleModel.h
@author     CAI Wenjin (100%)
@co-author

----------------------------------------------------------------------------

@course     CSD2401F23
@section    CSD2401F23-A
@project    CSD2401F23 Software Engineering Project 3

@brief      TriangleModel class declarations to create and render a triangle.
*//*______________________________________________________________________*/

#ifndef TRIANGLE_MODEL_H
#define TRIANGLE_MODEL_H

#include "Model.h"
#include <array>

class TriangleModel : public Model
{
private:
	std::array<glm::vec2, 3> mPos;
	glm::vec4 mClr;
public:

	/*!
	 * @brief 
	 * Constructor for the TriangleModel class.
	*/
	TriangleModel();
	/*!
	 * @brief 
	 * Constructor for the TriangleModel class with user defined positions for the triangle vertices
	 * and color. Vertices should be in CCW order to prevent backface culling.
	 * @param _v0 
	 * 1st vertex of the triangle.
	 * @param _v1 
	 * 2nd vertex of the triangle.
	 * @param _v2 
	 * 3rd vertex of the triangle.
	 * @param _mClr 
	 * Color of the triangle to render with.
	*/
	TriangleModel(const glm::vec2& _v0, const glm::vec2& _v1, const glm::vec2& _v2, const glm::vec4& _mClr);
	/*!
	 * @brief 
	 * Destructor for TriangleModel class.
	*/
	~TriangleModel();

	/*!
	 * @brief 
	 * Initializes the triangle model to the vbo.
	*/
	void Init();

	/*!
	 * @brief 
	 * Renders the triangle model.
	*/
	void Render();

	/*!
	 * @brief 
	 * Exit function for the TriangleModel class.
	*/
	void Exit();

};

#endif // !TRIANGLE_MODEL_H



