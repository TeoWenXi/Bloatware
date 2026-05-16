/*!
All content © 2023 DigiPen Institute of Technology Singapore, all rights reserved.
@file       KeyWallShader.vert
@author     Jovan Zhuo Wen LOW (100%)
@co-author	

----------------------------------------------------------------------------

@course     CSD2401F23
@section    CSD2401F23-A
@project    CSD2401F23 Software Engineering Project 3

@brief      This file implements the vertex shader for OpenGL use, integrated to create Key Wall texture.
*//*______________________________________________________________________*/

#version 450 core

// inputs
layout(location = 0) in vec2 aVertexPosition;
layout(location = 1) in vec4 aVertexColor;
layout(location = 2) in vec2 aTexCoord;

// uniform variable inputs
uniform mat3 uModel_to_NDC;

// outputs
layout(location = 0) out vec4 vColor;
layout(location = 1) out vec2 vTexPos;


void main()
{
	gl_Position = vec4(vec2(uModel_to_NDC * vec3(aVertexPosition, 1.f)), 0.0, 1.0);
	vColor = aVertexColor;
	vTexPos = aTexCoord;
}