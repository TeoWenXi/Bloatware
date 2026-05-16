/*!
All content © 2023 DigiPen Institute of Technology Singapore, all rights reserved.
@file       GfxDebug.vert
@author     CAI Wenjin (100%)
@co-author	

----------------------------------------------------------------------------

@course     CSD2401F23
@section    CSD2401F23-A
@project    CSD2401F23 Software Engineering Project 3

@brief      This file implements the vertex shader for graphics debugging use.
*//*______________________________________________________________________*/

#version 450 core

// inputs
layout(location = 0) in vec2 aVertexPosition;
layout(location = 1) in vec4 aVertexColor;

// uniform variable inputs
uniform mat3 uModel_to_NDC;				
uniform bool uShowVelocityLine;		// flag to show velocity line
uniform bool uIsColliding;				// flag to show that the object is colliding with something

// outputs
layout(location = 0) out vec4 vColor;

void main()
{
	// if show velocity line is enabled WIP
	if (uShowVelocityLine)
	{
	}

	if (uIsColliding)
		vColor = vec4(1, 0, 0, 1);
	else
		// color the pixels to green first
		vColor = vec4(0, 0.7, 0, 1);
	
	gl_Position = vec4(vec2(uModel_to_NDC * vec3(aVertexPosition, 1.f)), 0.0, 1.0);
}