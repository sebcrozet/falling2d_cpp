#include "stdafx.h"
#include "OBBtree.h"

OBBtree::OBBtree(OBBtree *r, OBBtree *l, OBB *o) : r(r), l(l), o(o)
{ }

OBBtree::~OBBtree()
{ delete o; }

void OBBtree::realeaseTree()
{
	if(r != 0)
		r->realeaseTree();
	if(l != 0)
		l->realeaseTree();
	delete this;
}

void OBBtree::traverseTree(OBBtree *a, OBBtree *b, std::vector<OBBIntersection*> &res)
{
	std::stack<OBBtree *> s;

	while(true)
	{
		if(a->o->intersects(b->o))
		{
			if(a->isLeaf() && b->isLeaf())
				res.push_back(new OBBIntersection(a->o, b->o));
			else if(b->isLeaf() || a->o->getAire() > b->o->getAire())
			{
				s.push(a->r);
				s.push(b);
				a = a->l;
				continue;
			}
			else
			{
				s.push(a);
				s.push(b->r);
				b = b->l;
				continue;
			}
		}
		if(s.empty())
			break;
		a = s.top(); s.pop();
		b = s.top(); s.pop();
	}
}

OBBIntersection::OBBIntersection(OBB *o1, OBB *o2) : o1(o1), o2(o2)
{ }