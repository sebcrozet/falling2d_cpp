// Generic solver using hybrid GJK-EPA collision detection/penetration depth calculator
// Compute obb-tree intersection, and then apply hybrid distance solver to every sub-shape
//
#ifndef __GENERIC_SOLVER__
#include "CollisionDetector.h"
#include "PairManager.h"
#include "OBBtree.h"
#include "DistanceSolver.h"
#define GS_IDBIGOFFSET 2063 // il ne devrait pas y avoir plus de GS_IDBIGOFFET OBB dans un arbre. GS_IDBIGOFFET est un nombre premier (réduction de collisions dans le pm?). 

class GenericSolver	: public CollisionDetector
{
private:
	Shape *s1, *s2;
	PairManager pm;	// cash all sub-shape collision couple

	static void deletePair(Pair &p);
	bool _solve(std::vector<SubCollision> &res);
	bool canDestroy();
public: 
	GenericSolver(Shape *s1, Shape *s2);
};
#define __GENERIC_SOLVER__
#endif