/*!
All content © 2024 DigiPen Institute of Technology Singapore, all rights reserved.
@file       GfxScaleRotate.vert
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
layout(location = 0) in vec2 aVertexPosition;
layout(location = 1) in vec4 aVertexColor;

// uniform variable inputs
uniform mat3 uModel_to_NDC;				
//uniform bool uShowVelocityLine;			// flag to show velocity line
//uniform bool uIsColliding;				 flag to show that the object is colliding with something

// outputs
layout(location = 0) out vec4 vColor;

void main()
{
	// if show velocity line is enabled WIP
//	if (uShowVelocityLine)
//	{
//	}

//	if (uIsColliding)
//		vColor = vec4(1, 0, 0, 1);
//	else
		// color the pixels to yellow first
		vColor = vec4(0.7, 0.7, 0, 1);
	
	gl_Position = vec4(vec2(uModel_to_NDC * vec3(aVertexPosition, 1.f)), 0.0, 1.0);
}