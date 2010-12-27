#include "affichage.h"

pObject::pObject(Point2D *pts,int n,bool iscircle, World &w, Point2D center)  : pts(pts), support(0)
{
	isdisk = iscircle;
	
	nb = n;
	if(!iscircle)
	{
		u = Vector2D(Polygon2D::getCentroid(pts,n));
		for(int i=0; i<n; i++)
			pts[i] -= u;
		p = new Polygon2D(pts,GeometryHelper::Transformation2D(u,1.57f),n, false);
		w.addObject(p);
	}
	else
	{
		
		nb = rand() % 250 + 10;
		d = new Disk(center, nb, false);
		w.addObject(d);
	}	  
	srand(time(0));
	r = rand()%156; 
	g = rand()%156;
	b = rand()%156;
}