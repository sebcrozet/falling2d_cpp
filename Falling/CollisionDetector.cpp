#include "stdafx.h"
#include "CollisionDetector.h"
#include "PenetrationSolver.h"


CollisionDetector::CollisionDetector() : isInactive(false)
{ }

bool CollisionDetector::solve(std::vector<SubCollision> &res)
{			
	if(!isInactive)
		return _solve(res);
	else
		return canDestroy();
}