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
#include "SAPsolver.h"

namespace Falling
{

    SAPsolver::SAPsolver(
	    void (*cb_addobj)(Pair *, Shape *, Shape *), 
	    bool (*cb_removeobj)(Pair *), 
	    void (*cb_deleteobj)(Pair &)) 
	: cb_addobj(cb_addobj), 
	cb_removeobj(cb_removeobj), 
	cb_deleteobj(cb_deleteobj)
    {
	pm.setDeletecallback(cb_deleteobj);
	// insert sentinels
	epx.push_back(EndPoint(-MACHINE_MAX, false, 0));
	epx.push_back(EndPoint(MACHINE_MAX, true, 0));
	epy.push_back(EndPoint(-MACHINE_MAX, false, 0));
	epy.push_back(EndPoint(MACHINE_MAX, true, 0));
	verifylist(epx);
	verifylist(epy);
    }

    SAPsolver::~SAPsolver()
    {
	aabbs.clear();
	epx.clear();
	epy.clear();
	updateids.clear();
	while(!emptyaabbs.empty())
	    emptyaabbs.pop();
    }

    void SAPsolver::removePair(Pair *p)
    {
	pm.removePair(p);
    }


    void SAPsolver::updateBoxCollisionPairs(AABB &b)
    {
	Real xm = epx[b.mins[0]].getValue(),
	     xM = epx[b.maxs[0]].getValue(),
	     ym = epy[b.mins[1]].getValue(),
	     yM = epy[b.maxs[1]].getValue();
	// take care of update order to avoid swap whith its own end points
	if(epx[b.mins[0] + 1].getValue() < xm)
	{
	    // update xM first
	    updateEndPointMax(epx, &b.maxs[0], xm, xM, ym, yM, 0);
	    updateEndPointMin(epx, &b.mins[0], xm, xM, ym, yM, true, 0);
	}
	else
	{
	    // update xm first
	    updateEndPointMin(epx, &b.mins[0], xm, xM, ym, yM, false, 0);
	    updateEndPointMax(epx, &b.maxs[0], xm, xM, ym, yM, 0);
	}
	// do the same for y
	if(epy[b.mins[1] + 1].getValue() < ym)
	{
	    // update yM first
	    updateEndPointMax(epy, &b.maxs[1], xm, xM, ym, yM, 1);
	    updateEndPointMin(epy, &b.mins[1], xm, xM, ym, yM, true, 1);
	}
	else
	{
	    // update ym first
	    updateEndPointMin(epy, &b.mins[1], xm, xM, ym, yM, false, 1);
	    updateEndPointMax(epy, &b.maxs[1], xm, xM, ym, yM, 1);
	}
	verifylist(epx);
	verifylist(epy);
    }

    void SAPsolver::updateEndPointMax(std::vector<EndPoint> &list,int *im, Real xm, Real xM, Real ym, Real yM, int xyid)
    {
	int i = *im;
	EndPoint en = list[i];
	Shape *spr = aabbs[en.getParent()].parent;
	//assert(!spr->isdeleting());
	bool sens = list[i + 1].getValue() < en.getValue();
	if(sens)
	{
	    // go right
	    do
	    {
		EndPoint e = list[i + 1];
		list[i] = e;
		if(!e.isMaxValue())
		{
		    // remap swapped obj endpoint id
		    aabbs[e.getParent()].mins[xyid] = i;
		    // may start collision
		    if(aabbs[e.getParent()].parent->AABBvsAABB(xm, xM, ym, yM))
		    {
			// start collision
			cb_addobj(pm.addPair(en.getParent(), e.getParent()), spr, aabbs[e.getParent()].parent);
		    }
		}
		else
		    // remap swapped obj endpoint id
		    aabbs[e.getParent()].maxs[xyid] = i;
		i++;
	    }
	    while(list[i + 1].getValue() < en.getValue());
	    list[i] = en;
	    *im = i;
	}
	else
	{
	    // go left
	    if(list[i - 1].getValue() > en.getValue())
	    {
		do
		{
		    EndPoint e = list[i - 1];
		    list[i] = e;
		    if(!e.isMaxValue())
		    {
			// remap swapped obj endpoint id
			aabbs[e.getParent()].mins[xyid] = i;
			// may stop collision
			if(!aabbs[e.getParent()].parent->AABBvsAABB(xm, xM, ym, yM))
			{
			    // stop collision
			    Pair* rp = pm.findPair(en.getParent(), e.getParent());
			    if(rp && cb_removeobj(rp))
				pm.removePair(rp);
			}
		    }
		    else
			// remap swapped obj endpoint id
			aabbs[e.getParent()].maxs[xyid] = i;
		    i--;
		}
		while(list[i - 1].getValue() > en.getValue());
		list[i] = en;
		*im = i;
	    }
	    //else don't move
	}
    }

