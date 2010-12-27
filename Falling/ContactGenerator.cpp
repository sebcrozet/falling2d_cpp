#include "stdafx.h"
#include "ContactGenerator.h"


void ContactGenerator::DeduceContactsDatas(std::vector<Collision *> &collisions, std::vector<Contact *> &c)
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
			// get tangeant
			cnt->tangeant = Vector2D(-cnt->normal.getY(),cnt->normal.getX());
			// now calculate closing velocity
			RigidBody *ra = c->sa->getParent();
			RigidBody *rb = c->sb->getParent();
			Vector2D lin1 = Vector2D(0,0,ra->getOmega()).cross(Vector2D(c->sa->toTranslatedInv(cnt->absoluteContactPoint))) + ra->getV();
			Vector2D lin2 = Vector2D(0,0,rb->getOmega()).cross(Vector2D(c->sb->toTranslatedInv(cnt->absoluteContactPoint))) + rb->getV();
			cnt->closingVelocity = cnt->toLocal(lin1 + lin2);
			// calculate total system's inertia
			cnt->totalInertia = ra->getInvM() + rb->getInvM() + ra->getInvI() + rb->getInvI();
		}
	}
}