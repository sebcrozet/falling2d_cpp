#include "stdafx.h"
#include "GeometryHelper.h"

GeometryHelper::Transformation2D::Transformation2D() : teta(0),coteta((Real)M_PI), siteta(0),  u(0,0)
{ }
GeometryHelper::Transformation2D::Transformation2D(const Vector2D &u, Real radTeta) : u(u)
{
  setTeta(radTeta);
}
GeometryHelper::Transformation2D::Transformation2D(const GeometryHelper::Transformation2D &t)
{
  siteta = t.siteta;
  coteta = t.coteta;
  teta = t.teta;
  u = Vector2D(t.u);
}

