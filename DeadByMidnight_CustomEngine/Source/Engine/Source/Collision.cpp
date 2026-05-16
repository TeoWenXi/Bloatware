/*!
All content © 2023 DigiPen Institute of Technology Singapore, all rights reserved.
@file       Collision.cpp
@author     SIA Xin Tian (100%)
@co-author

----------------------------------------------------------------------------

@course     CSD2401F23
@section    CSD2401F23-A
@project    CSD2401F23 Software Engineering Project 3

@brief     Collision class function definitions
*//*______________________________________________________________________*/

#include "Collision.h"
#include "TileComponent.h"
#include "WindowsManager.h"
#include "EntityManager.h"
#include "PhysicsComponent.h"


bool Collision::CollisionDynamicAABB(const Collider_AABB& _aabb1, const Collider_AABB& _aabb2, const Vec3& _velocity1, const Vec3& _velocity2)
{
	//static collision check
	if (_aabb1.mMax.X() < _aabb2.mMin.X() ||
		_aabb1.mMax.Y() < _aabb2.mMin.Y() ||
		_aabb1.mMin.X() > _aabb2.mMax.X() ||
		_aabb1.mMin.Y() > _aabb2.mMax.Y())
		return false;

	float Time_First_C = 0; //dist between right of static obj and left of moving obj
	float Time_Last_C{ (float)WindowsManager::GetInstance()->mDeltaTime };  //dist between left of static obj and right of moving obj
	Vec3 ResultantVelocity = _velocity2 - _velocity1;

	//x-axis; assuming positive direction is to the right
	if (ResultantVelocity.X() < 0) //moving object(aabb2) is moving to the left
	{
		if (_aabb1.mMin.X() > _aabb2.mMax.X()) //moving obj is to the left of static obj
			return false;

		if (_aabb1.mMax.X() < _aabb2.mMin.X()) //moving obj is to the right of static obj
			Time_First_C = (_aabb1.mMax.X() - _aabb2.mMin.X()) / ResultantVelocity.X();
		if (_aabb1.mMin.X() < _aabb2.mMax.X())
			Time_Last_C = (_aabb1.mMin.X() - _aabb2.mMax.X()) / ResultantVelocity.X();
	}
	else //ResultantVelocity > 0, moving obj(aabb2) is moving to right
	{
		if (_aabb1.mMin.X() > _aabb2.mMax.X()) //moving obj is to the left of static obj
			Time_First_C = (_aabb1.mMin.X() - _aabb2.mMax.X()) / ResultantVelocity.X();
		if (_aabb1.mMax.X() > _aabb2.mMin.X())
			Time_Last_C = (_aabb1.mMax.X() - _aabb2.mMin.X()) / ResultantVelocity.X();


		if (_aabb1.mMax.X() < _aabb2.mMin.X()) //moving obj is to the right of static obj
			return false;

	}

	//y-axis; assuming positive direction is up
	if (ResultantVelocity.Y() < 0) //moving object(aabb2) is moving down
	{
		if (_aabb1.mMin.Y() > _aabb2.mMax.Y()) //moving obj is below static obj
			return false;

		if (_aabb1.mMax.Y() < _aabb2.mMin.Y()) //moving obj is above static obj
			Time_First_C = std::max(_aabb1.mMax.Y() - _aabb2.mMin.Y() / ResultantVelocity.Y(), Time_First_C);
		if (_aabb1.mMin.Y() < _aabb2.mMax.Y())
			Time_Last_C = std::min((_aabb1.mMin.Y() - _aabb2.mMax.Y()) / ResultantVelocity.Y(), Time_Last_C);
	}
	else //ResultantVelocity.y >= 0, moving object is moving up
	{
		if (_aabb1.mMin.Y() > _aabb2.mMax.Y()) //moving obj is below static obj
			Time_First_C = std::max((_aabb1.mMin.Y() - _aabb2.mMax.Y()) / ResultantVelocity.Y(), Time_First_C);
		if (_aabb1.mMax.Y() > _aabb2.mMin.Y())
			Time_Last_C = std::min((_aabb1.mMax.Y() - _aabb2.mMin.Y()) / ResultantVelocity.Y(), Time_Last_C);

		if (_aabb1.mMax.Y() < _aabb2.mMin.Y()) //moving obj is above static obj
			return false;
	}

	//No intersection
	if (Time_First_C > Time_Last_C)
		return false;

	//Intersects in this frame
	if (Time_First_C <= WindowsManager::GetInstance()->mDeltaTime) 
		return true;
	
	return false;

}