    void SAPsolver::updateEndPointMin(std::vector<EndPoint> &list,int *im, Real xm, Real xM, Real ym, Real yM, bool sens, int xyid)
    {
	int i = *im;
	EndPoint en = list[i];
	Shape *spr = aabbs[en.getParent()].parent;
	if(sens)
	{
	    // go right
	    do
	    {
		EndPoint e = list[i + 1];
		list[i] = e;
		if(e.isMaxValue())
		{
		    // remap swapped obj endpoint id
		    aabbs[e.getParent()].maxs[xyid] = i;
		    // may stop collision
		    if(!aabbs[e.getParent()].parent->AABBvsAABB(xm, xM, ym, yM))
		    {
			// stop collision
			Pair* rp = pm.findPair(en.getParent(), e.getParent());
			if(rp && cb_removeobj(rp))
			    pm.removePair(rp);
		    }
		}
		else
		    // remap swapped obj endpoint id
		    aabbs[e.getParent()].mins[xyid] = i;
		i++;
	    }
	    while(list[i + 1].getValue() < en.getValue());
	    list[i] = en;
	    *im = i;
	}
	else
	{
	    // go left
	    if(list[i - 1].getValue() > en.getValue())
	    {
		do
		{
		    EndPoint e = list[i - 1];
		    list[i] = e;
		    if(e.isMaxValue())
		    {
			// remap swapped obj endpoint id
			aabbs[e.getParent()].maxs[xyid] = i;
			// may start collision
			if(aabbs[e.getParent()].parent->AABBvsAABB(xm, xM, ym, yM))
			{
			    // start collision
			    cb_addobj(pm.addPair(en.getParent(), e.getParent()), spr, aabbs[e.getParent()].parent);
			}
		    }
		    else
			// remap swapped obj endpoint id
			aabbs[e.getParent()].mins[xyid] = i;
		    i--;
		}
		while(list[i - 1].getValue() > en.getValue());
		list[i] = en;
		*im = i;
	    }
	    //else don't move
	}
    }

    Pair *SAPsolver::solve(int *nbres)
    {
	verifyMovedObjects();
	int n = updateids.size();
	for(int i = 0; i < n; i++)
	{
	    AABB &b = aabbs[updateids[i]];
	    b.parent->updateAABB(&epx[b.mins[0]], &epx[b.maxs[0]], &epy[b.mins[1]], &epy[b.maxs[1]]);
	    updateBoxCollisionPairs(b);
	}
	*nbres = pm.getNbActivePairs();
	verifylist(epx);
	verifylist(epy);
	return pm.getActivePairs();
    }

    void SAPsolver::notifyBoxMoved(Shape *s)
    {
	updateids.push_back(s->getAABBid());
	s->updateAABB();
    }

    void SAPsolver::verifyMovedObjects()
    {
	updateids.clear();
	for(unsigned int i=0; i<aabbs.size(); i++)
	{
	    Shape *s = aabbs[i].parent;
	    if(s->getMoved())
	    {
		notifyBoxMoved(s);
		s->setMoved(false);
	    }
	}
    }

