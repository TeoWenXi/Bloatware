/*!
All content © 2023 DigiPen Institute of Technology Singapore, all rights reserved.
@file       CircleModel.h
@author     CAI Wenjin (100%)
@co-author

----------------------------------------------------------------------------

@course     CSD2401F23
@section    CSD2401F23-A
@project    CSD2401F23 Software Engineering Project 3

@brief      CircleModel class declarations to create and render a circle.
*//*______________________________________________________________________*/

#ifndef CIRCLE_MODEL_H
#define CIRCLE_MODEL_H

#include <glm/gtc/constants.hpp>
#include <vector>
#include "Model.h"

class CircleModel : public Model
{
private:
	GLint mTriCount;
	GLint mSlices;
	glm::vec4 mClr;

public:
	/*!
	 * @brief 
	 * Constructor for CircleModel class.
	*/
	CircleModel();
	/*!
	 * @brief 
	 * Constructor for CircleModel class with user-defined slices and color.
	 * @param _slices 
	 * Number of slices for tthe circle model to have.
	 * @param _clr 
	 * The color of the circle model.
	*/
	CircleModel(const GLint& _slices, const glm::vec4& _clr);
	/*!
	 * @brief 
	 * Destructor for the CircleModel class.
	*/
	~CircleModel();

	/*!
	 * @brief 
	 * Initializes the circle model with slices + 2 triangles.
	*/
	void Init();

	/*!
	 * @brief 
	 * Renders the circle model.
	*/
	void Render();

	/*!
	 * @brief 
	 * Exit function for the circle model.
	*/
	void Exit();
};

#endif // !CIRCLE_MODEL_H


