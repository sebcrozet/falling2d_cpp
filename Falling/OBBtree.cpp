/* Copyright (C) 2011 CROZET Sébastien

 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */
#include "stdafx.h"
#include "OBBtree.h"

namespace Falling
{
    OBBtree::OBBtree(OBBtree *r, OBBtree *l, OBB *o) : r(r), l(l), o(o)
    { }

    OBBtree::~OBBtree()
	{
		if(r)
			delete r;
		if(l)
			delete l;
		delete o;
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
		else if(b->isLeaf() || (!a->isLeaf() && a->o->getAire() > b->o->getAire()))
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
	    b = s.top();
	    s.pop();
	    a = s.top();
	    s.pop();
	}
    }

    OBBIntersection::OBBIntersection(OBB *o1, OBB *o2) : o1(o1), o2(o2)
    { }
}
