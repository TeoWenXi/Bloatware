/*!
All content © 2023 DigiPen Institute of Technology Singapore, all rights reserved.
@file       TextManager.cpp
@author     TAN Kang Zheng (40%)
@co-author  TEO Wen Xi (30%), Jovan Zhuo Wen LOW (30%)

----------------------------------------------------------------------------

@course     CSD2401F23
@section    CSD2401F23-A
@project    CSD2401F23 Software Engineering Project 3

@brief      Manages the text for the engine
*//*______________________________________________________________________*/

#include "TextManager.h"
#include "AssetManager.h"
#include "WindowsManager.h"
#include "GraphicsSystem.h"
#include "CoreEngine.h"
#include "InputManager.h"

#if _ENGINE
#include "DebugLogger.h"
#endif // _ENGINE

#pragma warning(push, 0)
#include <glm/ext.hpp>
#pragma warning(pop)

#include <iomanip>
#include <sstream>

//INVERSELY PROPORTIONAL
#define TEXT_WRAP_GLFLOAT_VALUE 605000.f
#define TEXT_WRAP_FLOAT_VALUE 380.f

void TextManager::Init()
{
	//Initialize the freetype library
	if (FT_Init_FreeType(&mLibrary))
	{
#if _ENGINE
		DebugLogger::GetInstance()->Log("ERROR: Unable to init Freetype Library\n");
#endif // _ENGINE
	}

	//Init font face size, Bigger font size = clearer texture
	mFontFaceSize = 2000;

	//Create vao & vbo arrays for a quad to render the text sprite on
	glGenVertexArrays(1, &mVaoid);
	glGenBuffers(1, &mVboid);
	glBindVertexArray(mVaoid);
	glBindBuffer(GL_ARRAY_BUFFER, mVboid);

	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//Unbind vertex array
	glBindVertexArray(0);
}

void TextManager::Load()
{
	//Load shader
	mFontShader.Load("Assets/Shaders/FontShader.vert", "Assets/Shaders/FontShader.frag");

	//Load fonts
	LoadFonts();
}

void TextManager::SetDialogueText(std::string _text)
{
	mDialogueText = _text;
}

void TextManager::SetOpeningDialogueText(std::string _text, std::string _font)
{
	mOpeningDialogueText = _text;
	mOpeningDialogueFont = _font;
}

void TextManager::SetVisualConsoleText(std::string _text, std::string _font, float _dur)
{
	mVisualConsoleText = _text;
	mVisualConsoleFont = _font;
	mEraseVisualConsoleTime = _dur;
	mEraseVisualConsoleFont = true;
}

void TextManager::SetVisualConsoleColor(Vec3 _color)
{
	mVisualConsoleFontColor = _color;
}

void TextManager::SetVisualConsoleDelete()
{
	mVisualConsoleText = "";
}

void TextManager::Update()
{
	if (InputManager::GetInstance()->GetKeyTriggered(KeyCode::G))
	{
		mShowFPSText = !mShowFPSText;

		if (mShowFPSText == false)
		{
			for (auto i : EntityManager::GetInstance()->mEntityList)
			{
				if (i.GetName() == "FPSCounter")
				{
					GameUITextHandler* text = i.GetComponent<GameUITextHandler>();
					if (text != nullptr)
						text->mText = "";
				}
			}
		}
	}
}

