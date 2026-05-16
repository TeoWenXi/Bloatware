/*!
All content © 2023 DigiPen Institute of Technology Singapore, all rights reserved.
@file       Vector3.h
@author     SIA Xin Tian (90%)
@co-author	CAI Wenjin (10%)

----------------------------------------------------------------------------

@course     CSD2401F23
@section    CSD2401F23-A
@project    CSD2401F23 Software Engineering Project 3

@brief      Vector3 class declarations.
*//*______________________________________________________________________*/

#include <algorithm>
#include <cmath>
#include <iostream>
#include <utility>
#include <glm/glm.hpp> //For conversion from glm::vec3

#ifndef VECTOR3_H
#define VECTOR3_H

class Vector3
{
private:

	/*!
	 * @brief
	 * Operator overload to add 2 Vector3 objects.
	 * @param lhs
	 * The 1st Vector3 object to add.
	 * @param rhs
	 * The 2nd Vector3 object to add.
	 * @return
	 * The resultant Vector3 after lhs + rhs.
	*/
	friend Vector3 operator+(const Vector3& lhs, const Vector3& rhs);


	/*!
	 * @brief
	 * Operator overload to subtract 2 Vector3 objects.
	 * @param lhs
	 * The 1st Vector3 object to subtract.
	 * @param rhs
	 * The 2nd Vector3 object to subtract.
	 * @return
	 * The resultant Vector3 after lhs - rhs.
	*/
	friend Vector3 operator-(const Vector3& lhs, const Vector3& rhs);


	/*!
	 * @brief
	 * Operator overload to perform dot product between 2 Vector3 objects.
	 * @param lhs
	 * The 1st Vector3 object for the dot product.
	 * @param rhs
	 * The 2nd Vector3 object for the dot product.
	 * @return
	 * The resultant dot product value.
	*/
	friend float operator*(const Vector3& lhs, const Vector3& rhs);


	/*!
	 * @brief
	 * Operator overload to perfom cross product between 2 Vector3 objects.
	 * @param lhs
	 * The 1st Vector3 object for the cross product.
	 * @param rhs
	 * The 2nd Vector3 object for the cross product.
	 * @return
	 * The resultant cross product vector.
	*/
	friend Vector3 operator^(const Vector3& lhs, const Vector3& rhs);


	/*!
	 * @brief
	 * Operator overload for printing out the information of the Vector3 object.
	 * @param os
	 * Reference to the std::ostream object.
	 * @param vec
	 * Reference to the Vector3 object to print.
	 * @return
	 * The information to print out to the ostream object
	 * in "[ x , y , z ]" format.
	*/
	friend std::ostream& operator<<(std::ostream& os, const Vector3& vec);

public:
	/*!
	 * @brief
	 * Constructor for Vector3 class.
	*/
	Vector3();

	/*!
	 * @brief
	 * Constructor for Vector3 class.
	 * @param x
	 * X component to set.
	 * @param y
	 * Y component to set.
	 * @param z
	 * Z component to set.
	*/
	Vector3(float x, float y, float z);


	/*!
	 * @brief
	 * Copy constructor for Vector3 class.
	 * @param rhs
	 * Read-only reference to the other Vector3 class.
	*/
	Vector3(const Vector3& rhs);

	/*!
	 * @brief
	 * Copy assignment for Vector3 class.
	 * @param rhs
	 * Read-only reference to the other Vector3 class.
	 * @return
	 * Reference to the modified Vector3 class.
	*/
	Vector3& operator=(const Vector3& rhs);

	/*!
	 * @brief
	 * Move constructor for Vector3 class.
	 * @param rhs
	 * R-value reference to the other Vector3 class.
	*/
	Vector3(Vector3&& rhs) noexcept;

	/*!
	 * @brief
	 * Move assignment for the Vector3 class.
	 * @param rhs
	 * R-value reference to the other Vector3 class.
	 * @return
	 * The modified reference to the Vector3 class.
	*/
	Vector3& operator=(Vector3&& rhs) noexcept;



	//For glm::vec3 conversion
	/*!
	 * @brief 
	 * glm::vec3 copy constructor
	 * @param rhs 
	 * Variable to copy
	*/
	Vector3(const glm::vec3& rhs);

	/*!
	 * @brief 
	 * glm::vec3 copy assignment
	 * @param rhs 
	 * Variable to copy
	 * @return 
	 * Vector
	*/
	Vector3& operator=(const glm::vec3& rhs);

	/*!
	 * @brief 
	 * glm::vec3 move constructor
	 * @param rhs 
	 * Variable to move
	*/
	Vector3(glm::vec3&& rhs) noexcept;

	/*!
	 * @brief 
	 * glm::vec3 move assignment
	 * @param rhs 
	 * Variable to move
	 * @return 
	 * Vector
	*/
	Vector3& operator=(glm::vec3&& rhs) noexcept;


	//For glm::vec2 conversion
	/*!
	 * @brief 
	 * glm::vec2 copy constructor
	 * @param rhs 
	 * Variable to copy
	*/
	Vector3(const glm::vec2& rhs);

	/*!
	 * @brief 
	 * glm::vec2 copy assignment
	 * @param rhs 
	 * Variable to copy
	 * @return 
	 * Vector
	*/
	Vector3& operator=(const glm::vec2& rhs);

