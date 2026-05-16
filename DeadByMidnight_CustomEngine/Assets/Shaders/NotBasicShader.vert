/*!
All content � 2023 DigiPen Institute of Technology Singapore, all rights reserved.
@file       NotBasicShader.vert
@author     TEO Wen Xi (100%)
@co-author	

----------------------------------------------------------------------------

@course     CSD2401F23
@section    CSD2401F23-A
@project    CSD2401F23 Software Engineering Project 3

@brief      This file implements the vertex shader for OpenGL use.
*//*______________________________________________________________________*/

#version 450 core
layout (location=0) in vec2 aVertexPosition;
layout (location=1) in vec2 aTexCoord;
layout (location=1) out vec2 vTexCoord;

void main() 
{
	// Calculate the position in normalized device coordinates (NDC) by transforming the vertex position
	gl_Position = vec4(aVertexPosition, 0.f, 1.f);
	// Set the output texture coordinate to the input texture coordinate if not task 0-2.
	vTexCoord = aTexCoord;
}