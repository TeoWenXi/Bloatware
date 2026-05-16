/*!
All content © 2023 DigiPen Institute of Technology Singapore, all rights reserved.
@file       PlayerBodyShader.vert
@author     Jovan Zhuo Wen LOW (100%)
@co-author	

----------------------------------------------------------------------------

@course     CSD2401F23
@section    CSD2401F23-A
@project    CSD2401F23 Software Engineering Project 3

@brief      This file implements the vertex shader for OpenGL use, used to animate the player character.
*//*______________________________________________________________________*/

#version 450 core

// inputs
layout(location = 0) in vec2 aVertexPosition;
layout(location = 1) in vec4 aVertexColor;
layout(location = 2) in vec2 aTexCoord;

// uniform variable inputs
uniform mat3 uModel_to_NDC;

				uniform float _FlipRight;
				uniform float _Progress;
				uniform float _ProgressX;
				uniform float _VerticalStretch;
				uniform float _IdleProgress;
				uniform float _RunningTimer;

// outputs
layout(location = 0) out vec4 vColor;
layout(location = 1) out vec2 vTexPos;

void main()
{
	float _ModX = 0.275f;
	float _ModY = 0.5f;

	vec2 texcoords = aVertexPosition.xy;
					vec2 test2 = aTexCoord - vec2(0.5f, _VerticalStretch);
					texcoords.xy += test2 * vec2(0.f, _Progress * _ModY + _IdleProgress * 0.06f * cos(pow(mod(0.65f * _RunningTimer, 1.0f), 0.9f) * 3.1415f * 2.f + 3.1415f));

					vec2 test3 = aTexCoord - vec2(0.f + 1.f - (1.f - _FlipRight), 0.5f);
					texcoords.xy += test3 * vec2(_ProgressX * _ModX, 0.f);


	gl_Position = vec4(vec2(uModel_to_NDC * vec3(texcoords.xy, 1.f)), 0.0f, 1.0f);
	vColor = aVertexColor;
	vTexPos = aTexCoord;
}