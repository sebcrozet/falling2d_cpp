#ifndef __PLANE_IMPLICIT_DIST_SOLVER__
#include "CollisionDetector.h"
#include "PairManager.h"
#include "Shapes.h"
#include "InfinitePlane.h"
#include "OBBtree.h"
#include <stack>
#define PS_DESTROYLIMIT 100

class PlaneImplicitShapeDistanceSolver : public CollisionDetector
{
private:
  ImplicitShape &is;
  InfinitePlane &p;
  int optid;
  ContactBatch cash;

  bool _solve(std::vector<ContactBackup *> &res);
public:
  PlaneImplicitShapeDistanceSolver(InfinitePlane &ip, ImplicitShape &is);
  bool canDestroy();
};

class PlaneShapeSolver : public CollisionDetector
{
private:
  PairManager pm;
  Shape *s;
  InfinitePlane *ip;

  void traverseTree(OBBtree *ot, std::vector<OBB *> &res);
  bool _solve(std::vector<ContactBackup *> &res); // returns whether current class should be destroyed
  static void pairDeleted(Pair &p);
public:
  PlaneShapeSolver(InfinitePlane *p, Shape *s);
  bool canDestroy();
};
#define __PLANE_IMPLICIT_DIST_SOLVER__
#endif
