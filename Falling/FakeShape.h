#ifndef __FAKE_SHAPE__
#include "Shapes.h"

// Fake shape for optimization purpose (to create shape-based linked lists' sentinels
class FakeShape : public Shape
{
	// Every function do not do anything
	Real getSurface();
	Real getInertiaMomentum(Real m); 
	int getShapeTypeID();
	void updateAABB();
};
#define __FAKE_SHAPE__
#endif
