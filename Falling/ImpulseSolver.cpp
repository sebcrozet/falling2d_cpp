#include "stdafx.h"
#include "ImpulseSolver.h"

void ImpulseSolver::solve(std::vector<Contact *> scs,float dt)
{
	for(int i = 0;i < scs.size() * 10;i++)
	{
		Vector2D rch[2];
		Vector2D vch[2];
		Contact *worst = 0;
		float worstV = 0;
		for(int j=0;j<scs.size();j++)
		{
			if(scs[j]->desiredVelocityChange > worstV)
			{
				worstV = scs[j]->desiredVelocityChange;
				worst = scs[j];
			}
		}
		if(worst)
		{
			applyVelocityChange(worst,rch,vch);

			// adjust other velocities
			for(int j=0;j<scs.size();j++)
			{
				Vector2D cp;
				if(scs[j]->s1 == worst->s1)
				{
					cp = Vector2D();//rch[0] ^ scs[j]->s1->toLocal(scs[j]->absoluteContactPoint);
					cp = cp + vch[0];
					scs[j]->closingVelocity = scs[j]->closingVelocity + scs[j]->toLocal(cp);
					scs[j]->updateVelChange(dt);
				}
				else if(scs[j]->s2 == worst->s1)
				{
					cp = Vector2D();//rch[0] ^ scs[j]->s2->toLocal(scs[j]->absoluteContactPoint);
					cp = cp + vch[0];
					scs[j]->closingVelocity = scs[j]->closingVelocity + scs[j]->toLocal(cp);
					scs[j]->updateVelChange(dt);
				}
				if(worst->s2)
				{
					if(scs[j]->s1 == worst->s2)
					{
						cp = Vector2D();//rch[1] ^ scs[j]->s1->toLocal(scs[j]->absoluteContactPoint);
						cp = cp + vch[1];
						scs[j]->closingVelocity = scs[j]->closingVelocity - scs[j]->toLocal(cp);
						scs[j]->updateVelChange(dt);
					}
					else if(scs[j]->s2 == worst->s2)
					{
						cp = Vector2D();//rch[1] ^ scs[j]->s2->toLocal(scs[j]->absoluteContactPoint);
						cp = cp + vch[1];
						scs[j]->closingVelocity = scs[j]->closingVelocity - scs[j]->toLocal(cp);
						scs[j]->updateVelChange(dt);
					}
				}
			}
		}
		else break;
	}
}

void ImpulseSolver::applyVelocityChange(Contact *c,Vector2D *rch,Vector2D *vch)
{
	Vector2D impulse = c->toGlobal(Vector2D(c->desiredVelocityChange/c->dvel,0,0));
	vch[0] = (impulse) * c->s1->getParent()->getInvM();
	rch[0] = ((impulse ^ c->s1->toLocal(c->absoluteContactPoint)) * c->s1->getParent()->getInvI()); 
	c->s1->getParent()->addV(vch[0]);
	//c->s1->getParent()->addRV(rch[0].getZ());
	if(c->s2)
	{
		vch[1] = impulse * (- c->s2->getParent()->getInvM());
		rch[1] = (impulse ^ c->s2->toLocal(c->absoluteContactPoint)) * -(c->s2->getParent()->getInvI()); 
		c->s2->getParent()->addV(vch[1]);
		//c->s2->getParent()->addRV(rch[1].getZ());
	}
}

