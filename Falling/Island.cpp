
#include "stdafx.h"
#include "Island.h"

Island::Island()
{ }

void Island::pushToLevelOneChain(Collision *next)
{
	if(stackLevels.empty())
	{
		next->nextlvlptr = next;
		next->prevlvlptr = next;
		stackLevels.push(next);
	}
	else
		next->insertInLevel(stackLevels.front());
}

void Island::calculateStackLevels()
{
	// Make a breadth first research
	Collision *levelHead = 0;
	Collision *levelLessOneHead = stackLevels.front(); // previous level'head
	int currLevel = 2;
	graphNodes.push(0); // push level-1 mark
	int iter = 0;
	do
	{
		iter ++;
		Shape *sh = graphNodes.front();
		graphNodes.pop();
		// see if a new level is detected
		if(!sh)
		{
			if(!graphNodes.empty())
			{
				currLevel++;
				graphNodes.push(0); // insert a new mark
				sh = graphNodes.front();
				graphNodes.pop();
				levelLessOneHead = levelHead;
				levelHead = 0;
				iter = 0;
			}
			else
				break; // end reached... leave!
		}
		Collision *next = sh->getCollisionList();
		next = next->nexta;
		while(next->sa != next->sb)
		{
			if(next->collisionStackLevel < 0)
			{
				if(next->sa == sh)
				{
					if(next->c.size())
					{
						if(next->sb->getStackLevel() < 0)
						{
							next->sb->setStackLevel(sh->getStackLevel() + 1);
							graphNodes.push(next->sb);
						}
						next->collisionStackLevel = (next->sb->getStackLevel() + next->sa->getStackLevel() + 1) / 2;
						if(next->collisionStackLevel == currLevel)
						{
							if(levelHead == 0)
							{
								next->nextlvlptr = next; // make it circular
								next->prevlvlptr = next;
								levelHead = next;
								stackLevels.push(levelHead); // push new graph level
							}
							else
								next->insertInLevel(levelHead);
						}
						else // insert in last level (levelLessOne must be != 0)
							next->insertInLevel(levelLessOneHead);
					}
					next = next->nexta;
				}
				else
				{
					if(next->c.size())
					{
						if(next->sa->getStackLevel() < 0)
						{
							next->sa->setStackLevel(sh->getStackLevel() + 1);
							graphNodes.push(next->sa);
						}
						next->collisionStackLevel = (next->sb->getStackLevel() + next->sa->getStackLevel() + 1) / 2;
						if(next->collisionStackLevel == currLevel)
						{
								if(levelHead == 0)
								{
									next->nextlvlptr = next; // make it circular
									next->prevlvlptr = next;
									levelHead = next;
									stackLevels.push(levelHead); // push new graph level
								}
								else
									next->insertInLevel(levelHead);
						}
						else // insert in last level (levelLessOne must be != 0)
							next->insertInLevel(levelLessOneHead);
					}
					next = next->nextb;
				}
			}
			else
			{
				if(next->sa == sh)
					next = next->nexta;
				else
					next = next->nextb;
			}
		}
	}
	while(true); // no test here => mark mechanism will detect the end of file
}

void Island::batchIsland(Island *isl,Shape *coll) // coll must not be fixed
{
	Collision *next;
	bool insertToOneLevel = false;
	next = coll->getCollisionList();
	//if(next->sa == coll) ==> always true in the first sentinel
	next = next->nexta;
	while(next->sa != next->sb) // second sentinel reached if ==
	{
		if(next->sa == coll)
		{
			// go on next node
			if(next->c.size())
			{
				if(next->sb->isFixed())
				{
					insertToOneLevel = true;
					next->sb->setStackLevel(0); // fixed objects are at level 0
					isl->pushToLevelOneChain(next);	
					next->collisionStackLevel = 1;
				}
				else
				{
					if(next->sb->getStackLevel() == -1)
					{
							next->sb->setStackLevel(-2); // another stack level invalid value (but != -1)
							batchIsland(isl,next->sb);
					}
					next->collisionStackLevel = -2;
				}
			}
			// go to next edge
			next = next->nexta;
		}
		else
		{
			// go on next node
			if(next->c.size())
			{
				if(next->sa->isFixed())
				{
					insertToOneLevel = true;
					next->sa->setStackLevel(0); // fixed objects are at level 0
					isl->pushToLevelOneChain(next);
					next->collisionStackLevel = 1;
				}
				else
				{
					if(next->sa->getStackLevel() == -1)
					{
							next->sa->setStackLevel(-2); // another stack level invalid value (but != -1)
							batchIsland(isl,next->sa);
					}
					next->collisionStackLevel = -2;
				}
			}
			// go to next edge
			next = next->nextb;
		}
	}
	if(insertToOneLevel)
	{
		coll->setStackLevel(1);
		// insert as base level
		isl->insertToLevelOne(coll);
	}
}

void Island::batchIslands(std::vector<Collision*> &colls, std::stack<Island*> &islands)
{
	// Reinit
	for(unsigned int i=0; i<colls.size();i++)
	{
		Collision *c = colls[i];
		c->collisionStackLevel = -1;
		c->sa->setStackLevel(-1);
		c->sb->setStackLevel(-1);
	}
	// Make a depth first research
	for(unsigned int i=0;i<colls.size();i++)
	{
		if(colls[i]->collisionStackLevel == -1)
		{
			// new island detected
			Island *isl = new Island();
			// recursive call with non-fixed object
			if(colls[i]->sa->isFixed())
			{
				colls[i]->sb->setStackLevel(-2);
				batchIsland(isl,colls[i]->sb);
				// island will always contain one object
			}
			else
			{
				colls[i]->sa->setStackLevel(-2);
				batchIsland(isl,colls[i]->sa);
				if(isl->isEmpty()) // empty island => no fixed objects in the graph
				{
					Shape *coll= colls[i]->sa;
					coll->setStackLevel(0); // select one object as base
					isl->insertToLevelOne(coll);
					// Compute the first level
					Collision *next;
					next = coll->getCollisionList();
					next = next->nexta;
					while(next->sa != next->sb)
					{
						if(next->sa == coll)
						{
							if(next->c.size() > 0)
							{
								next->sb->setStackLevel(1);
								next->collisionStackLevel = 1;
								isl->pushToLevelOneChain(next);
							}
							next = next->nexta;
						}
						else
						{
							if(next->c.size() > 0)
							{
								next->collisionStackLevel = 1;
								next->sa->setStackLevel(1);
								isl->pushToLevelOneChain(next);
							}
							next = next->nextb;
						}
					}
				}
			}
			// push to islands stack
			islands.push(isl);
		}
	}
}