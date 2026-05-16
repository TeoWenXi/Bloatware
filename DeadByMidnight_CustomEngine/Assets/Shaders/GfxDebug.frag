/*!
All content © 2023 DigiPen Institute of Technology Singapore, all rights reserved.
@file       GfxDebug.frag
@author     CAI Wenjin (100%)
@co-author	

----------------------------------------------------------------------------

@course     CSD2401F23
@section    CSD2401F23-A
@project    CSD2401F23 Software Engineering Project 3

@brief      This file implements the fragment shader for graphics debugging use.
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
