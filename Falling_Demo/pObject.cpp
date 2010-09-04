#include "affichage.h"

pObject::pObject(Point2D *pts,int n,bool iscircle, Point2D center)  : pts(pts), support(0)
{
	isdisk = iscircle;
	
	nb = n;
	if(!iscircle)
	{
		Vector2D u = Vector2D(Polygon2D::getCentroid(pts,n));
		for(int i=0; i<n; i++)
			pts[i] -= u;
		p = new Polygon2D(pts,GeometryHelper::Transformation2D(u,0),n);
	}
	else
	{
		
		nb = rand() % 250 + 10;
		d = new Disk(center, nb);
	}	  
	srand(time(0));
	r = rand()%156; 
	g = rand()%156;
	b = rand()%156;
}