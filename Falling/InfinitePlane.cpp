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
float InfinitePlane::getInertiaMomentum(float)
{ return FLT_MAX; }
float InfinitePlane::getSurface()
{ return FLT_MAX; }
void InfinitePlane::updateAABB()
{
	if(dir.getX() == 0)
	{
		if(dir.getY() < 0)
		{
			aabb_ym = pt.getY();
			aabb_yM = FLT_MAX / 2;
		}
		else
		{
			aabb_yM = pt.getY();
			aabb_ym = -FLT_MAX / 2;
		}
		aabb_xm = - FLT_MAX / 2;
		aabb_xM = FLT_MAX / 2;
	}
	else if(dir.getY() == 0)
	{
		if(dir.getX() < 0)
		{
			aabb_xm = pt.getX();
			aabb_xM = FLT_MAX / 2;
		}
		else
		{
			aabb_xM = pt.getX();
			aabb_xm = - FLT_MAX / 2;
		}
		aabb_ym = - FLT_MAX / 2;
		aabb_yM = FLT_MAX / 2;
	}
	else
	{
		aabb_ym = - FLT_MAX / 2;
		aabb_yM = FLT_MAX / 2;
		aabb_xm = - FLT_MAX / 2;
		aabb_xM = FLT_MAX / 2;
	}
}
