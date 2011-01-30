#include "stdafx.h"
#include "CollisionArbiter.h"

Collision::Collision(Shape *s)
{
	sa = s;
	sb = s;	   
	// TODO: remove: useless but OK for debug	 
	preva =0;
	prevb = 0;
	nexta=0;
	nextb=0;
	//
}
Collision::Collision(Shape *s, Shape *s2)
{
	// TODO: remove: useless but OK for debug	 
	preva =0;
	prevb = 0;
	nexta=0;
	nextb=0;
	//
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

void Collision::removeFromList()
{
	if(preva->sa == sa)
		preva->nexta = nexta;
	else
		preva->nextb = nexta;

	if(nexta->sa == sa)
		nexta->preva = preva;
	else
		nexta->prevb = preva;

	if(prevb->sa == sb)
		prevb->nexta = nextb;
	else
		prevb->nextb = nextb;

	if(nextb->sa == sb)
		nextb->preva = prevb;
	else
		nextb->prevb = prevb;
	
	// TODO: remove: Useless but OK for debug purpoise
	preva =0;
	prevb = 0;
	nexta=0;
	nextb=0;
}
void Collision::autoInsert()
{
	// TODO: remove: validity test
	if(preva != 0)
		return; // error...
	// end TODO
	Collision *ca = sa->getCollisionList();
	Collision *cb = sb->getCollisionList();
	preva = ca;
	prevb = cb;

	if(sa == ca->sa)
	{
		nexta = ca->nexta;
		if(ca->nexta->sa == sa)
			ca->nexta->preva = this;
		else
			ca->nexta->prevb = this;
		ca->nexta = this;
	}
	else
	{
		nexta = ca->nextb;
		if(ca->nextb->sa == sa)
			ca->nextb->preva = this;
		else
			ca->nextb->prevb = this;
		ca->nextb = this;
	}
	// Insert in b list
	if(sb == cb->sa)
	{
		nextb = cb->nexta;
		if(cb->nexta->sa == sb)
			cb->nexta->preva = this;
		else
			cb->nexta->prevb = this;
		cb->nexta = this;
	}
	else
	{
		nextb = cb->nextb;
		if(cb->nextb->sa == sb)
			cb->nextb->preva = this;
		else
			cb->nextb->prevb = this;
		cb->nextb = this;
	}
}

void Collision::insertInLevel(Collision *c)
{
	nextlvlptr = c->nextlvlptr;
	prevlvlptr = c;
	c->nextlvlptr = this;
	nextlvlptr->prevlvlptr = this;
}

Collision *Collision::inPlaceSortList(Collision *lbegin)
{
	Collision *lend = lbegin->prevlvlptr;
	Collision *curr = lbegin->nextlvlptr;   // begin with the second element
	Collision *next = curr; // save next element
	Collision *res = lbegin;
	// for all, do:
	while(curr != lend) // lend is a spetial case (handled separately)
	{
		next = curr->nextlvlptr;
		float currpen = curr->worstPenetrationAmount;
		while(true) // see stop condition in the next if statement
		{
			if(curr->prevlvlptr == lend) // it's a doubly-linked list (so, detect circularity to stop sorting)
			{
				res = curr; // save the first element
				break;
			}
			if(curr->prevlvlptr->worstPenetrationAmount < currpen)
			{
				// swap
				// TODO: swap only at last iteration!
				Collision *prevptr = curr->prevlvlptr;
				// 6 pointers => 6 affectations
				prevptr->prevlvlptr->nextlvlptr = curr;
				curr->prevlvlptr = prevptr->prevlvlptr;
				prevptr->prevlvlptr = curr;
				prevptr->nextlvlptr = curr->nextlvlptr;
				curr->nextlvlptr->prevlvlptr = prevptr;
				curr->nextlvlptr = prevptr;
			}
			else
				break;  
		}				 
		curr = next;
	}
	// handle lend case
	Collision *newend = curr->prevlvlptr;
	float currpen = curr->worstPenetrationAmount;
		while(true) // see stop condition in the next if statment
		{
			if(curr->prevlvlptr->worstPenetrationAmount < currpen)
			{
				// swap
				// TODO: swap only at last iteration!
				Collision *prevptr = curr->prevlvlptr;	
				// 6 pointers => 6 affectations
				prevptr->prevlvlptr->nextlvlptr = curr;
				curr->prevlvlptr = prevptr->prevlvlptr;
				prevptr->prevlvlptr = curr;
				prevptr->nextlvlptr = curr->nextlvlptr;	  
				curr->nextlvlptr->prevlvlptr = prevptr;
				curr->nextlvlptr = prevptr;
			}
			else
				break;
			if(curr->prevlvlptr == newend) // it's a doubly-linked list (so, detect circularity to stop sorting)
			{
				res = curr;
				break;
			}
		}
	// done
	return res;
}

// Collision arbiter
CollisionArbiter::CollisionArbiter() : sap(addP,removeP,deleteP)
{ }

CollisionArbiter::~CollisionArbiter()
{ 
	s.clear(); 
}

bool CollisionArbiter::removeP(Pair *p)
{
	if(!((Collision *)p->e)->cd->getIsInactive()) //To avoid deletion redondency problems
		((Collision *)p->e)->removeFromList(); // Remove this from collision graph
	if(!((Collision *)p->e)->cd->canDestroy())
	{
		((Collision *)p->e)->cd->setInactive(true); // mark as inactive but keep it in cash
		return false;
	}
	return true;
}

void CollisionArbiter::deleteP(Pair &p)
{
	delete (Collision *)p.e;
}

void CollisionArbiter::addP(Pair *p, Shape *s, Shape *s2)
{
	if(p->e)
	{
		if(((Collision *)p->e)->cd->getIsInactive())
			((Collision *)p->e)->cd->setInactive(false); // mark as active
		else
			return; // avoid redundency in collision deletion
	}
	else
		p->e = new Collision(s, s2);
	((Collision *)p->e)->autoInsert(); // Auto instert in collision graph
}

void CollisionArbiter::addObject(Shape *s)
{
	// Prepare object's collision list sentinels
	Collision *head = new Collision(s);
	Collision *tail = new Collision(s);
	head->nexta = tail;
	tail->preva = head;
	// other pointers are useless for sentinels, so don't initialize them
	s->setCollisionList(head,tail);
	////
	sap.addObject(s);
}

void CollisionArbiter::deleteObject(Shape *s)
{
	sap.removeObject(s);
	// Free object's collision list sentinels
	// All other collision should have been deleed by SAP,
	// so, head = first collision / tail = second collision
	Collision *c = s->getCollisionList();
	delete c->nexta;
	delete c;
	//
}

void CollisionArbiter::notifyObjectMoved(Shape *s)
{
	sap.notifyBoxMoved(s);
}

void CollisionArbiter::solve(std::vector<Collision*> &res)
{
	int n = 0;
	Pair *p;
	p = sap.solve(&n);

	std::stack<Pair *> todel;
	for(int i = 0; i < n; i ++)
	{
		bool collisionLost = false;
		if(!((Collision*)p[i].e)->sa->getParent()->isSleeping() || !((Collision*)p[i].e)->sb->getParent()->isSleeping())
		{
			collisionLost = ((Collision *)p[i].e)->c.size() > 0;
			((Collision *)p[i].e)->c.clear();
		/*if(*/((Collision*)p[i].e)->cd->solve(((Collision *)p[i].e)->c);//)
		//	;//todel.push(&p[i]);
		//else
		}	  
		if(((Collision*)p[i].e)->c.size())
			res.push_back((Collision*)p[i].e);
		else if(collisionLost)
		{
			// Wake up objects
			((Collision*)p[i].e)->sa->getParent()->setAwake(true);
			((Collision*)p[i].e)->sb->getParent()->setAwake(true);
		}
	}
	/*while(!todel.empty())
	{
		sap.removePair(todel.top());
		todel.pop();
	}*/
}