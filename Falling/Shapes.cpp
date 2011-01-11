#include "stdafx.h" 
#include "Shapes.h"

void ImplicitShape::addMargin(Vector2D &d, Point2D *pt)
{ *pt += d * margin; }

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

void Shape::updateAABB(EndPoint *xm, EndPoint *xM, EndPoint *ym, EndPoint *yM)
{
	xm->setValue(aabb_xm);
	xM->setValue(aabb_xM);
	ym->setValue(aabb_ym);
	yM->setValue(aabb_yM);
}