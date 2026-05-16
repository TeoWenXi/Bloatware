/*!
All content © 2023 DigiPen Institute of Technology Singapore, all rights reserved.
@file       Animation.frag
@author     CAI Wenjin (100%)
@co-author	

----------------------------------------------------------------------------

@course     CSD2401F23
@section    CSD2401F23-A
@project    CSD2401F23 Software Engineering Project 3

@brief      This file implements the fragment shader for sprite animations.
*//*______________________________________________________________________*/

#version 450 core

// inputs
layout (location=0) in vec4 vInterpColor;
layout (location=1) in vec2 vTexCoord;

// uniform variable inputs
uniform sampler2D uTex2d;

// outputs
layout (location=0) out vec4 fFragColor;


void main () 
{
	fFragColor = texture(uTex2d, vTexCoord);
}
