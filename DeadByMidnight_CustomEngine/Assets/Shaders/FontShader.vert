/*!
All content © 2023 DigiPen Institute of Technology Singapore, all rights reserved.
@file       FontShader.vert
@author     TEO Wen Xi
@co-author

@course     CSD2401F23
@section    CSD2401F23-A
@project    CSD2401F23 Software Engineering Project 3

@brief      
	This file implements the font vertex shader for drawing of fonts using OpenGL.
*//*______________________________________________________________________*/

#version 450 core

layout(location = 0) in vec4 aVertexPosition;

layout(location = 0) out vec2 vTexCoords;

uniform mat3 xformMat;

void main()
{
	gl_Position = vec4(vec2(xformMat * vec3(aVertexPosition.xy, 1.f)), 0.0, 1.0);
	vTexCoords = aVertexPosition.zw;
}