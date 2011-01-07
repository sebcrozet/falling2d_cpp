#include "stdafx.h"
#include "ContactGenerator.h"

void Contact::updateVelChange(float t)
{
	float vFromAcc = s1->getParent()->getAcc() * t * normal;
	if(s2)
		vFromAcc -= s2->getParent()->getAcc() * t * normal;
	float fakerest = 0.2f;
	if(abs(closingVelocity.getX()) < 0.1f)
		fakerest = 0;
	desiredVelocityChange = -closingVelocity.getX() - fakerest /* restitution */ * (closingVelocity.getX() - vFromAcc);
}
void Contact::awakeIfNeeded()
{
	if(!s2) return;
	bool s1sl = s1->getParent()->isSleeping();
	bool s2sl = s2->getParent()->isSleeping();
	if(s1sl ^ s2sl)
	{
		if(s1sl) // s2 not sleeping
			s1->getParent()->setAwake(true);
		else
			s2->getParent()->setAwake(true);
	}
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
			cnt->relContactPoint[0] = cnt->s1->toTranslatedInv(cnt->absoluteContactPoint);
			if(cnt->s2)
				cnt->relContactPoint[1] = cnt->s2->toTranslatedInv(cnt->absoluteContactPoint);
			RigidBody *ra = cnt->s1->getParent();
			Vector2D lin1 = cnt->toLocal(Vector2D(0,0,ra->getOmega()).cross(cnt->relContactPoint[0]) + ra->getV());
			Vector2D lin2;
			RigidBody *rb = (cnt->s2?cnt->s2->getParent():0);
			cnt->lin1 = lin1.magnitude();
			cnt->lin2 = 0;
			if(rb)
			{
				lin2 = cnt->toLocal(Vector2D(0,0,rb->getOmega()).cross(cnt->relContactPoint[1]) + rb->getV());
				cnt->lin2 = lin2.magnitude();
			}
			cnt->closingVelocity = lin1 - lin2;
			cnt->updateVelChange(dt);
			Vector2D dvel = ((cnt->relContactPoint[0] ^ cnt->normal) * ra->getInvI())^cnt->relContactPoint[0];
			cnt->angin[0] = dvel * cnt->normal;
			cnt->linin[0] = ra->getInvM();
			cnt->dvel = cnt->angin[0] + cnt->linin[0];
			Vector2D dvely = ((cnt->relContactPoint[0] ^ cnt->tangeant) * ra->getInvI())^cnt->relContactPoint[0];
			cnt->dvely = dvely * cnt->tangeant + ra->getInvM();
			if(rb)
			{
				dvel = ((cnt->relContactPoint[1] ^ cnt->normal) * rb->getInvI())^cnt->relContactPoint[1];
				cnt->angin[1] = dvel * cnt->normal;
				cnt->linin[1] = rb->getInvM();
				cnt->dvel += cnt->angin[1] + cnt->linin[1];
				dvely = ((cnt->relContactPoint[1] ^ cnt->tangeant) * rb->getInvI())^cnt->relContactPoint[1];
				cnt->dvely += dvely * cnt->tangeant + rb->getInvM();
			}
			cnt->totalInertia = 1.0f / (cnt->angin[0] + cnt->linin[0] + (rb?cnt->angin[1] + cnt->linin[1] :0.));
			cnt->unitlinmov[0] = cnt->linin[0] * cnt->totalInertia;
			cnt->unitangmov[0] = (((cnt->relContactPoint[0] ^ cnt->normal) * ra->getInvI()).getZ())*cnt->totalInertia;
			if(rb)
			{
				cnt->unitlinmov[1] = cnt->linin[1] * cnt->totalInertia;
				cnt->unitangmov[1] = (((cnt->relContactPoint[1] ^ cnt->normal) * rb->getInvI()).getZ())*cnt->totalInertia;
			}
			cts.push_back(cnt);

		}
	}
}