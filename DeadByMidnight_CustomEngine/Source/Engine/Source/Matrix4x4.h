/*!
All content © 2023 DigiPen Institute of Technology Singapore, all rights reserved.
@file       Matrix4x4.h
@author     CAI Wenjin (100%)
@co-author

----------------------------------------------------------------------------

@course     CSD2401F23
@section    CSD2401F23-A
@project    CSD2401F23 Software Engineering Project 3

@brief      Matrix 4x4 class declarations.
*//*______________________________________________________________________*/

#include "MathDefines.h"
#include "Vector4.h"
#include "Vector4.h"
#include <array>
#include <cmath>
#include <iostream>
#include <utility>
#include <glm/gtc/matrix_transform.hpp>

#ifndef MATRIX4X4_H
#define MATRIX4X4_H

class Matrix4x4
{
private:
	std::array<float, 16> mMat;

	/*!
	 * @brief
	 * Matrix multiplication between 2 Matrix4x4.
	 * @param lhs
	 * The 1st Matrix4x4 for the matrix multiplication.
	 * @param _rhs
	 * The 2nd Matrix4x4 for the matrix multiplication.
	 * @return
	 * The resultant Matrix4x4.
	*/
	friend Matrix4x4 operator*(const Matrix4x4& _lhs, const Matrix4x4& _rhs);

	/*!
	 * @brief
	 * Matrix multiplication between a Matrix4x4 and Vector4.
	 * @param lhs
	 * The matrix to perform the multiplication with.
	 * @param _rhs
	 * The Vector4 to perform the multiplication with.
	 * @return
	 * The Vector4 result.
	*/
	friend Vector4 operator*(const Matrix4x4& _lhs, const Vector4& _rhs);

	/*!
	 * @brief
	 * Performs a scalar multiplication onto the matrix.
	 * @param lhs
	 * The matrix to perform the scalar multiplication on.
	 * @param _val
	 * The scalar value to multiply by.
	 * @return
	 * The resultant matrix after scalar multiplication.
	*/
	friend Matrix4x4 operator*(const Matrix4x4& _lhs, float _val);

	/*!
	 * @brief
	 * Operator overload for printing out the information of the Matrix4x4 object.
	 * @param os
	 * Reference to the std::ostream object.
	 * @param mat
	 * Reference to the Matrix4x4 object to print.
	 * @return
	 * The information to print out to the ostream object.
	 * The format is as follows:
	 * | [0] , [4] , [8] , [12] |
	 * | [1] , [5] , [9] , [13] |
	 * | [2] , [6] , [10], [14] |
	 * | [3] , [7] , [11], [15] |
	*/
	friend std::ostream& operator<<(std::ostream& _os, const Matrix4x4& _mat);
public:

	/*!
	 * @brief
	 * Constructor for Matrix4x4 class.
	*/
	Matrix4x4();

	/*!
	 * @brief 
	 * Constructor for Matrix4x4 class.
	 * @param _m00 
	 * Row 0, Col 0 value.
	 * @param _m10 
	 * Row 1, Col 0 value.
	 * @param _m20 
	 * Row 2, Col 0 value.
	 * @param _m30 
	 * Row 3, Col 0 value.
	 * @param _m01 
	 * Row 0, Col 1 value.
	 * @param _m11
	 * Row 1, Col 1 value.
	 * @param _m21
	 * Row 2, Col 1 value.
	 * @param _m31 
	 * Row 3, Col 1 value.
	 * @param _m02
	 * Row 0, Col 2 value.
	 * @param _m12
	 * Row 1, Col 2 value.
	 * @param _m22
	 * Row 2, Col 2 value.
	 * @param _m32 
	 * Row 3, Col 2 value.
	 * @param _m03 
	 * Row 0, Col 3 value.
	 * @param _m13 
	 * Row 1, Col 3 value.
	 * @param _m23 
	 * Row 2, Col 3 value.
	 * @param _m33 
	 * Row 3, Col 3 value
	*/
	Matrix4x4(float _m00, float _m10, float _m20, float _m30,
						float _m01, float _m11, float _m21, float _m31,
						float _m02, float _m12, float _m22, float _m32,
						float _m03, float _m13, float _m23, float _m33);

	/*!
	 * @brief
	 * Copy constructor for Matrix4x4 class.
	 * @param _rhs
	 * Read-only reference to the other Matrix4x4 object.
	*/
	Matrix4x4(const Matrix4x4& _rhs);