bool Collision::CollisionIntersectionCircleCircle(const Collider_Circle& _circle1, const Collider_Circle& _circle2)
{
	float Dist_X = _circle1.mCenter.mX - _circle2.mCenter.mX;
	float Dist_Y = _circle1.mCenter.mY - _circle2.mCenter.mY;
	double Distance_Btw_Center = sqrt(Dist_X * Dist_X + Dist_Y * Dist_Y);
	float Dist_of_radius = _circle1.mRadius + _circle2.mRadius;

	//if distance bewteen the centers of the two circles > sum of their radius, no collision
	if (Distance_Btw_Center > Dist_of_radius)
		return false;
	else return true;
}

int Collision::CheckInstanceBinaryMapCollision(float _posX, float _posY, Transform* _xform, bool _extendBottom)
{
	float scaleX = _xform->mScale.mX, scaleY = _xform->mScale.mY;
	int Flag = 0;

	std::pair<int, std::pair<int, int>> currentCellValue;

	//left side
	int leftx1, lefty1, leftx2, lefty2;
	leftx1 = (int)(_posX - scaleX / 2);
	lefty1 = (int)(_posY + scaleY / 4);

	leftx2 = (int)(_posX - scaleX / 2);
	lefty2 = (int)(_posY - scaleY / 4);

	//touching wall on the left
	currentCellValue = TileComponent::GetCellValue_YX(leftx2, lefty2);
	if (currentCellValue.first && (currentCellValue.second.first + 1 >= TileComponent::mMapWidth || !(TileComponent::mBinaryMap[currentCellValue.second.second][currentCellValue.second.first + 1]))) {
		Flag |= TileComponent::COLLISION_LEFT;
	}
	else
	{
		currentCellValue = TileComponent::GetCellValue_YX(leftx2, lefty2);
		if (currentCellValue.first && (currentCellValue.second.first + 1 >= TileComponent::mMapWidth || !(TileComponent::mBinaryMap[currentCellValue.second.second][currentCellValue.second.first + 1]))) {
			Flag |= TileComponent::COLLISION_LEFT;
		}
	}

	//right side
	int rightx1, righty1, rightx2, righty2;
	rightx1 = (int)(_posX + scaleX / 2);
	righty1 = (int)(_posY + scaleY / 4);

	rightx2 = (int)(_posX + scaleX / 2);
	righty2 = (int)(_posY - scaleY / 4);

	currentCellValue = TileComponent::GetCellValue_YX(rightx2, righty2);
	if (currentCellValue.first && (currentCellValue.second.first - 1 < 0 || !(TileComponent::mBinaryMap[currentCellValue.second.second][currentCellValue.second.first - 1]))) {
		Flag |= TileComponent::COLLISION_RIGHT;
	}
	else
	{
		currentCellValue = TileComponent::GetCellValue_YX(rightx1, righty1);
		if (currentCellValue.first && (currentCellValue.second.first - 1 < 0 || !(TileComponent::mBinaryMap[currentCellValue.second.second][currentCellValue.second.first - 1]))) {
			Flag |= TileComponent::COLLISION_RIGHT;
		}
	}

	//top 
	int topx1, topy1, topx2, topy2;
	topx1 = (int)(_posX - scaleX / 4);
	topy1 = (int)(_posY + scaleY / 2-1);

	topx2 = (int)(_posX + scaleX / 4);
	topy2 = (int)(_posY + scaleY / 2-1);

	currentCellValue = TileComponent::GetCellValue_YX(topx2, topy2);
	if (currentCellValue.first && (currentCellValue.second.second - 1 < 0 || !(TileComponent::mBinaryMap[currentCellValue.second.second - 1][currentCellValue.second.first]))) {
		Flag |= TileComponent::COLLISION_TOP;
	}
	else
	{
		currentCellValue = TileComponent::GetCellValue_YX(topx1, topy1);
		if (currentCellValue.first && (currentCellValue.second.second - 1 < 0 || !(TileComponent::mBinaryMap[currentCellValue.second.second - 1][currentCellValue.second.first]))) {
			Flag |= TileComponent::COLLISION_TOP;
		}
	}

	//bottom
	int bottomx1, bottomy1, bottomx2, bottomy2;

	//extendBottom = false;

	if (_extendBottom)
		bottomx1 = (int)(_posX - scaleX / 2); //If falling down, divide by 2 instead of 4.
	else
		bottomx1 = (int)(_posX - scaleX / 4);
	if (false)
		bottomy1 = (int)(_posY - scaleY / 4 - 1); //I think this code doesnt make sense
	else
		bottomy1 = (int)(_posY - scaleY / 2 - 1);

	if (_extendBottom)
		bottomx2 = (int)(_posX + scaleX / 2); //If falling down, divide by 2 instead of 4.
	else
		bottomx2 = (int)(_posX + scaleX / 4);
	if (false)
		bottomy2 = (int)(_posY - scaleY / 4 - 1);
	else
		bottomy2 = (int)(_posY - scaleY / 2 - 1);
	
	currentCellValue = TileComponent::GetCellValue_YX(bottomx2, bottomy2);
	if (currentCellValue.first && (currentCellValue.second.second + 1 >= TileComponent::mMapHeight || !(TileComponent::mBinaryMap[currentCellValue.second.second + 1][currentCellValue.second.first]))) {
		Flag |= TileComponent::COLLISION_BOTTOM;
	}
	else
	{
		currentCellValue = TileComponent::GetCellValue_YX(bottomx1, bottomy1);
		if (currentCellValue.first && (currentCellValue.second.second + 1 >= TileComponent::mMapHeight || !(TileComponent::mBinaryMap[currentCellValue.second.second + 1][currentCellValue.second.first]))) {
			Flag |= TileComponent::COLLISION_BOTTOM;
		}
	}

	return Flag;
}

