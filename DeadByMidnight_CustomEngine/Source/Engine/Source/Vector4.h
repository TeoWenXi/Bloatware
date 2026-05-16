/*!
All content © 2023 DigiPen Institute of Technology Singapore, all rights reserved.
@file       Vector4.h
@author     CAI Wenjin 
@co-author	

----------------------------------------------------------------------------

@course     CSD2401F23
@section    CSD2401F23-A
@project    CSD2401F23 Software Engineering Project 3

@brief      Vector4 class declarations.
*//*______________________________________________________________________*/

#include <algorithm>
#include <cmath>
#include <iostream>
#include <utility>
#include <glm/glm.hpp> //For conversion from glm::vec4

#ifndef VECTOR4_H
#define VECTOR4_H

class Vector4
{
private:

	/*!
	 * @brief
	 * Operator overload to add 2 Vector4 objects.
	 * @param _lhs
	 * The 1st Vector4 object to add.
	 * @param _rhs
	 * The 2nd Vector4 object to add.
	 * @return
	 * The resultant Vector4 after _lhs + _rhs.
	*/
	friend Vector4 operator+(const Vector4& _lhs, const Vector4& _rhs);


	/*!
	 * @brief
	 * Operator overload to subtract 2 Vector4 objects.
	 * @param _lhs
	 * The 1st Vector4 object to subtract.
	 * @param _rhs
	 * The 2nd Vector4 object to subtract.
	 * @return
	 * The resultant Vector4 after _lhs - _rhs.
	*/
	friend Vector4 operator-(const Vector4& _lhs, const Vector4& _rhs);

	/*!
	 * @brief
	 * Operator overload to perform dot product between 2 Vector4 objects.
	 * @param _lhs
	 * The 1st Vector4 object for the dot product.
	 * @param _rhs
	 * The 2nd Vector4 object for the dot product.
	 * @return
	 * The resultant dot product value.
	*/
	friend float operator*(const Vector4& _lhs, const Vector4& _rhs);

	/*!
	 * @brief
	 * Operator overload for printing out the information of the Vector4 object.
	 * @param _os
	 * Reference to the std::ostream object.
	 * @param _vec
	 * Reference to the Vector4 object to print.
	 * @return
	 * The information to print out to the ostream object
	 * in "[ x , y , z ]" format.
	*/
	friend std::ostream& operator<<(std::ostream& _os, const Vector4& _vec);

public:
	/*!
	 * @brief
	 * Constructor for Vector4 class.
	*/
	Vector4();

	/*!
	 * @brief
	 * Constructor for Vector4 class.
	 * @param _x
	 * X component to set.
	 * @param _y
	 * Y component to set.
	 * @param _z
	 * Z component to set.
	 * @param _w
	 * W component to set.
	 * Defaults to 1.f.
	*/
	Vector4(float _x, float _y, float _z, float _w = 1.f);


	/*!
	 * @brief
	 * Copy constructor for Vector4 class.
	 * @param _rhs
	 * Read-only reference to the other Vector4 class.
	*/
	Vector4(const Vector4& _rhs);

	/*!
	 * @brief
	 * Copy assignment for Vector4 class.
	 * @param _rhs
	 * Read-only reference to the other Vector4 class.
	 * @return
	 * Reference to the modified Vector4 class.
	*/
	Vector4& operator=(const Vector4& _rhs);

	/*!
	 * @brief
	 * Move constructor for Vector4 class.
	 * @param _rhs
	 * R-value reference to the other Vector4 class.
	*/
	Vector4(Vector4&& _rhs) noexcept;

	/*!
	 * @brief
	 * Move assignment for the Vector4 class.
	 * @param _rhs
	 * R-value reference to the other Vector4 class.
	 * @return
	 * The modified reference to the Vector4 class.
	*/
	Vector4& operator=(Vector4&& _rhs) noexcept;

	//For glm::vec4 conversion
	/*!
	 * @brief 
	 * glm::vec4 copy constructor
	 * @param _rhs 
	 * Variable to copy
	*/
	Vector4(const glm::vec4& _rhs);

	/*!
	 * @brief 
	 * glm::vec4 copy assignment
	 * @param _rhs 
	 * Variable to copy
	 * @return 
	 * Vector
	*/
	Vector4& operator=(const glm::vec4& _rhs);

	/*!
	 * @brief 
	 * glm::vec4 move constructor
	 * @param _rhs 
	 * Variable to move
	*/
	Vector4(glm::vec4&& _rhs) noexcept;

	/*!
	 * @brief 
	 * glm::vec4 move assignment
	 * @param _rhs 
	 * Variable to move
	 * @return 
	 * Vector
	*/
	Vector4& operator=(glm::vec4&& _rhs) noexcept;


	//For glm::vec3 conversion
	/*!
	 * @brief 
	 * glm::vec3 copy constructor
	 * @param _rhs 
	 * Variable to copy
	*/
	Vector4(const glm::vec3& _rhs);

	/*!
	 * @brief 
	 * glm::vec3 copy assignment
	 * @param _rhs 
	 * Variable to copy
	 * @return 
	 * Vector
	*/
	Vector4& operator=(const glm::vec3& _rhs);

	/*!
	 * @brief 
	 * glm::vec3 move constructor
	 * @param _rhs 
	 * Vector to move
	*/
	Vector4(glm::vec3&& _rhs) noexcept;

