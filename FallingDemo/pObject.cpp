#include "stdafx.h"
#include "pObject.h"

#define SCALE 1.0f

float pObject::poly = 0;
float pObject::polx = 0;
int pObject::icall = 0;

pObject::pObject(Point2D *pts,int n,bool iscircle, World &w, ObjectType cir,Point2D center)  : pts(pts), support(0), diskcenter(center)
{
	otype = cir;
	
	nb = n;
	drawLimit = nb;
	switch (otype)
	{			 
	case pObject::O_POLY:
		rb = RigidBody::build_polygonalBody(
			pts,
			n,
			iscircle,
			10.f,
			Vector2D(),
			0.f
			);
		u = rb->getPos();
		for(int i=0; i<n; i++)
			pts[i] -= u;
		p = (Polygon2D*)(rb->getShape());
		w.addObject(rb);
		break;
	case pObject::O_CIRCLE:
		rb = RigidBody::build_circularBody(
			center, 
			nb, 
			iscircle,
			nb,
			Vector2D(center),
			0.f
			);
		d = (Disk *)(rb->getShape());
		w.addObject(rb);			
		break;
	case pObject::O_PLANE:
		rb = RigidBody::build_planarBody(center, Vector2D(center, pts[0]));
		w.addObject(rb);
		break;
	}

	srand(time(0));
	if(rb->isFixed())
	{
		r = 180;
		g = 180;
		b = 180;
	}
	else
	{
		r = rand()%176 + 80; 
		g = rand()%176 + 80;
		b = rand()%176 + 80;
	}
}

void pObject::toogleFixed()
{
    rb->setFixed(!rb->isFixed());
    srand(time(0));
    if(rb->isFixed())
    {
	r = 180;
	g = 180;
	b = 180;
    }
    else
    {
	r = rand()%176 + 80; 
	g = rand()%176 + 80;
	b = rand()%176 + 80;
    }
}

void pObject::drawp(sf::RenderWindow &rw, Poly *pol)
{
	DoubleLinkedList<Point *> *curr = pol->pts;
	srand(icall);
	int rcomp = rand()%250;	
	int gcomp = rand()%250;	
	int bcomp = rand()%250;
	sf::Shape ppol;
	do
	{
		ppol.AddPoint(
			curr->getValue()->pt.getX() + polx, 
			curr->getValue()->pt.getY() + poly,
			sf::Color(rcomp, gcomp, bcomp)
			);
		curr = curr->getnext();
	} while(curr != pol->pts);
	rw.Draw(ppol);
	icall+= 10;
}

void pObject::draw(const MachineState &ms)
{		
	sf::Shape sh;
	Vector2D pos;
	icall = 0;
	poly = rb->getPos().getY();
	polx = rb->getPos().getX();
	/*t->parcgraph(rw, pObject::drawp);*/
	switch(otype)
	{					
	case pObject::O_CIRCLE:
		pos = rb->getPos();
		sh = sf::Shape::Circle(
			pos.getX()*SCALE, 
			pos.getY()*SCALE, 
			nb, 
			sf::Color(r,g,b), 
			(ms.drawstate & MachineState::DRAW_BORDERS) ? 3.f : 0.f, 
			sf::Color(r - 70, g - 70,b - 70)
			);
		ms.rwin.Draw(sh);
		ms.rwin.Draw(
			sf::Shape::Circle(
			pos.getX(),
			pos.getY(),
			3, sf::Color(r - 70, g - 70, b - 70), 2
			)
			);
		break;
	case pObject::O_POLY:
		if(p->nbrSubShapes)
		{
			if(!rb->isFixed() || (ms.drawstate & MachineState::DRAW_COMPONENTS))
			{
				for(int j = 0; j < p->nbrSubShapes;j++)
				{
					sf::Shape sh;
					int rrand;
					int grand;
					int brand;
					if(ms.drawstate & MachineState::DRAW_COMPONENTS)
					{
						srand(r+j);
						rrand = rand()%255;
						grand = rand()%255;
						brand = rand()%255;
					}
					else
					{
						rrand = r;
						grand = g;
						brand = b;
					}
					for(int i = 0; i < p->subShapes[j]->nbrPts; i++)	
					{																	  
						Point2D ptsi = p->toGlobal(p->subShapes[j]->pts[i]);
						sh.AddPoint(
							ptsi.getX()*SCALE,
							ptsi.getY()*SCALE, 
							sf::Color(rrand,grand,brand)
							);
					}
					ms.rwin.Draw(sh);
				}
			}
			else
			{
				for(int j = 0; j < p->nbrSubShapes;j++)
				{
					sf::Shape sh;
					for(int i = 0; i < p->subShapes[j]->nbrPts; i++)	
					{																	  
						Point2D ptsi = p->toGlobal(p->subShapes[j]->pts[i]);
						sh.AddPoint(
							ptsi.getX()*SCALE,
							ptsi.getY()*SCALE, 
							sf::Color(r,g,b)
							);
					}
					ms.rwin.Draw(sh);
				}
			}
		}  
		{
			unsigned int j = nb-1;
			if(ms.drawstate & MachineState::DRAW_BORDERS)
			{

				for(unsigned int i = 0; i < nb; j = i,i++)
				{
					Point2D vptsi = p->toGlobal(pts[j]);   
					Point2D vptsi1 = p->toGlobal(pts[i]);
					ms.rwin.Draw(
						sf::Shape::Line(
						vptsi.getX()*SCALE, 
						vptsi.getY()*SCALE, 
						vptsi1.getX()*SCALE, 
						vptsi1.getY()*SCALE, 
						3.f, 
						sf::Color(r - 70, g - 70,b - 70)
						)
						);
				}		  
			}
		}
		ms.rwin.Draw(
			sf::Shape::Circle(
			p->toGlobal(Point2D(0,0)).getX(), 
			p->toGlobal(Point2D(0,0)).getY(),
			3, sf::Color(r - 70, g - 70, b - 70),2
			)
			);
		break;
	case pObject::O_PLANE:
		break;
	}
}
