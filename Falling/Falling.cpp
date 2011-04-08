// Falling.cpp : définit les fonctions exportées pour l'application DLL.
//
#include "stdafx.h"
#include "Falling.h"
#include "DoubleLinkedList.h"
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
      std::remove(objs.begin(),objs.end(),r);
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
void World::solve(Real dt)
{
  // add and remove objects now
  // TODO : uncomment the Sleep function
  dumpAddDelete();
  if(paused)
    return;
  checkSleeps(dt);
  integrate(dt);
  solvePenetrationsAndImpulse(dt);
}


void World::integrate(Real dt)
{
	VitessSolver::integrate(objs,dt);
}

void World::solvePenetrationsAndImpulse(Real dt)
{  // solve distances (collision detection)
  std::vector<Collision *> colls;
  ca.solve(colls);
  if(colls.size())
    {
      std::vector<Contact *> ctcts;
      std::stack<Island *> isls,isls2;
	  // Build islands
	  Island::batchIslands(colls,isls);
	  //printf("NBR ISLS ==>  %i\n",isls.size());
	  while(!isls.empty())
	  {
		  // TODO:  avoid this horrible stack transfert
		  isls2.push(isls.top());
		  isls.top()->calculateStackLevels();
		  isls.pop();
	  }
	  //
	  ContactGenerator::DeduceContactsDatas(colls,ctcts,dt);
	  //PenetrationSolver::solve(ctcts);
	  //printf("Nbr islands: %i\n", isls2.size());
	  while(!isls2.empty())
	  {
		  int basecoeff = isls2.top()->getNbrCtcts() / 10;
		  if(basecoeff < 5)
			  basecoeff = 5;
		  for(int i = 0; i < basecoeff; i++)
		  {
			  // stack implosion
			  //PenetrationSolver::solve(ctcts);
			  PenetrationSolver::solve(isls2.top(), true, (basecoeff-i) / 2);//*basecoeff/2);
			  // stack explosion
			  PenetrationSolver::solve(isls2.top(), false, i);//basecoeff);
		  }
		  delete isls2.top();
		  isls2.pop();
	  }
	  ImpulseSolver::solve(ctcts,dt);
    }
}