bool Collision::CollisionStaticAABB(const Vector3& _pos, const Transform& _transform)
{
	if (_pos.mX >= (_transform.GetPosition().mX - (_transform.mScale.mX / 2)) //Check within x from left side
		&& _pos.mX <= (_transform.GetPosition().mX + (_transform.mScale.mX / 2)) //Check within x from left side
		&& _pos.mY >= (_transform.GetPosition().mY - (_transform.mScale.mY / 2)) //Check within y from bottom
		&& _pos.mY <= (_transform.GetPosition().mY + (_transform.mScale.mY / 2)))//Check within y from bottom
	{
		return true;
	}

	return false;
}

Collision::ConvexPoly Collision::GetBoxVertices(const Transform& _xform)
{
	ConvexPoly BOX{};
	BOX.VerticesNum = 4;
	BOX.Vertices = new Vec3[BOX.VerticesNum];

	float width = _xform.mScale.mX;
	float height = _xform.mScale.mY;

	float left = -width / 2;
	float right = left + width;
	float top = -height / 2;
	float bottom = top + height;

	//storing vertices and returning it
	BOX.Vertices[0] = { _xform.GetPosition().mX + left, _xform.GetPosition().mY + top };
	BOX.Vertices[1] = { _xform.GetPosition().mX + right, _xform.GetPosition().mY + top };
	BOX.Vertices[2] = { _xform.GetPosition().mX + right, _xform.GetPosition().mY + bottom };
	BOX.Vertices[3] = { _xform.GetPosition().mX + left, _xform.GetPosition().mY + bottom };

	return BOX;
}

