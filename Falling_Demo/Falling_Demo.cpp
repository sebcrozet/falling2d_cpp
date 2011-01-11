// Falling_Demo.cpp : définit le point d'entrée pour l'application console.
//

#include "stdafx.h"
#include "affichage.h"
#include "stdlib.h"

World ca;
int timerc;

void testPairsManager()
{
	PairManager pm;
	srand(time(0));
	std::vector<Pair> ps;
	//printf("test0"); 	
	// add random pairs
	for(int i = 0; i < 1009; i++)
		ps.push_back(*pm.addPair(rand(), rand()));
	for(int i = 0; i < 1009; i++)
	{
		Pair * fp = pm.findPair(ps[i].id1, ps[i].id2);
		if(fp->id1 != ps[i].id1 || fp->id2 != ps[i].id2)
			printf("test"); 	
	}				 
	for(int i = 0; i < 1009; i++)
	{
		pm.removePair(ps[i].id1, ps[i].id2);
		Pair * fp = pm.findPair(ps[i].id1, ps[i].id2);
		if(fp != 0)
			printf("test111"); 	
		for(int a = i + 1; a < 1009; a++)
		{
			Pair * fp = pm.findPair(ps[a].id1, ps[a].id2);
			if(fp == 0 || ps[a].id1 != ps[a].id1 || fp->id2 != ps[a].id2)
				printf("test"); 	
		}	
	}
}

int _tmain(int argc, _TCHAR* argv[])
{
	//testPairsManager();
	initSDL();
	return 0;
}





void initSDL()
{			 
	std::vector<Point2D> vpts;
	std::vector<pObject *> ps;
	Vector2D sp = Vector2D(0,1);
	GeometryHelper::Transformation2D t;
	Point2D *pts;
	bool trace = false;
	bool slidemode = false;
	SDL_Surface *screen;
	SDL_Event e;
	int v=1; 
    int depth = 0, depth2 = 0;
	SDL_Init(SDL_INIT_VIDEO);
	screen = SDL_SetVideoMode(1000, 1000, 32, SDL_HWSURFACE | SDL_RESIZABLE | SDL_DOUBLEBUF);
	timerc = SDL_GetTicks();
	while(v)
	{
		if(!SDL_PollEvent(&e))
		{
			update(screen,ps,depth,depth2,sp);
			continue;
		}
		switch(e.type)
		{
			case SDL_QUIT:
				v = 0;
				break;
			case SDL_KEYDOWN:
				if(e.key.keysym.sym == SDLK_s)
					slidemode = !slidemode;
				else if(e.key.keysym.sym == SDLK_t)
				{	
					int n = vpts.size();
					pts = new Point2D[n];
					for(int i=0; i<n; i++)
					pts[i] = vpts[i];
					ps.push_back(new pObject(pts,n,false, ca,false));
					vpts.clear();					  
					update(screen, ps,depth,depth2,sp);
				}
				else if(e.key.keysym.sym == SDLK_c)
				{
					ps.clear();
					update(screen,ps,depth,depth2,sp);
				}	   
				else if(e.key.keysym.sym == SDLK_n)
				{
					depth++;		
					update(screen,ps,depth,depth2,sp);
				}
				else if(e.key.keysym.sym == SDLK_d)
				{
					depth--;  
					update(screen,ps,depth,depth2,sp);
				}
				else if(e.key.keysym.sym == SDLK_m)
				{
					depth=0;   
					update(screen,ps,depth,depth2,sp);
				}
				else if(e.key.keysym.sym == SDLK_q)
				{
					depth2++;		
					update(screen,ps,depth,depth2,sp);
				}
				else if(e.key.keysym.sym == SDLK_w)
				{
					depth2--;  
					update(screen,ps,depth,depth2,sp);
				}
				else if(e.key.keysym.sym == SDLK_e)
				{
					depth2=0;   
					update(screen,ps,depth,depth2,sp);
				}
				break;
			case SDL_MOUSEBUTTONDOWN:
				if(!slidemode)
				{
					trace = true;
					vpts.push_back(Point2D(e.button.x,e.button.y));
				}
				if(slidemode)
				{				   
					ps.push_back(new pObject(0,100,true,ca, true,Point2D(e.button.x,e.button.y)));
					update(screen, ps,depth, depth2,sp);
				} 
				break;
			case SDL_MOUSEMOTION:
				if(trace && !slidemode )
				{
					vpts.push_back(Point2D(e.motion.x,e.motion.y));	
				}
				sp = Vector2D(e.motion.x,e.motion.y);  
				//update(screen,ps,depth,depth2,sp);
				break;
			case SDL_MOUSEBUTTONUP:
				if(trace)
				{
					trace = false;
					if(!slidemode)
					{
						int n = vpts.size();
						pts = new Point2D[n];
						for(int i=0; i<n; i++)
							pts[i] = vpts[i];
						if(n>3 || e.button.button == SDL_BUTTON_MIDDLE)
							ps.push_back(new pObject(pts,n,e.button.button == SDL_BUTTON_LEFT, ca,e.button.button == SDL_BUTTON_MIDDLE));
						vpts.clear();					  
						update(screen, ps,depth, depth2,sp);
					}
				}
				break;
		}
	}
	SDL_Quit();
}

