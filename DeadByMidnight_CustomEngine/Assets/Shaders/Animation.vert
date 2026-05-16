/*!
All content © 2023 DigiPen Institute of Technology Singapore, all rights reserved.
@file       Animation.vert
@author     CAI Wenjin (50%)
@co-author	Jovan Zhuo Wen Low (50%)

----------------------------------------------------------------------------

@course     CSD2401F23
@section    CSD2401F23-A
@project    CSD2401F23 Software Engineering Project 3

@brief      This file implements the vertex shader for sprite animations.
*//*______________________________________________________________________*/

#version 450 core

// inputs
layout(location = 0) in vec2 aVertexPosition;
layout(location = 1) in vec4 aVertexColor;
layout(location = 2) in vec2 aTexCoord;

// uniform variable inputs
uniform mat3 uModel_to_NDC;
uniform int uCurrFrame;
uniform int uMaxFrames;
uniform float FlipX;

// outputs
layout(location = 0) out vec4 vColor;
layout(location = 1) out vec2 vTexPos;

void main()
{
	//From top-left (assume square dimensions)
	int framesPerSide = int(ceil(sqrt(float(uMaxFrames))));
	int textureX = int(mod(uCurrFrame, framesPerSide));
	int textureY = (uCurrFrame - textureX) / framesPerSide;
	float frameLength = 1.0f / float(framesPerSide);
	
	// Flips sprite according to current spritesheet range
	float conditionFlipX = step(0.5, FlipX);

	// adjusts the texcoord for vertex to the current animation frame
	vTexPos = vec2(aTexCoord.x, aTexCoord.y) * frameLength;
	vTexPos.x = conditionFlipX * (frameLength - vTexPos.x) + (1 - conditionFlipX) * vTexPos.x;
	vTexPos += vec2(float(textureX) * frameLength, float(framesPerSide - 1 - textureY) * frameLength);

	gl_Position = vec4(vec2(uModel_to_NDC * vec3(aVertexPosition, 1.f)), 0.0, 1.0);
	vColor = aVertexColor;
}