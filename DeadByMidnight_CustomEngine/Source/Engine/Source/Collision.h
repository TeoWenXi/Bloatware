/*!
All content © 2023 DigiPen Institute of Technology Singapore, all rights reserved.
@file       Collision.h
@author     SIA Xin Tian (100%)
@co-author	

----------------------------------------------------------------------------

@course     CSD2401F23
@section    CSD2401F23-A
@project    CSD2401F23 Software Engineering Project 3

@brief      Collision class containing all functions for collision checking
*//*______________________________________________________________________*/

#ifndef COLLISION_H
#define COLLISION_H

#include "MathDefines.h"
#include "Vector3.h"
#include "Collider_AABB.h"
#include "Collider_Circle.h"
#include "Transform.h"

class PhysicsComponent;
class Collision
{
public:
	struct ConvexPoly {
		Vector3* Vertices = 0;
		Vector3 Center;
		float Radius = 0;
		int VerticesNum = 0;
	};

	enum CollisionDir
	{
		CD_NONE,
		CD_LEFT,
		CD_TOP_LEFT,
		CD_TOP,
		CD_TOP_RIGHT,
		CD_RIGHT,
		CD_BOTTOM_RIGHT,
		CD_BOTTOM,
		CD_BOTTOM_LEFT
	};
public:

	/*!
	 * @brief
	 * Checks for AABB collision between 2 objects
	 * @param aabb1
	 * bounding box of first object
	 * @param aabb2
	 * bounding box of second object
	 * @param Velocity1
	 * velocity of first object
	 * @param Velocity2
	 * velocity of second object
	 * @return
	 * Boolean value that represents whether objects are colliding
	*/
	static bool CollisionDynamicAABB(const Collider_AABB& _aabb1, const Collider_AABB& _aabb2, const Vec3& _velocity1, const Vec3& _velocity2);

	/*!
	 * @brief 
	 * To check for the direction of the collision
	 * @param _go1 
	 * Entity id of first object to check collision for
	 * @param _go2 
	 * Entity id of second object to check collision for
	 * @return 
	 * Direction of collision
	*/
	static Collision::CollisionDir SeparatingAxisTheoremAABB(Collider_AABB* aabb1, Collider_AABB* aabb2, PhysicsComponent* go1Phy, PhysicsComponent* go2Phy, Transform* trans1, Transform* trans2);

	/*!
	 * @brief
	 * Checks for collision between 2 circles
	 * @param circle1
	 * Circle bounding box of first object
	 * @param circle2
	 * Circle bounding box of second object
	 * @return
	 * Boolean value that represents whether objects are colliding
	*/
	static bool CollisionIntersectionCircleCircle(const Collider_Circle& _circle1, const Collider_Circle& _circle2);


	/*!
	 * @brief
	 * Creates 2 hot spots on each side of the object instance and checks if each of these hot spots is in a collision area.
	 * @param PosX
	 * Position X of object
	 * @param PosY
	 * Position Y of object
	 * @return
	 * Returns collision data
	*/
	static int CheckInstanceBinaryMapCollision(float _posX, float _posY, Transform* _xform, bool _extendBottom = false);


	/*!
	* @brief
	* This function calculates if a given pos is within a transform using AABB
	* @param _pos
	* Position of point
	* @_transform
	* Contains data of object
	* @return
	* Boolean value that represents whether objects are colliding
	*/
	static bool CollisionStaticAABB(const Vector3& _pos, const Transform& _transform);



	/*!
	 * @brief
	 * Gets the box vertices from transform
	 * @param xform
	 * Contains data of object
	 * @return
	 * Calculated vertices of the box
	*/
	static ConvexPoly GetBoxVertices(const Transform& _xform);

	/*!
	 * @brief
	 * Rotating vertices based off the angle given
	 * @param pos
	 * Position of a point to rotate
	 * @param angle
	 * Angle to rotate point against
	 * @return
	 * Rotated vector
	*/
	static Vector3 RotateVertices(Vector3 _pos, float _angle);

	/*!
	* @brief
	* Get transformed vertices to their new position after rotation
	* @param C
	* Contains data of object
	* return
	* ConvexPoly
	*/
	static ConvexPoly GetTransformedVertices(const Transform& _c);


	/*!
	 * @brief
	 * Compute projection interval
	 * @param C
	 * Contains data of ConvexPoly
	 * @param D
	 * Vector as an axis to project onto
	 * @param min
	 * Minimum value of the object on the axis
	 * @param max
	 * Maximum value of the object on the axis
	*/
	static void ComputeProjInterval(ConvexPoly _c, Vec3 _d, float& _min, float& _max);


	/*!
	 * @brief
	 * Check for collision between intersection polygons
	 * @param C0
	 * Contains position, rotation and scale of first object
	 * @param C1
	 * Contains position, rotation and scale of second object
	 * @return
	 * Boolean value that represents whether objects are colliding
	*/
	static bool IntersectionPolygons(const Transform& _vertice1, const Transform& _vertice2);


	/*!
	 * @brief
	 * Stores variables of circle into ConvexPoly
	 * @param circle
	 * Variables of circle
	 * @return
	 * Stored value of object
	*/
	static ConvexPoly GetCircleVertices(const Collider_Circle& _circle);

	/*!
	 * @brief
	 * Finds projection of vector
	 * @param circle
	 * Contains variables of circle
	 * @param D
	 * Axis to be projected on
	 * @param min
	 * Minimum value of projection
	 * @param max
	 * Maximum value of projection
	*/
	static void ComputeCircleProj(const ConvexPoly _circle, Vec3 _d, float& _min, float& _max);

	/*!
	 * @brief
	 * Find the foot of perpendicular of the center of the circle to the closest line
	 * @param C0
	 * Contains variables of first object
	 * @param C1
	 * Contains variables of second object
	 * @return
	 * Point
	*/
	static Vector3 ClosestPointOnPolygon(ConvexPoly _c0, ConvexPoly _c1);

	/*!
	 * @brief
	 * Checks if circle is colliding with polygon
	 * @param C0
	 * Contains variables of first object
	 * @param C1
	 * Contains variables of second object
	 * @return
	 * Boolean value that represents whether objects are colliding
	*/
	static bool IntersectionCirclePolygons(const Transform& _c0, const Collider_Circle& _c1);


};

#endif