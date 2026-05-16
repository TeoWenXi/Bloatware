/*!
All content © 2023 DigiPen Institute of Technology Singapore, all rights reserved.
@file       TextManager.h
@author     TAN Kang Zheng (40%)
@co-author  TEO Wen Xi (30%), Jovan Zhuo Wen LOW (30%)

----------------------------------------------------------------------------

@course     CSD2401F23
@section    CSD2401F23-A
@project    CSD2401F23 Software Engineering Project 3

@brief      Manages the text for the engine
*//*______________________________________________________________________*/

#ifndef TEXT_MANAGER_H
#define TEXT_MANAGER_H

//Other Includes
#include "Singleton.h"
#include "Manager.h"

#include <iostream>
#include <string>
#include <map>
#include <vector>

// Font Character class to store 
// information about each character
#include "FontCharacter.h"

// For Matrix4x4
#include <glm/glm.hpp>

// Shader for rendering
#include "Shader.h" 

//Freetype includes
#include <ft2build.h>
#include FT_FREETYPE_H

class TextManager : public Singleton <TextManager>, public Manager
{
public:
	/*!
	 * @brief
	 * Offsets the dialogue text Horizontal
	 */
	float DialogueTextOffsetX = 0.0f;
	/*!
	 * @brief 
	 * Offsets the dialogue text Height
	 */
	float DialogueTextOffsetY = 0.0f;

	/*!
	 * @brief
	 * Init virtual function for manager classes
	*/
	void Init();

	/*!
	 * @brief
	 * Load virtual function for manager classes
	*/
	void Load();

	/*!
	 * @brief
	 * Update virtual function for manager classes
	*/
	void Update();

	/*!
	 * @brief
	 * Unload virtual function for manager classes
	*/
	void Unload();

	/*!
	 * @brief
	 *	Renders all the text currently in queue
	*/
	void Render();

	/*!
	 * @brief 
	 *	Function to signal to TextManager to store information for later text rendering
	 * @param _fontName
	 *	The name of the font to use
	 * @param _text
	 *	The text to be rendered
	 * @param _fontSize
	 *	The size of the text
	 * @param _posx
	 *	The X position of the text
	 * @param _posy
	 *	The Y position of the text
	 * @param _color
	 *	The color of the text
	 * @param _renderLayer
	 *	An integer representing the rendering layer of the text
	 * @param _alpha
	 *  The alpha value of the text, ranging from 0.0f to 1.0f
	*/
	void DrawText(std::string _fontName, std::string _text, GLfloat _fontSize, GLfloat _posx, GLfloat _posy, Vector3 _color, int _renderLayer, float _alpha = 1.0f);

	/*!
	 * @brief
	 *	Function to signal to TextManager to render all text on a given layer
	 * @param _renderLayer
	 *	The int representing the layer to render
	*/
	void RenderText(int _renderLayer);

	/*!
	 * @brief
	 *	Function to signal to TextManager to clear all render information
	*/
	void ClearRenderInformation();

	/*!
	 * @brief 
	 *	Function to signal to TextManager to add a font to queue for loading
	 * @param _filePath 
	 *	The file path of the font file
	 * @param _fontName 
	 *	The name of the font, to be used to call the font when rendering text
	*/
	void CreateFontAsset(std::string _filePath, std::string _fontName);

	/*!
	 * @brief
	 * Updates the dialogue text to be shown on screen
	 * @param _text 
	 * The updated dialogue text
	*/
	void SetDialogueText(std::string _text);

	/*!
	 * @brief
	 * Updates the opening dialogue text to be shown on screen
	 * @param _text
	 * The updated opening dialogue text
	 * @param
	 * The font to be used for the text
	*/
	void SetOpeningDialogueText(std::string _text, std::string _font);

	/*!
	 * @brief
	 * Updates the Visual Console text to be shown on screen
	 * @param
	 * The updated Visual Console text
	 * @param
	 * The font to be used for the text
	 * @param
	 * The duration of the text
	*/
	void SetVisualConsoleText(std::string _text, std::string _font, float _dur);
	
