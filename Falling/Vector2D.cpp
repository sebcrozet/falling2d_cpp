#include "stdafx.h"
#include "Point2D.h"

Vector2D::Vector2D(const Vector2D &v) : x(v.x), y(v.y), z(v.z)
{ }
Vector2D::Vector2D(Point2D &pt) : x(pt.getX()), y(pt.getY()), z(0)
{ }
Vector2D::Vector2D(Point2D &p1, Point2D &p2) : x(p2.getX() - p1.getX()), y(p2.getY() - p1.getY()), z(0) 
{ }
Vector2D::Vector2D(float x, float y, float z) : x(x), y(y), z(z)
{ }

