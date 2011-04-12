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
#include "GenericSolver.h"
#include <stack>

GenericSolver::GenericSolver(Shape *s1, Shape *s2) : s1(s1), s2(s2), pm(deletePair)
{ }

void GenericSolver::deletePair(Pair &p)
{
  delete ((GJKsolver *)p.e);
}

bool GenericSolver::canDestroy()
{
  std::stack<Pair *> todel;
  Pair *ap = pm.getActivePairs();
  int n = pm.getNbActivePairs();
  for(int i = 0; i < n; i++)
    {
      if(((GJKsolver *)ap[i].e)->canDestroy())
        todel.push(ap);
    }
  while(!todel.empty())
    {
      pm.removePair(todel.top());
      todel.pop();
    }
  return pm.getNbActivePairs() == 0; // no valid cash datas
}

bool GenericSolver::_solve(std::vector<ContactBackup *> &res)
{
  std::vector<OBBIntersection *> oi;
  OBBtree::traverseTree(s1->getOtree(), s2->getOtree(), oi);
  int s = (int)oi.size();
  for(int i = 0; i < s; i++)
    {
      OBBIntersection *oin = oi[i];
      Pair* p = pm.addPair(oin->o1->getID(), oin->o2->getID() + GS_IDBIGOFFSET);
      // add GS_IDBIGOFFET to ensure id unicity
      if(p->e) // pair already exists. Mark as still active
        ((GJKsolver *)p->e)->setInactive(false);
      else // new pair
        p->e = new GJKsolver(*oin->o1->getParent(), *oin->o2->getParent());
      delete oin;
      oi[i] = 0;
    }
  oi.clear();
  Pair *ap = pm.getActivePairs();
  int n = pm.getNbActivePairs();
  std::stack<Pair *> todel;
  for(int i = 0; i < n; i++)
    {
      if(((GJKsolver *)ap[i].e)->solve(res))
        todel.push(&ap[i]);
      else
        ((GJKsolver *)ap[i].e)->setInactive(true); // mark as inactive
    }
  while(!todel.empty())
    {
      pm.removePair(todel.top());
      todel.pop();
    }
  return pm.getNbActivePairs() == 0;
}