void TextManager::Render()
{
	Vec3 cameraPos = GraphicsSystem::GetInstance()->mCamera.mCurrCamInfo.mPosition;
	Vec3 worldSize = GraphicsSystem::GetInstance()->mWorldSize * (1 / GraphicsSystem::GetInstance()->mCamera.mCurrCamInfo.mZoom);

	DrawFont(mVisualConsoleFont, mVisualConsoleText, 12, cameraPos.mX - worldSize.mX / 2, cameraPos.mY + worldSize.mY / 2 - 12.f, mVisualConsoleFontColor, WindowsManager::GetInstance()->mSceneTrueOpacity);
	if(mEraseVisualConsoleFont)
	{
		if (mEraseVisualConsoleElapsed >= mEraseVisualConsoleTime)
		{
			mVisualConsoleText = "";
			mEraseVisualConsoleFont = false;
			mEraseVisualConsoleElapsed = 0.0f;
		}
		else
		{
			mEraseVisualConsoleElapsed += WindowsManager::GetInstance()->mDeltaTime;
		}
	}

	DrawFont(mOpeningDialogueFont, mOpeningDialogueText, 8, cameraPos.mX - worldSize.mX / 2, cameraPos.mY + worldSize.mY / 2 - 12.f, Vector3(0.75f, 0.75f, 0.75f), WindowsManager::GetInstance()->mSceneTrueOpacity);


	std::stringstream sstream;
	sstream << mDialogueText;
	std::string fpsText = sstream.str();
	fpsText = LongTextSplicer("IosevkaNerdFont-Bold", fpsText, 9 + 1); //additional for leniency

	float fontSize = 9.f;
	float additionalSpace = 10.f * ((float)GetNumberOfBackslashN(fpsText)) + fontSize / 2;

	float moreAdditionalSize = 0.f;
	if (GraphicsSystem::GetInstance()->mCamera.mCurrCamInfo.mZoom == 1.f)
		moreAdditionalSize = 10.f;

	mTextboxSize = DrawFont("IosevkaNerdFont-Bold", fpsText, fontSize, cameraPos.mX - (fontSize / 10.f) * (int)(fpsText.size() / 2) + additionalSpace + moreAdditionalSize + DialogueTextOffsetX, cameraPos.mY - (worldSize.mY / 2) + additionalSpace + DialogueTextOffsetY, glm::vec3(1, 1, 1), WindowsManager::GetInstance()->mSceneTrueOpacity, worldSize.mX / 2 / (GLfloat)TEXT_WRAP_FLOAT_VALUE);
	//DebugLogger::GetInstance()->Log("X: %s\n Y: %s\n", textboxSize.x, textboxSize.y);

	//Draw all text inside RenderInformation vector for the frame
	for (auto& i : mTextRenderInformation)
	{
		DrawFont(i.mFontName, i.mText, i.mFontSize, i.mPosx, i.mPosy, i.mColor, i.mAlpha);
	}
	//Clear the vector
	mTextRenderInformation.clear();

	//Displaying the FPSText
	if (mShowFPSText)
	{
		for (auto i : EntityManager::GetInstance()->mEntityList)
		{
			if (i.GetName() == "FPSCounter")
			{
				GameUITextHandler* text = i.GetComponent<GameUITextHandler>();
				if (text != nullptr)
					text->mText = "FPS:" + std::to_string(CoreEngine::GetInstance()->mRenderFPS);

				Transform* transform = i.GetComponent<Transform>();
				if (transform != nullptr)
				{
					transform->SetPosition(GraphicsSystem::GetInstance()->mCamera.mCurrCamInfo.mPosition);
					worldSize = GraphicsSystem::GetInstance()->mWorldSize * (1 / GraphicsSystem::GetInstance()->mCamera.mCurrCamInfo.mZoom);
					transform->SetPosition(Vector3(transform->GetPosition().mX , transform->GetPosition().mY + (worldSize.mY / 2) - 15, transform->GetPosition().mZ));
				}
			}
		}
	}
}

glm::vec2 TextManager::GetTextboxSize()
{
	if (mTextboxSize.x == NULL) return glm::vec2(0, 0);

	return mTextboxSize;
}

std::string TextManager::LongTextSplicer(std::string _fontName, std::string _text, GLfloat _fontSize)
{
	std::string _newText = "";
	GLfloat _length = 0.f;
	//Iterate through all characters
	std::string::const_iterator c;
	for (c = _text.begin(); c != _text.end(); c++)
	{
		FontCharacter ch = mFontList[_fontName][*c];
		if (*c == '\n')
		{
			_newText += *c;
			_length = 0;
		}
		else
		{
			if (*c == ' ')
			{
				GLfloat stringWidth = 0.f;
				std::string::const_iterator c_j;
				for (c_j = c, ++c_j; c_j != _text.end() && *c_j != '\n' && *c_j != ' '; c_j++)
				{
					FontCharacter ch_j = mFontList[_fontName][*c_j];
					stringWidth += (ch_j.mAdvance >> 6) * _fontSize;
				}

				if (_length + stringWidth >= TEXT_WRAP_GLFLOAT_VALUE)
				{
					_newText += '\n';
					_length = 0;
				}
				else
				{
					_newText += *c;
					_length += (ch.mAdvance >> 6) * _fontSize;
				}
			}
			else
			{
				_newText += *c;
				_length += (ch.mAdvance >> 6) * _fontSize;

				if (_length >= TEXT_WRAP_GLFLOAT_VALUE)
				{
					_newText += '\n';
					_length = 0;
				}
			}
		}
	}
	return _newText;
}

