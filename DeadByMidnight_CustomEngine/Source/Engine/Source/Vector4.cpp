/*!
All content © 2023 DigiPen Institute of Technology Singapore, all rights reserved.
@file       Vector4.cpp
@author     CAI Wenjin
@co-author

----------------------------------------------------------------------------

@course     CSD2401F23
@section    CSD2401F23-A
@project    CSD2401F23 Software Engineering Project 3

@brief      Vector4 class definitions.
*//*______________________________________________________________________*/

#include "Vector4.h"
#if _ENGINE
#include "DebugLogger.h"
#endif // _ENGINE

Vector4::Vector4()
	: mX(), mY(), mZ(), mW()
{
}

Vector4::Vector4(float _x, float _y, float _z, float _w)
	: mX(_x), mY(_y), mZ(_z), mW(_w)
{
}

Vector4::Vector4(const Vector4& _rhs)
	: mX(_rhs.mX), mY(_rhs.mY), mZ(_rhs.mZ), mW(_rhs.mW)
{
}

Vector4& Vector4::operator=(const Vector4& _rhs)
{
	mX = _rhs.mX;
	mY = _rhs.mY;
	mZ = _rhs.mZ;
	mW = _rhs.mW;

	return *this;
}

Vector4::Vector4(Vector4&& _rhs) noexcept
	: mX(std::exchange(_rhs.mX, 0.f)),
		mY(std::exchange(_rhs.mY, 0.f)),
		mZ(std::exchange(_rhs.mZ, 0.f)),
		mW(std::exchange(_rhs.mW, 0.f))
{
}

Vector4& Vector4::operator=(Vector4&& _rhs) noexcept
{
	std::swap(mX, _rhs.mX);
	std::swap(mY, _rhs.mY);
	std::swap(mZ, _rhs.mZ);
	std::swap(mW, _rhs.mW);

	return *this;
}

Vector4::Vector4(const glm::vec4& _rhs)
	: mX(_rhs.x), mY(_rhs.y), mZ(_rhs.z), mW(_rhs.w)
{
}

Vector4& Vector4::operator=(const glm::vec4& _rhs)
{
	mX = _rhs.x;
	mY = _rhs.y;
	mZ = _rhs.z;
	mW = _rhs.w;

	return *this;
}

Vector4::Vector4(glm::vec4&& _rhs) noexcept
	: mX(std::exchange(_rhs.x, 0.f)),
		mY(std::exchange(_rhs.y, 0.f)),
		mZ(std::exchange(_rhs.z, 0.f)),
		mW(std::exchange(_rhs.w, 0.f))
{
}

Vector4& Vector4::operator=(glm::vec4&& _rhs) noexcept
{
	std::swap(mX, _rhs.x);
	std::swap(mY, _rhs.y);
	std::swap(mZ, _rhs.z);
	std::swap(mW, _rhs.w);

	return *this;
}

Vector4::Vector4(const glm::vec3& _rhs)
	: mX(_rhs.x), mY(_rhs.y), mZ(_rhs.z), mW(1.f)
{
}

Vector4& Vector4::operator=(const glm::vec3& _rhs)
{
	mX = _rhs.x;
	mY = _rhs.y;
	mZ = _rhs.z;
	mW = 1.f;

	return *this;
}

Vector4::Vector4(glm::vec3&& _rhs) noexcept
	: mX(std::exchange(_rhs.x, 0.f)),
		mY(std::exchange(_rhs.y, 0.f)),
		mZ(std::exchange(_rhs.z, 0.f)),
		mW(1.f)
{
}

Vector4& Vector4::operator=(glm::vec3&& _rhs) noexcept
{
	std::swap(mX, _rhs.x);
	std::swap(mY, _rhs.y);
	std::swap(mZ, _rhs.z);
	mW = 1;

	return *this;
}

float Vector4::Dot(const Vector4& _rhs) const
{
	return mX * _rhs.mX + mY * _rhs.mY + mZ * _rhs.mZ + mW * _rhs.mW;
}

