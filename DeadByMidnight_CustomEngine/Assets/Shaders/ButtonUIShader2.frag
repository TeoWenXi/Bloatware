/*!
All content © 2023 DigiPen Institute of Technology Singapore, all rights reserved.
@file       ButtonUIShader2.frag
@author     Jovan Zhuo Wen LOW (100%)
@co-author	

----------------------------------------------------------------------------

@course     CSD2401F23
@section    CSD2401F23-A
@project    CSD2401F23 Software Engineering Project 3

@brief      This file implements the fragment shader for OpenGL use, integrated to create animated Menu Buttons.
*//*______________________________________________________________________*/

#version 450 core

// inputs
layout (location=0) in vec4 vInterpColor;
layout (location=1) in vec2 vTexCoord;

// uniform variable inputs
uniform sampler2D uTex2d;
uniform sampler2D uTex2d_Blank;
uniform bool uTextureEnabled;
uniform float _IsPressed;

// outputs
layout (location=0) out vec4 fFragColor;

void main () 
{      
    vec4 _TintA = vec4(0.867f, 0.867f, 0.867f, 1.f);
    vec4 _TintB = vec4(0.537f, 0.537f, 0.537f, 0.471f);
    vec4 _WarningColor = vec4(0.973f, 0.361f, 0.361f, 1.f);
    float _AlphaMod = 1.f; //TBD uniform
    float _WarningProgress = 0.f; //TBD uniform
    vec2 TheCoords = vTexCoord;
    
vec4 dummyvariable = texture(uTex2d, vTexCoord);
vec4 dummyvariable2 = texture(uTex2d_Blank, vTexCoord);

                float _IsPressed2 = 0.75f + _IsPressed * 0.25f;
                TheCoords.x -= step(TheCoords.x, 0.12f + 0.015f) * 0.12f * (1.f - _IsPressed2);
                if (TheCoords.x <= 0.0f) discard;
                vec4 colGetAround = vec4(_TintA.x * _IsPressed + _TintB.x * (1.f - _IsPressed), _TintA.y * _IsPressed + _TintB.y * (1.f - _IsPressed), _TintA.z * _IsPressed + _TintB.z * (1.f - _IsPressed), _TintA.w * _IsPressed + _TintB.w * (1.f - _IsPressed));
                if (step(TheCoords.x, 0.12f) < 0.5f) colGetAround = _TintB;
                vec4 col = colGetAround;

                col.xyz *= vec3((1.f - _WarningProgress) + _WarningProgress * _WarningColor.x, (1.f - _WarningProgress) + _WarningProgress * _WarningColor.y, (1.f - _WarningProgress) + _WarningProgress * _WarningColor.z);

            col.a *= 1.f - clamp(smoothstep(0.45f, 1.0f, TheCoords.x), 0.f, 1.f);

            if (abs(TheCoords.x - 0.12f) - 0.015f <= 0.0f) discard;
            vec2 theuv = vec2(6.f * (TheCoords.x - 0.5f) + 2.4f, TheCoords.y);

            if (length(theuv - vec2(0.12f, 0.5f)) - 0.15f * _IsPressed <= 0.0f) discard;
            if (step(length(vec2(0.5f, 0.5f) - TheCoords),  0.68f) - 0.01f <= 0.0f) discard;

            fFragColor = vec4(col.x, col.y, col.z, col.w);

            fFragColor.a *= _AlphaMod;

            fFragColor.rgb *= fFragColor.a;
}
