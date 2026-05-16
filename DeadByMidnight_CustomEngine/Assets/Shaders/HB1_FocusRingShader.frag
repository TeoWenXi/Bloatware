/*!
All content © 2023 DigiPen Institute of Technology Singapore, all rights reserved.
@file       HB1_FocusRingShader.frag
@author     Jovan Zhuo Wen LOW (100%)
@co-author	

----------------------------------------------------------------------------

@course     CSD2401F23
@section    CSD2401F23-A
@project    CSD2401F23 Software Engineering Project 3

@brief      This file implements the fragment shader for implementing a VFX component of the AI's Boss Hyper Beam attack.
*//*______________________________________________________________________*/

#version 450 core

// inputs
layout (location=0) in vec4 vInterpColor;
layout (location=1) in vec2 vTexCoord;

// uniform variable inputs
uniform sampler2D uTex2d;
uniform bool uTextureEnabled;
uniform float _Progress;

// outputs
layout (location=0) out vec4 fFragColor;

void main () 
{
    float lenthres = 0.48f;
    vec4 dummyvar = texture(uTex2d, vTexCoord);

  // set pixel color based on whether the texture flag is enabled
  if (uTextureEnabled)
  {
  					float initAlpha = clamp(10.0f * _Progress, 0.0f, 1.0f);
					float actualprog = pow(_Progress, 1.7f);

					float sizeprog = 0;
					vec4 c = texture(uTex2d, (1.0f + 2 * sizeprog) * (vTexCoord - vec2(0.5f, 0.5f)) + vec2(0.5f, 0.5f)) * vec4(1.0f, 0.0f, 0.0f, 1.0f);

					vec2 circleCoords = vTexCoord - vec2(0.5f, 0.5f);
					float thelength = length(circleCoords);
					if (thelength - (lenthres - 0.05f * actualprog) * (1.0f - 0.8f * sizeprog) < 0.0f) discard;
					float angle = atan(circleCoords.y, circleCoords.x) + 3.147f + 9.0f * actualprog;
					float angValue = abs(0.15f * 3.147f - mod(angle, (1.0f / 6.0f) * 2.0f * 3.147f)) - 0.2f * actualprog;
					if (angValue < 0.0f) discard;
					
					c.a *= 1.0f - clamp(actualprog * 10.0f, 0.0f, 1.0f) * pow(1.0f - angValue / (((1.0f / 6.0f) * 2.0f * 3.147f) - 0.2f * actualprog), 2.0f);

					c.a *= initAlpha;

					c.rgb *= c.a;

					fFragColor = c;
  }
  else
    fFragColor = vInterpColor;
}
