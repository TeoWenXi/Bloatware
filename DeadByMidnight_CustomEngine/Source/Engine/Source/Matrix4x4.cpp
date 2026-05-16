/*!
All content © 2023 DigiPen Institute of Technology Singapore, all rights reserved.
@file       Matrix4x4.cpp
@author     CAI Wenjin (100%)
@co-author

----------------------------------------------------------------------------

@course     CSD2401F23
@section    CSD2401F23-A
@project    CSD2401F23 Software Engineering Project 3

@brief      Matrix 4x4 class definitions.
*//*______________________________________________________________________*/

#include "Matrix4x4.h"
#if _ENGINE
#include "DebugLogger.h"
#endif // _ENGINE



Matrix4x4::Matrix4x4()
{
	SetToIdentity();
}

Matrix4x4::Matrix4x4(float _m00, float _m10, float _m20, float _m30,
										 float _m01, float _m11, float _m21, float _m31,
										 float _m02, float _m12, float _m22, float _m32,
										 float _m03, float _m13, float _m23, float _m33)
{
	mMat[0] = _m00,	 mMat[1] = _m10,  mMat[2] = _m20,  mMat[3] = _m30,
	mMat[4] = _m01,	 mMat[5] = _m11,  mMat[6] = _m21,  mMat[7] = _m31,
	mMat[8] = _m02,	 mMat[9] = _m12,  mMat[10] = _m22, mMat[11] = _m32,
	mMat[12] = _m03, mMat[13] = _m13, mMat[14] = _m23, mMat[15] = _m33;
}

Matrix4x4::Matrix4x4(const Matrix4x4& _rhs)
{
	mMat = _rhs.mMat;
}

Matrix4x4& Matrix4x4::operator=(const Matrix4x4& _rhs)
{
	if (this != &_rhs)
		mMat = _rhs.mMat;

	return *this;
}

Matrix4x4::Matrix4x4(Matrix4x4&& _rhs) noexcept
{
	mMat = std::move(_rhs.mMat);
}

Matrix4x4& Matrix4x4::operator=(Matrix4x4&& _rhs) noexcept
{
	if (this != &_rhs)
		mMat = std::move(_rhs.mMat);

	return *this;
}

float& Matrix4x4::operator[](unsigned _val)
{
	if (_val > 15)
	{
		try
		{
			throw std::out_of_range("Error: attempt to access invalid index in matrix.\n");

		}
		catch (const std::exception& e)
		{
			std::cerr << e.what() << std::endl;
			return mMat[0];
		}
	}
	else
		return mMat[_val];
}

Matrix4x4::~Matrix4x4()
{
}

void Matrix4x4::SetToIdentity()
{
	mMat.fill(0.f);
	mMat[0] = mMat[5] = mMat[10] = mMat[15] = 1.f;
}

void Matrix4x4::SetToZero()
{
	mMat.fill(0.f);
}

Vector4 Matrix4x4::GetCol(unsigned int _col) const
{
	if (_col >= 4)
	{
		try
		{
			throw std::out_of_range("Error: attempt to access invalid column in matrix.\n");

		}
		catch (const std::exception& e)
		{
			std::cerr << e.what() << std::endl;
		}

		return Vector4();
	}

	return Vector4(mMat[_col], mMat[_col + 4], mMat[_col + 8], mMat[_col + 12]);
}

Vector4 Matrix4x4::GetRow(unsigned int row) const
{
	if (row >= 4)
	{
		try
		{
			throw std::out_of_range("Error: attempt to access invalid row in matrix.\n");

		}
		catch (const std::exception& e)
		{
			std::cerr << e.what() << std::endl;
		}

		return Vector4();
	}

	return Vector4(mMat[row * 4], mMat[row * 4 + 1], mMat[row * 4 + 2], mMat[row * 4 + 3]);
}

void Matrix4x4::Transpose()
{
	std::swap(mMat[1], mMat[4]);
	std::swap(mMat[2], mMat[8]);
	std::swap(mMat[3], mMat[12]);
	std::swap(mMat[7], mMat[13]);
	std::swap(mMat[11], mMat[14]);
}

void Matrix4x4::Inverse()
{
	// TODO: to implement if really needed
	//float determinant =
	//	(mMat[0] * (mMat[4] * mMat[8] - mMat[5] * mMat[7])) -
	//	(mMat[1] * (mMat[3] * mMat[8] - mMat[5] * mMat[6])) +
	//	(mMat[2] * (mMat[3] * mMat[7] - mMat[4] * mMat[6]));

	//if (determinant != 0.f)
	//{
	//	Matrix4x4 adjugate{};

	//	adjugate.mMat[0] = (mMat[4] * mMat[8] - mMat[5] * mMat[7]);
	//	adjugate.mMat[1] = (mMat[2] * mMat[7] - mMat[1] * mMat[8]);
	//	adjugate.mMat[2] = (mMat[1] * mMat[5] - mMat[2] * mMat[4]);
	//	adjugate.mMat[3] = (mMat[5] * mMat[6] - mMat[3] * mMat[8]);
	//	adjugate.mMat[4] = (mMat[0] * mMat[8] - mMat[2] * mMat[6]);
	//	adjugate.mMat[5] = (mMat[2] * mMat[3] - mMat[0] * mMat[5]);
	//	adjugate.mMat[6] = (mMat[3] * mMat[7] - mMat[4] * mMat[6]);
	//	adjugate.mMat[7] = (mMat[1] * mMat[6] - mMat[0] * mMat[7]);
	//	adjugate.mMat[8] = (mMat[0] * mMat[4] - mMat[1] * mMat[3]);

	//	for (int i = 0; i < 9; ++i)
	//		mMat[i] = (1.f / determinant) * adjugate.mMat[i];
	//}
	//else
	//{
	//	// need do error handling here
	//	// current implementation will set to identity matrix
	//	std::cout << "Matrix is non-invertable, setting matrix to identity.\n";
	//	SetToIdentity();
	//}
}

