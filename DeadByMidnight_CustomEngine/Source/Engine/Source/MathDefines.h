/*!
All content © 2023 DigiPen Institute of Technology Singapore, all rights reserved.
@file       MathDefines.h
@author     CAI Wenjin (100%)
@co-author

----------------------------------------------------------------------------

@course     CSD2401F23
@section    CSD2401F23-A
@project    CSD2401F23 Software Engineering Project 3

@brief      Math definitions.
*//*______________________________________________________________________*/

#ifndef MATH_DEFINES_H
#define MATH_DEFINES_H
#include <cmath>

constexpr float PI = 3.14159265358979323846f;
constexpr float TWO_PI = 2.f * PI;
constexpr float HALF_PI = 0.5f * PI;

constexpr float DEG_TO_RAD = PI / 180.f;
constexpr float RAD_TO_DEG = 180.f / PI;

/*!
 * @brief
 * Converts the angle from degrees to radians.
 * @param _deg
 * The angle in degrees.
 * @return
 * The angle converted to radians.
*/
inline float DegreeToRadians(float _deg)
{
	return _deg * DEG_TO_RAD;
}

/*!
 * @brief
 * Converts the angle from radians to degrees.
 * @param _rad
 * The angle in radians.
 * @return
 * The angle converted to degrees.
*/
inline float RadiansToDegree(float _rad)
{
	return _rad * RAD_TO_DEG;
}

#endif // !MATH_DEFINES_H
