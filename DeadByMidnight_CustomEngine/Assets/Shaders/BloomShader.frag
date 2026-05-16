/*!
All content © 2023 DigiPen Institute of Technology Singapore, all rights reserved.
@file       BloomShader.frag
@author     Jovan Zhuo Wen LOW (100%)
@co-author	

----------------------------------------------------------------------------

@course     CSD2401F23
@section    CSD2401F23-A
@project    CSD2401F23 Software Engineering Project 3

@brief      This file implements the fragment shader for OpenGL use, used for Post-Processing.
*//*______________________________________________________________________*/

#version 450 core
layout (location=0) in vec4 vInterpColor;
layout (location=1) in vec2 vTexCoord;

uniform sampler2D scene;
uniform int uWidth;
uniform int uHeight;
uniform float mOpacity;
uniform float mRedIntensity;
uniform float _TimeUniform;
uniform float _DeathUniform;
uniform sampler2D _NoiseTexture;
uniform float _DisruptProgress;
uniform float _DisruptSpeedProgress;
uniform float _GlowProgress;
uniform float _BloomAmount;

layout (location=0) out vec4 fFragColor;

void main()
{
    //Pre-Filter
    vec4 ColOne;
            //Supposed to use screen width/width/height respectively...
            float texelSize = 1.0f / float(uWidth);
            float texelSize2 = 2.0f / float(uWidth);
            float texelSizeY = 1.0f / float(uHeight);

            vec4 A = texture(scene, vTexCoord + texelSize * vec2(-1.0f, -1.0f));
            vec4 B = texture(scene, vTexCoord + texelSize * vec2(0.0f, -1.0f));
            vec4 C = texture(scene, vTexCoord + texelSize * vec2(1.0f, -1.0f));

            vec4 D = texture(scene, vTexCoord + texelSize * vec2(-0.5f, -0.5f));
            vec4 E = texture(scene, vTexCoord + texelSize * vec2(0.5f, -0.5f));

            vec4 F = texture(scene, vTexCoord + texelSize * vec2(-1.0f, 0.0f));
            vec4 G = texture(scene, vTexCoord);
            vec4 H = texture(scene, vTexCoord + texelSize * vec2(1.0f, 0.0f));

            vec4 I = texture(scene, vTexCoord + texelSize * vec2(-0.5f, 0.5f));
            vec4 J = texture(scene, vTexCoord + texelSize * vec2(0.5f, 0.5f));

            vec4 K = texture(scene, vTexCoord + texelSize * vec2(-1.0f, 1.0f));
            vec4 L = texture(scene, vTexCoord + texelSize * vec2(0.0f, 1.0f));
            vec4 M = texture(scene, vTexCoord + texelSize * vec2(1.0f, 1.0f));
            
            vec2 div = (1.0f / 4.0f) * vec2(0.5f, 0.125f);
            vec4 o = (D + E + I + J) * div.x;
            o += (A + B + G + F) * div.y;
            o += (B + C + H + G) * div.y;
            o += (F + G + L + K) * div.y;
            o += (G + H + M + L) * div.y;

            ColOne = o;
            ColOne.r = clamp(ColOne.r, 0.0f, 1.0f);
            ColOne.g = clamp(ColOne.g, 0.0f, 1.0f);
            ColOne.b = clamp(ColOne.b, 0.0f, 1.0f);
            ColOne.a = 1.0f;

            float threshold = 0.3f;
            float ThresholdKnee = 0.7f;
            float highestRGB = max(max(ColOne.r, ColOne.g), ColOne.b);
            float softness = clamp(highestRGB - threshold + ThresholdKnee, 0.0f, 2.0f * ThresholdKnee);
            softness = (softness * softness) / (4.0f * ThresholdKnee + 0.0001f);
            float multiplier = max(highestRGB - threshold, softness) / max(highestRGB, 0.0001f);
            ColOne *= multiplier;
            ColOne.r = clamp(ColOne.r, 0.0f, 1.0f);
            ColOne.g = clamp(ColOne.g, 0.0f, 1.0f);
            ColOne.b = clamp(ColOne.b, 0.0f, 1.0f);
            ColOne.a = 1.0f;

    //Horizontal Blur
    float horblurMag = 0.6f;
    vec4 ColTwo;
            vec4 c0 = texture(scene, vTexCoord - vec2(texelSize2 * 4.0f * horblurMag, 0.0f));
            vec4 c1 = texture(scene, vTexCoord - vec2(texelSize2 * 3.0f * horblurMag, 0.0f));
            vec4 c2 = texture(scene, vTexCoord - vec2(texelSize2 * 2.0f * horblurMag, 0.0f));
            vec4 c3 = texture(scene, vTexCoord - vec2(texelSize2 * 1.0f * horblurMag, 0.0f));
            vec4 c4 = texture(scene, vTexCoord);
            vec4 c5 = texture(scene, vTexCoord + vec2(texelSize2 * 1.0f * horblurMag, 0.0f));
            vec4 c6 = texture(scene, vTexCoord + vec2(texelSize2 * 2.0f * horblurMag, 0.0f));
            vec4 c7 = texture(scene, vTexCoord + vec2(texelSize2 * 3.0f * horblurMag, 0.0f));
            vec4 c8 = texture(scene, vTexCoord + vec2(texelSize2 * 4.0f * horblurMag, 0.0f));

            ColTwo = c0 * 0.01621622f + c1 * 0.05405405f + c2 * 0.12162162f + c3 * 0.19459459f
                        + c4 * 0.22702703f
                        + c5 * 0.19459459f + c6 * 0.12162162f + c7 * 0.05405405f + c8 * 0.01621622f;

    //Vertical Blur
    float vertblurMag = 0.6f;
    vec4 ColThree;
            c0 = texture(scene, vTexCoord - vec2(0.0f, texelSizeY * 3.23076923f * vertblurMag));
            c1 = texture(scene, vTexCoord - vec2(0.0f, texelSizeY * 1.38461538f * vertblurMag));
            c2 = texture(scene, vTexCoord);
            c3 = texture(scene, vTexCoord + vec2(0.0f, texelSizeY * 1.38461538f * vertblurMag));
            c4 = texture(scene, vTexCoord + vec2(0.0f, texelSizeY * 3.23076923f * vertblurMag));

            ColThree = c0 * 0.07027027f + c1 * 0.31621622f
                        + c2 * 0.22702703f
                        + c3 * 0.31621622f + c4 * 0.07027027f;

            
            float originalSubstance = (ColOne.r + ColOne.g + ColOne.b) / 3.0f;
            float lowestVal = 0.6f + 0.35f * pow(1.0f - originalSubstance, 2.0f);
            float redContrast = lowestVal + (1.0f - lowestVal) * (1.0f - mRedIntensity) - 0.1f * pow(ColOne.r, 1.5f) * mRedIntensity;
            float intensity = 0.85f + 0.15f * (1.0f - mRedIntensity);

            fFragColor =
            _BloomAmount * intensity * 0.5f * vec4(ColTwo.r, redContrast * ColTwo.g, redContrast * ColTwo.b, ColTwo.a)
            + _BloomAmount * intensity * 0.5f * vec4(ColThree.r, redContrast * ColThree.g, redContrast * ColThree.b, ColThree.a)
            + _BloomAmount * vec4(ColOne.r, ColOne.g, ColOne.b, ColOne.a)
            + (1.0f - _BloomAmount) * texture(scene, vTexCoord);

            //Final Sequence Override Particles (Back)
			float modthresX = 1.0f / 28.0f;
			float modthresY = 1.0f / 60.0f;
			vec2 noiseCoords = vec2(vTexCoord.x + (0.30f + 0.40f * _DisruptSpeedProgress) * _TimeUniform, vTexCoord.y - 0.2f * _TimeUniform);
			vec2 moddedTexcoords = vec2(noiseCoords.x - mod(noiseCoords.x, modthresX), noiseCoords.y - mod(noiseCoords.y, modthresY)) + 0.5f * vec2(modthresX, modthresY);
			float noiseColor = texture(_NoiseTexture, moddedTexcoords).r;
			float disruptCondition = step(0.36f + 0.28f, noiseColor);
			disruptCondition = clamp(disruptCondition, 0.0f, 1.0f) * _DisruptProgress * 0.8f * (0.85f + 0.15f * texture(_NoiseTexture, moddedTexcoords + vec2(0.3f, -0.7f)).r);
			fFragColor.rgb = fFragColor.rgb * (1 - disruptCondition) + vec3(0.7f, 0.03f, 0.03f) * disruptCondition;

            //Final Sequence Override Particles (Front)
			modthresX = 1.0f / 20.0f;
			modthresY = 1.0f / 40.0f;
			noiseCoords = vec2(vTexCoord.x + (0.50f + 0.40f * _DisruptSpeedProgress) * _TimeUniform, vTexCoord.y + 0.3f * _TimeUniform) + vec2(0.3f, 0.77f);
			moddedTexcoords = vec2(noiseCoords.x - mod(noiseCoords.x, modthresX), noiseCoords.y - mod(noiseCoords.y, modthresY)) + 0.5f * vec2(modthresX, modthresY);
			noiseColor = texture(_NoiseTexture, moddedTexcoords).r;
			disruptCondition = step(0.33f + 0.28f, noiseColor);
			disruptCondition = clamp(disruptCondition, 0.0f, 1.0f) * _DisruptProgress * (0.95f + 0.05f * texture(_NoiseTexture, moddedTexcoords + vec2(-0.5f, 0.4f)).r);
			fFragColor.rgb = fFragColor.rgb * (1 - disruptCondition) + vec3(0.9f, 0.04f, 0.04f) * disruptCondition;

            //Red border aura
            float leftThreshold = (0.010f + 0.070f * sin(3.14159f * pow(_DeathUniform, 2.0f))) + 0.002f * sin(2.f * _TimeUniform);
            float upThreshold = leftThreshold * (uWidth / uHeight);
            float leftVal = mRedIntensity * 0.1f * clamp(
            clamp((leftThreshold - (0.5f - abs(0.5f - vTexCoord.x))) / leftThreshold, 0.0f, 1.0f)
            + clamp((upThreshold - (0.5f - abs(0.5f - vTexCoord.y))) / upThreshold, 0.0f, 1.0f)
            , 0.0f, 1.0f);
            fFragColor = vec4(fFragColor.r * (1.0f - leftVal) + 1.0f * leftVal, fFragColor.g * (1.0f - leftVal), fFragColor.b * (1.0f - leftVal), fFragColor.a);

            //Final Sequence Flashbang
			float RealProgress = pow(_GlowProgress, 4.0f);
			float RealProgress2 = pow(_GlowProgress, 5.5f);
			float GlowValueJudge = (1.0f - vTexCoord.x) - RealProgress - 0.2f * RealProgress2 * (1.0f - pow(abs(0.5f - vTexCoord.y) / 0.5f, 2.0f));
			float GlowValue = clamp(smoothstep(0.05f * _GlowProgress, 0.0f, GlowValueJudge), 0.0f, 1.0f);
			fFragColor.rgb = fFragColor.rgb * (1.0f - GlowValue) + vec3(1.0f, 1.0f, 1.0f) * GlowValue;

            //Fade out for scenes, highest rendering priority
            fFragColor.rgb *= mOpacity;
}
