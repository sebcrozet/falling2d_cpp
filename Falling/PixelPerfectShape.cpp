#include "stdafx.h"
#include "PixelPerfectShape.h"

PixelPerfectShape::PixelPerfectShape(int dw, int dx, bool (*isSolidPixel)(Real, Real))
  : dw(dw), dh(dx), isSolid(isSolidPixel)
{
  fixedobj = true;
}

Real PixelPerfectShape::getSurface()
{
  return dh * dw * 4;
}
Real PixelPerfectShape::getInertiaMomentum(Real)
{
  return 0;
}
int PixelPerfectShape::getShapeTypeID()
{
  return 20;
}
void PixelPerfectShape::updateAABB()
{
  Vector2D u = t.getU();
  aabb_xM = u.getX() + dw;
  aabb_xm = u.getX() - dw;
  aabb_xm = u.getY() - dh;
  aabb_yM = u.getY() + dh;
}