	/*!
	 * @brief 
	 * glm::vec2 move constructor
	 * @param rhs 
	 * Vector to move
	*/
	Vector3(glm::vec2&& rhs) noexcept;

	/*!
	 * @brief 
	 * glm::vec2 move assignment
	 * @param rhs 
	 * variable to be assigned
	 * @return 
	 * Vector
	*/
	Vector3& operator=(glm::vec2&& rhs) noexcept;


	/*!
	 * @brief
	 * Compute dot product of 2 vectors.
	 * @param rhs
	 * The other vector to compute the dot product with.
	 * @return
	 * The dot product result.
	*/
	float Dot(const Vector3& rhs) const;

	/*!
	 * @brief
	 * Compute the cross product of 2 vectors.
	 * @param rhs
	 * The other vector to compute the cross product with.
	 * @return
	 * The resultant cross product.
	*/
	Vector3 Cross(const Vector3& rhs) const;

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
	 * Operator overload for addition between 2 Vector3 objects.
	 * @param rhs
	 * The other Vector3 object to add.
	 * @return
	 * The modified reference to the Vector3 object.
	*/
	Vector3& operator+=(const Vector3& rhs);

	/*!
	 * @brief
	 * Operator overload for subtraction between 2 Vector3 objects.
	 * @param rhs
	 * The other Vector3 object to subtract from.
	 * @return
	 * The modified reference to the Vector3 object.
	*/
	Vector3& operator-=(const Vector3& rhs);

	/*!
	 * @brief
	 * Operator overload for scalar multiplication of the Vector3 object.
	 * @param val
	 * The scalar value to multiply with.
	 * @return
	 * The reference to the modified Vector3 object.
	*/
	Vector3& operator*=(float val);


	/*!
	 * @brief
	 * Operator overload for scalar division of the Vector3 object.
	 * @param val
	 * The scalar value to divide with.
	 * @return
	 * The reference to the modified Vector3 object.
	*/
	Vector3& operator/=(float val);


	/*!
	 * @brief
	 * Operator overload to return the negated values of the Vector3 object.
	 * @return
	 * The reference to the modified Vector3 object.
	*/
	Vector3& operator-();


	/*!
	 * @brief
	 * Converts the current Vector3 into glm::vec3.
	*/
	operator glm::vec3() const;

	/*!
	 * @brief
	 * Converts the current Vector3 into glm::vec2.
	*/
	operator glm::vec2() const;


	/*!
	 * @brief
	 * Access the X component of the Vector3 object.
	 * @return
	 * X component of the Vector3 object.
	*/
	float X() const;

	/*!
	 * @brief
	 * Access the Y component of the Vector3 object.
	 * @return
	 * Y component of the Vector3 object.
	*/
	float Y() const;

	/*!
	 * @brief
	 * Access the Z component of the Vector3 object.
	 * @return
	 * Z component of the Vector3 object.
	*/
	float Z() const;


	/*!
	 * @brief
	 * Modifies the X component to the new value 'val'.
	 * @param val
	 * The new value to set.
	*/
	void X(float val);

	/*!
	 * @brief
	 * Modifies the Y component to the new value 'val'.
	 * @param val
	 * The new value to set.
	*/
	void Y(float val);

	/*!
	 * @brief
	 * Modifies the Z component to the new value 'val'.
	 * @param val
	 * The new value to set.
	*/
	void Z(float val);

	//Variables
	float mX, mY, mZ;


	/*!
	 * @brief
	 * Destructor for the Vector3 class.
	*/
	~Vector3();
};

// non-member functions
/*!
 * @brief
 * Operator overload to multiply a value to a Vector3 object.
 * @param vec
 * The Vector3 object for the multiplication.
 * @param val
 * The value to multiply.
 * @return
 * The resultant value.
*/
Vector3 operator*(const Vector3& vec, float val);

/*!
 * @brief
 * Operator overload to multiply a value to a Vector3 object.
 * @param val
 * The value to multiply.
 * @param vec
 * The Vector3 object for the multiplication.
 * @return
 * The resultant value.
*/
Vector3 operator*(float val, const Vector3& vec);

/*!
 * @brief
 * Operator overload for scalar division of the Vector3 object.
 * @param vec
 * Vector object to use as the numerator.
 * @param val
 * The scalar value to divide with.
 * @return
 * The reference to the modified Vector3 object.
*/
Vector3 operator/(const Vector3& vec, float val);

/*!
 * @brief 
 * operator== to compare 2 vectors.
 * @param lhs 
 * First vector to use for the comparison.
 * @param rhs 
 * Second vector to use for the comparison.
 * @return 
 * Boolean value to represent if both objects are the same.
*/
bool operator==(const Vector3& lhs, const Vector3& rhs);

/*!
 * @brief 
 * operator!= to comapare 2 vectors.
 * @param lhs 
 * First vector to use for the comparison.
 * @param rhs 
 * Second vector to use for the comparison.
 * @return 
 * Boolean value to represent if both objects are the same.
*/
bool operator!=(const Vector3& lhs, const Vector3& rhs);

// aliases
using Vec3 = Vector3;

#endif // !VECTOR3_H


