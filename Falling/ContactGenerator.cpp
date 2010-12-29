#include "stdafx.h"
#include "ContactGenerator.h"

void Contact::updateVelChange(float t)
{
	float vFromAcc = s1->getParent()->getAcc() * t * normal;
	if(s2)
		vFromAcc -= s2->getParent()->getAcc() * t * normal;
	desiredVelocityChange = -closingVelocity.getX() - 1.f /* restitution */ * (closingVelocity.getX()/* - vFromAcc*/);
}

void ContactGenerator::DeduceContactsDatas(std::vector<Collision *> &collisions, std::vector<Contact *> &cts,float dt)
{
	for(int i=0; i<collisions.size();i++)
	{
		Collision *c = collisions[i];
		int max = c->c.size();
		for(int j=0;j<max;j++)
		{
			SubCollision sc = c->c[j];
			Contact *cnt = new Contact();
			cnt->s1 = c->sa;
			cnt->s2 = c->sb;
			cnt->absoluteContactPoint= Point2D::getMiddle(sc.ptA,sc.ptB);
			cnt->normal = Vector2D(sc.ptA,sc.ptB);
			cnt->penetration = cnt->normal.normalise(); // normalise normal and return penetration depth
			if(cnt->s1->isFixed())
			{
				cnt->s1 = cnt->s2;
				cnt->s2 = 0;
				cnt->normal.reflect();
			}
			else if(cnt->s2->isFixed())
				 cnt->s2 = 0;
			// get tangeant
			cnt->tangeant = Vector2D(-cnt->normal.getY(),cnt->normal.getX());
			// now calculate closing velocity
			RigidBody *ra = cnt->s1->getParent();
			Vector2D lin1 = cnt->toLocal(/*Vector2D(0,0,ra->getOmega()).cross(Vector2D(cnt->s1->toLocal(cnt->absoluteContactPoint))) + */ra->getV());
			Vector2D lin2;
			RigidBody *rb = (cnt->s2?cnt->s2->getParent():0);
			if(rb)
				lin2 = cnt->toLocal(/*Vector2D(0,0,rb->getOmega()).cross(Vector2D(cnt->s2->toLocal(cnt->absoluteContactPoint))) +*/ rb->getV());
			cnt->closingVelocity = lin1 - lin2;
			// calculate total system's inertia
			cnt->totalInertia = 1.0f / (ra->getInvM() /*+ ra->getInvI()*/ + (rb?rb->getInvM() /*+ rb->getInvI()*/:0));
			cnt->updateVelChange(dt);
			Vector2D dvel = ((Vector2D(cnt->s1->toLocal(cnt->absoluteContactPoint)) ^ cnt->normal) * ra->getInvI())^cnt->s1->toLocal(cnt->absoluteContactPoint);
			cnt->dvel = /*dvel * cnt->normal + */ra->getInvM();
			if(rb)
			{
				dvel = ((Vector2D(cnt->s2->toLocal(cnt->absoluteContactPoint)) ^ cnt->normal) * rb->getInvI())^cnt->s2->toLocal(cnt->absoluteContactPoint);
				cnt->dvel += /*dvel * cnt->normal*/ + rb->getInvM();
			}
			cts.push_back(cnt);

		}
	}
}