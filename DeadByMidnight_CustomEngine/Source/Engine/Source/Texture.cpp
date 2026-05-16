/*!
All content © 2023 DigiPen Institute of Technology Singapore, all rights reserved.
@file       Texture.cpp
@author     CAI Wenjin (100%)
@co-author

----------------------------------------------------------------------------

@course     CSD2401F23
@section    CSD2401F23-A
@project    CSD2401F23 Software Engineering Project 3

@brief      Texture class definitions.
*//*______________________________________________________________________*/

#include "Texture.h"
#include <iostream>

#pragma warning(push, 0)
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#pragma warning(pop)

#if _ENGINE
#include "DebugLogger.h"
#endif // _ENGINE

Texture::Texture()
	: mTexWidth{0}, mTexHeight{0}, mTexBytesPerPixel{0}, mTexSize{0}
{
}

Texture::~Texture()
{
}

GLuint Texture::LoadTexture(std::string _fileName)
{
	//GLuint texobj;
	//GLboolean* img = stbi_load(_fileName.c_str(), &mTexWidth, &mTexHeight, &mTexBytesPerPixel, 0);
	//if (!img)
	//{
	//	std::cout << "Error in loading image from: " << _fileName << std::endl;
	//}
	//glCreateTextures(GL_TEXTURE_2D, 1, &texobj);
	//glTextureStorage2D(texobj, 1, GL_RGBA8, mTexWidth, mTexHeight);
	//glTextureSubImage2D(texobj, 0, 0, 0, mTexWidth, mTexHeight, GL_RGBA, GL_UNSIGNED_BYTE, img);
	//mTexSize = mTexWidth * mTexHeight * mTexBytesPerPixel;
	//stbi_image_free(img);
	//mTexID = texobj;
	//return texobj;

	/*
	* Alternative way to generate textures using glTexImage2D
	*/
	GLuint texobj;
	//glCreateTextures(GL_TEXTURE_2D, 1, &texobj);?????
	glGenTextures(1, &texobj);
	glBindTexture(GL_TEXTURE_2D, texobj);
	// set the texture wrapping/filtering options (on the currently bound texture object)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 8); //Set mipmap level
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	GLboolean* img = stbi_load(_fileName.c_str(), &mTexWidth, &mTexHeight, &mTexBytesPerPixel, 0);
	if (!img)
	{
#if _ENGINE
		DebugLogger::GetInstance()->Log("Failed to load texture");
#endif // _ENGINE
	}
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, mTexWidth, mTexHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, img);
	glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(img);
	mTexSize = mTexWidth * mTexHeight * mTexBytesPerPixel;
	mTexID = texobj;
	return mTexID;
}

void Texture::CreateImageAsset(std::string _fullPath, void*& _dataPtr)
{
	Texture* newText = new Texture();
	newText->LoadTexture(_fullPath);
	_dataPtr = newText;
}

void Texture::DeleteImageAsset(void*& _dataPtr)
{
	glDeleteTextures(1, &static_cast<Texture*>(_dataPtr)->mTexID);
	delete static_cast<Texture*>(_dataPtr);
}

GLint Texture::GetTexWidth() const
{
	return mTexWidth;
}

GLint Texture::GetTexHeight() const
{
	return mTexHeight;
}

GLint Texture::GetTexBytesPerPixel() const
{
	return mTexBytesPerPixel;
}

GLint Texture::GetTexSize() const
{
	return mTexSize;
}

void Texture::SetDimensions(GLint _width, GLint _height)
{
	mTexWidth = _width;
	mTexHeight = _height;
}