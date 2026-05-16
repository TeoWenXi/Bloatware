/*!
All content � 2023 DigiPen Institute of Technology Singapore, all rights reserved.
@file       BGParallaxB.frag
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

// outputs
layout (location=0) out vec4 fFragColor;

void main () 
{      
vec4 dummyvariable = texture(uTex2d, vTexCoord);

  // set pixel color based on whether the texture flag is enabled
  if (uTextureEnabled)
    fFragColor = texture(uTex2d, 
    (vTexCoord + vec2(_XCoord * _ParallaxXRate / 200.f + _TrafficSpeed * _Time, _YCoord * _ParallaxYRate / 200.f))
    * vec2(_Tex_ST.x, _Tex_ST.y) + vec2(_Tex_ST.z, _Tex_ST.w)
    ) * vec4(1.f, 1.f, 1.f, _AlphaMultiplier);
  else
    fFragColor = vInterpColor;
}
