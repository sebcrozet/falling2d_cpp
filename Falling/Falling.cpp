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
// Falling.cpp : définit les fonctions exportées pour l'application DLL.
//
#include "stdafx.h"
#include "Falling.h"
#include "DoubleLinkedList.h"

namespace Falling
{
  World::World()
    : paused(false)
  { }
  World::~World()
  { }

  void World::addObject(RigidBody *s)
  {
    addWaitingQueue.push(s);
  }

  void World::removeObject(RigidBody *s)
  {
    removeWaitingQueue.push(s);
  }

  void World::dumpAddDelete()
  {
    while(!removeWaitingQueue.empty())
    {
      RigidBody * r = removeWaitingQueue.top();
      ca.deleteObject(r->getShape());
      removeWaitingQueue.pop();
      objs.erase(std::remove(objs.begin(),objs.end(),r), objs.end());
      delete r;
    }
    while(!addWaitingQueue.empty())
    {
      RigidBody * r = addWaitingQueue.top();
      ca.addObject(r->getShape());
      addWaitingQueue.pop();
      objs.push_back(r);
    }
  }

  void World::notifyObjectMoved(RigidBody *s)
  {
    dumpAddDelete();
    ca.notifyObjectMoved(s->getShape());
  }

  void World::checkSleeps(Real dt)
  {
    for(unsigned int i = 0; i < objs.size(); i++)
      if(!objs[i]->isSleeping())
        objs[i]->updateSleepState(dt);
  }


  // return type should be void
  std::vector<Collision *> World::solve(Real dt)
  {
    // add and remove objects now
    dumpAddDelete();
    std::vector<Collision *> colls;
    if(paused)
    {
      ca.solve(colls);
      return colls;
    }
    return solvePenetrationsAndImpulseWithLCP(dt);
  }


  void World::integrate(Real dt)
  {
    VitessSolver::integrate(objs,dt);
  }

  /*
   * the return value should be void. Collisions are returned only for debug
   */
  std::vector<Collision *> World::solvePenetrationsAndImpulseWithLCP(Real dt)
  {
    // solve distances (collision detection)
    std::vector<Collision *> colls;
    ca.solve(colls);
    if(colls.size())
    {
      std::stack<Island *> isls; // FIXME: use a vector instead.
      // Build islands
      Island::batchIslands(colls,isls);
      while(!isls.empty())
      {
        Island *isl = isls.top();
        isl->doit(dt);
        isls.pop();
        delete isl;
      }
    }
    integrate(dt);
    checkSleeps(dt);
    return colls;
  }
}
