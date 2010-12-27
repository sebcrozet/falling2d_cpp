#include "stdafx.h"
#include "CollisionArbiter.h"

Collision::Collision(Shape *s, Shape *s2)
{
	// choose correct collision detector and order shapes
	int idsum = s->getShapeTypeID() + s2->getShapeTypeID();
	if(idsum == 2)
	{
		// disk-disk
		cd = new DiskDiskSolver((Disk *)s, (Disk *)s2);
		sa = s;
		sb = s2;
	}
	else if(idsum > 10)
	{
		// plane-shape
		if(s->getShapeTypeID() == 10)
		{
			cd = new PlaneShapeSolver((InfinitePlane *)s, (Shape *)s2);
			sa = s;
			sb = s2;
		}
		else
		{
			cd = new PlaneShapeSolver((InfinitePlane *)s2, (Shape *)s);
			sa = s2;
			sb = s;
		}
	}
	else
	{
		// nothing special
		cd = new GenericSolver(s, s2);
		sa = s;
		sb = s2;
	}
}

Collision::~Collision()
{ 
	c.clear();
	int idsum = sa->getShapeTypeID() + sb->getShapeTypeID();
	if(idsum == 2)
		delete (DiskDiskSolver *)cd; 
	else if(idsum > 10)
		delete (PlaneShapeSolver *)cd; 
	else
		delete (GenericSolver *)cd;
}

// Collision arbiter
CollisionArbiter::CollisionArbiter() : sap(addP,removeP,deleteP)
{ }

CollisionArbiter::~CollisionArbiter()
{ s.clear(); }

bool CollisionArbiter::removeP(Pair *p)
{
	if(!((Collision *)p->e)->cd->canDestroy())
	{
		((Collision *)p->e)->cd->setInactive(true); // mark as inactive but keep it in cash
		return false;
	}
	else
		return true;
}

void CollisionArbiter::deleteP(Pair &p)
{
	delete (Collision *)p.e;
}

void CollisionArbiter::addP(Pair *p, Shape *s, Shape *s2)
{
	if(p->e)
		((Collision *)p->e)->cd->setInactive(false); // mark as active
	else
		p->e = new Collision(s, s2);
}

void CollisionArbiter::addObject(Shape *s)
{
	sap.addObject(s);
}

void CollisionArbiter::deleteObject(Shape *s)
{
	sap.removeObject(s);
}

void CollisionArbiter::notifyObjectMoved(Shape *s)
{
	sap.notifyBoxMoved(s);
}

void CollisionArbiter::solve(std::vector<Collision*> &res)
{
	int n;
	Pair *p;
	p = sap.solve(&n);

	std::stack<Pair *> todel;
	for(int i = 0; i < n; i ++)
	{
		((Collision *)p[i].e)->c.clear();
		if(((Collision*)p[i].e)->cd->solve(((Collision *)p[i].e)->c))
			;//todel.push(&p[i]);
		else
			res.push_back((Collision*)p[i].e);
	}
	/*while(!todel.empty())
	{
		sap.removePair(todel.top());
		todel.pop();
	}*/
}