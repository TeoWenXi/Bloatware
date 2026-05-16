/*!
All content © 2023 DigiPen Institute of Technology Singapore, all rights reserved.
@file       Texture.h
@author     CAI Wenjin (100%)
@co-author

----------------------------------------------------------------------------

@course     CSD2401F23
@section    CSD2401F23-A
@project    CSD2401F23 Software Engineering Project 3

@brief      Texture class declarations.
*//*______________________________________________________________________*/

#ifndef TEXTURE_H
#define TEXTURE_H

#include <glad/glad.h>
#include <string>

class Texture
{
private:
	GLint mTexWidth{};
	GLint mTexHeight{};
	GLint mTexBytesPerPixel{};
	GLint mTexSize{};

public:
	GLuint mTexID{};

	/*!
	 * @brief
	 * Constructor for texture class.
	*/
	Texture();
	/*!
	 * @brief
	 * Destructor for texture class.
	*/
	~Texture();

	/*!
	 * @brief
	 * Loads the texture from the given filename.
	 * @param _fileName
	 * The texture filename to load.
	 * @return
	 * The handle to the texture object after loading.
	*/
	GLuint LoadTexture(std::string _fileName);
	/*!
	 * @brief
	 * Create the image/texture assets from the file directory.
	 * @param _fullPath
	 * The file directory containing the images/textures.
	 * @param _dataPtr
	 * The pointer to the image/texture data.
	*/
	static void CreateImageAsset(std::string _fullPath, void*& _dataPtr);
	/*!
	 * @brief
	 * Frees the image/texture asset back into free store.
	 * @param _dataPtr
	 * The pointer to the image/texture data.
	*/
	static void DeleteImageAsset(void*& _dataPtr);
	/*!
	 * @brief
	 * Gets the image/texture's width.
	 * @return
	 * The width of the image/texture.
	*/
	GLint GetTexWidth() const;
	/*!
	 * @brief
	 * Gets the image/textures height.
	 * @return
	 * The height of the image/texture.
	*/
	GLint GetTexHeight() const;
	/*!
	 * @brief
	 * Gets the number of bytes per pixel in the image/texture.
	 * @return
	 * The bytes per pixel in the image/texture.
	*/
	GLint GetTexBytesPerPixel() const;
	/*!
	 * @brief
	 * Gets the image/texture's total size.
	 * @return
	 * The total size of the image/texture.
	*/
	GLint GetTexSize() const;

	/*!
	 * @brief
	 * Sets the texture's dimensions
	 * @param _width
	 * Texture Width to be set.
	 * @param _height
	 * Texture Height to be set.
	*/
	void SetDimensions(GLint _width, GLint _height);
};

#endif // !TEXTURE_H

