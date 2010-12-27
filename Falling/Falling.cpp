// Falling.cpp : définit les fonctions exportées pour l'application DLL.
//

#include "stdafx.h"
#include "Falling.h"
												
void World::addObject(Shape *s)
{
	addWaitingQueue.push(s);
}

void World::removeObject(Shape *s)
{	
	removeWaitingQueue.push(s);
}

void World::dumpAddDelete()
{				
	while(!removeWaitingQueue.empty())
	{
		ca.deleteObject(removeWaitingQueue.top());
		removeWaitingQueue.pop();
	}
	while(!addWaitingQueue.empty())
	{
		ca.addObject(addWaitingQueue.top());
		addWaitingQueue.pop();
	}
}

void World::notifyObjectMoved(Shape *s)
{ 
	dumpAddDelete();
	ca.notifyObjectMoved(s); 
}

// return type should be void
std::vector<Collision *> World::solve(float dt)
{
	// add and remove objects now
	dumpAddDelete();
	// solve distances (collision detection)
	std::vector<Collision *> colls;
	ca.solve(colls);
	return colls;
}

