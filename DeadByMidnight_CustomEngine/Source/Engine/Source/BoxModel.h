/*!
All content © 2023 DigiPen Institute of Technology Singapore, all rights reserved.
@file       BoxModel.h
@author     CAI Wenjin (100%)
@co-author

----------------------------------------------------------------------------

@course     CSD2401F23
@section    CSD2401F23-A
@project    CSD2401F23 Software Engineering Project 3

@brief      BoxModel class declarations to create and render a box.
*//*______________________________________________________________________*/

#ifndef BOX_MODEL_H
#define BOX_MODEL_H

#include "Model.h"
#include <array>
#include <vector>

class BoxModel : public Model
{
private:
	struct Pts
	{
		glm::vec2 mPos;
		glm::vec4 mclr;
		glm::vec2 mTexCoord;
	};

	GLfloat mWidth;
	std::array<Pts, 4> mPts;
public:
	
	/*!
	 * @brief 
	 * Initializes the box model.
	*/
	void Init();
	/*!
	 * @brief 
	 * Renders the box model.
	*/
	void Render();
	/*!
	 * @brief 
	 * Exit function of the box model.
	*/
	void Exit();

	/*!
	 * @brief 
	 * Constructor for the BoxModel class.
	*/
	BoxModel();

	/*!
	 * @brief 
	 * Constructor for BoxModelc class with user-defined line width, the 4 points containing the
	 * position. color and texture coordinates.
	 * @param _mWidth 
	 * The width of the line to define.
	 * @param _pts 
	 * The array of 4 points to define.
	*/
	BoxModel(const GLfloat& _mWidth, const std::array<Pts, 4>& _pts);
};
#endif // !BOX_MODEL_H
