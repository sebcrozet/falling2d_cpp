#include "stdafx.h"
#include "Point2D.h"

// TODO: remove padding initialisation
Vector2D::Vector2D(const Vector2D &v) : x(v.x), y(v.y), z(v.z), padding(0)
{ }
Vector2D::Vector2D(const Point2D &pt) : x(pt.getX()), y(pt.getY()), z(0), padding(0)
{ }
Vector2D::Vector2D(const Point2D &p1, const Point2D &p2) : x(p2.getX() - p1.getX()), y(p2.getY() - p1.getY()), z(0), padding(0)
{ }
Vector2D::Vector2D(Real x, Real y, Real z) : x(x), y(y), z(z), padding(0)
{ }