Vector3 Collision::RotateVertices(Vector3 _pos, float _angle)
{
	float rad = (_angle / 180) * PI;

	Vector3 result{};
	result.mX = _pos.mX * cos(rad) - _pos.mY * sin(rad);
	result.mY = _pos.mX * sin(rad) + _pos.mY * cos(rad);
	return result;
}

Collision::ConvexPoly Collision::GetTransformedVertices(const Transform& _c)
{
	ConvexPoly transform;
	transform.Vertices = new Vec3[4];
	ConvexPoly v = GetBoxVertices(_c);

	Vector3 right = RotateVertices({1, 0, 0}, _c.mRotation);
	Vector3 up = RotateVertices({0, 1, 0}, _c.mRotation);
	right.Normalize();
	up.Normalize();

	for (int i = 0; i < 4; i++)
	{
		//shift vertex back to origin for rotation
		Vec3 localPos = v.Vertices[i] - _c.GetPosition();

		//apply rotation to the vertex
		Vec3 rotatedPos;
		rotatedPos.mX = localPos.mX * right.mX + localPos.mY * up.mX;
		rotatedPos.mY = localPos.mX * right.mY + localPos.mY * up.mY;

		//shift vertex back to its world position
		transform.Vertices[i] = rotatedPos + _c.GetPosition();
	}

	return transform;
}

void Collision::ComputeProjInterval(ConvexPoly _c, Vec3 _d, float& _min, float& _max)
{
	_min = _max = _d.Dot(_c.Vertices[0]);

	//loop through all projection of vertices to find the min and max
	for (int i = 1; i < 4; i++)
	{
		float value = _d.Dot(_c.Vertices[i]);
		if (value < _min)
			_min = value;
		else if (value > _max)
			_max = value;
	}
}

bool Collision::IntersectionPolygons(const Transform& _c0, const Transform& _c1)
{
	ConvexPoly v0 = GetTransformedVertices(_c0);
	ConvexPoly v1 = GetTransformedVertices(_c1);

	//loop through for C0
	for (int i = 0; i < 4; i++)
	{
		Vec3 current = v0.Vertices[i];
		Vec3 next = v0.Vertices[(i + 1) % 4]; //loop through the vertices

		Vec3 directionvec = next - current;
		Vec3 axis = { -directionvec.mY, directionvec.mX, 1 }; //axis that we are projecting onto, normal vector of directionvec

		//projecting vectors onto axis using dot product
		float min0, max0, min1, max1;
		ComputeProjInterval(v0, axis, min0, max0);
		ComputeProjInterval(v1, axis, min1, max1);

		if (max0 < min1 || max1 < min0)
			return false;
	}

	//loop through for C1
	for (int i = 0; i < 4; i++)
	{
		Vec3 current = v1.Vertices[i];
		Vec3 next = v1.Vertices[(i + 1) % 4]; //loop through the vertices

		Vec3 directionvec = next - current;
		Vec3 axis = { -directionvec.mY, directionvec.mX, 1 }; //axis that we are projecting onto, normal vector of directionvec

		//projecting vectors onto axis using dot product
		float min0, max0, min1, max1;
		ComputeProjInterval(v1, axis, min0, max0);
		ComputeProjInterval(v0, axis, min1, max1);

		if (max0 < min1 || max1 < min0)
			return false;
	}

	return true;
}

Collision::ConvexPoly Collision::GetCircleVertices(const Collider_Circle& _circle)
{
	ConvexPoly CIRCLE{};
	CIRCLE.VerticesNum = 1;
	CIRCLE.Center = _circle.mCenter;
	CIRCLE.Radius = _circle.mRadius;
	return CIRCLE;
}

