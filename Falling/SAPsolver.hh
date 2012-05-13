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
#ifndef _SAPSOLVER_H
#include "TunningConstants.hh"
#include "PairManager.hh"
#include "Shapes.hh"
#include "AABB.hh"
#include <stack>

namespace Falling
{
    class SAPsolver
    {
	private:
	    PairManager pm;
	    std::vector<AABB> aabbs;
	    std::vector<EndPoint> epx;
	    std::vector<EndPoint> epy;
	    std::vector<int> updateids;
	    std::stack<int> emptyaabbs;
	    void (*cb_addobj)(Pair *, Shape *, Shape *);
	    bool (*cb_removeobj)(Pair *);
	    void (*cb_deleteobj)(Pair &);

	    // TODO: remove (debug)
	    void verifylist(std::vector<EndPoint> &);
	    // end TODO
	    void updateBoxCollisionPairs(AABB &b);
	    void updateAddedBoxCollisionPairs(AABB &);
	    void updateEndPointMax(std::vector<EndPoint> &list,int *im, Real xm, Real xM, Real ym, Real yM, int xyid);
	    void updateEndPointMin(std::vector<EndPoint> &list,int *im, Real xm, Real xM, Real ym, Real yM, bool sens, int xyid);

	public:
	    void removePair(Pair *);
	    SAPsolver(void (*cb_addobj)(Pair *, Shape *, Shape *), bool (*cb_removeobj)(Pair *), void (*cb_deleteobj)(Pair &));
	    ~SAPsolver();
	    void addObject(Shape *s);
	    void removeObject(Shape *s);
	    void notifyBoxMoved(Shape *s);
	    void verifyMovedObjects();
	    Pair *solve(int *nbres);
    };
}
#define _SAPSOLVER_H
#endif
