#include "stdafx.h"
#include "FakeShape.h"

Real FakeShape::getSurface()
{
  return 0;
}
Real FakeShape::getInertiaMomentum(Real)
{
  return 0;
}
int FakeShape::getShapeTypeID()
{
  return 0;
}
void FakeShape::updateAABB()
{
  return;
}
