#include "stdafx.h"
#include "InfinitePlane.h"

InfinitePlane::InfinitePlane(const Point2D &p, const Vector2D &normal)
{
	parent = this;
	margin = 0.0;
	pt = p;
	dir = normal.direction();
	perpdir = Vector2D(-dir.getY(), dir.getX());
	spt = p + perpdir;
	fixedobj = true;
}

Real InfinitePlane::getBoundingSphereRadius() const
{
	return MACHINE_EPSILON / 2;
}
Vector2D InfinitePlane::getCenter() const
{
	return pt;
}
int InfinitePlane::getSupportPoint(const Vector2D &d, Point2D *res) const
{
	*res = pt;
	return 0;
}
int InfinitePlane::getSupportPoint(const Vector2D &d, Point2D *res, int optimisationId) const
{
	*res = pt;
	return 0;
}


int InfinitePlane::getShapeTypeID() const
{ return 100; }
Real InfinitePlane::getInertiaMomentum(Real) const
{ return MACHINE_MAX; }
Real InfinitePlane::getSurface() const
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

bool InfinitePlane::containsPoint(const Point2D &pt) const
{
    return pt.isLeftTo(perpdir, spt);
}