void exploreOBBtree(SDL_Surface *screen,OBBtree *t, Vector2D v, int deepness, int curr)
{
	if(t==0)
		return;
	float ux = v.getX();
	float uy = v.getY();
	if(curr == deepness)
	{		 
		lineRGBA(screen, ux + t->o->pts[0].getX(), uy + t->o->pts[0].getY(), ux + t->o->pts[1].getX(), uy + t->o->pts[1].getY(),0,0,0,125);   
		lineRGBA(screen, ux + t->o->pts[1].getX(), uy + t->o->pts[1].getY(), ux + t->o->pts[2].getX(), uy + t->o->pts[2].getY(),0,0,0,125);
		lineRGBA(screen, ux + t->o->pts[2].getX(), uy + t->o->pts[2].getY(), ux + t->o->pts[3].getX(), uy + t->o->pts[3].getY(),0,0,0,125);
		lineRGBA(screen, ux + t->o->pts[3].getX(), uy + t->o->pts[3].getY(), ux + t->o->pts[0].getX(), uy + t->o->pts[0].getY(),0,0,0,125);
	}
	else if(curr < deepness && t->l == 0 && t->r == 0)
	{
		lineRGBA(screen, ux + t->o->pts[0].getX(), uy + t->o->pts[0].getY(), ux + t->o->pts[1].getX(), uy + t->o->pts[1].getY(),255,0,0,125);   
		lineRGBA(screen, ux + t->o->pts[1].getX(), uy + t->o->pts[1].getY(), ux + t->o->pts[2].getX(), uy + t->o->pts[2].getY(),255,0,0,125);
		lineRGBA(screen, ux + t->o->pts[2].getX(), uy + t->o->pts[2].getY(), ux + t->o->pts[3].getX(), uy + t->o->pts[3].getY(),255,0,0,125);
		lineRGBA(screen, ux + t->o->pts[3].getX(), uy + t->o->pts[3].getY(), ux + t->o->pts[0].getX(), uy + t->o->pts[0].getY(),255,0,0,125);

	}
	else
	{
		exploreOBBtree(screen,t->r, v, deepness, curr+1);
		exploreOBBtree(screen,t->l, v, deepness, curr+1);
	}
}

