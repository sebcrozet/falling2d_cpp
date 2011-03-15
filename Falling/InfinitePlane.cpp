#include "stdafx.h"
#include "InfinitePlane.h"

InfinitePlane::InfinitePlane(Point2D p, Vector2D normal)
{
	pt = p;
	dir = normal.direction();
	perpdir = Vector2D(-dir.getY(), dir.getX());
	spt = p + perpdir;
	fixedobj = true;
}
int InfinitePlane::getShapeTypeID()
{ return 100; }
Real InfinitePlane::getInertiaMomentum(Real)
{ return MACHINE_MAX; }
Real InfinitePlane::getSurface()
{ return MACHINE_MAX; }
void InfinitePlane::updateAABB()
{
	if(dir.getX() == 0)
	{
		if(dir.getY() < 0)
		{
			aabb_ym = pt.getY();
			aabb_yM = MACHINE_MAX / 2;
		}
		else
		{
			aabb_yM = pt.getY();
			aabb_ym = -MACHINE_MAX / 2;
		}
		aabb_xm = - MACHINE_MAX / 2;
		aabb_xM = MACHINE_MAX / 2;
	}
	else if(dir.getY() == 0)
	{
		if(dir.getX() < 0)
		{
			aabb_xm = pt.getX();
			aabb_xM = MACHINE_MAX / 2;
		}
		else
		{
			aabb_xM = pt.getX();
			aabb_xm = - MACHINE_MAX / 2;
		}
		aabb_ym = - MACHINE_MAX / 2;
		aabb_yM = MACHINE_MAX / 2;
	}
	else
	{
		aabb_ym = - MACHINE_MAX / 2;
		aabb_yM = MACHINE_MAX / 2;
		aabb_xm = - MACHINE_MAX / 2;
		aabb_xM = MACHINE_MAX / 2;
	}
}
