#include "stdafx.h"
#include "DiskDiskSolver.h"

DiskDiskSolver::DiskDiskSolver(Disk *d1, Disk *d2) : d1(d1), d2(d2)
{ }

bool DiskDiskSolver::canDestroy()
{ return true; }

bool DiskDiskSolver::_solve(std::vector<SubCollision> &resl)
{
	// ignore OBB tree, direct result is available
	Vector2D p1 = d1->getCenter(), p2 = d2->getCenter();
	Real r1 = d1->getRadius(), r2 = d2->getRadius();
	Real sr = r1 + r2;
	Vector2D dp = p2 - p1;
	Real sqd = dp * dp;
	if(sqd > sr * sr)
		return canDestroy(); // no collision
	else
	{
		if(dp.isNull())
			dp.setY(1);
		else
			dp.normalise();
		Vector2D res = p1 + (dp * r1);
		Point2D ptA = Point2D(res.getX(), res.getY());	
		res = p2 - (dp * r2);		   
		Point2D ptB = Point2D(res.getX(), res.getY());
		resl.push_back(SubCollision(ptA, ptB));
		return false;
	}		 	
}
