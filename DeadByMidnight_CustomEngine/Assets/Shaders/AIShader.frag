/*!
All content © 2023 DigiPen Institute of Technology Singapore, all rights reserved.
@file       AIShader.frag
@author     Jovan Zhuo Wen LOW (100%)
@co-author	

----------------------------------------------------------------------------

@course     CSD2401F23
@section    CSD2401F23-A
@project    CSD2401F23 Software Engineering Project 3

@brief      This file implements the fragment shader for OpenGL use, used for the AI Entity's Pixelized & Dynamic Graphics.
*//*______________________________________________________________________*/

#version 450 core

// inputs
layout (location=0) in vec4 vInterpColor;
layout (location=1) in vec2 vTexCoord;

// uniform variable inputs
uniform sampler2D uTex2d;
uniform sampler2D uTex2d_Iris;
uniform bool uTextureEnabled;
uniform float _eyeX; //Must stay within 0~1
uniform float _eyeY; //Must stay within 0~1
uniform float _Time;
uniform float _TalkProgress;
uniform float _alphaMultiplier;
uniform float _eyeRadiusUniform;
uniform float _demonVarUniform;
uniform float _eyelidAUniform;

// outputs
layout (location=0) out vec4 fFragColor;

void main () 
{
	vec2 _BGTex_ST = vec2(8.f, 15.f);
	float _pixelMultiplier = 2.f;
	float _eyeRadius = _eyeRadiusUniform;
	float _eyelidBTop = 0.f;
	float _eyelidBBottom = 0.f;
	float _NewPulseSpeed = 0.55f;
	float _demonVar = _demonVarUniform;
	float _eyelidA = _eyelidAUniform;
	vec4 _eyeColReal = vec4(0.95f, 0.95f, 0.95f, 1.f); //vec4(0.75f, 0.675f, 0.675f, 1.f);
	vec4 _EyeColor = vec4(0.435f, 0.435f, 0.435f, 0.8f);
	vec4 _eyeColDemon = vec4(0.75f, 0.f, 0.f, 1.0f);


  // set pixel color based on whether the texture flag is enabled
  if (uTextureEnabled)
  {
  					vec2 modthres = vec2(1.f / _BGTex_ST.x, 1.f / _BGTex_ST.y) / _pixelMultiplier;
					vec2 moddedTexcoords = vec2(vTexCoord.x - mod(vTexCoord.x, modthres.x), vTexCoord.y - mod(vTexCoord.y, modthres.y)) + 0.5f * vec2(modthres.x, modthres.y);
					float distanceCenter = length(vTexCoord - vec2(0.5f, 0.5f)); //0.0 ~ 0.5;
					float moddedDistanceCenter = length(moddedTexcoords - vec2(0.5f, 0.5f));
					if (0.5f - distanceCenter <= 0.0f) discard; //Equivalent of clip from HLSL Language

					vec2 eyeCoordinates = vec2(_eyeX, _eyeY);
					vec2 eyebrowCoordinates = vec2(0.5f, 0.8f) + vec2(0.3f * (eyeCoordinates.x - 0.5f), 0.1f * (eyeCoordinates.y - 0.5f));
					float distanceCenterFromEye = length(vTexCoord - eyeCoordinates);
					float moddedDistanceCenterFromEye = length(moddedTexcoords - eyeCoordinates);
					float moddedDistanceCenterFromEyebrow = length((moddedTexcoords - eyebrowCoordinates) * vec2(0.8f, 1.5f));

					float ConditionToBeEye = step(0.01f, _eyeRadius) * clamp(smoothstep(moddedDistanceCenterFromEye - 0.02f, moddedDistanceCenterFromEye + 0.02f, _eyeRadius), 0, 1);
					float FlickerCondition = step(mod(_TalkProgress, 0.16f), 0.11f);
					float ConditionToBeEyeCenter = step(0.01f, _eyeRadius) 
						*
						(
							pow(clamp(smoothstep(distanceCenterFromEye - 0.1f, distanceCenterFromEye + 0.1f, 0.6f * _eyeRadius), 0.f, 1.f), 2.f)
						);

					float EyeRingProgress = pow(mod(_Time * _NewPulseSpeed, 1.0f), 0.5f);
					float ConditionToBeEyeRing = step(0.01f, _eyeRadius)
						* clamp(smoothstep(moddedDistanceCenterFromEye - 0.02f, moddedDistanceCenterFromEye + 0.02f, 1.5f * EyeRingProgress), 0.f, 1.f)
						* (1.f - clamp(smoothstep(moddedDistanceCenterFromEye - 0.02f, moddedDistanceCenterFromEye + 0.02f, 0.8f * EyeRingProgress), 0.f, 1.f))
						;

					float ConditionToBeEyebrow = clamp(smoothstep(moddedDistanceCenterFromEyebrow, moddedDistanceCenterFromEyebrow + 0.02f, 0.12f), 0, 1);

					_eyelidA *= 1.f - 0.3f * _demonVar;

					vec2 trueTexcoordIris = ((12.f - 14.f * _eyeRadius) / _eyelidA) * (vTexCoord - eyeCoordinates) + eyeCoordinates;
					vec4 irisTex = texture(uTex2d_Iris, trueTexcoordIris) * (1.f - clamp(smoothstep(0.2f, 0.35f, length(trueTexcoordIris - vec2(0.5f, 0.5f))), 0.f, 1.f));

					float UnmoddedDistanceCenterFromEye = length(vTexCoord - eyeCoordinates);

					//BODY COLOR
					fFragColor = vec4(0.01f, 0.01f, 0.01f, 1.f);
					fFragColor += (1.f - ConditionToBeEyebrow) * (1.f - ConditionToBeEye) * texture(uTex2d, vTexCoord * _BGTex_ST) * step(0.1f, texture(uTex2d, vTexCoord * _BGTex_ST).r)
						* (1.f - pow(clamp(smoothstep(0.25f, 0.45f, moddedDistanceCenter), 0.f, 1.f), 1.5f))
						* (1.f - pow(clamp(smoothstep(_eyeRadius * 1.2f, _eyeRadius * 2.f, moddedDistanceCenterFromEye), 0.f, 0.5f), 1.5))
						* (1.f + ConditionToBeEyeRing * 0.9f * (1.f - pow(clamp(smoothstep(0.25f, 0.45f, moddedDistanceCenter), 0.f, 1.f), 1.5f)))
						+ ConditionToBeEyebrow * _EyeColor * (1.f - pow(clamp(smoothstep(0.25f, 0.45f, moddedDistanceCenter), 0.f, 1.f), 1.5f))
						+ ConditionToBeEyeCenter
						;

					fFragColor.xyz *= (1.f - ConditionToBeEye)
						+ ConditionToBeEye * irisTex.xyz * irisTex.w * pow(_alphaMultiplier, 2.f)
						* step(abs(trueTexcoordIris.x - 0.5f), 0.5f) * step(abs(trueTexcoordIris.y - 0.5f), 0.5f)
						* (_eyeColReal.xyz * (1.f - _demonVar) + _eyeColDemon.xyz * _demonVar)
						* (1.f + 2.f * pow(sin(3.1419f * _TalkProgress), 2.f) * FlickerCondition)
						;

				fFragColor.w *= 0.7f;

				fFragColor.xyz *= fFragColor.w;
				fFragColor.xyz += 0.003f * step(fFragColor.x, 0.01f) * vec3(1.f, 1.f, 1.f);

				fFragColor.w = 1.f * _alphaMultiplier;

	//fFragColor.w = clamp(fFragColor.w, 0.f, 1.f);
    //fFragColor = texture(uTex2d, vTexCoord);
  }
  else
    fFragColor = vInterpColor;
}
