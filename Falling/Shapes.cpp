#include "stdafx.h" 
#include "Shapes.h"

void ImplicitShape::addMargin(Vector2D &d, Point2D *pt)
{ *pt += d.direction() * margin; }

int ImplicitShape::getMarginedSupportPoint(Vector2D &d, Point2D *res)
{
	int opt = getSupportPoint(d, res);
	addMargin(d, res); 
	return opt;
}

int ImplicitShape::getMarginedSupportPoint(Vector2D &d, Point2D *res, int optimisationId)
{
	int opt = getSupportPoint(d, res, optimisationId);
	addMargin(d, res); 
	return opt;
}