void Collision::ComputeCircleProj(const ConvexPoly _circle, Vec3 _d, float& _min, float& _max)
{
	Vector3 NormalizedVec = _d;
	NormalizedVec.Normalize();
	Vector3 direc_radius = NormalizedVec * _circle.Radius;

	Vector3 P0 = _circle.Center + direc_radius;
	Vector3 P1 = _circle.Center + direc_radius;

	_min = P0.Dot(_d);
	_max = P1.Dot(_d);

}


//C0 is polygon, C1 is circle
Vector3 Collision::ClosestPointOnPolygon(ConvexPoly _c0, ConvexPoly _c1)
{
	//axis to project on
	Vec3 P0P1 = _c0.Vertices[1] - _c0.Vertices[0];
	P0P1.Normalize();

	//vector to project onto axis
	Vec3 P0C1 = _c1.Center - _c0.Vertices[0];

	//projeted vector
	Vec3 P0FOP = (P0C1.Dot(P0P1)) * (P0P1);

	//point on C0 that center of circle is perpendicular to
	Vec3 FOP = _c0.Center + P0FOP;

	//dist of FOP
	float distOfFOP = FOP.Length();


	//loop through all the vertices to find all FOP and compare
	for (int i = 1; i < _c0.VerticesNum; i++)
	{
		//axis to project on
		Vec3 axis = _c0.Vertices[(i + 1) % _c0.VerticesNum] - _c0.Vertices[i];
		axis.Normalize();
		
		//projected vector
		Vec3 vToProj = _c1.Center - _c0.Vertices[i];

		//point on C0 that center of circle is perpendicular to
		P0FOP = (vToProj.Dot(axis)) * axis;

		//foot of perpendicular
		Vector3 FOPnext = _c0.Vertices[i] + P0FOP; 
		float distOfFOPnext = FOPnext.Length();

		//compare with FOP
		if (distOfFOP > distOfFOPnext)
			FOP = FOPnext;
	}

	return FOP;
}

bool Collision::IntersectionCirclePolygons(const Transform& _c0, const Collider_Circle& _c1)
{
	//C0 is polygon, C1 is circle
	ConvexPoly C0 = GetBoxVertices(_c0);
	ConvexPoly C1 = GetCircleVertices(_c1);

	Vec3 ClosestPoint = ClosestPointOnPolygon(C0, C1);

	float dist = ClosestPoint.Length();

	if (dist > C1.Radius)
		return false;

	return true;
}

