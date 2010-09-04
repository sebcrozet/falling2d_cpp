#include "stdafx.h"
#include "OBB.h"

OBB::OBB(Point2D a,Point2D b,Point2D c,Point2D d,ImplicitShape *parent, float aire) : parent(parent), aire(aire)
{
	pts[0] = Vector2D(a);
	pts[1] = Vector2D(b);
	pts[2] = Vector2D(c);
	pts[3] = Vector2D(d);
	axis[0] = pts[1] - pts[0];
	axis[1] = pts[3] - pts[0];

	for(int i=0; i<2 ; i++)
	{
		axis[i] /= axis[i] * axis[i];
		origin[i] = pts[0] * axis[i];	
	}
}

bool OBB::intersects(OBB *o)
{ return intersects2axis(o) && o->intersects2axis(this); }

bool OBB::intersects2axis(OBB *o)
{
	for(int i = 0; i < 2; i++)
	{
		float dot = o->pts[0] * axis[i];
		float dotMin = dot;
		float dotMax = dot;

		for(int j = 1; j < 3; j++)
		{
			dot = o->pts[j] * axis[i];
			if(dot > dotMax)
				dotMax = dot;
			else if(dot < dotMin)
				dotMin = dot;
		}
		if(dotMax < origin[i] || dotMin > 1 + origin[i])
			return false;
	}
	return true;
}

Point2D OBB::getCenter() const
{ return Point2D((pts[0].getX() + pts[2].getX()) / 2, (pts[0].getY() + pts[2].getY()) / 2); }

void OBB::getMedialAxis(Point2D *center, Point2D *refpt)
{
	*center = getCenter();
	Vector2D a0 = pts[1] - pts[0], a1 =	pts[3] - pts[0];
	if(a0*a0 > a1*a1)
		*refpt = *center + (a1 / 2);
	else								  
		*refpt = *center + (a0 / 2);
} 