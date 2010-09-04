#include "stdafx.h"
#include "Point2D.h"

Point2D::Point2D(float x, float y):x(x),y(y)
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