Collision::CollisionDir Collision::SeparatingAxisTheoremAABB(Collider_AABB* aabb1, Collider_AABB* aabb2, PhysicsComponent* go1Phy, PhysicsComponent* go2Phy, Transform* trans1, Transform* trans2)
{
	Vector3 vel1 = go1Phy->mCurrentVelocity;
	Vector3 vel2 = go2Phy->mCurrentVelocity;

	Vector3 pos1 = trans1->GetPosition();
	Vector3 pos2 = trans2->GetPosition();

	//Static collision check
	if (aabb1->mMax.mX < aabb2->mMin.mX ||
		aabb1->mMax.mY < aabb2->mMin.mY ||
		aabb1->mMin.mX > aabb2->mMax.mX ||
		aabb1->mMin.mY > aabb2->mMax.mY)
		return CD_NONE;

	//Init and calculate new velocity of Vb
	Vector3 Vb = { vel2.mX - vel1.mX, vel2.mY - vel1.mY, 0 };
	float tFirst = 0;
	float tLast = (float)WindowsManager::GetInstance()->mDeltaTime;

	//X Axis Check
	if (Vb.mX < 0)
	{
		//Case 1 (B is on the left of A and moving towards the left (away from A))
		if (aabb1->mMin.mX > aabb2->mMax.mX)
			return CD_NONE;

		//Case 4 (B is on the right of A and moving towards the left (towards A))
		if (aabb1->mMax.mX < aabb2->mMin.mX)
			tFirst = (aabb1->mMax.mX - aabb2->mMin.mX) / Vb.mX;
		if (aabb1->mMin.mX < aabb2->mMax.mX)
			tLast = (aabb1->mMin.mX - aabb2->mMax.mX) / Vb.mX;
	}
	else if (Vb.mX >= 0)
	{
		//Case 2 (B is on the left of A and moving towards the right (towards A))
		if (aabb1->mMin.mX > aabb2->mMax.mX)
			tFirst = (aabb1->mMin.mX - aabb2->mMax.mX) / Vb.mX;
		if (aabb1->mMax.mX > aabb2->mMin.mX)
			tLast = (aabb1->mMax.mX - aabb2->mMin.mX) / Vb.mX;

		//Case 3 (B is on the right of A and moving towards the right (away from A))
		if (aabb1->mMax.mX < aabb2->mMin.mX)
			return CD_NONE;
	}

	//Y Axis Check
	if (Vb.mY < 0)
	{
		//Case 1 (B is on the bottom of A and moving downwards (away from A))
		if (aabb1->mMin.mY > aabb2->mMax.mY)
			return CD_NONE;

		//Case 4 (B is on the top of A and moving downwards (towards A))
		if (aabb1->mMax.mY < aabb2->mMin.mY)
			tFirst = std::max((aabb1->mMax.mY - aabb2->mMin.mY) / Vb.mY, tFirst);
		if (aabb1->mMin.mY < aabb2->mMax.mY)
			tLast = std::min((aabb1->mMin.mY - aabb2->mMax.mY) / Vb.mY, tLast);
	}
	else if (Vb.mY >= 0)
	{
		//Case 2 (B is on the bottom of A and moving upwards (towards A))
		if (aabb1->mMin.mY > aabb2->mMax.mY)
			tFirst = std::max((aabb1->mMin.mY - aabb2->mMax.mY) / Vb.mY, tFirst);
		if (aabb1->mMax.mY > aabb2->mMin.mY)
			tLast = std::min((aabb1->mMax.mY - aabb2->mMin.mY) / Vb.mY, tLast);

		//Case 3 (B is on the top of A and moving upwards (away from A))
		if (aabb1->mMax.mY < aabb2->mMin.mY)
			return CD_NONE;
	}
	//Case 5, no intersection
	if (tFirst > tLast)
		return CD_NONE;


	//Intersects in this frame if the first timer is less than deltaTime
	if (tFirst <= WindowsManager::GetInstance()->mDeltaTime)
	{
		//Collision Direction is _go1's direction of collision with _go2

		//_go1 is below _go2
		if (aabb1->mMax.mY < aabb2->mMin.mY - Vb.mY * WindowsManager::GetInstance()->mDeltaTime)
		{
			//_go2 on the left of _go1
			if (pos1.mX > pos2.mX)
				return CD_TOP_LEFT;
			//_go2 on the right of _go1
			else if (pos1.mX < pos2.mX)
				return CD_TOP_RIGHT;
			//_go2 directly on top of _go1
			else
				return CD_TOP;
		}
		//_go1 is above _go2
		else if (aabb1->mMin.mY > aabb2->mMax.mY - Vb.mY * WindowsManager::GetInstance()->mDeltaTime)
		{
			//_go2 on the left of _go1
			if (pos1.mX > pos2.mX)
				return CD_BOTTOM_LEFT;
			//_go2 on the right of _go1
			else if (pos1.mX < pos2.mX)
				return CD_BOTTOM_RIGHT;
			//_go2 directly below _go1
			else
				return CD_BOTTOM;
		}
		//_go1 and _go2 are side by side
		else
		{
			//_go2 on the left of _go1
			if (pos1.mX > pos2.mX)
				return CD_LEFT;
			//_go2 on the right of _go1
			else if (pos1.mX < pos2.mX)
				return CD_RIGHT;
		}
	}

	return CD_NONE;
}
