/*!
All content © 2023 DigiPen Institute of Technology Singapore, all rights reserved.
@file       PointModel.h
@author     CAI Wenjin (100%)
@co-author

----------------------------------------------------------------------------

@course     CSD2401F23
@section    CSD2401F23-A
@project    CSD2401F23 Software Engineering Project 3

@brief      PointModel class declarations to create and render a point.
*//*______________________________________________________________________*/

#ifndef POINT_MODEL_H
#define POINT_MODEL_H

#include "Model.h"

class PointModel : public Model
{
private:
	GLfloat mSize;
	glm::vec2 mPos;
	glm::vec4 mClr;

public:
	/*!
	 * @brief 
	 * Constructor for PointModel class.
	*/
	PointModel();
	/*!
	 * @brief 
	 * Constructor for PointModel class with user defined point size, position and color.
	 * @param _mSize 
	 * Size of the point to render.
	 * @param _mPos 
	 * Position of the point to render.
	 * @param _mClr 
	 * Color of the point to render.
	*/
	PointModel(const GLfloat& _mSize, const glm::vec2& _mPos, const glm::vec4& _mClr);
	/*!
	 * @brief 
	 * Destructor for PointModel class.
	*/
	~PointModel();

	/*!
	 * @brief 
	 * Initializes the point model.
	*/
	void Init();
	/*!
	 * @brief 
	 * Render the poiint model.
	*/
	void Render();

	/*!
	 * @brief 
	 * Exit function for the point model.
	*/
	void Exit();
};

#endif // 

#pragma once


