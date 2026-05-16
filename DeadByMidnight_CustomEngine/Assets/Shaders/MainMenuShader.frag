/*!
All content © 2023 DigiPen Institute of Technology Singapore, all rights reserved.
@file       MainMenuShader.frag
@author     Jovan Zhuo Wen LOW (100%)
@co-author	

----------------------------------------------------------------------------

@course     CSD2401F23
@section    CSD2401F23-A
@project    CSD2401F23 Software Engineering Project 3

@brief      This file implements the fragment shader for OpenGL use, integrated to create an animated Main Menu Background Texture.
*//*______________________________________________________________________*/

#version 450 core

// inputs
layout (location=0) in vec4 vInterpColor;
layout (location=1) in vec2 vTexCoord;

// uniform variable inputs
uniform sampler2D uTex2d;
uniform sampler2D uTex2d_BG;
uniform sampler2D uTex2d_Noise;
uniform float _Time;
uniform bool uTextureEnabled;

// outputs
layout (location=0) out vec4 fFragColor;

void main () 
{      
vec4 dummyvariable = texture(uTex2d, vTexCoord);

					float pixelDivi = 135.4f;

					vec2 modthres = vec2(1.0f  / pixelDivi, 1.0f  / pixelDivi);
					vec2 moddedTexcoords = vec2(vTexCoord.x - mod(vTexCoord.x, modthres.x), vTexCoord.y - mod(vTexCoord.y, modthres.y)) + 0.5f * vec2(modthres.x, modthres.y);

					vec4 rawColor = texture(uTex2d_BG, vTexCoord);
					vec4 rawColor2 = texture(uTex2d_BG, moddedTexcoords);
					vec3 c = rawColor.rgb;
					vec3 c2 = rawColor2.rgb;
					vec4 truecolor = vec4(c.r, c.g, c.b, 1.f);
					vec4 truecolor2 = vec4(c2.r, c2.g, c2.b, 1.f);

					float dist = length(moddedTexcoords - vec2(0.5f, 0.5f));

					float loopingCoord = mod(abs(dist - 0.5f * _Time), 1.5f);

					vec2 moddedTexRadius = moddedTexcoords - vec2(0.5f, 0.5f);
					float funnyradian = atan(moddedTexRadius.y, moddedTexRadius.x) / (3.1419f);
					float loopstart = 0.15f + 0.1f * texture(uTex2d_Noise, vec2(0.5f + 0.5f * funnyradian, 0.1f)).r;
					float loopend = 0.65f + 0.1f * texture(uTex2d_Noise, vec2(0.5f + 0.5f * funnyradian, 0.9f)).r;

					float magValue = clamp(abs(loopingCoord - (loopstart + loopend) / 2.f) / (loopend - loopstart), 0.f, 1.f);

					float WithinCircleByteCondition = step(loopstart, loopingCoord) * step(loopingCoord, loopend) * step(0.1f, truecolor.r) * step(0.1f, truecolor2.r) * step(length(vTexCoord - moddedTexcoords), 1.f / ((3.f + 40.f * pow(magValue, 2.f)) * pixelDivi));
					
					float EyeCond = step(length(vTexCoord - vec2(0.5f, 0.5f)), 0.1f);
					
					vec3 _MagicColor = vec3(0.8f, 0.8f, 0.8f);

					float otherCondEarly = step(length(vTexCoord - vec2(0.5f, 0.5f)), 0.13f);
					truecolor.rgb = (1 - otherCondEarly) * truecolor.rgb + otherCondEarly * vec3(0.f, 0.f, 0.f);
					truecolor.rgb += otherCondEarly * _MagicColor * clamp(1 - length(vTexCoord - vec2(0.5f, 0.5f)) / 0.09f, 0.f, 1.f);

					truecolor.rgb += (1.f - EyeCond) * ((1.f - WithinCircleByteCondition) * vec3(0.f, 0.f, 0.f) 
						+ WithinCircleByteCondition * _MagicColor * (0.2f + 0.8f * step(truecolor.b, 0.3f)));

					truecolor.rgb *= 0.5f * EyeCond + 
						//(1.f - EyeCond) *
						(0.02f
						+ 20.f * 0.02f * WithinCircleByteCondition * step(0.5f, c.r) * vec3(1.f, 1.f, 1.f)
						* pow(clamp(smoothstep(loopstart, (loopstart + loopend) / 2.f, loopingCoord), 0.f, 1.f), 1.f)
						* pow(1.f - clamp(smoothstep((loopstart + loopend) / 2.f, loopend, loopingCoord), 0.f, 1.f), 1.f)
						);

					float testcond2 = (1.f - clamp(smoothstep(0.05f, 0.1f, length(vTexCoord - vec2(0.5f, 0.5f))), 0.f, 1.f));
					truecolor.rgb = truecolor.rgb * (1 - EyeCond) + EyeCond * (truecolor.rgb * testcond2);

					float otherCond = step(length(vTexCoord - vec2(0.5f, 0.5f)), 0.13f) * step(0.1f, length(vTexCoord - vec2(0.5f, 0.5f)));
					truecolor.rgb = (1 - otherCond) * truecolor.rgb + otherCond * vec3(0.f, 0.f, 0.f);
					//float otherCond2 = step(length(vTexCoord - vec2(0.5f, 0.5f)), 0.10f);
					//float testvar = (1.f - smoothstep(length(vTexCoord - vec2(0.5f, 0.5f)), 0.0f, 0.10f));
					//truecolor.rgb *= (1 - otherCond2) + otherCond2 * testvar * (step(0.2f, testvar) + (1 - step(0.2f, testvar)) * 0.0f);

				truecolor.a = 1.f;

				fFragColor = truecolor;
}
