#include "stdafx.h"
#include "PenetrationSolver.h"

void PenetrationSolver::solve(Island *isl,bool implode)
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
				applyPositionChangePerLevel(worst,trchange,rchange,implode);
				// adjust other penetrations
				if(!worst->s2 || 
					(implode ? worst->s1->getStackLevel() <= worst->s2->getStackLevel()
					: worst->s1->getStackLevel() >= worst->s2->getStackLevel()))
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
						for(unsigned int j = 0;j < cl->c.size();j++)
						{
							// update and save worst contact
							if(scs[j]->s1 == worst->s1)
							{
								float m = (worst->normal*trchange[0])*scs[j]->normal + (Vector2D(0,0,rchange[0])^scs[j]->relContactPoint[0])*scs[j]->normal;
								scs[j]->setPenetration(scs[j]->getPenetration() - m);
							}
							else// if(scs[j]->s2 == worst->s1)
							{
								scs[j]->setPenetration(scs[j]->getPenetration() + (worst->normal*trchange[0])*scs[j]->normal);
								scs[j]->setPenetration(scs[j]->getPenetration() + (Vector2D(0,0,rchange[0])^scs[j]->relContactPoint[0])*scs[j]->normal);
							}
							if(scs[j]->getPenetration() > cl->worstPenetrationAmount)
							{
								cl->worstContact = scs[j];
								cl->worstPenetrationAmount = scs[j]->getPenetration();
							}
						}		   
						Collision *toRemove = cl;
						if(cl->sa == worst->s1)
							cl = cl->nexta;
						else
							cl = cl->nextb;	
						// TODO: find a better way to do that
						//if(toRemove == curr)
						//	toRemove->removeFromList(); // remove to avoid double correction
						// end TODO
					}
				}
				if(worst->s2 &&	(implode ? worst->s1->getStackLevel() >= worst->s2->getStackLevel()
										 : worst->s1->getStackLevel() <= worst->s2->getStackLevel()))
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
						for(unsigned int j = 0;j < cl->c.size();j++)
						{
							// update and save worst contact
							if(scs[j]->s1 == worst->s2)
							{
								float m = (worst->normal*trchange[1])*scs[j]->normal + (Vector2D(0,0,rchange[1])^scs[j]->relContactPoint[1])*scs[j]->normal;
								scs[j]->setPenetration(scs[j]->getPenetration() - m);
							}
							else// if(scs[j]->s2 == worst->s2)
							{
								float m = (worst->normal*trchange[1])*scs[j]->normal + (Vector2D(0,0,rchange[1])^scs[j]->relContactPoint[1])*scs[j]->normal;
								scs[j]->setPenetration(scs[j]->getPenetration() + (worst->normal*trchange[1])*scs[j]->normal);
								scs[j]->setPenetration(scs[j]->getPenetration() + (Vector2D(0,0,rchange[1])^scs[j]->relContactPoint[1])*scs[j]->normal);
							}
							if(scs[j]->getPenetration() > cl->worstPenetrationAmount)
							{
								cl->worstContact = scs[j];
								cl->worstPenetrationAmount = scs[j]->getPenetration();
							}
						}		
						Collision *toRemove = cl;
						if(cl->sa == worst->s2)
							cl = cl->nexta;
						else
							cl = cl->nextb;
						// TODO: find a better way to do that
						//if(toRemove == curr)
						//	toRemove->removeFromList(); // remove to avoid double correction
						// end TODO				   
					}	     
				}
				// reinsert in graph
				//curr->autoInsert();
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
	// TODO: Remove: Validity check
		for(unsigned int j=0;j<scs.size();j++)
		{
			if(!(scs[j]->getPenetration() == scs[j]->getPenetration()))
				scs[j]->setPenetration(0);
		}
	// END: Validity check					
	for(unsigned int id = 0;id < 2/*scs.size()*/;id++)
	{
		float trchange[2];
		float rchange[2];
		Contact *worst = 0;
		float worstP = 0.01;
		for(unsigned int j=0;j<scs.size();j++)
		{
			if(scs[j]->getPenetration() > worstP)
			{
				worstP = scs[j]->getPenetration();
				worst = scs[j];
			}
		}
		if(worst)
		{
			worst->awakeIfNeeded();
			applyPositionChange(worst,trchange,rchange);
			// adjust other penetrations
			for(unsigned int j=0;j<scs.size();j++)
			{
				if(scs[j]->s1 == worst->s1)
				{
					float m = (worst->normal*trchange[0])*scs[j]->normal + (Vector2D(0,0,rchange[0])^scs[j]->relContactPoint[0])*scs[j]->normal;
					scs[j]->setPenetration(scs[j]->getPenetration() - m);				   
					// TODO: Remove: Validity check
							if(!(scs[j]->getPenetration() == scs[j]->getPenetration()))
								scs[j]->setPenetration(0);
					// END: Validity check
				}
				else if(scs[j]->s2 == worst->s1)
				{
					scs[j]->setPenetration(scs[j]->getPenetration() + (worst->normal*trchange[0])*scs[j]->normal);
					scs[j]->setPenetration(scs[j]->getPenetration() + (Vector2D(0,0,rchange[0])^scs[j]->relContactPoint[0])*scs[j]->normal);  
					// TODO: Remove: Validity check
							if(!(scs[j]->getPenetration() == scs[j]->getPenetration()))
								scs[j]->setPenetration(0);
				}
				if(worst->s2)
				{
					if(scs[j]->s1 == worst->s2)
					{
						float m = (worst->normal*trchange[1])*scs[j]->normal + (Vector2D(0,0,rchange[1])^scs[j]->relContactPoint[1])*scs[j]->normal;
						scs[j]->setPenetration(scs[j]->getPenetration() - m);    
					// TODO: Remove: Validity check
							if(!(scs[j]->getPenetration() == scs[j]->getPenetration()))
								scs[j]->setPenetration(0);
					}
					else if(scs[j]->s2 == worst->s2)
					{
						float m = (worst->normal*trchange[1])*scs[j]->normal + (Vector2D(0,0,rchange[1])^scs[j]->relContactPoint[1])*scs[j]->normal;
						scs[j]->setPenetration(scs[j]->getPenetration() + (worst->normal*trchange[1])*scs[j]->normal);
						scs[j]->setPenetration(scs[j]->getPenetration() + (Vector2D(0,0,rchange[1])^scs[j]->relContactPoint[1])*scs[j]->normal);    
					// TODO: Remove: Validity check
							if(!(scs[j]->getPenetration() == scs[j]->getPenetration()))
								scs[j]->setPenetration(0);
					}
				}	  
					// TODO: Remove: Validity check
							if(!(scs[j]->getPenetration() == scs[j]->getPenetration()))
								scs[j]->setPenetration(0);
			}
		}
		else 
			break;
	}
	//PenetrationSolver::solveRelax(scs);
}

