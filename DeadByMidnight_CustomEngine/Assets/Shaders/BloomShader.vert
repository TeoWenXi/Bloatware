/*!
All content © 2023 DigiPen Institute of Technology Singapore, all rights reserved.
@file       BloomShader.vert
@author     Jovan Zhuo Wen LOW (100%)
@co-author	

----------------------------------------------------------------------------

@course     CSD2401F23
@section    CSD2401F23-A
@project    CSD2401F23 Software Engineering Project 3

@brief      This file implements the vertex shader for OpenGL use, used for Post-Processing.
*//*______________________________________________________________________*/

#version 450 core
layout(location = 0) in vec2 aVertexPosition;
layout(location = 1) in vec4 aVertexColor;
layout(location = 2) in vec2 aTexCoord;
//layout (location=0) in vec4 vertex; // <vec2 position, vec2 texCoords>

layout(location = 0) out vec4 vColor;
layout(location = 1) out vec2 vTexPos;

void main()
{
    gl_Position = vec4(aVertexPosition, 0.0f, 1.0f); 
    vColor = aVertexColor;
	vTexPos = aTexCoord;
} 