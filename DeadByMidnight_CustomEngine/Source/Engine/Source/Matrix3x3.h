/*!
All content © 2023 DigiPen Institute of Technology Singapore, all rights reserved.
@file       Matrix3x3.h
@author     CAI Wenjin (100%)
@co-author

----------------------------------------------------------------------------

@course     CSD2401F23
@section    CSD2401F23-A
@project    CSD2401F23 Software Engineering Project 3

@brief      Matrix 3x3 class declarations.
*//*______________________________________________________________________*/

#include "MathDefines.h"
#include "Vector3.h"
#include <array>
#include <cmath>
#include <iostream>
#include <utility>
#include <glm/gtc/matrix_transform.hpp>

#ifndef MATRIX3X3_H
#define MATRIX3X3_H

class Matrix3x3
{
private:
	std::array<float, 9> mMat;


	/*!
	 * @brief
	 * Matrix multiplication between 2 Matrix3x3.
	 * @param lhs
	 * The 1st Matrix3x3 for the matrix multiplication.
	 * @param _rhs
	 * The 2nd Matrix3x3 for the matrix multiplication.
	 * @return
	 * The resultant Matrix3x3.
	*/
	friend Matrix3x3 operator*(const Matrix3x3& _lhs, const Matrix3x3& _rhs);

	/*!
	 * @brief
	 * Matrix multiplication between a Matrix3x3 and Vector3.
	 * @param lhs
	 * The matrix to perform the multiplication with.
	 * @param _rhs
	 * The vector3 to perform the multiplication with.
	 * @return
	 * The vector3 result.
	*/
	friend Vector3 operator*(const Matrix3x3& _lhs, const Vector3& _rhs);

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
	friend Matrix3x3 operator*(const Matrix3x3& _lhs, float _val);

	/*!
	 * @brief
	 * Operator overload for printing out the information of the Matrix3x3 object.
	 * @param os
	 * Reference to the std::ostream object.
	 * @param mat
	 * Reference to the Matrix3x3 object to print.
	 * @return
	 * The information to print out to the ostream object.
	 * The format is as follows:
	 * | [0] , [3] , [6] |
	 * | [1] , [4] , [7] |
	 * | [2] , [5] , [8] |
	*/
	friend std::ostream& operator<<(std::ostream& _os, const Matrix3x3& _mat);
public:

	/*!
	 * @brief
	 * Constructor for Matrix3x3 class.
	*/
	Matrix3x3();

	/*!
	 * @brief
	 * Constructor for Matrix3x3 class.
	 * @param _m00
	 * Row 0, Col 0 value.
	 * @param _m10
	 * Row 1, Col 0 value.
	 * @param _m20
	 * Row 2, Col 0 value.
	 * @param _m01
	 * Row 0, Col 1 value.
	 * @param _m11
	 * Row 1, Col 1 value.
	 * @param _m21
	 * Row 2, Col 1 value.
	 * @param _m02
	 * Row 0, Col 2 value.
	 * @param _m12
	 * Row 1, Col 2 value.
	 * @param _m22
	 * Row 2, Col 2 value.
	*/

	Matrix3x3(float _m00, float _m10, float _m20,
						float _m01, float _m11, float _m21,
						float _m02, float _m12, float _m22);

	/*!
	 * @brief
	 * Copy constructor for Matrix3x3 class.
	 * @param _rhs
	 * Read-only reference to the other Matrix3x3 object.
	*/
	Matrix3x3(const Matrix3x3& _rhs);

	/*!
	 * @brief
	 * Copy assignment for Matrix3x3 class.
	 * @param _rhs
	 * Read-only reference to the other Matrix3x3 object.
	 * @return
	 * The reference to the modified Matrix3x3 object.
	*/
	Matrix3x3& operator=(const Matrix3x3& _rhs);

	/*!
	 * @brief
	 * Move constructor for Matrix3x3 class.
	 * @param _rhs
	 * R-value reference to the Matrix3x3 class.
	*/
	Matrix3x3(Matrix3x3&& _rhs) noexcept;

	/*!
	 * @brief
	 * Move assignment for Matrix3x3 class.
	 * @param _rhs
	 * R-value reference to the Matrix3x3 class.
	 * @return
	 * The reference to the modified Matrix3x3 object.
	*/
	Matrix3x3& operator=(Matrix3x3&& _rhs) noexcept;

	/*!
	 * @brief
	 * Subscrip operator overload for accessing the element in the Matrix3x3 object.
	 * @param _val
	 * The index to search for.
	 * @return
	 * The value of the element from the index.
	*/
	float& operator[](unsigned _val);

	/*!
	 * @brief
	 * Destructor for Matrix3x3 class.
	*/
	~Matrix3x3();

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
	 * Gets the Vector3 representation of the specified column of the matrix.
	 * @param _col
	 * The column to access.
	 * @return
	 * The Vector3 representation of the specified column of the matrix.
	*/
	Vector3 GetCol(unsigned int _col) const;

	/*!
	 * @brief
	 * Gets the Vector3 representation of the specified _row of the matrix.
	 * @param _row
	 * The _row to access.
	 * @return
	 * The Vector3 representation of the specified _row of the matrix.
	*/
	Vector3 GetRow(unsigned int _row) const;

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
	Matrix3x3 Translate(float _x, float _y, float _z = 1.f);

	/*!
	 * @brief
	 * Builds the translation matrix using a given Vector3.
	 * @param _vec
	 * The vector to build the translation matrix with.
	 * @return
	 * The translation matrix.
	*/
	Matrix3x3 Translate(const Vector3& _vec);

	/*!
	 * @brief
	 * Builds the rotation matrix by the angle specified in degrees.
	 * @param _deg
	 * The angle to rotate in degrees.
	 * @return
	 * The rotation matrix.
	*/
	Matrix3x3 RotateDegrees(float _deg);

	/*!
	 * @brief
	 * Builds the rotation matrix by the angle specified in radians.
	 * @param _rad
	 * The angle to rotate in radians.
	 * @return
	 * The rotation matrix.
	*/
	Matrix3x3 RotateRadians(float _rad);

	/*!
	 * @brief
	 * Builds the scaling matrix by the factors specified in the 3 values.
	 * @param _x
	 * Scaling factor of _x.
	 * @param _y
	 * Scaling factor of _y.
	 * @param _z
	 * Scaling factor of _z.
	 * @return
	 * The scaling matrix.
	*/
	Matrix3x3 Scale(float _x, float _y, float _z = 1.f);

	/*!
	 * @brief
	 * Builds the scaling matrix by the values in the Vector3.
	 * @param _vec
	 * The scaling factors in a Vector3.
	 * @return
	 * The scaling matrix.
	*/
	Matrix3x3 Scale(const Vector3& _vec);

	/*!
	 * @brief
	 * Conversion operator overload to glm::mat3 for gfx system.
	*/
	operator glm::mat3() const;
};

// aliases
using Mat33 = Matrix3x3;
using Mat3x3 = Matrix3x3;
#endif // !MATRIX3X3_H



