/*!
All content © 2023 DigiPen Institute of Technology Singapore, all rights reserved.
@file       QuadModel.h
@author     CAI Wenjin (100%)
@co-author

----------------------------------------------------------------------------

@course     CSD2401F23
@section    CSD2401F23-A
@project    CSD2401F23 Software Engineering Project 3

@brief      QuadModel class declarations to create and render a rectangle.
*//*______________________________________________________________________*/

#ifndef QUAD_MODEL_H
#define QUAD_MODEL_H

#include <vector>
#include "Model.h"

class QuadModel : public Model
{
public:
	/*!
	 * @brief 
	 * Initializes the quad model.
	 * Current implementation is for rendering a texture over it using the viewport dimensions.
	*/
	void Init();
	/*!
	 * @brief 
	 * Renders the quad model.
	*/
	void Render();
	/*!
	 * @brief 
	 * Exit function for the quad model.
	*/
	void Exit();
};

#endif // !QUAD_MODEL_H