    void SAPsolver::addObject(Shape *s)
    {
	int i;
	if(emptyaabbs.empty())
	{
	    i = aabbs.size();
	    aabbs.push_back(AABB());
	}
	else
	{
	    // fill one hole
	    i = emptyaabbs.top();
	    emptyaabbs.pop();
	}
	s->setAABBid(i);
	int endi = epx.size();
	epx.insert(epx.end() - 1, EndPoint(0,false, i));
	epx.insert(epx.end() - 1, EndPoint(0,true, i));
	epy.insert(epy.end() - 1, EndPoint(0,false, i));
	epy.insert(epy.end() - 1, EndPoint(0,true, i));
	AABB& b = aabbs[i];
	b.parent = s;
	b.maxs[0] = endi;
	b.mins[0] = endi - 1;
	b.maxs[1] = endi;
	b.mins[1] = endi - 1;
	s->updateAABB();
	s->updateAABB(&epx[endi - 1], &epx[endi],&epy[endi - 1], &epy[endi]);
	updateAddedBoxCollisionPairs(b);
	verifylist(epx);
	verifylist(epy);
    }

    void SAPsolver::verifylist(std::vector<EndPoint> &ep)
    {
	for(unsigned int i = 0; i < ep.size() - 1; i++)
	{
	    //assert(ep[i+1].getValue() >= ep[i].getValue());
	    //assert(ep[i].getValue() == -MACHINE_MAX || ep[i].getValue() == MACHINE_MAX || !aabbs[ep[i].getParent()].parent->isdeleting());
	}
	//assert(ep[ep.size() - 1].getValue() == -MACHINE_MAX || ep[ep.size() - 1].getValue() == MACHINE_MAX || !aabbs[ep[ep.size() - 1].getParent()].parent->isdeleting());
    }

    void SAPsolver::updateAddedBoxCollisionPairs(AABB &b)
    {
	//
	// insert in epy without collision handling
	int i = b.maxs[1];
	EndPoint e = epy[i];
	Real yM = e.getValue();
	EndPoint em = epy[i - 1];
	Real ym = em.getValue();
	// inert max and reuse i as upper bound for min index
	while(epy[i - 2].getValue() > e.getValue())
	{
	    epy[i] = epy[i - 2];
	    if(epy[i - 2].isMaxValue())
		aabbs[epy[i - 2].getParent()].maxs[1] = i;
	    else
		aabbs[epy[i - 2].getParent()].mins[1] = i;
	    i--;
	}
	epy[i] = e;
	b.maxs[1] = i;
	// insert min
	i--;
	while(epy[i - 1].getValue() > em.getValue())
	{
	    epy[i] = epy[i - 1];
	    if(epy[i - 1].isMaxValue())
		aabbs[epy[i - 1].getParent()].maxs[1] = i;
	    else
		aabbs[epy[i - 1].getParent()].mins[1] = i;
	    i--;
	}
	epy[i] = em;
	b.mins[1] = i;
	//
	// insert in epx
	// no collision handling with xM (causes only removals)
	i = b.maxs[0];
	e = epx[i];
	Real xM = e.getValue();
	em = epx[i - 1];
	Real xm = em.getValue();
	while(epx[i - 2].getValue() > e.getValue())
	{
	    epx[i] = epx[i - 2];
	    if(epx[i - 2].isMaxValue())
		aabbs[epx[i - 2].getParent()].maxs[0] = i;
	    else
		aabbs[epx[i - 2].getParent()].mins[0] = i;
	    i--;
	}
	epx[i] = e;
	b.maxs[0] = i;
	// scan for collisions whith xm
	for(int j = epx.size() - 2; j > i; j--)
	{
	    if(epx[j].isMaxValue())
	    {
		// maybe a collision
		if((!aabbs[epx[j].getParent()].parent->isFixed() || !b.parent->isFixed()) && aabbs[epx[j].getParent()].parent->AABBvsAABB(xm, xM, ym, yM))
		    cb_addobj(pm.addPair(epx[j].getParent(), e.getParent()),aabbs[epx[j].getParent()].parent, b.parent);
	    }
	}
	// placing xm and collision handling // (reuse i as upper bound for min index)
	i--;
	while(epx[i - 1].getValue() > em.getValue())
	{
	    epx[i] = epx[i - 1];
	    if(epx[i - 1].isMaxValue())
	    {
		aabbs[epx[i - 1].getParent()].maxs[0] = i;
		// maybe a collision
		if((!aabbs[epx[i].getParent()].parent->isFixed() || !b.parent->isFixed()) && aabbs[epx[i].getParent()].parent->AABBvsAABB(xm, xM, ym, yM))
		    cb_addobj(pm.addPair(epx[i].getParent(), e.getParent()), aabbs[epx[i].getParent()].parent, b.parent);
	    }
	    else
		aabbs[epx[i - 1].getParent()].mins[0] = i;
	    i--;
	}
	epx[i] = em;
	b.mins[0] = i;
	// finished!
    }

