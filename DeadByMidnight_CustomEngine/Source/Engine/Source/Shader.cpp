/*!
All content © 2023 DigiPen Institute of Technology Singapore, all rights reserved.
@file       Shader.cpp
@author     CAI Wenjin (90%)
@co-author	TEO Wen Xi (5%)
@co-author	Jovan Zhuo Wen LOW (5%)

----------------------------------------------------------------------------

@course     CSD2401F23
@section    CSD2401F23-A
@project    CSD2401F23 Software Engineering Project 3

@brief      Shader class definitions.
*//*______________________________________________________________________*/
#include "Shader.h"
#if _ENGINE
#include "DebugLogger.h"
#endif // _ENGINE

std::string Shader::ReadShaderFromFile(const std::string& _filepath)
{
	std::ifstream shaderFile{ _filepath };
	if (!shaderFile)
	{
#if _ENGINE
		DebugLogger::GetInstance()->Log("Shader file not found: %s\n", _filepath.c_str());
#endif // _ENGINE
		return "";
	}

	std::stringstream shaderBuffer;
	shaderBuffer << shaderFile.rdbuf();
	shaderFile.close();

	return shaderBuffer.str();
}

GLuint Shader::CompileShader(GLenum _shaderType, const char* _source)
{
	GLuint shaderID = glCreateShader(_shaderType);
	glShaderSource(shaderID, 1, &_source, nullptr);
	glCompileShader(shaderID);

	GLint success;
	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		GLchar infoLog[512];
		glGetShaderInfoLog(shaderID, 512, nullptr, infoLog);
#if _ENGINE
		DebugLogger::GetInstance()->Log("Shader program linking failed: %s\n", infoLog);
#endif // _ENGINE
		return 0;
	}

	return shaderID;
}

GLuint Shader::LinkShaders(GLuint _vtxShdr, GLuint _frgShdr)
{
	GLuint programID = glCreateProgram();
	glAttachShader(programID, _vtxShdr);
	glAttachShader(programID, _frgShdr);
	glLinkProgram(programID);

	GLint success;
	glGetProgramiv(programID, GL_LINK_STATUS, &success);
	if (!success)
	{
		GLchar infoLog[512];
		glGetProgramInfoLog(programID, 512, nullptr, infoLog);
#if _ENGINE
		DebugLogger::GetInstance()->Log("Shader program linking failed: %s\n", infoLog);
#endif // _ENGINE
		return 0;
	}

	return programID;
}

void Shader::Load(std::string _vtxShdr, std::string _frgShdr)
{
	// read shader code from files
	std::string vtxCode = ReadShaderFromFile(_vtxShdr);
	std::string frgCode = ReadShaderFromFile(_frgShdr);

	// compile vertex shader
	GLuint vertexShader = CompileShader(GL_VERTEX_SHADER, vtxCode.c_str());
	if (vertexShader == 0)
		return;

	// compile fragment shader
	GLuint fragmentShader = CompileShader(GL_FRAGMENT_SHADER, frgCode.c_str());
	if (fragmentShader == 0)
	{
		// clean up the vertex shader if compilation failed
		glDeleteShader(vertexShader);
		return;
	}

	// link shader program
	mShaderID = LinkShaders(vertexShader, fragmentShader);

	// clean up shaders since successfully linked
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

void Shader::Use()
{
	glUseProgram(mShaderID);
}

void Shader::UnUse()
{
	glUseProgram(0);
}

void Shader::SetUniform(const std::string& _name, GLint _val) const
{
	GLint loc = glGetUniformLocation(mShaderID, _name.c_str());
	if (loc >= 0) {
		glUniform1i(loc, _val);
	}
	else {
#if _ENGINE
		//DebugLogger::GetInstance()->Log("Uniform variable %s doesn't exist\n", _name.c_str());
#endif // _ENGINE
	}
}

void Shader::SetUniform(const std::string& _name, GLfloat _val) const
{
	GLint loc = glGetUniformLocation(mShaderID, _name.c_str());
	if (loc >= 0) {
		glUniform1f(loc, _val);
	}
	else {
#if _ENGINE
		//DebugLogger::GetInstance()->Log("Uniform variable %s doesn't exist\n", _name.c_str());
#endif // _ENGINE
	}
}

void Shader::SetUniform(const std::string& _name, GLboolean _val) const
{
	GLint loc = glGetUniformLocation(mShaderID, _name.c_str());
	if (loc >= 0) {
		glUniform1i(loc, _val);
	}
	else {
#if _ENGINE
		//DebugLogger::GetInstance()->Log("Uniform variable %s doesn't exist\n", _name.c_str());
#endif // _ENGINE
	}
}

void Shader::SetUniform(const std::string& _name, GLdouble _val) const
{
	GLint loc = glGetUniformLocation(mShaderID, _name.c_str());
	if (loc >= 0)
	{
		glUniform1d(loc, _val);
	}
	else
	{
#if _ENGINE
		//DebugLogger::GetInstance()->Log("Uniform variable %s doesn't exist\n", _name.c_str());
#endif // _ENGINE
	}
}

void Shader::SetUniform(GLchar const* _name, glm::mat3 const& _val) const
{
	GLint loc = glGetUniformLocation(mShaderID, _name);
	if (loc >= 0) {
		glUniformMatrix3fv(loc, 1, GL_FALSE, &_val[0][0]);
	}
	else {
#if _ENGINE
		//DebugLogger::GetInstance()->Log("Uniform variable %s doesn't exist\n", _name);
#endif // _ENGINE
	}
}

void Shader::SetUniform(GLchar const* _name, glm::mat4 const& _val) const
{
	GLint loc = glGetUniformLocation(mShaderID, _name);
	if (loc >= 0) {
		glUniformMatrix4fv(loc, 1, GL_FALSE, &_val[0][0]);
	}
	else {
#if _ENGINE
		//DebugLogger::GetInstance()->Log("Uniform variable %s doesn't exist\n", _name);
#endif // _ENGINE
	}
}

void Shader::SetUniform(const std::string& _name, glm::vec3 _val) const
{
	GLint loc = glGetUniformLocation(mShaderID, _name.c_str());
	if (loc >= 0) {
		glUniform3f(loc, _val.x, _val.y, _val.z);
	}
	else {
#if _ENGINE
		//DebugLogger::GetInstance()->Log("Uniform variable %s doesn't exist\n", _name.c_str());
#endif // _ENGINE
	}
}

void Shader::SetUniform(const std::string& _name, glm::vec4 _val) const
{
	GLint loc = glGetUniformLocation(mShaderID, _name.c_str());
	if (loc >= 0) {
		glUniform4f(loc, _val.x, _val.y, _val.z, _val.w);
	}
	else {
#if _ENGINE
		//DebugLogger::GetInstance()->Log("Uniform variable %s doesn't exist\n", _name.c_str());
#endif // _ENGINE
	}
}

void Shader::DeleteShaderProgram()
{
	if (mShaderID > 0)
		glDeleteProgram(mShaderID);
}

void Shader::CreateShaderAsset(std::string _fullPath, void*& _dataPtr)
{
	size_t splitPpos = _fullPath.find_first_of('|');
	if (splitPpos == std::string::npos)
		return;//Error with shader/vertex paths

	_dataPtr = new Shader();
	static_cast<Shader*>(_dataPtr)->Load(_fullPath.substr(0, splitPpos), _fullPath.substr(splitPpos + 1, _fullPath.size() - splitPpos - 1));
}

void Shader::DeleteShaderAsset(void*& _dataPtr)
{
	static_cast<Shader*>(_dataPtr)->DeleteShaderProgram();
	delete _dataPtr;
}
