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

#ifndef GRID_MODEL_H
#define GRID_MODEL_H

#include "Model.h"
#include "Vector3.h"
#include <vector>

class GridModel : public Model
{
private:
	GLfloat mWidth;
	glm::vec4 mClr;
	GLuint mDrawCount;

public:
	
	/*!
	 * @brief 
	 * Ctor
	*/
	GridModel();

	/*!
	 * @brief 
	 * Overloaded Ctorwith line width & color
	 * @param _lineWidth 
	 * line width of every line
	 * @param _lineColor 
	 * line color of every line
	*/
	GridModel(const GLfloat& _lineWidth, const Vector3& _lineColor);

	/*!
	 * @brief 
	 * Init function for the model
	*/
	void Init();

	/*!
	 * @brief 
	 * Render function for the model
	*/
	void Render();

	/*!
	 * @brief 
	 * Exit function for the model
	*/
	void Exit();

	/*!
	 * @brief 
	 * Generates the grid model
	 * @param _isCollisionGrid 
	 * if this is the collision grid or display grid
	*/
	void GenerateGridModel(bool _isCollisionGrid);
};

#endif // !GRID_MODEL_H