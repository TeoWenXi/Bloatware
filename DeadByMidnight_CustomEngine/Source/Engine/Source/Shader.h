/*!
All content © 2023 DigiPen Institute of Technology Singapore, all rights reserved.
@file       Shader.h
@author     CAI Wenjin (90%)
@co-author	TEO Wen Xi (5%)
@co-author	Jovan Zhuo Wen LOW (5%)

----------------------------------------------------------------------------

@course     CSD2401F23
@section    CSD2401F23-A
@project    CSD2401F23 Software Engineering Project 3

@brief      Shader class declarations.
*//*______________________________________________________________________*/

#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>
#include <map>
#include <utility>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

#include <glm/glm.hpp> //for set uniform(mat3)

class Shader
{
public:
	GLuint mShaderID;

	/*!
	* @brief
	* Reads the source code from the shader from the given file path.
	* @param _filepath
	* The location of where the shader is located at.
	* @return
	* The string containing the source code of the shader file.
	*/
	std::string ReadShaderFromFile(const std::string& _filepath);

	/*!
	 * @brief
	 * Compiles the specified shader from the given source code.
	 * @param _shaderType
	 * The type of shader to compile, currently only GL_VERTEX_SHADER and GL_FRAGMENT_SHADER
	 * are used.
	 * @param _source
	 * The source code of the shader file to compile.
	 * @return
	 * The ID of the compiled shader.
	*/
	GLuint CompileShader(GLenum _shaderType, const char* _source);

	/*!
	 * @brief
	 * Links the vertex and fragment shaders to create the shader program.
	 * @param _vtxShdr
	 * The ID of the vertex shader to link.
	 * @param _frgShdr
	 * The ID of the fragment shader to link.
	 * @return
	 * The ID of the linked shader program.
	*/
	GLuint LinkShaders(GLuint _vtxShdr, GLuint _frgShdr);

	/*!
	 * @brief
	 * Loads the shaders from the filepath.
	 * @param shdrName
	 * The _name of the pair of shaders.
	 * @param _vtxShdr
	 * File path to where the vertex shader is located.
	 * @param _frgShdr
	 * File path to where the fragment shader is located.
	*/
	void Load(std::string _vtxShdr, std::string _frgShdr);

	/*!
	 * @brief
	 * Start using the shaders.
	*/
	void Use();

	/*!
	 * @brief
	 * Stop using the shaders.
	*/
	void UnUse();

	/*!
	 * @brief
	 * Sets the variable in shader with int value.
	 * @param _name
	 * Name of the variable in the shader.
	 * @param _val
	 * The value to set into the shader.
	*/
	void SetUniform(const std::string& _name, GLint _val) const;

	/*!
	 * @brief
	 * Sets the variable in shader with float value.
	 * @param _name
	 * Name of the variable in the shader.
	 * @param _val
	 * The value to set into the shader.
	*/
	void SetUniform(const std::string& _name, GLfloat _val) const;

	/*!
	 * @brief
	 * Sets the variable in shader with boolean value.
	 * @param _name
	 * Name of the variable in the shader.
	 * @param _val
	 * The value to set into the shader.
	*/
	void SetUniform(const std::string& _name, GLboolean _val) const;

	/*!
	 * @brief
	 * Sets the variable in shader with double value.
	 * @param _name
	 * Name of the variable in the shader.
	 * @param _val
	 * The value to set into the shader.
	*/
	void SetUniform(const std::string& _name, GLdouble _val) const;

	/*!
	 * @brief
	 * Sets the variable in shader with glm::vec3 value
	 * @param _name
	 * Name of the variable in the shader.
	 * @param _val
	 * The value to set into the shader.
	*/
	void SetUniform(const std::string& _name, glm::vec3 _val) const;

	/*!
	 * @brief
	 * Sets the variable in shader with glm::vec4 value
	 * @param _name
	 * Name of the variable in the shader.
	 * @param _val
	 * The value to set into the shader.
	*/
	void SetUniform(const std::string& _name, glm::vec4 _val) const;

	/*!
	 * @brief
	 * Sets the variable in shader with glm::mat3 value
	 * @param _name
	 * Name of the variable in the shader.
	 * @param _val
	 * The value to set into the shader.
	*/
	void SetUniform(GLchar const* _name, glm::mat3 const& _val) const;

	/*!
	 * @brief
	 * Sets the variable in shader with glm::mat4 value
	 * @param _name
	 * Name of the variable in the shader.
	 * @param _val
	 * The value to set into the shader.
	*/
	void SetUniform(GLchar const* _name, glm::mat4 const& _val) const;

	/*!
	 * @brief
	 * Deletes the shader programs.
	*/
	void DeleteShaderProgram();

	/*!
	 * @brief
	 * Create the shader assets from the file directory.
	 * @param _fullPath
	 * The file directory containing the shaders.
	 * @param _dataPtr
	 * The pointer to the shader data.
	*/
	static void CreateShaderAsset(std::string _fullPath, void*& _dataPtr);

	/*!
	 * @brief
	 * Frees the shader asset back into free store.
	 * @param _dataPtr
	 * The pointer to the shader data.
	*/
	static void DeleteShaderAsset(void*& _dataPtr);
};

#endif // !SHADER_H