GLfloat TextManager::GetStringWidth(std::string _fontName, std::string _text, GLfloat _fontSize)
{
	GLfloat _length = 0.f;
	//Iterate through all characters
	std::string::const_iterator c;
	for (c = _text.begin(); c != _text.end(); c++)
	{
		FontCharacter ch = mFontList[_fontName][*c];
		_length += (ch.mAdvance >> 6) * _fontSize;
	}
	return _length;
}

int TextManager::GetNumberOfBackslashN(std::string _text)
{
	int _count = 0;
	//Iterate through all characters
	std::string::const_iterator c;
	for (c = _text.begin(); c != _text.end(); c++)
	{
		if (*c == '\n')
		{
			_count++;
		}
	}
	return _count;
}

void TextManager::Unload()
{
	//Delete all character textures
	for (auto& mapObj : mFontList)
	{
		for (auto& charObj : mapObj.second)
			glDeleteTextures(1, &charObj.second.mTexID);
	}

#if _DEBUG
	DebugLogger::GetInstance()->Log("Deleting font shader\n");
#endif // _DEBUG
	mFontShader.DeleteShaderProgram();

	//Clean up Library
	FT_Done_FreeType(mLibrary);

	//Delete buffers
	glDeleteVertexArrays(1, &mVaoid);
	glDeleteBuffers(1, &mVboid);

	//Destroy instance
	Destroy();
}

void TextManager::DrawText(std::string _fontName, std::string _text, GLfloat _fontSize, GLfloat _posx, GLfloat _posy, Vector3 _color, int _renderLayer, float _alpha)
{
	TextInformation newTextInfo{ _fontName, _text, _fontSize, _posx, _posy, _color, _renderLayer, _alpha};
	mTextRenderInformation.push_back(newTextInfo);
}

void TextManager::RenderText(int _renderLayer)
{
	UNREFERENCED_PARAMETER(_renderLayer);
	//for (auto& i : mTextRenderInformation)
	//{
	//	if (i.mRenderLayer == _renderLayer)
	//	{
	//		DrawFont(i.mFontName, i.mText, i.mFontSize, i.mPosx, i.mPosy, i.mColor, i.mAlpha);
	//	}
	//}
}

void TextManager::ClearRenderInformation()
{
	mTextRenderInformation.clear();
}

glm::vec2 TextManager::DrawFont(std::string _fontName, std::string _text, GLfloat _fontSize, GLfloat _posx, GLfloat _posy, Vector3 _color, GLfloat _alpha, GLfloat _maxWidth_Visual)
{
	glm::vec2 finalTextSize = glm::vec2(0.f, 0.f);
	glm::vec2 addedTextboxSize = glm::vec2(0.f, 21.f);

	// activate corresponding render state	
	mFontShader.Use();
	mFontShader.SetUniform("textColor", glm::vec3{ _color.X(), _color.Y(), _color.Z()});
	mFontShader.SetUniform("xformMat", glm::mat3(GraphicsSystem::GetInstance()->mCamera.mXformMat));
	mFontShader.SetUniform("alphaVal", _alpha);

	//Bind buffers
	glBindFramebuffer(GL_FRAMEBUFFER, GraphicsSystem::GetInstance()->mFrameBufferObject);
	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(mVaoid);

	//Adjust font size based on face size
	_fontSize /= mFontFaceSize;

	GLfloat OriginalPosX = _posx;

	bool TextInitialLengthVerified = false;

	//Iterate through all characters
	std::string::const_iterator c;
	for (c = _text.begin(); c != _text.end(); c++)
	{
		FontCharacter ch = mFontList[_fontName][*c];

		if (*c == '\n')
		{
			_posx = OriginalPosX; addedTextboxSize.x = OriginalPosX;
			_posy -= 10.f; addedTextboxSize.y += 10.f;
			TextInitialLengthVerified = false;
			continue;
		}

		if (!TextInitialLengthVerified)
		{
			GLfloat stringWidth = 0.f;
			std::string::const_iterator c_j;
			for (c_j = c; c_j != _text.end() && *c_j != '\n'; c_j++)
			{
				FontCharacter ch_j = mFontList[_fontName][*c_j];
				stringWidth += (ch_j.mAdvance >> 6) * _fontSize;
			}
			_posx -= stringWidth * _maxWidth_Visual; addedTextboxSize.x -= stringWidth * _maxWidth_Visual;
			TextInitialLengthVerified = true;
		}

		float xpos = _posx + ch.mBearing.mX * _fontSize; //addedTextboxSize.x += ch.mBearing.mX * _fontSize;
		float ypos = _posy - (ch.mTextSize.mY - ch.mBearing.mY) * _fontSize;

		if (fabsf(addedTextboxSize.x + ch.mBearing.mX * _fontSize) > finalTextSize.x) finalTextSize.x = fabsf(addedTextboxSize.x + ch.mBearing.mX * _fontSize);
		if (fabsf(addedTextboxSize.y) > finalTextSize.y) finalTextSize.y = fabsf(addedTextboxSize.y);

		float w = ch.mTextSize.mX * _fontSize;
		float h = ch.mTextSize.mY * _fontSize;

		//Update VBO for each character
		float vertices[6][4] =
		{
			{ xpos,     ypos + h,   0.0f, 0.0f },
			{ xpos,     ypos,       0.0f, 1.0f },
			{ xpos + w, ypos,       1.0f, 1.0f },

			{ xpos,     ypos + h,   0.0f, 0.0f },
			{ xpos + w, ypos,       1.0f, 1.0f },
			{ xpos + w, ypos + h,   1.0f, 0.0f }
		};

		//Bind character texture
		//glBindTextureUnit(ch.mTexID, ch.mTexID);
		glBindTexture(GL_TEXTURE_2D, ch.mTexID);
		//mFontShader.SetUniform("uTex2D", (GLint)ch.mTexID);

		//Update VBO
		glBindBuffer(GL_ARRAY_BUFFER, mVboid);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		//Render quad
		glDrawArrays(GL_TRIANGLES, 0, 6);

		//Advance cursors for next glyph (note that advance is number of 1/64 pixels)
		_posx += (ch.mAdvance >> 6) * _fontSize; // Bitshift by 6 to get value in pixels (2^6 = 64)
		addedTextboxSize.x += (ch.mAdvance >> 6) * _fontSize;
	}
	//Unuse shader
	mFontShader.UnUse();

	//Unbind buffers
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	return finalTextSize;
}

