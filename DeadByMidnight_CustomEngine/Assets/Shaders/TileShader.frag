/*!
All content © 2023 DigiPen Institute of Technology Singapore, all rights reserved.
@file       TileShader.frag
@author     Jovan Zhuo Wen LOW (100%)
@co-author	

----------------------------------------------------------------------------

@course     CSD2401F23
@section    CSD2401F23-A
@project    CSD2401F23 Software Engineering Project 3

@brief      This file implements the tiles' fragment shader for OpenGL use.
*//*______________________________________________________________________*/

#version 450 core

// inputs
layout (location=0) in vec4 vInterpColor;
layout (location=1) in vec2 vTexCoord;

// uniform variable inputs
uniform sampler2D uTex2d;
uniform bool uTextureEnabled;
uniform float uIntensity;

// outputs
layout (location=0) out vec4 fFragColor;

void main () 
{
	if (uTextureEnabled)
	{
		fFragColor = texture(uTex2d, vTexCoord);
		float lowestNumber = 0.9f + 0.1f * (1.0f - pow(fFragColor.a, 3.0f));
		fFragColor = vec4(fFragColor.r, fFragColor.gb * (lowestNumber + (1.0f - lowestNumber) * (1.0f - uIntensity)), fFragColor.a * (0.75f + 0.25f * (1.0f - uIntensity)));
	}
	else
		fFragColor = vec4(vInterpColor.r, vInterpColor.gb * (0.7f + 0.3f * (1.0f - uIntensity)), 0.75f + 0.25f * (1.0f - uIntensity));
}
