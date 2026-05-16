/*!
All content © 2024 DigiPen Institute of Technology Singapore, all rights reserved.
@file       GfxScaleRotate.frag
@author     CAI Wenjin (90%)
@co-author	SIM Mong Chuan (10%)

----------------------------------------------------------------------------

@course     CSD2451F24
@section    CSD2451F24-A
@project    CSD2451F24 Software Engineering Project 4

@brief      This file implements the fragment shader for scale and rotate highlighting use.
*//*______________________________________________________________________*/

#version 450 core

// inputs
layout (location=0) in vec4 vInterpColor;

// outputs
layout (location=0) out vec4 fFragColor;

void main () 
{
	fFragColor = vInterpColor;
}
