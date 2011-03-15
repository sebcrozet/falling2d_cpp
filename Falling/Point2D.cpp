#include "stdafx.h"
#include "floatTests.h"
#include "Point2D.h"

Point2D::Point2D(Real x, Real y):x(x),y(y)
{ }
Point2D::Point2D(const Vector2D &v):x(v.getX()), y(v.getY())
{ }

bool Point2D::isInCCWTriangle(const Point2D &p,const Point2D &p2, const Point2D &p3) const
{ return (isLeftTo(p , p2) >= 0.0) && (isLeftTo(p2 , p3) >= 0.0) && (isLeftTo(p3 , p) >= 0.0); }

bool Point2D::isInCWTriangle(const Point2D &p,const Point2D &p2, const Point2D &p3) const
{ return (isLeftTo(p , p2) <= 0.0) && (isLeftTo(p2 , p3) <= 0.0) && (isLeftTo(p3 , p) <= 0.0); }	   

bool Point2D::isInUnorientedTriangle(const Point2D &p,const Point2D &p2, const Point2D &p3) const
{ 
	Real l1 = isLeftTo(p , p2);
	if(l1 < 0.0)
		return isLeftTo(p2 , p3) < 0.0 && isLeftTo(p3 , p) < 0.0;
	else
		return l1 != 0.0 && isLeftTo(p2 , p3) > 0.0 && isLeftTo(p3 , p) > 0.0;
}

Real Point2D::intersectSegments(Point2D &p, Point2D &p2, Point2D &p3, Point2D &p4, Point2D *res,Real *bparam2)
{
	Vector2D s1(p,p2);
	Vector2D s2(p3,p4);
	Vector2D s13(p3,p);
	Real denom =  s1.perp(s2);
	if(denom == 0.0)
		return -1; // parallels
	Real u1 = s2.perp(s13) / denom; 
	if(u1 < 0.0 || u1 > 1.0)
		return -1;
	Real u2 = s1.perp(s13) / denom;
	if(u2 < 0.0 || u2 > 1.0)
		return -1;
	// else intersects
	*res = Point2D(p.getX() + u1 * s1.getX(),p.getY() + u1 * s1.getY());
	*bparam2 = u2;
	// TODO: remove test
	if(res->getX() != res->getX() || res->getY() != res->getY())
		while(true);
	// end test
	return u1; // return barycentric coordinate
}
// view simple algorithm in
// (source:) http://www.engr.colostate.edu/~dga/dga/papers/point_in_polygon.pdf
bool Point2D::pointInPolygon(const Point2D &pt, Point2D *pts, int n)
{
    Real windingnumber = 0.0; // Real but will be an int at the end
    Real x = pt.getX();
    Real y = pt.getY();
    for(int j = n-1, i = 0; i < n; j = i, i++)
    {
	Real xi = pts[j].getX() - x;
	Real yi = pts[j].getY() - y;
	Real xi1 = pts[i].getX() - x;
	Real yi1 = pts[i].getY() - y;
	if(yi*yi1 < 0.0)
	{
	    Real r = xi + yi*(xi1-xi)/(yi-yi1);
	    if(r > 0.0)
	    {
		if(y < 0.0)
		    windingnumber++;
		else
		    windingnumber--;
	    }
	}
	else if(Float::zero(yi) && xi > 0.0)
	{
	    if(!Float::zero(yi1))
	    {
		if(yi1 > 0.0)
		    windingnumber += 0.5;
		else
		    windingnumber -= 0.5; 
	    }
	}
	else if(Float::zero(yi1) && xi1 > 0.0)
	{
	    if(!Float::zero(yi))
	    {
		if(yi < 0.0)
		    windingnumber += 0.5;
		else
		    windingnumber -= 0.5; 
	    }
	}
    }
    return (((int)windingnumber) & 0x0001); // in polygon if not even
}
