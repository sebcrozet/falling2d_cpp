#include "stdafx.h"
#include "PenetrationSolver.h"

void PenetrationSolver::solve(std::vector<Contact *> scs)
{
	for(int i = 0;i < scs.size() * 10;i++)
	{
		float trchange[2];
		Contact *worst = 0;
		float worstP = 0.000000000001;
		for(int j=0;j<scs.size();j++)
		{
			if(scs[j]->penetration > worstP)
			{
				worstP = scs[j]->penetration;
				worst = scs[j];
			}
		}
		if(worst)
		{
			applyPositionChange(worst,trchange);

			// adjust other penetrations
			for(int j=0;j<scs.size();j++)
			{
				if(scs[j]->s1 == worst->s1)
				{
					scs[j]->penetration -= trchange[0];
				}
				else if(scs[j]->s2 == worst->s1)
				{
					scs[j]->penetration += trchange[0];
				}
				if(worst->s2)
				{
					if(scs[j]->s1 == worst->s2)
					{
						scs[j]->penetration -= trchange[1];
					}
					else if(scs[j]->s2 == worst->s2)
					{
						scs[j]->penetration += trchange[1];
					}
				}
			}
		}
		else break;
	}
}

void PenetrationSolver::applyPositionChange(Contact *c,float *ch)
{
	ch[0] = c->penetration*c->s1->getParent()->getInvM()*c->totalInertia;
	c->s1->translate(c->normal*ch[0]);
	if(c->s2)
	{
		ch[1] = (-c->penetration)*c->s2->getParent()->getInvM()*c->totalInertia;
		c->s2->translate(c->normal*ch[1]);
	}
}