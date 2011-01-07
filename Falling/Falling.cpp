// Falling.cpp : définit les fonctions exportées pour l'application DLL.
//

#include "stdafx.h"
#include "Falling.h"
												
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

void World::checkSleeps(float dt)
{
	for(int i = 0;i < objs.size();i++)
		if(!objs[i]->isSleeping())
			objs[i]->updateSleepState(dt);
}


// return type should be void
std::vector<Collision *> World::solve(float dt)
{
	// add and remove objects now
	//checkSleeps(dt);
	dumpAddDelete();
	VitessSolver::integrate(objs,dt);
	// solve distances (collision detection)
	std::vector<Collision *> colls;
	ca.solve(colls);
	if(colls.size())
	{
		std::vector<Contact *> ctcts;
		ContactGenerator::DeduceContactsDatas(colls,ctcts,dt);
		PenetrationSolver::solve(ctcts);
		ImpulseSolver::solve(ctcts,dt);
	}
	return colls;
}

