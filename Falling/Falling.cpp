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
		std::stack<Island *> isls,isls2;
		// Build islands

		Island::batchIslands(colls,isls);
		
		printf("NBR ISLS ==>  %i\n",isls.size());
		while(!isls.empty())
		{
			// TODO: ce trensfert de pile est très moche!
			isls2.push(isls.top());
			isls.top()->calculateStackLevels();
			isls.pop();
		}
		//
		ContactGenerator::DeduceContactsDatas(colls,ctcts,dt);
		while(!isls2.empty())
		{
			PenetrationSolver::solve(isls2.top());
			isls2.pop();																									
		}
		ImpulseSolver::solve(ctcts,dt);
	}
	return colls;
}

