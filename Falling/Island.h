#ifndef __ISLANDS__
#include "CollisionArbiter.h"

class PenetrationSolver;
class Island
{
  friend class PenetrationSolver;
  friend class ImpulseSolver;
private:
  int nbrCtcts;
  std::vector<Collision *> stackLevels;
  std::queue<Shape *> graphNodes; // first graph's nodes for breadth first search.

  static void batchIsland(Island *isl,Shape *coll); // recursive call
public:
  Island();
  ~Island();
  void calculateStackLevels();
  void pushToLevelOneChain(Collision *c);

  inline void insertToLevelOne(Shape *c)
  { graphNodes.push(c); }
  inline bool isEmpty()
  { return graphNodes.empty(); }
  inline int getNbrCtcts()
  { return nbrCtcts; }

  static void batchIslands(std::vector<Collision *> &colls,std::stack<Island *> &islands);
};
#define __ISLANDS__
#endif
