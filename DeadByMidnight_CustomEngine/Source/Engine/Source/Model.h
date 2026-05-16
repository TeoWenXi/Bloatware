/*!
All content © 2023 DigiPen Institute of Technology Singapore, all rights reserved.
@file       Model.h
@author     CAI Wenjin (100%)
@co-author

----------------------------------------------------------------------------

@course     CSD2401F23
@section    CSD2401F23-A
@project    CSD2401F23 Software Engineering Project 3

@brief      Parent class for the models to inherit from.
*//*______________________________________________________________________*/

#ifndef MODEL_H
#define MODEL_H

#include <glad/glad.h>
#include <glm/glm.hpp>

class Model
{
public:
	GLuint mVaoid{}, mVboid{};
	GLboolean mActive{ GL_TRUE };

	/*!
	 * @brief 
	 * Initialise the Model class.
	*/
	virtual void Init();
	/*!
	 * @brief 
	 * Renders the mode.
	*/
	virtual void Render();
	/*!
	 * @brief 
	 * Exit function for the model class.
	*/
	virtual void Exit();
};

#endif // !MODEL_H
