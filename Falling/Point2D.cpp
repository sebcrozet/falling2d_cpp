#include "stdafx.h"
#include "floatTests.h"
#include "Point2D.h"

Point2D::Point2D(float x, float y):x(x),y(y)
{ }
Point2D::Point2D(const Vector2D &v):x(v.getX()), y(v.getY())
{ }

bool Point2D::isInCCWTriangle(const Point2D &p,const Point2D &p2, const Point2D &p3) const
{ return (isLeftTo(p , p2) >= 0.0f) && (isLeftTo(p2 , p3) >= 0.0f) && (isLeftTo(p3 , p) >= 0.0f); }

bool Point2D::isInCWTriangle(const Point2D &p,const Point2D &p2, const Point2D &p3) const
{ return (isLeftTo(p , p2) <= 0.0f) && (isLeftTo(p2 , p3) <= 0.0f) && (isLeftTo(p3 , p) <= 0.0f); }	   

bool Point2D::isInUnorientedTriangle(const Point2D &p,const Point2D &p2, const Point2D &p3) const
{ 
	float l1 = isLeftTo(p , p2);
	if(l1 < 0.f)
		return isLeftTo(p2 , p3) < 0.f && isLeftTo(p3 , p) < 0.f;
	else
		return l1 != 0.f && isLeftTo(p2 , p3) > 0.f && isLeftTo(p3 , p) > 0.f;
}

float Point2D::intersectSegments(Point2D &p, Point2D &p2, Point2D &p3, Point2D &p4, Point2D *res,float *bparam2)
{
	Vector2D s1(p,p2);
	Vector2D s2(p3,p4);
	Vector2D s13(p3,p);
	float denom =  s1.perp(s2);
	if(denom == 0.f)
		return -1; // parallels
	float u1 = s2.perp(s13) / denom; 
	if(u1 < 0.f || u1 > 1.f)
		return -1;
	float u2 = s1.perp(s13) / denom;
	if(u2 < 0.f || u2 > 1.f)
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
    //TODO: remove test
    printf("PIP!\n");
    // end TODO
    float windingnumber = 0.f; // float but will be an int at the end
    float x = pt.getX();
    float y = pt.getY();
    for(int j = n-1, i = 0; i < n; j = i, i++)
    {
	float xi = pts[j].getX() - x;
	float yi = pts[j].getY() - y;
	float xi1 = pts[i].getX() - x;
	float yi1 = pts[i].getY() - y;
	if(yi*yi1 < 0.f)
	{
	    float r = xi + yi*(xi1-xi)/(yi-yi1);
	    if(r > 0.f)
	    {
		if(y < 0.f)
		    windingnumber++;
		else
		    windingnumber--;
	    }
	}
	else if(Float::zero(yi) && xi > 0.f)
	{
	    if(!Float::zero(yi1))
	    {
		if(yi1 > 0.f)
		    windingnumber += 0.5f;
		else
		    windingnumber -= 0.5f; 
	    }
	}
	else if(Float::zero(yi1) && xi1 > 0.f)
	{
	    if(!Float::zero(yi))
	    {
		if(yi < 0.f)
		    windingnumber += 0.5f;
		else
		    windingnumber -= 0.5f; 
	    }
	}
    }
    return ((((int)windingnumber)) % 2);//& 0x0001); // in polygon if non even
}
