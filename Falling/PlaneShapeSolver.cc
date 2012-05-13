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
#include "stdafx.hh"
#include "PlaneShapeSolver.hh"

namespace Falling
{
    PlaneShapeSolver::PlaneShapeSolver(InfinitePlane *p, Shape *s)  : pm(pairDeleted), s(s), ip(p)
    { }

    void PlaneShapeSolver::pairDeleted(Pair &p)
    {
	delete (PlaneImplicitShapeDistanceSolver *)p.e;
    }

    bool PlaneShapeSolver::canDestroy()
    {
	std::stack<Pair *> todel;
	Pair *ap = pm.getActivePairs();
	int n = pm.getNbActivePairs();
	for(int i = 0; i < n; i++)
	{
	    if(((PlaneImplicitShapeDistanceSolver *)ap[i].e)->canDestroy())
		todel.push(ap);
	}
	while(!todel.empty())
	{
	    pm.removePair(todel.top());
	    todel.pop();
	}
	return pm.getNbActivePairs() == 0; // no valid cash datas
    }

    bool PlaneShapeSolver::_solve(std::vector<ContactBackup *> &res)
    {
	std::vector<OBB *> oi;
	traverseTree(s->getOtree(),  oi);
	int size = (int)oi.size();
	for(int i = 0; i < size; i++)
	{
	    OBB *oin = oi[i];
	    Pair* p = pm.addPair(oin->getID(), 0);
	    if(p->e) // pair already exists. Mark as still active
		((PlaneImplicitShapeDistanceSolver *)p->e)->setInactive(false);
	    else // new pair
		p->e = new PlaneImplicitShapeDistanceSolver(*ip, *oin->getParent());
	    oi[i] = 0;
	}
	oi.clear();
	Pair *ap = pm.getActivePairs();
	int n = pm.getNbActivePairs();
	std::stack<Pair *> todel;
	for(int i = 0; i < n; i++)
	{
	    if(((PlaneImplicitShapeDistanceSolver *)ap[i].e)->solve(res))
		todel.push(&ap[i]);
	    else
		((PlaneImplicitShapeDistanceSolver *)ap[i].e)->setInactive(true); // mark as inactive
	}
	while(!todel.empty())
	{
	    pm.removePair(todel.top());
	    todel.pop();
	}
	return pm.getNbActivePairs() == 0;
    }

    void PlaneShapeSolver::traverseTree(OBBtree *a, std::vector<OBB *> &res)
    {
	std::stack<OBBtree *> s;

	while(true)
	{
	    if(a->o->intersectsPlane(ip))
	    {
		if(a->isLeaf())
		    res.push_back(a->o);
		else
		{
		    s.push(a->r);
		    a = a->l;
		    continue;
		}
	    }
	    if(s.empty())
		break;
	    a = s.top();
	    s.pop();
	}
    }

    PlaneImplicitShapeDistanceSolver::PlaneImplicitShapeDistanceSolver(InfinitePlane &ip, ImplicitShape &is) : is(is), p(ip), cash(ip, is)
    {
	Point2D unusued;
	optid = is.getSupportPoint(ip.getNormal().reflexion(), &unusued);
    }

    bool PlaneImplicitShapeDistanceSolver::canDestroy()
    {
	return p.sqDistToPlane(Point2D(is.getCenter()) - is.getBoundingSphereRadius()) > PS_DESTROYLIMIT;
    }

    bool PlaneImplicitShapeDistanceSolver::_solve(std::vector<ContactBackup *> &res)
    {
	Point2D ppt;
	optid = is.getMarginedSupportPoint(p.getNormal().reflexion(), &ppt, optid);
	if(p.isInSolidHalfSpace(ppt))
	    cash.addContact(SubCollision(p.getProjectedPt(ppt), ppt));
	if(cash.getNbrContacts())
	{
	    for(int i = 0; i < cash.getNbrContacts(); i++)
		res.push_back(cash.getContacts(i));
	    return false;
	}
	return canDestroy();
    }
}
