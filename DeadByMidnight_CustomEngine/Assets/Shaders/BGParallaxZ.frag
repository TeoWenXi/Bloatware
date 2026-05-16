/*!
All content © 2023 DigiPen Institute of Technology Singapore, all rights reserved.
@file       BGParallaxZ.frag
@author     Jovan Zhuo Wen LOW (100%)
@co-author	

----------------------------------------------------------------------------

@course     CSD2401F23
@section    CSD2401F23-A
@project    CSD2401F23 Software Engineering Project 3

@brief      This file implements the fragment shader for OpenGL use, integrated to create Moving Parallax Background Textures.
*//*______________________________________________________________________*/

#version 450 core

// inputs
layout (location=0) in vec4 vInterpColor;
layout (location=1) in vec2 vTexCoord;

// uniform variable inputs
uniform sampler2D uTex2d;
uniform bool uTextureEnabled;

uniform float _ParallaxXRate;
uniform float _ParallaxYRate;
uniform float _XCoord;
uniform float _YCoord;
uniform float _TrafficSpeed;
uniform float _Time;
uniform vec4 _Tex_ST;
uniform float _AlphaMultiplier;
uniform float _EyeRelativeX;
uniform float _EyeRelativeY;

// outputs
layout (location=0) out vec4 fFragColor;

void main () 
{      

  // set pixel color based on whether the texture flag is enabled
  if (uTextureEnabled)
  {
    fFragColor = texture(uTex2d, 
    (vTexCoord + vec2(_XCoord * _ParallaxXRate / 200.f + _TrafficSpeed * _Time, _YCoord * _ParallaxYRate / 200.f))
    * vec2(_Tex_ST.x, _Tex_ST.y) + vec2(_Tex_ST.z, _Tex_ST.w)
    ) * vec4(1.f, 1.f, 1.f, _AlphaMultiplier);


					//left to right
					float dist = length(vTexCoord.xy - vec2(_EyeRelativeX, _EyeRelativeY))
						+ 0.15 * sin(atan(vTexCoord.y, vTexCoord.x));
					float loopingCoord = mod(abs(dist - 0.5f * _Time), 1.5f);

					float loopstart = 0.1f;
					float loopend = 0.5f;

					fFragColor.rgb *= (0.4f
						+ clamp(
						0.6f * step(0.5f, fFragColor.r)
						* pow(clamp(smoothstep(loopstart, (loopstart + loopend) / 2.f, loopingCoord), 0.f, 1.f), 1.f)
						* pow(1.f - clamp(smoothstep((loopstart + loopend) / 2.f, loopend, loopingCoord), 0.f, 1.f), 1.f)
						+ 0.6f * step(0.5f, fFragColor.r)
						* (1.0f - clamp(smoothstep(0.2f, 0.35f, dist), 0.f, 1.f))
						, 0.f, 0.6f)
						);


  }
  else
    fFragColor = vInterpColor;
}
