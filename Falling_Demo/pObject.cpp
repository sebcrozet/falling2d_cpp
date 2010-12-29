#include "affichage.h"

pObject::pObject(Point2D *pts,int n,bool iscircle, World &w, Point2D center)  : pts(pts), support(0)
{
	isdisk = false;//iscircle;
	
	nb = n;
	if(true)
	{
		u = Vector2D(Polygon2D::getCentroid(pts,n));
		for(int i=0; i<n; i++)
			pts[i] -= u;
		rb = RigidBody::build_polygonalBody(pts,n,iscircle,10,u,0);
		p = (Polygon2D*)(rb->getShape());
		w.addObject(rb);
	}
	else
	{
		
		nb = rand() % 250 + 10;
		rb = RigidBody::build_circularBody(center, nb, false,0,Vector2D(center),0);
		d = (Disk *)(rb->getShape());
		w.addObject(rb);
	}	  
	srand(time(0));
	r = rand()%156; 
	g = rand()%156;
	b = rand()%156;
}