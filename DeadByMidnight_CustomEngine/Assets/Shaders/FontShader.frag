/*!
All content © 2023 DigiPen Institute of Technology Singapore, all rights reserved.
@file       FontShader.frag
@author     TEO Wen Xi
@co-author

@course     CSD2401F23
@section    CSD2401F23-A
@project    CSD2401F23 Software Engineering Project 3

@brief      
	This file implements the font fragment shader for drawing of fonts using OpenGL.
*//*______________________________________________________________________*/

#version 450 core

layout(location = 0) in vec2 vTexCoords;

layout(location = 0) out vec4 fFragColor;

uniform sampler2D uTex2D;
uniform vec3 textColor;
uniform float alphaVal;

void main () 
{
	vec4 sampledTex = vec4(1.0, 1.0, 1.0, texture(uTex2D, vTexCoords).r);
	fFragColor = vec4(textColor, alphaVal) * sampledTex;
}
