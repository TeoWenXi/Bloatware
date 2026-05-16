/*!
All content � 2023 DigiPen Institute of Technology Singapore, all rights reserved.
@file       NotBasicShader.frag
@author     TEO Wen Xi (100%)
@co-author	

----------------------------------------------------------------------------

@course     CSD2401F23
@section    CSD2401F23-A
@project    CSD2401F23 Software Engineering Project 3

@brief      This file implements the fragment shader for OpenGL use.
*//*______________________________________________________________________*/

#version 450 core
layout (location=1) in vec2 vTexCoord;
layout (location=0) out vec4 fFragColor;
									
// Uniform variable representing 2D Texture.
uniform sampler2D	uTex2d;
						
void main()
{
	fFragColor = vec4(texture(uTex2d, vTexCoord));
	//fFragColor = vec4(fFragColor.r * 0.4f, 1.0f, fFragColor.b * 0.4f, 1.0f);
}
                  