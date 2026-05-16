/*!
All content © 2023 DigiPen Institute of Technology Singapore, all rights reserved.
@file       LaserShader.frag
@author     Jovan Zhuo Wen LOW (100%)
@co-author	

----------------------------------------------------------------------------

@course     CSD2401F23
@section    CSD2401F23-A
@project    CSD2401F23 Software Engineering Project 3

@brief      This file implements the fragment shader for OpenGL use, integrated to create Animated Laser Obstacle Textures.
*//*______________________________________________________________________*/

#version 450 core

// inputs
layout (location=0) in vec4 vInterpColor;
layout (location=1) in vec2 vTexCoord;

// uniform variable inputs
uniform sampler2D uTex2d;
uniform sampler2D uTex2d_Noise;
uniform bool uTextureEnabled;

uniform float _Interval;
uniform float _LaserSpeed;
uniform float _Time;

// outputs
layout (location=0) out vec4 fFragColor;

void main () 
{      
	vec4 _LaserColor = vec4(0.7f, 0.08f, 0.05f, 1.0f);

  // set pixel color based on whether the texture flag is enabled
  if (uTextureEnabled)
  {
  			float tanvalu = _Time * 1.23f * 3.14159f * 2.f;
			float inter = _Interval + 0.02f * clamp(tan(tanvalu), -1.f, 1.f) * (1.f - pow(abs(3.14159 - mod(tanvalu, 2.f * 3.14159f))/ 3.14159f, 3.f));
			float ActualTime = (_Time * _LaserSpeed) - mod(_Time * _LaserSpeed, inter);

            fFragColor = texture(uTex2d, vec2(vTexCoord.x, vTexCoord.y - ActualTime) * vec2(1.f, 4.f)) * 0.2f;
			fFragColor.rgb *= fFragColor.a;

            			float toPointBlank = clamp(
						smoothstep(0.35f, 0.5f, length(
							(vTexCoord - vec2(0.5f, 0.5f)) * vec2(1.2f, 1.f)
						))
						, 0.f, 1.f);

                        vec4 trueLaserColor = vec4(_LaserColor.xyz, _LaserColor.w);

            			fFragColor += trueLaserColor
						* (1.f - clamp(smoothstep(0.03f, 0.07f, abs(vTexCoord.x - 0.5f)), 0.f, 1.f))
						;

						//if (fFragColor.a <= 0.01f) discard;
                        float absval = abs(0.5f - vTexCoord.x
						+ 0.2f * 
						(
							-step(0.5f, vTexCoord.x) * tan(3.1419f * (-3.f * ActualTime + vTexCoord.y + 0.4f))
							+ step(vTexCoord.x, 0.5f) * tan(3.1419f * (-3.f * ActualTime + vTexCoord.y ))
						
							
						)
						* texture(uTex2d_Noise, (vTexCoord - vec2(0, ActualTime)) * vec2(1.f , 2.f)).x
						);
						fFragColor += trueLaserColor * 2.f * clamp(smoothstep(0.10f, 0.15f, absval) * (1.f - smoothstep(0.15f, 0.2f, absval)), 0.f, 1.f);

                        if (fFragColor.r <= 0.01f) discard;

						fFragColor.x = clamp(fFragColor.x, 0.f, 1.f);
						fFragColor.y = clamp(fFragColor.y, 0.f, 1.f);
						fFragColor.z = clamp(fFragColor.z, 0.f, 1.f);
						fFragColor.w = clamp(fFragColor.w, 0.f, 1.f);
  }
  else
    fFragColor = vInterpColor;
}
