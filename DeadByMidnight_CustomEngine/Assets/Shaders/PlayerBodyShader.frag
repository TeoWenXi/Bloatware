/*!
All content © 2023 DigiPen Institute of Technology Singapore, all rights reserved.
@file       PlayerBodyShader.frag
@author     Jovan Zhuo Wen LOW (100%)
@co-author	

----------------------------------------------------------------------------

@course     CSD2401F23
@section    CSD2401F23-A
@project    CSD2401F23 Software Engineering Project 3

@brief      This file implements the fragment shader for OpenGL use, used to animate the player character.
*//*______________________________________________________________________*/

#version 450 core

// inputs
layout (location=0) in vec4 vInterpColor;
layout (location=1) in vec2 vTexCoord;

// uniform variable inputs
uniform sampler2D uTex2d;
uniform bool uTextureEnabled;
				uniform float _FlipRight;
				uniform float _Progress;
				uniform float _ProgressX;
				uniform float _VerticalStretch;
				uniform float _IdleProgress;
				uniform float _RunningTimer;
				uniform float _RespawnProgress;


// outputs
layout (location=0) out vec4 fFragColor;

void main () 
{
	float _ModX = 0.275f;
	float _ModY = 0.5f;
	vec4 _RespawnColor = vec4(1, 1, 1, 1);

  // set pixel color based on whether the texture flag is enabled
  if (uTextureEnabled)
  {
					float mag = 0.12f;
					float val = step(0.5f, _FlipRight) * (1.f + (1.f - _FlipRight) * mag)
					+ (1.f - step(0.5f, _FlipRight)) * (0.f - _FlipRight * mag)
					;
					fFragColor = texture(uTex2d, vec2( ///RIGHT TO LEFT -> 1 to 1.03 step -0.03 to 0
						(1.f - vTexCoord.x) * (1.f - val) + vTexCoord.x * val
						, vTexCoord.y));
					float smoothcol = clamp(smoothstep(0.5f, 1.f, _RespawnProgress), 0.f, 1.f);
					fFragColor.xyz = fFragColor.xyz * smoothcol + (1.f - smoothcol) * _RespawnColor.xyz;
					fFragColor.w *= clamp(smoothstep(0.f, 0.5f, _RespawnProgress), 0.f, 1.f);
					fFragColor.xyz *= fFragColor.w;
  }
  else
    fFragColor = vInterpColor;
}
