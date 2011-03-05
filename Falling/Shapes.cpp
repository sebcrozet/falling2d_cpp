#include "stdafx.h" 
#include "Shapes.h"

void ImplicitShape::addMargin(const Vector2D &d, Point2D *pt) const
{ *pt += d * margin; }

int ImplicitShape::getMarginedSupportPoint(const Vector2D &d, Point2D *res) const
{
	int opt = getSupportPoint(d, res);
	addMargin(d, res); 
	return opt;
}

int ImplicitShape::getMarginedSupportPoint(const Vector2D &d, Point2D *res, int optimisationId) const
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
