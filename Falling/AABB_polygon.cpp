#include "stdafx.h"
#include "AABB_polygon.h"

AABB_polygon::AABB_polygon(ImplicitShape *parent)
{				 
	Vector2D vx(1,0);
	Vector2D vy(0,1);
	Point2D pt;
	ixM = parent->getSupportPoint(vx,&pt);
	xmax = EndPoint(pt.getX(), true, parent);
	ixm = parent->getSupportPoint(vx.reflexion(),&pt);
	xmin = EndPoint(pt.getX(), false, parent);
	iyM = parent->getSupportPoint(vy,&pt);
	ymax = EndPoint(pt.getY(), true, parent);
	iym = parent->getSupportPoint(vy.reflexion(),&pt);
	ymin = EndPoint(pt.getY(), false, parent);
}

void AABB_polygon::update()
{
	ImplicitShape *p = xmax.getParent();
	Vector2D vx(1,0);
	Vector2D vy(0,1);
	Point2D pt;
	ixM = p->getSupportPoint(vx,&pt,ixM);
	xmax.setValue(pt.getX());
	ixm = p->getSupportPoint(vx.reflexion(),&pt,ixm);
	xmin.setValue(pt.getX());
	iyM = p->getSupportPoint(vy,&pt,iyM);
	ymax.setValue(pt.getY());
	iym = p->getSupportPoint(vy.reflexion(),&pt,iym);
	ymin.setValue(pt.getY());
}