/*!
All content © 2023 DigiPen Institute of Technology Singapore, all rights reserved.
@file       FontCharacter.h
@author		TEO Wen Xi (75%)
@co-author	TAN Kang Zheng (25%)

----------------------------------------------------------------------------

@course     CSD2401F23
@section    CSD2401F23-A
@project    CSD2401F23 Software Engineering Project 3

@brief      FontCharacter class to store data for each letter of the font
*//*______________________________________________________________________*/

#include "Vector3.h"
#include <glad/glad.h>

class FontCharacter
{
public:

	/*!
	 * @brief
	 * Default Constructor for the FontCharacter class
	*/
	FontCharacter() {}

	/*!
	 * @brief
	 * Constructor for the FontCharacter class
	 * @param _texID 
	 * The texture ID of the charcter
	 * @param _textSize 
	 * The size of the character
	 * @param _bearing 
	 * The offset to the top/left of the character
	 * @param _advance
	 * The offset to the next character
	*/
	FontCharacter(GLuint _texID, Vector3 _textSize, Vector3 _bearing, GLuint _advance)
		: mTexID(_texID), mTextSize(_textSize), mBearing(_bearing), mAdvance(_advance)
	{}

	GLuint mTexID = 0;	//ID handle of the character texture
	Vector3 mTextSize{};		//Size of the character
	Vector3 mBearing{};		//Offset from baseline to left/top of character
	GLuint mAdvance = 0;	//Offset to advance to next character
};