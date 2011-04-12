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
#ifndef _OBBtree
#include "TunningConstants.h"
#include "OBB.h"
#include <vector>
#include <stack>

namespace Falling
{
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
    {
	return (r == l);
    }
}
#define _OBBtree
#endif
