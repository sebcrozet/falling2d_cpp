#include "stdafx.h"
#include "Disk.h"

Disk::Disk(Point2D &pt, float dradius)
{
	radius = dradius;
	margin = 0.04f;
	radiuslessM = radius - margin;
	t = new GeometryHelper::Transformation2D(Vector2D (pt), 0);
	aabb = new AABB_disk(this);
	obb = new OBB(Point2D(-radius, radius),Point2D(radius, radius),Point2D(radius, -radius),Point2D(-radius, -radius), this, radius * radius);
	otree = new OBBtree(0, 0, obb);
}
float Disk::getSurface()
{ return M_PI * radius * radius; }

float Disk::getInertiaMomentum(float m)
{ return m * radius * radius / 2; }
Vector2D Disk::getCenter()
{ return t->getU(); }

int Disk::getSupportPoint(Vector2D &d, Point2D *res)
{
	Vector2D v = getCenter();
	v += d.direction() * radiuslessM;
	*res =  Point2D(v.getX(), v.getY()); 
	return 0; 
}
int Disk::getSupportPoint(Vector2D &d, Point2D *res, int optimisationId)
{	  
	Vector2D v = getCenter();
	v += d.direction() * radiuslessM;
	*res =  Point2D(v.getX(), v.getY()); 
	return 0; 
}