/*!
All content © 2023 DigiPen Institute of Technology Singapore, all rights reserved.
@file       KeyWallShader.frag
@author     Jovan Zhuo Wen LOW (100%)
@co-author	

----------------------------------------------------------------------------

@course     CSD2401F23
@section    CSD2401F23-A
@project    CSD2401F23 Software Engineering Project 3

@brief      This file implements the vertex shader for OpenGL use, integrated to create Key Wall texture.
*//*______________________________________________________________________*/

#version 450 core

// inputs
layout (location=0) in vec4 vInterpColor;
layout (location=1) in vec2 vTexCoord;

// uniform variable inputs
uniform sampler2D uTex2d;
uniform sampler2D uTex2d_Base;
uniform sampler2D uTex2d_Noise;
uniform bool uTextureEnabled;

uniform float _WindThreshold;
uniform float _Time;
uniform float _IsOpen;
uniform float _AlphaMultiplier;

// outputs
layout (location=0) out vec4 fFragColor;

void main () 
{      
	vec4 _WindColor = vec4(0.03f, 0.63f, 0.f, 1.0f);
	vec4 _BlockadeColor = vec4(1.f, 0.4f, 0.4f, 1.0f);

vec4 dummyvariable = texture(uTex2d, vTexCoord);

  // set pixel color based on whether the texture flag is enabled
  if (uTextureEnabled)
  {
                vec4 c = _WindColor;
                //PART A: BODY
                float thresholdA = 0.2f;
                float thresholdB = 1.f;
                c.a *= 0.4f * (1.f - clamp(smoothstep(thresholdA - 0.2f, thresholdA + 0.05f, vTexCoord.y + 0.5f * pow(abs(0.5f - vTexCoord.x), 2.5f)), 0.f, 1.f))
                    + 0.2f * (1.f - clamp(smoothstep(thresholdB - 0.2f, thresholdB + 0.05f, vTexCoord.y + 0.5f * pow(abs(0.5f - vTexCoord.x), 3.f)), 0.f, 1.f))
                + 0.4f * (1.f - clamp(smoothstep(thresholdA - 0.2f, thresholdA + 0.05f, 1.f - vTexCoord.y + 0.5f * pow(abs(0.5f - vTexCoord.x), 2.5f)), 0.f, 1.f))
                    ;
                c.a *= (1.f + 2.f * pow(clamp(smoothstep(0.4f, 0.5f, abs(0.5f - vTexCoord.x)), 0.f, 1.f), 1.4f));
                c.a *= (clamp(smoothstep(0.f, 0.01f, vTexCoord.y), 0.f, 1.f));
                float thecond = clamp(_IsOpen + 1.f - clamp(smoothstep(0.0f, 0.3f, abs(0.5f - vTexCoord.x)), 0.f, 1.f), 0.f, 1.f);
                c.rgb = _BlockadeColor.rgb * (1 - thecond) + c.rgb * thecond;

                //PART B: Glitchy Effects!
                vec4 contentC = _WindColor;
                float distortion = texture(uTex2d_Noise, vTexCoord + vec2(_Time * 0.3f, 0.f)).r;
                distortion -= 0.7f;
                distortion *= 0.5f;
                vec2 windCoord = vTexCoord * vec2(1.f, 2.04f) - vec2(0.f, 0.1f * _Time);
                float windValue = texture(uTex2d_Base, windCoord).r + distortion;
                float killValue = step(_WindThreshold, windValue);
                contentC.a *= killValue * smoothstep(_WindThreshold, 1.0f, windValue);
                float theval = 0.15f * (1.f - pow(clamp(smoothstep(0.0f, 0.8f, vTexCoord.y * 2.04f), 0.f, 1.f), 2.f));

                float absval = abs(0.5f - vTexCoord.x);
                float otherval = clamp(smoothstep(0.43f, 0.5f, absval), 0.f, 1.f);
                contentC.a += killValue * otherval; //adds edges
                otherval = clamp(smoothstep(0.225f, 0.25f, absval) * (1.f - smoothstep(0.25f, 0.275f, absval)), 0.f, 1.f);
                contentC.a += 0.5f * killValue * otherval;

                //PART C: Glitchy Center
                otherval = clamp(smoothstep(0.f, 0.f, absval) * (1.f - smoothstep(0.005f, 0.02f, absval)), 0.f, 1.f);
                contentC.a += killValue * otherval;

                //REMOVE THIS IF WE GET A LOOPING TEXTURE
                contentC.a *= (1.f - clamp(smoothstep(0.4f, 0.49f, abs(0.5f - (mod((abs(vTexCoord.y - 0.049f * _Time) * 2.04f - 0.02f), 1.f)))), 0.f, 1.f));

                contentC.a *= (1.f - pow(clamp(smoothstep(0.25f, 0.5f, abs(0.5f - vTexCoord.x)), 0.f, 1.f), 1.4f));
                contentC.a *= (1.f - clamp(smoothstep(thresholdB - 0.25f, thresholdB - 0.05f, vTexCoord.y + 0.5f * pow(abs(0.5f - vTexCoord.x), 2.f)), 0.f, 1.f));
                c.a += contentC.a;

                c.a = clamp(c.a, 0.f, 1.f);
                c.a *= _AlphaMultiplier;
                //c.rgb *= c.a;

                fFragColor = c;
  }
  else
    fFragColor = vInterpColor;
}
