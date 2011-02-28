#include "stdafx.h"
#include "ImpulseSolver.h"

void ImpulseSolver::solve(std::vector<Contact *> &scs,float dt)
{
	for(unsigned int i = 0;i < scs.size()*10;i++)
	{
		Vector2D rch[2];
		Vector2D vch[2];
		Contact *worst = 0;
		float worstV = 0.01f;
		for(unsigned int j=0;j<scs.size();j++)
		{
			if(scs[j]->getPenetration() > -0.001f && scs[j]->desiredVelocityChange > worstV)
			{   
				worstV = scs[j]->desiredVelocityChange;
				worst = scs[j];
			}
		}
		if(worst)
		{
			applyVelocityChange(worst,rch,vch);

			// adjust other velocities
			for(unsigned int j=0;j<scs.size();j++)
			{
				Vector2D cp;
				if(scs[j]->s1 == worst->s1)
				{
					cp = rch[0] ^ scs[j]->relContactPoint[0];
					cp = cp + vch[0];
					scs[j]->closingVelocity = scs[j]->closingVelocity + scs[j]->toLocal(cp);
					scs[j]->updateVelChange(dt);
				}
				else if(scs[j]->s2nfixed == worst->s1)
				{
					cp = rch[0] ^ scs[j]->relContactPoint[1];
					cp = cp + vch[0];
					scs[j]->closingVelocity = scs[j]->closingVelocity - scs[j]->toLocal(cp);
					scs[j]->updateVelChange(dt);
				}
				if(worst->s2nfixed)
				{
					if(scs[j]->s1 == worst->s2nfixed)
					{
						cp = rch[1] ^ scs[j]->relContactPoint[0];
						cp = cp + vch[1];
						scs[j]->closingVelocity = scs[j]->closingVelocity + scs[j]->toLocal(cp);
						scs[j]->updateVelChange(dt);
					}
					else if(scs[j]->s2nfixed == worst->s2nfixed)
					{
						cp = rch[1] ^ scs[j]->relContactPoint[1];
						cp = cp + vch[1];
						scs[j]->closingVelocity = scs[j]->closingVelocity - scs[j]->toLocal(cp);
						scs[j]->updateVelChange(dt);
					}
				}
			}
		}
		else break;
	}
	//solveRelax(scs,dt);
}

void ImpulseSolver::applyVelocityChange(Contact *c,Vector2D *rch,Vector2D *vch)
{
	Vector2D impulse = c->toGlobal(Vector2D(c->desiredVelocityChange/(c->dvel),-(0.5f*c->closingVelocity.getY())/(c->dvely),0));
	vch[0] = (impulse) * c->s1->getParent()->getInvM();
	rch[0] = ((c->relContactPoint[0] ^ impulse) * c->s1->getParent()->getInvI()); 
	c->s1->getParent()->addV(vch[0]);
	c->s1->getParent()->addRV(rch[0].getZ());
	if(c->s2nfixed)
	{
		vch[1] = impulse * (- c->s2nfixed->getParent()->getInvM());
		rch[1] = (c->relContactPoint[1] ^ impulse) * -(c->s2nfixed->getParent()->getInvI()); 
		c->s2nfixed->getParent()->addV(vch[1]);
		c->s2nfixed->getParent()->addRV(rch[1].getZ());
	}
}


void ImpulseSolver::solveRelax(std::vector<Contact *> &scs,float dt)
{
	for(unsigned int i = 0;i < scs.size() * 1000;i++)
	{
		Vector2D rch[2];
		Vector2D vch[2];
		Contact *worst = 0;
		float worstV = 0.01;
		for(unsigned int ji=0;ji<scs.size();ji++)
		{
				worst=scs[ji];
				if(worst->desiredVelocityChange<=worstV)
					continue;
				applyVelocityChangeRelax(worst,rch,vch);
				// adjust other velocities
				for(unsigned int j=0;j<scs.size();j++)
				{
					Vector2D cp;
					if(scs[j]->s1 == worst->s1)
					{
						cp = rch[0] ^ scs[j]->relContactPoint[0];
						cp = cp + vch[0];
						scs[j]->closingVelocity = scs[j]->closingVelocity + scs[j]->toLocal(cp);
						scs[j]->updateVelChange(dt);
					}
					else if(scs[j]->s2 == worst->s1)
					{
						cp = rch[0] ^ scs[j]->relContactPoint[1];
						cp = cp + vch[0];
						scs[j]->closingVelocity = scs[j]->closingVelocity - scs[j]->toLocal(cp);
						scs[j]->updateVelChange(dt);
					}
					if(worst->s2)
					{
						if(scs[j]->s1 == worst->s2)
						{
							cp = rch[1] ^ scs[j]->relContactPoint[0];
							cp = cp + vch[1];
							scs[j]->closingVelocity = scs[j]->closingVelocity + scs[j]->toLocal(cp);
							scs[j]->updateVelChange(dt);
						}
						else if(scs[j]->s2 == worst->s2)
						{
							cp = rch[1] ^ scs[j]->relContactPoint[1];
							cp = cp + vch[1];
							scs[j]->closingVelocity = scs[j]->closingVelocity - scs[j]->toLocal(cp);
							scs[j]->updateVelChange(dt);
						}
					}
		else break;
				}
		}
	}
}

void ImpulseSolver::applyVelocityChangeRelax(Contact *c,Vector2D *rch,Vector2D *vch)
{
	Vector2D impulse = c->toGlobal(Vector2D(((c->desiredVelocityChange)<0.5f?c->desiredVelocityChange:0.5f)/c->dvel,0,0));
	vch[0] = (impulse) * c->s1->getParent()->getInvM();
	rch[0] = ((c->relContactPoint[0] ^ impulse) * c->s1->getParent()->getInvI()); 
	c->s1->getParent()->addV(vch[0]);
	c->s1->getParent()->addRV(rch[0].getZ());
	if(c->s2)
	{
		vch[1] = impulse * (- c->s2->getParent()->getInvM());
		rch[1] = (c->relContactPoint[1] ^ impulse) * -(c->s2->getParent()->getInvI()); 
		c->s2->getParent()->addV(vch[1]);
		c->s2->getParent()->addRV(rch[1].getZ());
	}
}