#include "stdafx.h"
#include "Point2D.h"

Point2D::Point2D(float x, float y):x(x),y(y)
{ }
Point2D::Point2D(Vector2D &v):x(v.getX()), y(v.getY())
{ }
bool Point2D::isInCCWTriangle(const Point2D &p,const Point2D &p2, const Point2D &p3)
{ return (isLeftTo(p , p2) >= 0.0f) && (isLeftTo(p2 , p3) >= 0.0f) && (isLeftTo(p3 , p) >= 0.0f); }
bool Point2D::isInCWTriangle(const Point2D &p,const Point2D &p2, const Point2D &p3)		   
{ return (isLeftTo(p , p2) <= 0.0f) && (isLeftTo(p2 , p3) <= 0.0f) && (isLeftTo(p3 , p) <= 0.0f); }	   
bool Point2D::isInUnorientedTriangle(const Point2D &p,const Point2D &p2, const Point2D &p3)		   
{ 
	float l1 =	isLeftTo(p , p2);  
	if(l1 < 0)
		return isLeftTo(p2 , p3) < 0 && isLeftTo(p3 , p) < 0;
	else
		return l1 != 0 && isLeftTo(p2 , p3) > 0 && isLeftTo(p3 , p) > 0;
}
float Point2D::intersectSegments(Point2D &p, Point2D &p2, Point2D &p3, Point2D &p4, Point2D *res,float *bparam2)
{
	Vector2D s1(p,p2);
	Vector2D s2(p3,p4);
	Vector2D s13(p3,p);
	float denom =  1.f / s1.perp(s2);
	if(denom == 0.f)
		return -1; // parallels
	float u1 = s2.perp(s13) * denom; 
	if(u1 < 0.f || u1 > 1.f)
		return -1;
	float u2 = s1.perp(s13) * denom;
	if(u2 < 0.f || u2 > 1.f)
		return -1;
	// else intersects
	*res = Point2D(p.getX() + u1 * s1.getX(),p.getY() + u1 * s1.getY());
	*bparam2 = u2;
	return u1; // return barycentric coordinate
}