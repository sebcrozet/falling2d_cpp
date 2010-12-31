#include "stdafx.h"
#include "PenetrationSolver.h"

void PenetrationSolver::solve(std::vector<Contact *> scs)
{
	//PenetrationSolver::solveRelax(scs);
	for(int id = 0;id < scs.size() * 10;id++)
	{
		float trchange[2];
		Contact *worst = 0;
		float worstP = 0.01;
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
			worst->awakeIfNeeded();
			applyPositionChange(worst,trchange);
			if(worstP>30)
				worstP = worstP;
			// adjust other penetrations
			for(int j=0;j<scs.size();j++)
			{
				if(scs[j]->s1 == worst->s1)
				{
					scs[j]->penetration -= (worst->normal*trchange[0])*scs[j]->normal;
				}
				else if(scs[j]->s2 == worst->s1)
				{
					scs[j]->penetration += (worst->normal*trchange[0])*scs[j]->normal;
				}
				if(worst->s2)
				{
					if(scs[j]->s1 == worst->s2)
					{
						scs[j]->penetration -= (worst->normal*trchange[1])*scs[j]->normal;
					}
					else if(scs[j]->s2 == worst->s2)
					{
						scs[j]->penetration += (worst->normal*trchange[1])*scs[j]->normal;
					}
				}
			}
		}
		else break;
	}
	//PenetrationSolver::solveRelax(scs);
}

void PenetrationSolver::applyPositionChange(Contact *c,float *ch)
{
	ch[0] = c->penetration*(c->s1->getParent()->getInvM()/*+c->lin1*/)*c->totalInertia;
	c->s1->translate(c->normal*ch[0]);
	if(c->s2)
	{
		ch[1] = -c->penetration*(c->s2->getParent()->getInvM()/*+c->lin2*/)*c->totalInertia;
		c->s2->translate(c->normal*ch[1]);
	}
}

void PenetrationSolver::solveRelax(std::vector<Contact *> scs)
{
	for(int id = 0;id < scs.size() * 10;id++)
	{
		float trchange[2];
		Contact *worst = 0;
		float worstP = 0.01;
		for(int i=0;i<scs.size();i++)
		{
			worst = scs[i];
			if(worst->penetration > worstP)
			{
				worst->awakeIfNeeded();
				applyPositionChangeRelax(worst,trchange);
				// adjust other penetrations
				for(int j=0;j<scs.size();j++)
				{
					if(scs[j]->s1 == worst->s1)
					{
						scs[j]->penetration -= (worst->normal*trchange[0])*scs[j]->normal;
					}
					else if(scs[j]->s2 == worst->s1)
					{
						scs[j]->penetration += (worst->normal*trchange[0])*scs[j]->normal;
					}
					if(worst->s2)
					{
						if(scs[j]->s1 == worst->s2)
						{
							scs[j]->penetration -= (worst->normal*trchange[1])*scs[j]->normal;
						}
						else if(scs[j]->s2 == worst->s2)
						{
							scs[j]->penetration += (worst->normal*trchange[1])*scs[j]->normal;
						}
					}
				}
			}
			else break;
		}
	}
}


void PenetrationSolver::applyPositionChangeRelax(Contact *c,float *ch)
{
	ch[0] = ((c->penetration)<0.5f?c->penetration:0.5f)*(c->s1->getParent()->getInvM()/*+c->lin1*/)*c->totalInertia;
	c->s1->translate(c->normal*ch[0]);
	if(c->s2)
	{
		ch[1] = ((c->penetration)<0.5f?-c->penetration:-0.5f)*(c->s2->getParent()->getInvM()/*+c->lin2*/)*c->totalInertia;
		c->s2->translate(c->normal*ch[1]);
	}
}