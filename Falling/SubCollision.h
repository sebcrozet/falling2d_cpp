#ifndef _SUB_COLLISION_INC
#include "Point2D.h"

struct SubCollision
{
  Point2D ptA, ptB; // penetration contact points. Penetration vector given by: vp = vector(ptA, ptB)

  SubCollision(const Point2D &a, const Point2D &b);
};
#define _SUB_COLLISION_INC
#endif