void PenetrationSolver::applyPositionChangePerLevel(Contact *c,float *ch,float *ah, bool implode)
{
	float limita;
	float pivot;
	float dch;
	bool lvlequ = !c->s2 || c->s1->getStackLevel() == c->s2->getStackLevel();
	if(!c->s2 || (implode ? c->s1->getStackLevel() <= c->s2->getStackLevel()
						  : c->s1->getStackLevel() >= c->s2->getStackLevel()))
	{
		float factor = (!lvlequ)?1.f/((c->linin[0] + c->angin[0])*c->totalInertia):1.f;
		ah[0] = c->getPenetration()*c->unitangmov[0]*factor;
		ch[0] = c->getPenetration()*c->unitlinmov[0]*factor;
		limita = c->s1->getParent()->getDeltaTeta();
		pivot = c->s1->getParent()->getConsumedDeltaTeta();
		dch = 0;
		if(limita < 0) 
		{
			if(ah[0] + pivot > 0)
			{
				dch = ((Vector2D(0,0,(ah[0] + pivot)) ^ c->relContactPoint[0])*c->normal);
				ah[0] = -pivot;
			}
			else if(ah[0] + pivot < limita)
			{	  
				dch = ((Vector2D(0,0,(ah[0] - limita + pivot)) ^ c->relContactPoint[0])*c->normal);
				ah[0] = limita - pivot;
			}
			//else ok
		}
		else // limita >= 0 // same thing
		{	   
			if(ah[0] + pivot < 0)
			{
				dch = ((Vector2D(0,0,(ah[0] + pivot)) ^ c->relContactPoint[0])*c->normal);
				ah[0] = -pivot;
			}
			else if(ah[0] + pivot > limita)
			{	  
				dch = ((Vector2D(0,0,(ah[0] - limita + pivot)) ^ c->relContactPoint[0])*c->normal);
				ah[0] = limita - pivot;
			}
			//else ok
		}
		c->s1->getParent()->addConsumedTeta(ah[0]);
		ch[0] += dch;
		c->s1->translate(c->normal*ch[0]);
		c->s1->rotate(-ah[0]);
	}
	if(c->s2 && (implode ? c->s1->getStackLevel() <= c->s2->getStackLevel()
						  : c->s1->getStackLevel() <= c->s2->getStackLevel()))
	{
		float factor = (!lvlequ)?1.f/((c->linin[1] + c->angin[1])*c->totalInertia):1.f;
		ch[1] = -c->getPenetration()*c->unitlinmov[1]*factor;
		ah[1] = -c->getPenetration()*c->unitangmov[1]*factor;
		
		limita = c->s2->getParent()->getDeltaTeta();
		pivot = c->s2->getParent()->getConsumedDeltaTeta();
		dch = 0;
		if(limita < 0) 
		{
			if(ah[1] + pivot > 0)
			{
				dch = ((Vector2D(0,0,(ah[1] + pivot)) ^ c->relContactPoint[1])*c->normal);
				ah[1] = -pivot;
			}
			else if(ah[1] + pivot < limita)
			{	  
				dch = ((Vector2D(0,0,(ah[1] - limita + pivot)) ^ c->relContactPoint[1])*c->normal);
				ah[1] = limita - pivot;
			}
			//else ok
		}
		else // limita >= 0 // same thing
		{	   
			if(ah[1] + pivot < 0)
			{
				dch = ((Vector2D(0,0,(ah[1] + pivot)) ^ c->relContactPoint[1])*c->normal);
				ah[1] = -pivot;
			}
			else if(ah[1] + pivot > limita)
			{	  
				dch = ((Vector2D(0,0,(ah[1] - limita + pivot)) ^ c->relContactPoint[1])*c->normal);
				ah[1] = limita - pivot;
			}
			//else ok
		}
		c->s2->getParent()->addConsumedTeta(ah[1]);
		ch[1] += dch;
		c->s2->translate(c->normal*ch[1]);
		c->s2->rotate(-ah[1]);
	}
}
void PenetrationSolver::applyPositionChange(Contact *c,float *ch,float *ah)
{
	ah[0] = c->getPenetration()*c->unitangmov[0];
	ch[0] = c->getPenetration()*c->unitlinmov[0];
	float limita = c->s1->getParent()->getDeltaTeta();
	float pivot = c->s1->getParent()->getConsumedDeltaTeta();
	float dch = 0;
	if(limita < 0) 
	{
		if(ah[0] + pivot > 0)
		{
			dch = ((Vector2D(0,0,(ah[0] + pivot)) ^ c->relContactPoint[0])*c->normal);
			ah[0] = -pivot;
		}
		else if(ah[0] + pivot < limita)
		{	  
			dch = ((Vector2D(0,0,(ah[0] - limita + pivot)) ^ c->relContactPoint[0])*c->normal);
			ah[0] = limita - pivot;
		}
		//else ok
	}
	else // limita >= 0 // same thing
	{	   
		if(ah[0] + pivot < 0)
		{
			dch = ((Vector2D(0,0,(ah[0] + pivot)) ^ c->relContactPoint[0])*c->normal);
			ah[0] = -pivot;
		}
		else if(ah[0] + pivot > limita)
		{	  
			dch = ((Vector2D(0,0,(ah[0] - limita + pivot)) ^ c->relContactPoint[0])*c->normal);
			ah[0] = limita - pivot;
		}
		//else ok
	}
	c->s1->getParent()->addConsumedTeta(ah[0]);
	ch[0] += dch;
	c->s1->translate(c->normal*ch[0]);
	c->s1->rotate(-ah[0]);
	if(c->s2)
	{
		ch[1] = -c->getPenetration()*c->unitlinmov[1];
		ah[1] = -c->getPenetration()*c->unitangmov[1];
		
		limita = c->s2->getParent()->getDeltaTeta();
		pivot = c->s2->getParent()->getConsumedDeltaTeta();
		dch = 0;
		if(limita < 0) 
		{
			if(ah[1] + pivot > 0)
			{
				dch = ((Vector2D(0,0,(ah[1] + pivot)) ^ c->relContactPoint[1])*c->normal);
				ah[1] = -pivot;
			}
			else if(ah[1] + pivot < limita)
			{	  
				dch = ((Vector2D(0,0,(ah[1] - limita + pivot)) ^ c->relContactPoint[1])*c->normal);
				ah[1] = limita - pivot;
			}
			//else ok
		}
		else // limita >= 0 // same thing
		{	   
			if(ah[1] + pivot < 0)
			{
				dch = ((Vector2D(0,0,(ah[1] + pivot)) ^ c->relContactPoint[1])*c->normal);
				ah[1] = -pivot;
			}
			else if(ah[1] + pivot > limita)
			{	  
				dch = ((Vector2D(0,0,(ah[1] - limita + pivot)) ^ c->relContactPoint[1])*c->normal);
				ah[1] = limita - pivot;
			}
			//else ok
		}
		c->s2->getParent()->addConsumedTeta(ah[1]);
		ch[1] += dch;
		c->s2->translate(c->normal*ch[1]);
		c->s2->rotate(-ah[1]);
	}
}