void TextManager::LoadFonts()
{
	for (size_t i = 0; i < mFontQueue.size(); i++)
	{
		//Create face
		FT_Face face = nullptr;
		if (FT_New_Face(mLibrary, mFontQueue.at(i).first.c_str(), 0, &face))
		{
#if _ENGINE
			DebugLogger::GetInstance()->Log("ERROR: Freetype: Unable to load font %s\n", mFontQueue.at(i).second.c_str());
#endif // _ENGINE
			return;
		}

		//Set face size
		FT_Set_Pixel_Sizes(face, 0, mFontFaceSize);

		//Get all text letters from the font
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // Disable byte-alignment restriction
		for (unsigned char c = 32; c < 127; c++)
		{
			// Load letter
			if (FT_Load_Char(face, c, FT_LOAD_RENDER))
			{
#if _ENGINE
				DebugLogger::GetInstance()->Log("ERROR:: Freetype: Failed to load letter \'%c\'\n", c);
#endif // _ENGINE
				continue;
			}

			// Generate texture
			unsigned int texture;
			glGenTextures(1, &texture);
			glBindTexture(GL_TEXTURE_2D, texture);
			glTexImage2D(GL_TEXTURE_2D,
				0,
				GL_RED,
				face->glyph->bitmap.width,
				face->glyph->bitmap.rows,
				0,
				GL_RED,
				GL_UNSIGNED_BYTE,
				face->glyph->bitmap.buffer);

			// Set texture options
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			// Create the character with the details
			FontCharacter character = {
				texture,
				Vector3((float)face->glyph->bitmap.width, (float)face->glyph->bitmap.rows, 1.f),
				Vector3((float)face->glyph->bitmap_left, (float)face->glyph->bitmap_top, 1.f),
				(unsigned int)face->glyph->advance.x
			};

			//Insert character into map
			mFontList[mFontQueue.at(i).second.c_str()].insert(std::pair<char, FontCharacter>(c, character));
		}

		//Cleanup
		FT_Done_Face(face);
	}
}

void TextManager::CreateFontAsset(std::string _filePath, std::string _fontName)
{
	mFontQueue.push_back(std::make_pair(_filePath, _fontName));
#if _ENGINE
	DebugLogger::GetInstance()->Log("Font: %s in font queue with path: <%s>\n", _fontName.c_str(), _filePath.c_str());
#endif // _ENGINE
}

void TextManager::ClearAllText()
{
	mTextRenderInformation.clear();
	mDialogueText = mVisualConsoleText = mVisualConsoleFont = mOpeningDialogueText = mOpeningDialogueFont = "";
}