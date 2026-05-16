/*!
All content © 2023 DigiPen Institute of Technology Singapore, all rights reserved.
@file       BasicShader.frag
@author     TEO Wen Xi (100%)
@co-author	

----------------------------------------------------------------------------

@course     CSD2401F23
@section    CSD2401F23-A
@project    CSD2401F23 Software Engineering Project 3

@brief      This file implements the fragment shader for OpenGL use.
*//*______________________________________________________________________*/

#version 450 core

// inputs
layout (location=0) in vec4 vInterpColor;
layout (location=1) in vec2 vTexCoord;

// uniform variable inputs
uniform sampler2D uTex2d;
uniform bool uTextureEnabled;
uniform bool uCustomAlphaEnabled;
uniform float uAlpha;

// outputs
layout (location=0) out vec4 fFragColor;

void main () 
{
	// check if we are using customized alpha values
	if (uCustomAlphaEnabled)
	{
		// set pixel color based on whether the texture flag is enabled
		if (uTextureEnabled)
			fFragColor = vec4(texture(uTex2d, vTexCoord).rgb, texture(uTex2d, vTexCoord).a * uAlpha);
		else
			fFragColor = vec4(vInterpColor.rgb, uAlpha);
	}
	else
	{
		if (uTextureEnabled)
			fFragColor = vec4(texture(uTex2d, vTexCoord));
		else
			fFragColor = vInterpColor;
	}
}