	/*!
	 * @brief
	 * Updates the color of the Visual Console text shown on screen
	 * @param
	 * A Vec3 representation of the RGB color of the text to be shown
	*/
	void SetVisualConsoleColor(Vec3 _color);

	/*!
	 * @brief
	 * Sets the Visual Console text to be empty
	*/
	void SetVisualConsoleDelete();

	/*!
	 * @brief returns mTextboxSize, makes sure it doesnt return null
	 * @return
	*/
	glm::vec2 GetTextboxSize();

	/*!
	 * @brief
	 * Clears all text data
	 * @return
	*/
	void ClearAllText();

private:
	//Singleton
	friend Singleton<TextManager>;

	//VAO & VBO for rendering
	GLuint mVaoid{}, mVboid{};

	//Font shader
	Shader mFontShader{};

	//Freetype library
	FT_Library mLibrary = nullptr;

	//Map containing the characters of the font for every font
	std::map<std::string, std::map<char, FontCharacter>> mFontList;

	//Loaded font face size
	GLuint mFontFaceSize{};

	//Vector to store the queue of fonts to load
	std::vector<std::pair<std::string, std::string>> mFontQueue;

	/*!
	 * @brief 
	 *	Function to render a set of text information onto the screen
	 * @param _fontName 
	 *	The name of the font to use
	 * @param _text 
	 *	The text to be rendered
	 * @param _fontSize 
	 *	The size of the text
	 * @param _posx 
	 *	The X position of the text
	 * @param _posy 
	 *	The Y position of the text
	 * @param _color 
	 *	The color of the text
	 * @param _alpha 
	 *	The alpha value of the text
	 * @return 
	*/
	glm::vec2 DrawFont(std::string _fontName, std::string _text, GLfloat _fontSize, GLfloat _posx, GLfloat _posy, Vector3 _color, GLfloat _alpha, GLfloat _maxWidth_Visual = 0.f);

	/*!
	 * @brief 
	 *	Function to begin loading of all queued fonts
	*/
	void LoadFonts();

	/*!
	 * @brief 
	 * Gets the total rendered length for a string of text
	 * @param
	 * The font that the text is being displayed with
	 * @param
	 * The text that is being displayed
	 * @param
	 * The font size of the text that is being displayed
	 * @return
	 * The total length of the text that is being rendered
	*/
	GLfloat GetStringWidth(std::string _fontName, std::string _text, GLfloat _fontSize);

	/*!
	 * @brief
	 * Obtain the number of newlines used in a string
	 * @param
	 * The string to parse
	 * @return 
	 * The  number of newlines in the string
	*/
	int GetNumberOfBackslashN(std::string _text);

	/*!
	 * @brief
	 * Parses a string and appends newlines to ensure that when rendered, the text remains within the screen
	 * @param
	 * The font used for the text
	 * @param
	 * The text being rendered
	 * @param
	 * The font size of the text
	 * @return 
	 * The newly appended string
	*/
	std::string LongTextSplicer(std::string _fontName, std::string _text, GLfloat _fontSize);

	//Struct and vector to store information of text to be rendered
	struct TextInformation
	{
		std::string mFontName;
		std::string mText;
		GLfloat mFontSize;
		GLfloat mPosx, mPosy;
		Vector3 mColor;
		int mRenderLayer;
		float mAlpha;
	};

	std::vector<TextInformation> mTextRenderInformation;

	std::string mDialogueText{ "" }, mVisualConsoleText{ "" }, mVisualConsoleFont{ "" }, mOpeningDialogueText{ "" }, mOpeningDialogueFont{ "" };
	Vec3 mVisualConsoleFontColor{ 0, 1, 0 };
	bool mEraseVisualConsoleFont{ false };
	double mEraseVisualConsoleTime{ 0.0 }, mEraseVisualConsoleElapsed{ 0.0 };

	bool mShowFPSText = false;
	glm::vec2 mTextboxSize;
};

#endif