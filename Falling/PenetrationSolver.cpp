#include "stdafx.h"
#include "PenetrationSolver.h"

void PenetrationSolver::solve(Island *isl)
{
	do
	{
		// next level
		Collision *curr = isl->stackLevels.front();
		// sort list
		curr = Collision::inPlaceSortList(curr);
		Collision *begining = curr;
		isl->stackLevels.pop();
		for(int id = 0;id < /*scs.size()**/50;id++)
		{
			float trchange[2];
			float rchange[2];
			Contact *worst = 0;
			worst = curr->worstContact;
			if(worst)
			{
				worst->awakeIfNeeded();
				applyPositionChange(worst,trchange,rchange);
				// adjust other penetrations
				if(worst->s1)
				{
					// Correct attached nodes
					// iterate through graph's edges:
					Collision * cl = worst->s1->getCollisionList();
					cl = cl->nexta; // skip sentinel
					while(cl->nexta != cl->nextb)	// while other sentinel not reached; do
					{
						Contact **scs = cl->cnts; // scs[] <==> Contact *[]
						cl->worstContact = 0;
						cl->worstPenetrationAmount = 0.01f;
						for(int j = 0;j < cl->c.size();j++)
						{
							// update and save worst contact
							if(scs[j]->s1 == worst->s1)
							{
								float m = (worst->normal*trchange[0])*scs[j]->normal + (Vector2D(0,0,rchange[0])^scs[j]->relContactPoint[0])*scs[j]->normal;
								scs[j]->penetration -= m;
							}
							else// if(scs[j]->s2 == worst->s1)
							{
								scs[j]->penetration += (worst->normal*trchange[0])*scs[j]->normal;
								scs[j]->penetration += (Vector2D(0,0,rchange[0])^scs[j]->relContactPoint[0])*scs[j]->normal;
							}
							if(scs[j]->penetration > cl->worstPenetrationAmount)
							{
								cl->worstContact = scs[j];
								cl->worstPenetrationAmount = scs[j]->penetration;
							}
						}
						if(cl->sa == worst->s1)
							cl = cl->nexta;
						else
							cl = cl->nextb;
					}
				}
				if(worst->s2)
				{
					// Correct attached nodes
					// iterate through graph's edges:
					Collision * cl = worst->s2->getCollisionList();	
					cl = cl->nexta; // skip sentinel
					while(cl->nexta != cl->nextb)	// while other sentinel not reached; do
					{
						Contact **scs = cl->cnts; // scs[] -> Contact *[]
						cl->worstContact = 0;
						cl->worstPenetrationAmount = 0.01f;
						for(int j = 0;j < cl->c.size();j++)
						{
							// update and save worst contact
							if(scs[j]->s1 == worst->s2)
							{
								float m = (worst->normal*trchange[1])*scs[j]->normal + (Vector2D(0,0,rchange[1])^scs[j]->relContactPoint[1])*scs[j]->normal;
								scs[j]->penetration -= m;
							}
							else// if(scs[j]->s2 == worst->s2)
							{
								float m = (worst->normal*trchange[1])*scs[j]->normal + (Vector2D(0,0,rchange[1])^scs[j]->relContactPoint[1])*scs[j]->normal;
								scs[j]->penetration += (worst->normal*trchange[1])*scs[j]->normal;
								scs[j]->penetration += (Vector2D(0,0,rchange[1])^scs[j]->relContactPoint[1])*scs[j]->normal;
							}
							if(scs[j]->penetration > cl->worstPenetrationAmount)
							{
								cl->worstContact = scs[j];
								cl->worstPenetrationAmount = scs[j]->penetration;
							}
						}
						if(cl->sa == worst->s2)
							cl = cl->nexta;
						else
							cl = cl->nextb;		   
					}
				}
				// sort list again
				curr = Collision::inPlaceSortList(begining);
				begining = curr;
				
				// TOTO: register which node has to be sorted (to avoid useless tests on already sorted elements)
				/*if(worst->s2)
				{
				}	*/
			}
			else break;
		}
	}
	while(!isl->stackLevels.empty()); 
}

void PenetrationSolver::solve(std::vector<Contact *> &scs)
{
	//PenetrationSolver::solveRelax(scs);
	for(int id = 0;id < scs.size()*10;id++)
	{
		float trchange[2];
		float rchange[2];
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
			applyPositionChange(worst,trchange,rchange);
			// adjust other penetrations
			for(int j=0;j<scs.size();j++)
			{
				if(scs[j]->s1 == worst->s1)
				{
					float m = (worst->normal*trchange[0])*scs[j]->normal + (Vector2D(0,0,rchange[0])^scs[j]->relContactPoint[0])*scs[j]->normal;
					scs[j]->penetration -= m;
				}
				else if(scs[j]->s2 == worst->s1)
				{
					scs[j]->penetration += (worst->normal*trchange[0])*scs[j]->normal;
					scs[j]->penetration += (Vector2D(0,0,rchange[0])^scs[j]->relContactPoint[0])*scs[j]->normal;
				}
				if(worst->s2)
				{
					if(scs[j]->s1 == worst->s2)
					{
						float m = (worst->normal*trchange[1])*scs[j]->normal + (Vector2D(0,0,rchange[1])^scs[j]->relContactPoint[1])*scs[j]->normal;
						scs[j]->penetration -= m;
					}
					else if(scs[j]->s2 == worst->s2)
					{
						float m = (worst->normal*trchange[1])*scs[j]->normal + (Vector2D(0,0,rchange[1])^scs[j]->relContactPoint[1])*scs[j]->normal;
						scs[j]->penetration += (worst->normal*trchange[1])*scs[j]->normal;
						scs[j]->penetration += (Vector2D(0,0,rchange[1])^scs[j]->relContactPoint[1])*scs[j]->normal;
					}
				}
			}
		}
		else break;
	}
	//PenetrationSolver::solveRelax(scs);
}

void PenetrationSolver::applyPositionChange(Contact *c,float *ch,float *ah)
{
	ch[0] = c->penetration*c->unitlinmov[0];
	ah[0] = c->penetration*c->unitangmov[0];
	c->s1->translate(c->normal*ch[0]);
	c->s1->rotate(-ah[0]);
	if(c->s2)
	{
		ch[1] = -c->penetration*c->unitlinmov[1];
		ah[1] = -c->penetration*c->unitangmov[1];
		c->s2->translate(c->normal*ch[1]);
		c->s2->rotate(-ah[1]);
	}
}

void PenetrationSolver::solveRelax(std::vector<Contact *> &scs)
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