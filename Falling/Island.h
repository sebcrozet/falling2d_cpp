#ifndef __ISLANDS__
#include "CollisionArbiter.h"

class PenetrationSolver;
class Island
{
	friend PenetrationSolver;
private:
	std::vector<Collision *> stackLevels;
	std::queue<Shape *> graphNodes; // first graph's nodes for breadth first search.

	static void batchIsland(Island *isl,Shape *coll); // recursive call
public:
	Island();
	void calculateStackLevels();
	void pushToLevelOneChain(Collision *c);
	inline void insertToLevelOne(Shape *c);
	inline bool isEmpty();

	static void batchIslands(std::vector<Collision *> &colls,std::stack<Island *> &islands);
};
inline void Island::insertToLevelOne(Shape *c)
{ graphNodes.push(c); }
inline bool Island::isEmpty()
{ return graphNodes.empty() ;}
#define __ISLANDS__
#endif