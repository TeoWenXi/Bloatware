/*!
All content © 2023 DigiPen Institute of Technology Singapore, all rights reserved.
@file       Vector3.cpp
@author     SIA Xin Tian (90%)
@co-author	CAI Wenjin (10%)

----------------------------------------------------------------------------

@course     CSD2401F23
@section    CSD2401F23-A
@project    CSD2401F23 Software Engineering Project 3

@brief      Vector3 class definitions.
*//*______________________________________________________________________*/

#include "Vector3.h"
#if _ENGINE
#include "DebugLogger.h"
#endif // _ENGINE


Vector3::Vector3()
	: mX(), mY(), mZ()
{
}

Vector3::Vector3(float _x, float _y, float _z)
	: mX(_x), mY(_y), mZ(_z)
{
}

Vector3::Vector3(const Vector3& _rhs)
	: mX(_rhs.mX), mY(_rhs.mY), mZ(_rhs.mZ)
{
}

Vector3& Vector3::operator=(const Vector3& _rhs)
{
	mX = _rhs.mX;
	mY = _rhs.mY;
	mZ = _rhs.mZ;
	return *this;
}

Vector3::Vector3(Vector3&& _rhs) noexcept
	: mX(std::exchange(_rhs.mX, 0.f)),
	mY(std::exchange(_rhs.mY, 0.f)),
	mZ(std::exchange(_rhs.mZ, 0.f))
{
}

Vector3& Vector3::operator=(Vector3&& _rhs) noexcept
{
	std::swap(mX, _rhs.mX);
	std::swap(mY, _rhs.mY);
	std::swap(mZ, _rhs.mZ);

	return *this;
}

Vector3::Vector3(const glm::vec3& _rhs)
	: mX(_rhs.x), mY(_rhs.y), mZ(_rhs.z)
{
}

Vector3& Vector3::operator=(const glm::vec3& _rhs)
{
	mX = _rhs.x;
	mY = _rhs.y;
	mZ = _rhs.z;
	return *this;
}

Vector3::Vector3(glm::vec3&& _rhs) noexcept
	: mX(std::exchange(_rhs.x, 0.f)),
	mY(std::exchange(_rhs.y, 0.f)),
	mZ(std::exchange(_rhs.z, 0.f))
{
}

Vector3& Vector3::operator=(glm::vec3&& _rhs) noexcept
{
	std::swap(mX, _rhs.x);
	std::swap(mY, _rhs.y);
	std::swap(mZ, _rhs.z);

	return *this;
}

Vector3::Vector3(const glm::vec2& _rhs)
	: mX(_rhs.x), mY(_rhs.y), mZ(1)
{
}

Vector3& Vector3::operator=(const glm::vec2& _rhs)
{
	mX = _rhs.x;
	mY = _rhs.y;
	mZ = 1;
	return *this;
}

Vector3::Vector3(glm::vec2&& _rhs) noexcept
	: mX(std::exchange(_rhs.x, 0.f)),
	mY(std::exchange(_rhs.y, 0.f)),
	mZ(1)
{
}

Vector3& Vector3::operator=(glm::vec2&& _rhs) noexcept
{
	std::swap(mX, _rhs.x);
	std::swap(mY, _rhs.y);
	mZ = 1;

	return *this;
}

float Vector3::Dot(const Vector3& _rhs) const
{
	return mX * _rhs.mX + mY * _rhs.mY + mZ * _rhs.mZ;
}

Vector3 Vector3::Cross(const Vector3& _rhs) const
{
	Vector3 temp{};
	temp.mX = mY - _rhs.mZ;
	temp.mY = mZ - _rhs.mX;
	temp.mZ = mX - _rhs.mY;
	return temp;
}

void Vector3::Normalize()
{
	float theLength = Length();
	mX /= theLength;
	mY /= theLength;
	mZ /= theLength;
}

float Vector3::Length() const
{
	return std::sqrtf(LengthSquared());
}

float Vector3::LengthSquared() const
{
	return mX * mX + mY * mY + mZ * mZ;
}

Vector3& Vector3::operator+=(const Vector3& _rhs)
{
	mX += _rhs.mX;
	mY += _rhs.mY;
	mZ += _rhs.mZ;

	return *this;
}

Vector3& Vector3::operator-=(const Vector3& _rhs)
{
	mX -= _rhs.mX;
	mY -= _rhs.mY;
	mZ -= _rhs.mZ;

	return *this;
}

Vector3& Vector3::operator*=(float _val)
{
	mX *= _val;
	mY *= _val;
	mZ *= _val;

	return *this;
}

Vector3& Vector3::operator/=(float _val)
{
	mX /= _val;
	mY /= _val;
	mZ /= _val;

	return *this;
}

Vector3& Vector3::operator-()
{
	mX = -mX;
	mY = -mY;
	mZ = -mZ;

	return *this;
}

Vector3::operator glm::vec3() const
{
	return glm::vec3(mX, mY, mZ);
}

Vector3::operator glm::vec2() const
{
	return glm::vec2(mX, mY);
}

float Vector3::X() const
{
	return mX;
}

float Vector3::Y() const
{
	return mY;
}

float Vector3::Z() const
{
	return mZ;
}

void Vector3::X(float _val)
{
	mX = _val;
}

void Vector3::Y(float _val)
{
	mY = _val;
}

void Vector3::Z(float _val)
{
	mZ = _val;
}

Vector3::~Vector3()
{
}


Vector3 operator+(const Vector3& _lhs, const Vector3& _rhs)
{
	Vector3 temp{ _lhs };
	return temp += _rhs;
}

Vector3 operator-(const Vector3& _lhs, const Vector3& _rhs)
{
	Vector3 temp{ _lhs };
	return temp -= _rhs;
}

float operator*(const Vector3& _lhs, const Vector3& _rhs)
{
	return _lhs.Dot(_rhs);
}

Vector3 operator^(const Vector3& _lhs, const Vector3& _rhs)
{
	return _lhs.Cross(_rhs);
}

std::ostream& operator<<(std::ostream& os, const Vector3& _vec)
{
	return os << "[ " << _vec.mX << " , " << _vec.mY << " , " << _vec.mZ << " ]\n";
}


Vector3 operator*(const Vector3& _vec, float _val)
{
	return Vector3(_vec.X() * _val, _vec.Y() * _val, _vec.Z() * _val);
}

Vector3 operator*(float _val, const Vector3& _vec)
{
	return Vector3(_vec.X() * _val, _vec.Y() * _val, _vec.Z() * _val);
}

Vector3 operator/(const Vector3& _vec, float _val)
{
	if (_val != 0.f)
		return Vector3(_vec.X() / _val, _vec.Y() / _val, _vec.Z() / _val);
	else
	{
#if _ENGINE
		DebugLogger::GetInstance()->Log("Error: attempt to divide by zero.\n");
#endif // _ENGINE
		return Vector3();
	}
}

bool operator==(const Vector3& _lhs, const Vector3& _rhs)
{
	return (_lhs.X() == _rhs.X()) && (_lhs.Y() == _rhs.Y()) && (_lhs.Z() == _rhs.Z());
}

bool operator!=(const Vector3& _lhs, const Vector3& _rhs)
{
	return !(_lhs == _rhs);
}