	/*!
	 * @brief
	 * Copy assignment for Matrix4x4 class.
	 * @param _rhs
	 * Read-only reference to the other Matrix4x4 object.
	 * @return
	 * The reference to the modified Matrix4x4 object.
	*/
	Matrix4x4& operator=(const Matrix4x4& _rhs);

	/*!
	 * @brief
	 * Move constructor for Matrix4x4 class.
	 * @param _rhs
	 * R-value reference to the Matrix4x4 class.
	*/
	Matrix4x4(Matrix4x4&& _rhs) noexcept;

	/*!
	 * @brief
	 * Move assignment for Matrix4x4 class.
	 * @param _rhs
	 * R-value reference to the Matrix4x4 class.
	 * @return
	 * The reference to the modified Matrix4x4 object.
	*/
	Matrix4x4& operator=(Matrix4x4&& _rhs) noexcept;

	/*!
	 * @brief
	 * Subscrip operator overload for accessing the element in the Matrix4x4 object.
	 * @param _val
	 * The index to search for.
	 * @return
	 * The value of the element from the index.
	*/
	float& operator[](unsigned _val);

	/*!
	 * @brief
	 * Destructor for Matrix4x4 class.
	*/
	~Matrix4x4();

	/*!
	 * @brief
	 * Sets the matrix to identity matrix.
	*/
	void SetToIdentity();

	/*!
	 * @brief
	 * Sets the matrix to zero matrix.
	*/
	void SetToZero();

	/*!
	 * @brief
	 * Gets the Vector4 representation of the specified column of the matrix.
	 * @param _col
	 * The column to access.
	 * @return
	 * The Vector4 representation of the specified column of the matrix.
	*/
	Vector4 GetCol(unsigned int _col) const;

	/*!
	 * @brief
	 * Gets the Vector4 representation of the specified _row of the matrix.
	 * @param _row
	 * The _row to access.
	 * @return
	 * The Vector4 representation of the specified _row of the matrix.
	*/
	Vector4 GetRow(unsigned int _row) const;

	/*!
	 * @brief
	 * Transpose the matrix.
	*/
	void Transpose();

	/*!
	 * @brief
	 * Inverse the matrix.
	*/
	void Inverse();

	/*!
	 * @brief
	 * Builds the translation matrix with the 3 float values.
	 * @param _x
	 * How much to translate in the _x axis.
	 * @param _y
	 * How much to translate in the _y axis.
	 * @param _z
	 * How much to translate in the _z axis.
	 * @return
	 * The translation matrix.
	*/
	Matrix4x4 Translate(float _x, float _y, float _z);

	/*!
	 * @brief
	 * Builds the translation matrix using a given Vector4.
	 * @param _vec
	 * The vector to build the translation matrix with.
	 * @return
	 * The translation matrix.
	*/
	Matrix4x4 Translate(const Vector4& _vec);

	/*!
	 * @brief
	 * Builds the rotation matrix by the angle specified in degrees.
	 * @param _deg
	 * The angle to rotate in degrees.
	 * @return
	 * The rotation matrix.
	*/
	Matrix4x4 RotateDegrees(float _deg);

	/*!
	 * @brief
	 * Builds the rotation matrix by the angle specified in radians.
	 * @param _rad
	 * The angle to rotate in radians.
	 * @return
	 * The rotation matrix.
	*/
	Matrix4x4 RotateRadians(float _rad);

	/*!
	 * @brief
	 * Builds the scaling matrix by the factors specified in the 3 values.
	 * @param _x
	 * Scaling factor of _x.
	 * @param _y
	 * Scaling factor of _y.
	 * @param _z
	 * Scaling factor of _z.
	 * @param _w
	 * Scaling factor of _w.
	 * Defaults to 1.f.
	 * @return
	 * The scaling matrix.
	*/
	Matrix4x4 Scale(float _x, float _y, float _z, float _w = 1.f);

	/*!
	 * @brief
	 * Builds the scaling matrix by the values in the Vector4.
	 * @param _vec
	 * The scaling factors in a Vector4.
	 * @return
	 * The scaling matrix.
	*/
	Matrix4x4 Scale(const Vector4& _vec);

	/*!
	 * @brief
	 * Conversion operator overload to glm::mat4 for gfx system.
	*/
	operator glm::mat4() const;
};

// aliases
using Mat44 = Matrix4x4;
using Mat4x4 = Matrix4x4;
#endif // !MATRIX3X3_H



