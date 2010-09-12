// Falling_Demo.cpp : définit le point d'entrée pour l'application console.
//

#include "stdafx.h"
#include "affichage.h"
#include "stdlib.h"

CollisionArbiter ca;

void testPairsManager()
{
	PairManager pm;
	srand(time(0));
	std::vector<Pair> ps;
	printf("test0"); 	
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
	testPairsManager();
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
	update(screen,ps,depth,depth2,sp);	  
	while(v)
	{
		SDL_WaitEvent(&e);
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
					ps.push_back(new pObject(pts,n,false, ca));
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
				if(!slidemode && e.button.button == SDL_BUTTON_LEFT)
				{
					trace = true;
					vpts.push_back(Point2D(e.button.x,e.button.y));
				}
				if(slidemode)
				{				   
					ps.push_back(new pObject(0,100,true,ca, Point2D(e.button.x,e.button.y)));
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
						ps.push_back(new pObject(pts,n,false, ca));
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

			   
			n = pob->nb;
			i = 0;
			j = n - 1;
			u = pob->u;
			ux = u.getX();
			uy = u.getY();
			while(i<n)
			{
				lineRGBA(screen, pob->pts[i].getX() + ux, pob->pts[i].getY() + uy, pob->pts[j].getX() + ux, pob->pts[j].getY() + uy, pob->r + 100,pob->g + 100, pob->b + 100,255);
				j = i;
				i++;
			}
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
					lineRGBA(screen,ip->pts[i].getX() + ux, ip->pts[i].getY() + uy, ip->pts[j].getX() + ux, ip->pts[j].getY() + uy, pob->r + 50,pob->g + 50, pob->b + 50,255);
					j = i;
					i++;
				} 
				if(depth2)
				{
					lineRGBA(screen, ux + ip->getOBB()->pts[0].getX(), uy + ip->getOBB()->pts[0].getY(), ux + ip->getOBB()->pts[1].getX(), uy + ip->getOBB()->pts[1].getY(),0,0,0,125);   
					lineRGBA(screen, ux + ip->getOBB()->pts[1].getX(), uy + ip->getOBB()->pts[1].getY(), ux + ip->getOBB()->pts[2].getX(), uy + ip->getOBB()->pts[2].getY(),0,0,0,125);
					lineRGBA(screen, ux + ip->getOBB()->pts[2].getX(), uy + ip->getOBB()->pts[2].getY(), ux + ip->getOBB()->pts[3].getX(), uy + ip->getOBB()->pts[3].getY(),0,0,0,125);
					lineRGBA(screen, ux + ip->getOBB()->pts[3].getX(), uy + ip->getOBB()->pts[3].getY(), ux + ip->getOBB()->pts[0].getX(), uy + ip->getOBB()->pts[0].getY(),0,0,0,125);  		
				}
			}

			
		}
		else
		{
			circleRGBA(screen, pob->d->getCenter().getX(), pob->d->getCenter().getY(), pob->nb,pob->r,pob->g, pob->b, 255);
		}
	}
	std::vector<Collision *> cols;
	ca.solve(cols);
	for(int i=0; i<cols.size();i++)
	{
		for(int j = 0; j<cols[i]->c.size(); j++)
		{
			lineRGBA(screen, cols[i]->c[j].ptA.getX(), cols[i]->c[j].ptA.getY(), cols[i]->c[j].ptB.getX(), cols[i]->c[j].ptB.getY(), 0,0, 0,255);

		}
	}
	SDL_Flip(screen);
}