	/*!
	 * @brief 
	 * glm::vec3 move assignment
	 * @param _rhs 
	 * variable to be assigned
	 * @return 
	 * Vector
	*/
	Vector4& operator=(glm::vec3&& _rhs) noexcept;

	/*!
	 * @brief
	 * Compute dot product of 2 vectors.
	 * @param _rhs
	 * The other vector to compute the dot product with.
	 * @return
	 * The dot product result.
	*/
	float Dot(const Vector4& _rhs) const;

	/*!
	 * @brief
	 * Normalize the vector
	*/
	void Normalize();

	/*!
	 * @brief
	 * Returns the length of the vector.
	 * @return
	 * Length of the vector.
	*/
	float Length() const;

	/*!
	 * @brief
	 * Returns the squared length of the vector.
	 * @return
	 * Sqared length of the vector.
	*/
	float LengthSquared() const;

	
	/*!
	 * @brief
	 * Operator overload for addition between 2 Vector4 objects.
	 * @param _rhs
	 * The other Vector4 object to add.
	 * @return
	 * The modified reference to the Vector4 object.
	*/
	Vector4& operator+=(const Vector4& _rhs);

	/*!
	 * @brief
	 * Operator overload for subtraction between 2 Vector4 objects.
	 * @param _rhs
	 * The other Vector4 object to subtract from.
	 * @return
	 * The modified reference to the Vector4 object.
	*/
	Vector4& operator-=(const Vector4& _rhs);

	/*!
	 * @brief
	 * Operator overload for scalar multiplication of the Vector4 object.
	 * @param _val
	 * The scalar value to multiply with.
	 * @return
	 * The reference to the modified Vector4 object.
	*/
	Vector4& operator*=(float _val);

	/*!
	 * @brief
	 * Operator overload for scalar division of the Vector4 object.
	 * @param _val
	 * The scalar value to divide with.
	 * @return
	 * The reference to the modified Vector4 object.
	*/
	Vector4& operator/=(float _val);

	/*!
	 * @brief
	 * Operator overload to return the negated values of the Vector4 object.
	 * @return
	 * The reference to the modified Vector4 object.
	*/
	Vector4& operator-();


	/*!
	 * @brief
	 * Converts the current Vector4 into glm::vec4.
	*/
	operator glm::vec4() const;

	/*!
	 * @brief
	 * Converts the current Vector4 into glm::vec3.
	*/
	operator glm::vec3() const;


	/*!
	 * @brief
	 * Access the X component of the Vector4 object.
	 * @return
	 * X component of the Vector4 object.
	*/
	float X() const;

	/*!
	 * @brief
	 * Access the Y component of the Vector4 object.
	 * @return
	 * Y component of the Vector4 object.
	*/
	float Y() const;

	/*!
	 * @brief
	 * Access the Z component of the Vector4 object.
	 * @return
	 * Z component of the Vector4 object.
	*/
	float Z() const;

	/*!
	 * @brief
	 * Access the W component of the Vector4 object.
	 * @return
	 * W component of the Vector4 object.
	*/
	float W() const;

	/*!
	 * @brief
	 * Modifies the X component to the new value '_val'.
	 * @param _val
	 * The new value to set.
	*/
	void X(float _val);

	/*!
	 * @brief
	 * Modifies the Y component to the new value '_val'.
	 * @param _val
	 * The new value to set.
	*/
	void Y(float _val);

	/*!
	 * @brief
	 * Modifies the Z component to the new value '_val'.
	 * @param _val
	 * The new value to set.
	*/
	void Z(float _val);

	/*!
	 * @brief
	 * Modifies the W component to the new value '_val'.
	 * @param _val
	 * The new value to set.
	*/
	void W(float _val);

	//Variables
	float mX, mY, mZ, mW;

	/*!
	 * @brief
	 * Destructor for the Vector4 class.
	*/
	~Vector4();
};

// non-member functions
/*!
 * @brief
 * Operator overload to multiply a value to a Vector4 object.
 * @param _vec
 * The Vector4 object for the multiplication.
 * @param _val
 * The value to multiply.
 * @return
 * The resultant value.
*/
Vector4 operator*(const Vector4& _vec, float _val);

/*!
 * @brief
 * Operator overload to multiply a value to a Vector4 object.
 * @param _val
 * The value to multiply.
 * @param _vec
 * The Vector4 object for the multiplication.
 * @return
 * The resultant value.
*/
Vector4 operator*(float _val, const Vector4& _vec);

/*!
 * @brief
 * Operator overload for scalar division of the Vector4 object.
 * @param _vec
 * Vector object to use as the numerator.
 * @param _val
 * The scalar value to divide with.
 * @return
 * The reference to the modified Vector4 object.
*/
Vector4 operator/(const Vector4& _vec, float _val);

/*!
 * @brief 
 * operator== to compare 2 vectors.
 * @param _lhs 
 * First vector to use for the comparison.
 * @param _rhs 
 * Second vector to use for the comparison.
 * @return 
 * Boolean value to represent if both objects are the same.
*/
bool operator==(const Vector4& _lhs, const Vector4& _rhs);

/*!
 * @brief 
 * operator!= to comapare 2 vectors.
 * @param _lhs 
 * First vector to use for the comparison.
 * @param _rhs 
 * Second vector to use for the comparison.
 * @return 
 * Boolean value to represent if both objects are the same.
*/
bool operator!=(const Vector4& _lhs, const Vector4& _rhs);

// aliases
using Vec4 = Vector4;

#endif // !VECTOR3_H


