#ifndef _OBBtree
#include "OBB.h"
#include <vector>
#include <stack>

struct OBBIntersection
{
	OBB* o1;
	OBB* o2;

	OBBIntersection(OBB* o1, OBB* o2);
};


struct OBBtree
{
	OBBtree *r, *l;
	OBB *o;
public:
	OBBtree(OBBtree *r, OBBtree *l, OBB *o);
	~OBBtree();
	void realeaseTree();
	inline bool isLeaf();
	static void traverseTree(OBBtree *a, OBBtree *b, std::vector<OBBIntersection*> &res);
};

inline bool OBBtree::isLeaf()
{ return (r == l); }
#define _OBBtree
#endif