Matrix4x4 Matrix4x4::Translate(float _x, float _y, float _z)
{
	Matrix4x4 mat{};

	mat[12] += _x;
	mat[13] += _y;
	mat[14] += _z;
	
	return mat;
}

Matrix4x4 Matrix4x4::Translate(const Vector4& _vec)
{
	return Translate(_vec.X(), _vec.Y(), _vec.Z());
}

Matrix4x4 Matrix4x4::RotateDegrees(float _deg)
{
	Matrix4x4 mat{};
	mat.mMat[0] = std::cosf(DegreeToRadians(_deg)); mat.mMat[3] = -std::sinf(DegreeToRadians(_deg));
	mat.mMat[1] = std::sinf(DegreeToRadians(_deg)); mat.mMat[4] = std::cosf(DegreeToRadians(_deg));

	return mat;
}

Matrix4x4 Matrix4x4::RotateRadians(float _rad)
{
	Matrix4x4 mat{};
	mat.mMat[0] = std::cosf(_rad); mat.mMat[3] = -std::sinf(_rad);
	mat.mMat[1] = std::sinf(_rad); mat.mMat[4] = std::cosf(_rad);

	return mat;
}

Matrix4x4 Matrix4x4::Scale(float _x, float _y, float _z, float _w)
{
	Matrix4x4 mat{};
	mat.mMat[0] = _x;
	mat.mMat[5] = _y;
	mat.mMat[10] = _z;
	mat.mMat[15] = _w;

	return mat;
}

Matrix4x4 Matrix4x4::Scale(const Vector4& _vec)
{
	return Scale(_vec.X(), _vec.Y(), _vec.Z(), _vec.W());
}

Matrix4x4::operator glm::mat4() const
{
	return glm::mat4
	{
		mMat[0], mMat[1], mMat[2], mMat[3],
		mMat[4], mMat[5], mMat[6], mMat[7],
		mMat[8], mMat[9], mMat[10],mMat[11],
		mMat[12], mMat[13], mMat[14], mMat[15]
	};
}

Matrix4x4 operator*(const Matrix4x4& _lhs, const Matrix4x4& _rhs)
{
	Matrix4x4 result{};

	for (size_t col{}; col < 4; ++col)
	{
		for (size_t row{}; row < 4; ++row)
		{
			float sum = 0.f;

			// perform the dot product of the column from lhs and the row from rhs.
			for (size_t i{}; i < 4; ++i)
				sum += _lhs.mMat[i * 4 + col] * _rhs.mMat[row * 4 + i];

			result.mMat[row * 4 + col] = sum;
		}
	}

	return result;
}

Vector4 operator*(const Matrix4x4& _lhs, const Vector4& _rhs)
{
	Vector4 result{};
	result.X((_lhs.mMat[0] * _rhs.X()) + (_lhs.mMat[4] * _rhs.Y()) + (_lhs.mMat[8] * _rhs.Z()) + (_lhs.mMat[12] * _rhs.W()));
	result.Y((_lhs.mMat[1] * _rhs.X()) + (_lhs.mMat[5] * _rhs.Y()) + (_lhs.mMat[9] * _rhs.Z()) + (_lhs.mMat[13] * _rhs.W()));
	result.Z((_lhs.mMat[2] * _rhs.X()) + (_lhs.mMat[6] * _rhs.Y()) + (_lhs.mMat[10] * _rhs.Z()) + (_lhs.mMat[14] * _rhs.W()));
	result.W((_lhs.mMat[3] * _rhs.X()) + (_lhs.mMat[7] * _rhs.Y()) + (_lhs.mMat[11] * _rhs.Z()) + (_lhs.mMat[15] * _rhs.W()));
	return result;
}

Matrix4x4 operator*(const Matrix4x4& _lhs, float _val)
{
	Matrix4x4 mat{};
	for (int i{}; i < 16; ++i)
		mat.mMat[i] = _lhs.mMat[i] * _val;

	return mat;
}

std::ostream& operator<<(std::ostream& _os, const Matrix4x4& _mat)
{
	_os << "| " << _mat.mMat[0] << " , " << _mat.mMat[4] << " , " << _mat.mMat[8]  << " , " << _mat.mMat[12] << " |\n";
	_os << "| " << _mat.mMat[1] << " , " << _mat.mMat[5] << " , " << _mat.mMat[9]  << " , " << _mat.mMat[13] << " |\n";
	_os << "| " << _mat.mMat[2] << " , " << _mat.mMat[6] << " , " << _mat.mMat[10] << " , " << _mat.mMat[14] << " |\n";
	_os << "| " << _mat.mMat[3] << " , " << _mat.mMat[7] << " , " << _mat.mMat[11] << " , " << _mat.mMat[15] << " |\n";
	return _os;
}
