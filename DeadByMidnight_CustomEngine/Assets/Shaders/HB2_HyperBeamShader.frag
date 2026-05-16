/*!
All content © 2023 DigiPen Institute of Technology Singapore, all rights reserved.
@file       HB2_HyperBeamShader.frag
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
uniform float _ProgressUniform;
uniform float _TimeUniform;
uniform sampler2D _TurbulenceTex;
uniform sampler2D _NoiseTex;

// outputs
layout (location=0) out vec4 fFragColor;

void main () 
{
    vec4 dummyvar = texture(uTex2d, vTexCoord);

  if (uTextureEnabled)
  {
					vec4 _Color = vec4(0.04f, 0.0f, 0.0f, 1.0f);
					vec4 _CoreColor = vec4(0.29f, 0.0f, 0.0f, 1.0f);
					vec4 _GlitchColor = vec4(0.29f, 0.0f, 0.0f, 1.0f);
					float _Height = 0.075f;
					float _StartCurveDistance = 0.06f;
					float _Progress = _ProgressUniform;
					float timeVal = _TimeUniform;

					float _Progress2 = clamp(_Progress * 2.0f - 1.0f, 0.0f, 1.0f);
					_Progress = clamp(_Progress * 2.0f, 0.0f, 1.0f);
					timeVal *= clamp(1.0f - _Progress2 * 1.2f, 0.0f, 1.0f);

					_Height *= (1.0f + 0.5f * _Progress2 * (sin(vTexCoord.x + vTexCoord.y) ) + _Progress2 * 
						(
							pow(1.0f -
							step(vTexCoord.y, 0.5f) * texture(_NoiseTex, vec2(vTexCoord.x * 0.2f, vTexCoord.y)).x
							+ (1.0f - step(vTexCoord.y, 0.5f)) * texture(_NoiseTex, vec2(vTexCoord.x * 0.3f + 0.2f, vTexCoord.y)).x
							, 2.0f)
						)
						);
					float DestructionValue = abs(vTexCoord.y - 0.5f) / (2.0f * _Height);
					if (step(_Progress2, 0.001f) + DestructionValue - _Progress2 - 0.01f < 0.0f) discard;

					float curvedProg = 1.0f - pow(1.0f - _Progress, 2.0f);

					float FluctuationFactor = sin(-timeVal * 50.0f + vTexCoord.x * 5.f);
					float FluctuationBodyFactor = sin(-timeVal * 10.0f + vTexCoord.x * 10.f);
					_Height *= (1.0f + 0.02f * FluctuationFactor + 0.03f * FluctuationBodyFactor);

					float XCurrMax = pow(_Progress, 3.0f);
					float BeamStartPow = pow(clamp(vTexCoord.x / _StartCurveDistance, 0.0f, 1.0f), 0.3f + 0.2f * (1.0f - curvedProg));
					float BeamStartPow2 = pow(clamp((vTexCoord.x - 0.1f * _StartCurveDistance) / _StartCurveDistance, 0.0f, 1.0f), 0.3f + 0.2f * (1.0f - curvedProg));
					float WithinBeamCondition = step(abs(vTexCoord.y - 0.5f), _Height * curvedProg
						* BeamStartPow
					)
						* step(vTexCoord.x, XCurrMax * (_Progress * 0.6f + 0.4f + (1.0f - _Progress) * 0.6f * sin(3.147f * 3.0f * (vTexCoord.y - _Height) / (2.0f * _Height * _Progress) )))
						;

					float testjitter = (sin(-50.0f * timeVal + 50.0f * vTexCoord.x) / 2.0f + 0.5f);
					float WithinTinierBeamCondition = step(abs(vTexCoord.y - 0.5f), 0.001f + 0.002f * (testjitter - mod(testjitter, 0.5f)));

					if (WithinBeamCondition + WithinTinierBeamCondition - 0.01f < 0.0f) discard;

					float CoreColorCondition = step(abs(vTexCoord.y - 0.5f), _Height * (0.2f + 0.01f * FluctuationFactor) * BeamStartPow2);
					float CoreColorSecondaryCondition = step(0.55f, texture(_NoiseTex, vec2(vTexCoord.x * 0.2f - timeVal, vTexCoord.y * 5.0f)).x);

					vec4 c = //texture(uTex2d, vTexCoord) *
						(
							CoreColorSecondaryCondition * _GlitchColor 
							+ (1 - CoreColorSecondaryCondition) *
							(
							CoreColorCondition * _CoreColor
							+ (1.0f - CoreColorCondition) * (_Color) * texture(_TurbulenceTex, vec2(vTexCoord.x * 0.5f - 3.0f * timeVal, vTexCoord.y)).x
								)
								);
					c.rgb *= c.a;

    fFragColor = c;
  }
  else
    fFragColor = vInterpColor;
}
