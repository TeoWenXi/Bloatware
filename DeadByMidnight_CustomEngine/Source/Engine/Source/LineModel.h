/*!
All content © 2023 DigiPen Institute of Technology Singapore, all rights reserved.
@file       LineModel.h
@author     CAI Wenjin (100%)
@co-author

----------------------------------------------------------------------------

@course     CSD2401F23
@section    CSD2401F23-A
@project    CSD2401F23 Software Engineering Project 3

@brief      LineModel class declarations to create and render a line.
*//*______________________________________________________________________*/

#ifndef LINE_MODEL_H
#define LINE_MODEL_H

#include "Model.h"
#include <vector>

class LineModel : public Model
{
private:
	GLfloat mWidth;
	glm::vec2 mStartPos, mEndPos;
	glm::vec4 mClr;

public:
	/*!
	 * @brief 
	 * Constructor for LineModel class.
	*/
	LineModel();
	/*!
	 * @brief 
	 * Constructor for the LineModel class with user defined line width, start and end points and the
	 * line color.
	 * @param _mWidth 
	 * Width of the line to render with.
	 * @param _mStart 
	 * Start position of the line to render from.
	 * @param _mEnd 
	 * End position of the line to render to.
	 * @param _mClr 
	 * The color of the line to render with.
	*/
	LineModel(const GLfloat& _mWidth, const glm::vec2& _mStart, const glm::vec2& _mEnd, const glm::vec4& _mClr);
	/*!
	 * @brief 
	 * Destructor for LineModel class.
	*/
	~LineModel();

	/*!
	 * @brief 
	 * Initializes the line model.
	*/
	void Init();

	/*!
	 * @brief 
	 * Renders the line model.
	*/
	void Render();
	/*!
	 * @brief 
	 * Exit function for the line model.
	*/
	void Exit();
};

#endif // !LINE_MODEL_H