    void SAPsolver::removeObject(Shape *s)
    {
	int removeid = s->getAABBid();
	// remove endpoints
	int nbep = epx.size();
	// remap endpoints ids
	for(int i = aabbs[removeid].maxs[0] + 1; i < nbep - 1; i++) // stop at nbep - 1 to not touch sentinels
	{
	    if(epx[i].isMaxValue())
		aabbs[epx[i].getParent()].maxs[0] = i - 2;
	    else
		aabbs[epx[i].getParent()].mins[0] = i - 2;
	}
	for(int i = aabbs[removeid].mins[0] + 1; i < aabbs[removeid].maxs[0]; i++)
	{
	    if(epx[i].isMaxValue())
		aabbs[epx[i].getParent()].maxs[0] = i - 1;
	    else
		aabbs[epx[i].getParent()].mins[0] = i - 1;
	}
	// same for y
	for(int i = aabbs[removeid].maxs[1] + 1; i < nbep - 1; i++) // stop at nbep - 1 to not touch sentinels
	{
	    if(epy[i].isMaxValue())
		aabbs[epy[i].getParent()].maxs[1] = i - 2;
	    else
		aabbs[epy[i].getParent()].mins[1] = i - 2;
	}
	for(int i = aabbs[removeid].mins[1] + 1; i < aabbs[removeid].maxs[1]; i++)
	{
	    if(epy[i].isMaxValue())
		aabbs[epy[i].getParent()].maxs[1] = i - 1;
	    else
		aabbs[epy[i].getParent()].mins[1] = i - 1;
	}
	// now remove max and then min
	epx.erase(epx.begin() + aabbs[removeid].maxs[0]);
	epx.erase(epx.begin() + aabbs[removeid].mins[0]);
	epy.erase(epy.begin() + aabbs[removeid].maxs[1]);
	epy.erase(epy.begin() + aabbs[removeid].mins[1]);
	// remove every collision pairs
	// find couples second id
	std::vector<int> toremove;
	Pair *prs = pm.getActivePairs();
	int nbprs = pm.getNbActivePairs();
	for(int i = 0; i < nbprs; i++)
	{
	    if(prs[i].id1 == removeid)
		toremove.push_back(prs[i].id2);
	    else if(prs[i].id2 == removeid)
		toremove.push_back(prs[i].id1);
	}
	nbprs = toremove.size();
	// remove all pairs; user will be notified by pm remove callback
	for(int i = 0; i < nbprs; i++)
	    pm.removePair(removeid, toremove[i]);
	// remove AABB
	// dont swap with last box to avoid false id in hash table (updating everything is costly)
	// so just let an hole and an index to emply aabb array place
	emptyaabbs.push(removeid);
	// remove the aabb from the pending updates list
	printf("(a) %i\n", updateids.size());
	updateids.erase(std::remove(updateids.begin(), updateids.end(), removeid), updateids.end());
	printf("(b) %i\n", updateids.size());
	s->setdeleting();
	verifylist(epx);
	verifylist(epy);
	// done
    }
}
