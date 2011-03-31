#include "stdafx.h"
#include "AABB.h"

AABB::AABB() : parent(0)
{ }

void AABB::update()
{
}

EndPoint::EndPoint() : parentid(-1), value(0)
{ }

EndPoint::EndPoint(Real v, bool isMax, int parent) : value(v)
{
  parentid = (parent << 1) | (isMax & 1);
}
