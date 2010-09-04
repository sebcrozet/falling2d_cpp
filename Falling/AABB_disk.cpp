#include "stdafx.h"
#include "AABB_disk.h"

AABB_disk::AABB_disk(ImplicitShape * parent)
{		 
	float r = parent->getMargin();
	Vector2D p = parent->getCenter();
	xmin = EndPoint(-r + p.getX(), false, parent);
	xmax = EndPoint(r + p.getX(), true, parent);
	ymin = EndPoint(-r + p.getY(), false, parent);
	ymax = EndPoint(r + p.getY(), true, parent);
}

void AABB_disk::update()
{
	ImplicitShape *parent = xmin.getParent();  
	float radius = parent->getMargin();
	Vector2D p = parent->getCenter();
	xmin = EndPoint(-radius + p.getX(), false, parent);
	xmax = EndPoint(radius + p.getX(), true, parent);
	ymin = EndPoint(-radius + p.getY(), false, parent);
	ymax = EndPoint(radius + p.getY(), true, parent);
}

