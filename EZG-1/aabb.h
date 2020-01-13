#pragma once
#include <array>
#include <vector>
#include <cmath>
#include "glm/vec3.hpp"

class aabb
{
public:
	glm::vec3 mMinPoint;
	glm::vec3 mMaxPoint;
	aabb()
	{
		mMinPoint = glm::vec3(0);
		mMaxPoint = glm::vec3(0);
	}
	aabb(glm::vec3 mMinPoint, glm::vec3 mMaxPoint) : mMinPoint(mMinPoint), mMaxPoint(mMaxPoint) { }

	void include(glm::vec3 p)
	{
		if (mMaxPoint.x < p.x)
		{
			mMaxPoint.x = p.x;
		}

		if (mMaxPoint.y < p.y)
		{
			mMaxPoint.y = p.y;
		}

		if (mMaxPoint.z < p.z)
		{
			mMaxPoint.z = p.z;
		}

		if (mMinPoint.x > p.x)
		{
			mMinPoint.x = p.x;
		}

		if (mMinPoint.y > p.y)
		{
			mMinPoint.y = p.y;
		}

		if (mMinPoint.z > p.z)
		{
			mMinPoint.z = p.z;
		}
	}

	bool isOverlappingAABB(aabb* other)
	{
		return (other->mMaxPoint.x > mMinPoint.x&& other->mMaxPoint.y > mMinPoint.y&& other->mMaxPoint.z > mMinPoint.z) &&
			(other->mMinPoint.x <= mMaxPoint.x && other->mMinPoint.y <= mMaxPoint.y && other->mMinPoint.z <= mMaxPoint.z);
	}

	bool containsPoint(glm::vec3 point)
	{
		return (mMinPoint.x <= point.x && point.x <= mMaxPoint.x && 
			mMinPoint.y <= point.y && point.y <= mMaxPoint.y && 
			mMinPoint.z <= point.z && point.z <= mMaxPoint.z);
	}

	void include(const aabb& b)
	{
		include(b.mMinPoint);
		include(b.mMaxPoint);
	}

	int getMaxExtentAxis()
	{
		glm::vec3 tempVec = mMaxPoint - mMinPoint;
		if (tempVec.x >= tempVec.y && tempVec.x >= tempVec.z)
		{
			return 0;
		}

		if (tempVec.y >= tempVec.z)
		{
			return 1;
		}

		return 2;
	}

	void splitAABB(aabb* lower, aabb* higher, int axis, float pos)
	{
		glm::vec3 lowCutPlanePos = mMinPoint;
		glm::vec3 highCutPlanePos = mMaxPoint;

		switch (axis) {
		case 0:
			lowCutPlanePos.x = pos;
			highCutPlanePos.x = pos;
			break;
		case 1:
			lowCutPlanePos.y = pos;
			highCutPlanePos.y = pos;
			break;
		case 2:
			lowCutPlanePos.z = pos;
			highCutPlanePos.z = pos;
			break;
		default:
			break;
		}
		lower->mMinPoint = mMinPoint;
		lower->mMaxPoint = highCutPlanePos;
		higher->mMinPoint = lowCutPlanePos;
		higher->mMaxPoint = mMaxPoint;

	}


	//bool intersect(const Ray& r, float& tmin, float& tmax) const;
};