void update(SDL_Surface *screen, std::vector<pObject *> ps,int depth,int depth2,Vector2D &sp)
{
	SDL_FillRect(screen,&screen->clip_rect,0xffffffff);
	if(ps.size())
	{
	pObject *pob;
	pob = ps[ps.size()-1];
	/*
		if(!pob->isdisk && !pob->p->isFixed())
		{		   
			pob->p->translate(Vector2D(0,2));
			pob->p->rotate(0.02);
		}
		*/
	}

	for(int po = 0 ; po < ps.size();po++)
	{
		pObject *pob;
		pob = ps[po];
		if(!pob->isdisk)
		{		   
			Polygon2D *p = pob->p;
			int n;
			int i;
			int j;
			Vector2D u;
			float ux, uy;

			   
			n = pob->p->nbrPts;
			i = 0;
			j = n - 1;
			u = pob->u;
			ux = u.getX();
			uy = u.getY();
			while(i<n)
			{
				Point2D ipt = (pob->p->toGlobal(pob->p->points[i]));
				Point2D jpt = (pob->p->toGlobal(pob->p->points[j]));
				if( pob->p->getStackLevel() == 1)
				lineRGBA(screen, ipt.getX() , ipt.getY(), jpt.getX(), jpt.getY(), pob->rb->isSleeping()?0:255,pob->rb->isSleeping()?0:0, pob->rb->isSleeping()?0: 0,255);
				else if ( pob->p->getStackLevel() == 2)
				lineRGBA(screen, ipt.getX() , ipt.getY(), jpt.getX(), jpt.getY(), pob->rb->isSleeping()?0:0,pob->rb->isSleeping()?0:255, pob->rb->isSleeping()?0: 0,255);
				else if ( pob->p->getStackLevel() == 3)
				lineRGBA(screen, ipt.getX() , ipt.getY(), jpt.getX(), jpt.getY(), pob->rb->isSleeping()?0:0,pob->rb->isSleeping()?0:0, pob->rb->isSleeping()?0: 255,255);
				else
				lineRGBA(screen, ipt.getX() , ipt.getY(), jpt.getX(), jpt.getY(), pob->rb->isSleeping()?0:100 + pob->p->getStackLevel() * 10,pob->rb->isSleeping()?0: + pob->p->getStackLevel() * 10, pob->rb->isSleeping()?0: + pob->p->getStackLevel() * 10,255);
				j = i;
				i++;
			}
			/*lineRGBA(screen, pob->p->aabb_xm, pob->p->aabb_ym, pob->p->aabb_xm, pob->p->aabb_yM, pob->r + 100,pob->g + 100, pob->b + 100,255);
			lineRGBA(screen, pob->p->aabb_xm, pob->p->aabb_ym, pob->p->aabb_xM, pob->p->aabb_ym, pob->r + 100,pob->g + 100, pob->b + 100,255);
			lineRGBA(screen, pob->p->aabb_xm, pob->p->aabb_yM, pob->p->aabb_xM, pob->p->aabb_yM, pob->r + 100,pob->g + 100, pob->b + 100,255);
			lineRGBA(screen, pob->p->aabb_xM, pob->p->aabb_yM, pob->p->aabb_xM, pob->p->aabb_ym, pob->r + 100,pob->g + 100, pob->b + 100,255);
			//*/
			
			for(int nsn=0;nsn<p->nbrSubShapes && nsn<depth;nsn++)
			{
				ImplicitPolygon2D * ip = p->subShapes[nsn];
				n = ip->nbrPts;
				i = 0;
				j = n - 1;
				u = pob->u;
				ux = u.getX(); uy = u.getY();
				while(i<n)
				{		
					Point2D ipt = (ip->toGlobal(ip->pts[i]));
					Point2D jpt = (ip->toGlobal(ip->pts[j]));
					lineRGBA(screen,ipt.getX(), ipt.getY(), jpt.getX(), jpt.getY(), pob->rb->isSleeping()?0:pob->r + 50,pob->rb->isSleeping()?0:pob->g + 50, pob->rb->isSleeping()?0:pob->b + 50,255);
					j = i;
					i++;
				} 
				if(depth2)
				{
					exploreOBBtree(screen,pob->p->getOtree(),pob->u,depth2,10);
					lineRGBA(screen, ip->toGlobal(ip->getOBB()->pts[0]).getX(), ip->toGlobal(ip->getOBB()->pts[0]).getY(), ip->toGlobal(ip->getOBB()->pts[1]).getX(), ip->toGlobal(ip->getOBB()->pts[1]).getY(),0,0,0,125);   
					lineRGBA(screen, ip->toGlobal(ip->getOBB()->pts[1]).getX(), ip->toGlobal(ip->getOBB()->pts[1]).getY(), ip->toGlobal(ip->getOBB()->pts[2]).getX(), ip->toGlobal(ip->getOBB()->pts[2]).getY(),0,0,0,125);
					lineRGBA(screen, ip->toGlobal(ip->getOBB()->pts[2]).getX(), ip->toGlobal(ip->getOBB()->pts[2]).getY(), ip->toGlobal(ip->getOBB()->pts[3]).getX(), ip->toGlobal(ip->getOBB()->pts[3]).getY(),0,0,0,125);
					lineRGBA(screen, ip->toGlobal(ip->getOBB()->pts[3]).getX(), ip->toGlobal(ip->getOBB()->pts[3]).getY(), ip->toGlobal(ip->getOBB()->pts[0]).getX(), ip->toGlobal(ip->getOBB()->pts[0]).getY(),0,0,0,125);  		
				}
			}

			
		}
		else
		{
			if(pob->d->getStackLevel() == 1)
			circleRGBA(screen, pob->d->getCenter().getX(), pob->d->getCenter().getY(), pob->nb,pob->rb->isSleeping()?0: 255,pob->rb->isSleeping()?0: 0, pob->rb->isSleeping()?0: 0, 255);
			else if(pob->d->getStackLevel() == 2)
			circleRGBA(screen, pob->d->getCenter().getX(), pob->d->getCenter().getY(), pob->nb,pob->rb->isSleeping()?0: 0,pob->rb->isSleeping()?0: 255, pob->rb->isSleeping()?0: 0, 255);
			else if(pob->d->getStackLevel() == 3)
			circleRGBA(screen, pob->d->getCenter().getX(), pob->d->getCenter().getY(), pob->nb,pob->rb->isSleeping()?0: 0,pob->rb->isSleeping()?0: 0, pob->rb->isSleeping()?0: 255, 255);
			else if(pob->d->getStackLevel() > 3)
			circleRGBA(screen, pob->d->getCenter().getX(), pob->d->getCenter().getY(), pob->nb,pob->rb->isSleeping()?0: 0,pob->rb->isSleeping()?0: 255, pob->rb->isSleeping()?0: 255, 255);
			else
			circleRGBA(screen, pob->d->getCenter().getX(), pob->d->getCenter().getY(), pob->nb,pob->rb->isSleeping()?0: (pob->d->getStackLevel() * 50+10)%256,pob->rb->isSleeping()?0: (pob->d->getStackLevel()+10)%256, pob->rb->isSleeping()?0: (pob->d->getStackLevel() * 100+10)%256, 255);
		}
	}
	//std::vector<Collision *> cols;
	//cols = ca.solve(0.016f);
	int ttt = SDL_GetTicks();
	std::vector<Collision *> cols;
	cols = ca.solve(0.016f);//(float(ttt-timerc))/1000.);
	timerc = SDL_GetTicks();
	printf("took %i\n",timerc-ttt);
	SDL_Delay((16-(timerc-ttt)<0)?0:16-(timerc-ttt));
	for(int i=0; i<cols.size();i++)
	{
		for(int j = 0; j<cols[i]->c.size(); j++)
		{
			lineRGBA(screen, cols[i]->c[j].ptA.getX(), cols[i]->c[j].ptA.getY(), cols[i]->c[j].ptB.getX(), cols[i]->c[j].ptB.getY(), 0,0, 0,255);

		}
	}
	SDL_Flip(screen);
}