/*		

			Polygon2D *p;
			int n;
			int i;
			int j;
			Vector2D u;
			float ux, uy;
			  
			p = pob->p;
			
			n = p->nbrPts;
			i = 0;
			j = n - 1;
			u = pob->u;
			ux = u.getX();
			uy = u.getY();
			while(i<n)
			{
				lineRGBA(screen, p->points[i].getX() + ux, p->points[i].getY() + uy, p->points[j].getX() + ux, p->points[j].getY() + uy, pob->r,pob->g, pob->b,255);
				j = i;
				i++;
			}
			/* 
			for(int nsn=0;nsn<p->nbrSubShapes && nsn<depth;nsn++)
			{
				ImplicitPolygon2D * ip = p->subShapes[nsn];
				n = ip->nbrPts;
				i = 0;
				j = n - 1;
				u = p->t.getU();
				ux = u.getX(); uy = u.getY();
				while(i<n)
				{
					lineRGBA(screen,ip->pts[i].getX() + ux, ip->pts[i].getY() + uy, ip->pts[j].getX() + ux, ip->pts[j].getY() + uy, pob->r + 50,pob->g + 50, pob->b + 50,255);
					j = i;
					i++;
				}   		
				lineRGBA(screen, ux + ip->obb.pts[0].getX(), uy + ip->obb.pts[0].getY(), ux + ip->obb.pts[1].getX(), uy + ip->obb.pts[1].getY(),0,0,0,125);   
				lineRGBA(screen, ux + ip->obb.pts[1].getX(), uy + ip->obb.pts[1].getY(), ux + ip->obb.pts[2].getX(), uy + ip->obb.pts[2].getY(),0,0,0,125);
				lineRGBA(screen, ux + ip->obb.pts[2].getX(), uy + ip->obb.pts[2].getY(), ux + ip->obb.pts[3].getX(), uy + ip->obb.pts[3].getY(),0,0,0,125);
				lineRGBA(screen, ux + ip->obb.pts[3].getX(), uy + ip->obb.pts[3].getY(), ux + ip->obb.pts[0].getX(), uy + ip->obb.pts[0].getY(),0,0,0,125);

			}


			
			for(int nsn=0;nsn<p->nbrSubShapes2 && nsn<depth2;nsn++)
			{
				ImplicitPolygon2D * ip = p->subShapes2[nsn];
				n = ip->nbrPts;
				i = 0;
				j = n - 1;
				u = p->t.getU();
				ux = u.getX(); uy = u.getY();
				while(i<n)
				{
					lineRGBA(screen,ip->pts[i].getX() + ux, ip->pts[i].getY() + uy, ip->pts[j].getX() + ux, ip->pts[j].getY() + uy, pob->r + 50,pob->g + 50, pob->b + 50,255);
					j = i;
					i++;
				}	
				lineRGBA(screen, ux + ip->obb.pts[0].getX(), uy + ip->obb.pts[0].getY(), ux + ip->obb.pts[1].getX(), uy + ip->obb.pts[1].getY(),0,0,0,125);   
				lineRGBA(screen, ux + ip->obb.pts[1].getX(), uy + ip->obb.pts[1].getY(), ux + ip->obb.pts[2].getX(), uy + ip->obb.pts[2].getY(),0,0,0,125);
				lineRGBA(screen, ux + ip->obb.pts[2].getX(), uy + ip->obb.pts[2].getY(), ux + ip->obb.pts[3].getX(), uy + ip->obb.pts[3].getY(),0,0,0,125);
				lineRGBA(screen, ux + ip->obb.pts[3].getX(), uy + ip->obb.pts[3].getY(), ux + ip->obb.pts[0].getX(), uy + ip->obb.pts[0].getY(),0,0,0,125);

			}
			Point2D pp, pp0;				 
			u = pob->u;
			ux = u.getX(); uy = u.getY();
			pob->support = p->chull->getSupportPoint(sp - u,&pp);
			circleRGBA(screen,pp.getX(), pp.getY(), 3 , 255,0, 0,255);	   
			pp0 = pp;
			pp =p->chull->toGlobal(p->chull->pts[p->chull->naiveClimb(0,p->chull->nbrPts-1,sp - u)]);
			exploreOBBtree(screen,p->getOtree(), u,depth,0);																				  
			//lineRGBA(screen, ux + p->chull->obb.pts[0].getX(), uy + p->chull->obb.pts[0].getY(), ux + p->chull->obb.pts[1].getX(), uy + p->chull->obb.pts[1].getY(), pob->r + 50,pob->g + 50, pob->b + 50,125);   
			//lineRGBA(screen, ux + p->chull->obb.pts[1].getX(), uy + p->chull->obb.pts[1].getY(), ux + p->chull->obb.pts[2].getX(), uy + p->chull->obb.pts[2].getY(), pob->r + 50,pob->g + 50, pob->b + 50,125);
			//lineRGBA(screen, ux + p->chull->obb.pts[2].getX(), uy + p->chull->obb.pts[2].getY(), ux + p->chull->obb.pts[3].getX(), uy + p->chull->obb.pts[3].getY(), pob->r + 50,pob->g + 50, pob->b + 50,125);
			//lineRGBA(screen, ux + p->chull->obb.pts[3].getX(), uy + p->chull->obb.pts[3].getY(), ux + p->chull->obb.pts[0].getX(), uy + p->chull->obb.pts[0].getY(), pob->r + 50,pob->g + 50, pob->b + 50,125);
			//circleRGBA(screen,pp.getX(), pp.getY(), 3 , 0,0,0,255);
			float d1 =	Vector2D(pp).dot(sp-u), d2 = Vector2D(pp0).dot(sp-u);

			bool b = d1 != d2;
			//Vector2D aaa = p->chull->toLocal(sp);
			//u = p->chull->toGlobal(Vector2D(p->chull->rightTgtPt(Point2D(aaa.getX(), aaa.getY())))); 
			//ux = u.getX(); uy = u.getY();
			lineRGBA(screen, ux , uy, sp.getX(), sp.getY(), pob->r + 50,pob->g + 50, pob->b + 50,125);
			n = p->chull->nbrPts;
			i = 0;
			j = n - 1;
			u = pob->u;
			ux = u.getX(); uy = u.getY();
			while(i<n)
			{
				lineRGBA(screen, p->chull->pts[i].getX() + ux, p->chull->pts[i].getY() + uy, p->chull->pts[j].getX() + ux, p->chull->pts[j].getY() + uy, pob->r + 50,pob->g + 50, pob->b + 50,255);
				j = i;
				i++;
			}

















for(int pocl = po + 1 ; pocl<ps.size();pocl++)
		{
			pObject *pob2 = ps[pocl];
			ImplicitShape *s1,*s2;
			if(!pob->isdisk)
				s1 = pob->p->chull;
			else
				s1 = pob->d;
			if(!pob2->isdisk)
				s2 = pob2->p->chull;
			else
				s2 = pob2->d;
			GJKsolver ss(*s1, *s2);
			Point2D pcl1, pcl2;
			// float sizep=ss.getClosestPoints(&pcl1, &pcl2);
			float sizep=ss.getPenDepth(&pcl1, &pcl2);
			if(sizep>0)
			{
				lineRGBA(screen, pcl1.getX(), pcl1.getY(), pcl2.getX(), pcl2.getY(), 0,0, 0,255);  
				lineRGBA(screen, 10,10,10+ abs(sizep),10, 0,0, 0,255);  
			}
		}
//*/