void Vector4::Normalize()
{
	mX /= Length();
	mY /= Length();
	mZ /= Length();
	mW /= Length();
}

float Vector4::Length() const
{
	return std::sqrtf(LengthSquared());
}

float Vector4::LengthSquared() const
{
	return mX * mX + mY * mY + mZ * mZ + mW * mW;
}

Vector4& Vector4::operator+=(const Vector4& _rhs)
{
	mX += _rhs.mX;
	mY += _rhs.mY;
	mZ += _rhs.mZ;
	mW += _rhs.mW;

	return *this;
}

Vector4& Vector4::operator-=(const Vector4& _rhs)
{
	mX -= _rhs.mX;
	mY -= _rhs.mY;
	mZ -= _rhs.mZ;
	mW -= _rhs.mW;

	return *this;
}

Vector4& Vector4::operator*=(float _val)
{
	mX *= _val;
	mY *= _val;
	mZ *= _val;
	mW *= _val;

	return *this;
}

Vector4& Vector4::operator/=(float _val)
{
	mX /= _val;
	mY /= _val;
	mZ /= _val;
	mW /= _val;

	return *this;
}

Vector4& Vector4::operator-()
{
	mX = -mX;
	mY = -mY;
	mZ = -mZ;
	mW = -mW;

	return *this;
}

Vector4::operator glm::vec4() const
{
	return glm::vec4(mX, mY, mZ, mW);
}

Vector4::operator glm::vec3() const
{
	return glm::vec3(mX, mY, mZ);
}

float Vector4::X() const
{
	return mX;
}

float Vector4::Y() const
{
	return mY;
}

float Vector4::Z() const
{
	return mZ;
}

float Vector4::W() const
{
	return mW;
}

void Vector4::X(float _val)
{
	mX = _val;
}

void Vector4::Y(float _val)
{
	mY = _val;
}

void Vector4::Z(float _val)
{
	mZ = _val;
}

void Vector4::W(float _val)
{
	mW = _val;
}

Vector4::~Vector4()
{
}

Vector4 operator+(const Vector4& _lhs, const Vector4& _rhs)
{
	Vector4 temp{ _lhs };
	return temp += _rhs;
}

Vector4 operator-(const Vector4& _lhs, const Vector4& _rhs)
{
	Vector4 temp{ _lhs };
	return temp -= _rhs;
}

float operator*(const Vector4& _lhs, const Vector4& _rhs)
{
	return _lhs.Dot(_rhs);
}

std::ostream& operator<<(std::ostream& os, const Vector4& _vec)
{
	return os << "[ " << _vec.mX << " , "
										<< _vec.mY << " , "
										<< _vec.mZ << " , "
										<< _vec.mW << " ]\n";
}

Vector4 operator*(const Vector4& _vec, float _val)
{
	return Vector4(_vec.X() * _val, _vec.Y() * _val, _vec.Z() * _val, _vec.W() * _val);
}

Vector4 operator*(float _val, const Vector4& _vec)
{
	return Vector4(_vec.X() * _val, _vec.Y() * _val, _vec.Z() * _val, _vec.W() * _val);
}

Vector4 operator/(const Vector4& _vec, float _val)
{
	if (_val != 0.f)
		return Vector4(_vec.X() / _val, _vec.Y() / _val, _vec.Z() / _val, _vec.W() / _val);
	else
	{
#if _ENGINE
		DebugLogger::GetInstance()->Log("Error: attempt to divide by zero.\n");
#endif // _ENGINE
		return Vector4();
	}
}

bool operator==(const Vector4& _lhs, const Vector4& _rhs)
{
	return (_lhs.X() == _rhs.X()) && (_lhs.Y() == _rhs.Y()) && (_lhs.Z() == _rhs.Z()) && (_lhs.W() == _rhs.W());
}

bool operator!=(const Vector4& _lhs, const Vector4& _rhs)
{
	return !(_lhs == _rhs);
}