void PenetrationSolver::solveRelax(std::vector<Contact *> &scs)
{
	for(unsigned int id = 0;id < scs.size() * 10;id++)
	{
		float trchange[2];
		Contact *worst = 0;
		float worstP = 0.01;
		for(unsigned int i=0;i<scs.size();i++)
		{
			worst = scs[i];
			if(worst->getPenetration() > worstP)
			{
				worst->awakeIfNeeded();
				applyPositionChangeRelax(worst,trchange);
				// adjust other penetrations
				for(unsigned int j=0;j<scs.size();j++)
				{
					if(scs[j]->s1 == worst->s1)
					{
						scs[j]->setPenetration(scs[j]->getPenetration() - (worst->normal*trchange[0])*scs[j]->normal);
					}
					else if(scs[j]->s2 == worst->s1)
					{
						scs[j]->setPenetration(scs[j]->getPenetration() + (worst->normal*trchange[0])*scs[j]->normal);
					}
					if(worst->s2)
					{
						if(scs[j]->s1 == worst->s2)
						{
							scs[j]->setPenetration(scs[j]->getPenetration() - (worst->normal*trchange[1])*scs[j]->normal);
						}
						else if(scs[j]->s2 == worst->s2)
						{
							scs[j]->setPenetration(scs[j]->getPenetration() + (worst->normal*trchange[1])*scs[j]->normal);
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
	ch[0] = ((c->getPenetration())<0.5f?c->getPenetration():0.5f)*(c->s1->getParent()->getInvM()/*+c->lin1*/)*c->totalInertia;
	c->s1->translate(c->normal*ch[0]);
	if(c->s2)
	{
		ch[1] = ((c->getPenetration())<0.5f?-c->getPenetration():-0.5f)*(c->s2->getParent()->getInvM()/*+c->lin2*/)*c->totalInertia;
		c->s2->translate(c->normal*ch[1]);
	}
}