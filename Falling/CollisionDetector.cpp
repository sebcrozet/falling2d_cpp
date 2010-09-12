#include "stdafx.h"
#include "CollisionDetector.h"


SubCollision::SubCollision(Point2D &a, Point2D &b) : ptA(a), ptB(b)
{ }

CollisionDetector::CollisionDetector() : isInactive(false)
{ }

bool CollisionDetector::solve(std::vector<SubCollision> &res)
{
	if(!isInactive)
		return _solve(res);
	else
		return canDestroy();
}