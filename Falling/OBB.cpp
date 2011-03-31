#include "stdafx.h"
#include "OBB.h"

OBB::OBB(Point2D a,Point2D b,Point2D c,Point2D d,ImplicitShape *parent, Real aire, int id) : parent(parent), aire(aire), obbid(id)
{
  pts[0] = Vector2D(a);
  pts[1] = Vector2D(b);
  pts[2] = Vector2D(c);
  pts[3] = Vector2D(d);
  axis[0] = pts[1] - pts[0];
  axis[1] = pts[3] - pts[0];

  for(int i=0; i<2 ; i++)
    {
      axis[i] /= axis[i] * axis[i];
      origin[i] = pts[0] * axis[i];
    }
}

void OBB::translate(const Vector2D &u)
{
  for(int i = 0; i < 4; i++)
    pts[i] += u;
  origin[0] = pts[0] * axis[0];
  origin[1] = pts[1] * axis[1];
}

bool OBB::intersects(OBB *o)
{
  return intersects2axis(o) && o->intersects2axis(this);
}

bool OBB::intersects2axis(OBB *o)
{
  for(int i = 0; i < 2; i++)
    {
      Vector2D ax = parent->toRotated(axis[i]);
      Real dot = parent->toTranslatedInv(o->parent->toGlobal(o->pts[0])) * ax;
      Real dotMin = dot;
      Real dotMax = dot;

      for(int j = 1; j < 4; j++)
        {
          dot = parent->toTranslatedInv(o->parent->toGlobal(o->pts[j])) * ax;
          if(dot > dotMax)
            dotMax = dot;
          else if(dot < dotMin)
            dotMin = dot;
        }
      if(dotMax < origin[i] || dotMin > 1 + origin[i])
        return false;
    }
  return true;
}

bool OBB::intersectsPlane(InfinitePlane *p)
{
  for(int i = 0; i < 4; i++)
    {
      if(p->isInSolidHalfSpace(parent->toGlobal(Point2D(pts[i]))))
        return true;
    }
  return false;
}

Point2D OBB::getCenter() const
{
  return Point2D((pts[0].getX() + pts[2].getX()) / 2, (pts[0].getY() + pts[2].getY()) / 2);
}

void OBB::getMedialAxis(Point2D *center, Point2D *refpt)
{
  *center = getCenter();
  Vector2D a0 = pts[1] - pts[0], a1 =	pts[3] - pts[0];
  if(a0*a0 > a1*a1)
    *refpt = *center + (a1 / 2);
  else
    *refpt = *center + (a0 / 2);
}
