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
#ifndef __PLANE_IMPLICIT_DIST_SOLVER__
#include "CollisionDetector.hh"
#include "PairManager.hh"
#include "Shapes.hh"
#include "InfinitePlane.hh"
#include "OBBtree.hh"
#include <stack>
#define PS_DESTROYLIMIT 100

namespace Falling
{
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
}
#define __PLANE_IMPLICIT_DIST_SOLVER__
#endif
