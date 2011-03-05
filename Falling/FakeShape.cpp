#include "stdafx.h"
#include "FakeShape.h"

float FakeShape::getSurface()
{ return 0; }
float FakeShape::getInertiaMomentum(float)
{ return 0; }
int FakeShape::getShapeTypeID()
{ return 0; }
void FakeShape::updateAABB()
{ return; }
