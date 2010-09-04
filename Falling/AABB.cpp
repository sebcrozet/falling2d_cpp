#include "stdafx.h"
#include "AABB.h"

AABB::AABB()
{ }

EndPoint::EndPoint() : parent(0)
{ }

EndPoint::EndPoint(float v, bool isMax, ImplicitShape *parent) : parent(parent), isMax(isMax), value(v)
{ }