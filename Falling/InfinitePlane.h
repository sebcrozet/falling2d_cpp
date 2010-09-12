#ifndef _INFINITEPLANE_H
#include "Shapes.h"

class InfinitePlane : public Shape
{
private:
	Point2D pt, spt;
	Vector2D dir;
	Vector2D perpdir;
public:
	InfinitePlane(Point2D p, Vector2D normal);

	float getSurface();
	float getInertiaMomentum(float m);
	int getShapeTypeID();
	void updateAABB();
	inline Point2D getProjectedPt(Point2D &pt);
	inline bool isInSolidHalfSpace(Point2D &pt);
	inline float sqDistToPlane(Point2D &pt);
	inline Vector2D getNormal() const;
};
inline bool InfinitePlane::isInSolidHalfSpace(Point2D &pt)
{ return pt.isLeftTo(pt, spt) >= 0; }
inline Point2D InfinitePlane::getProjectedPt(Point2D &p)
{
	Vector2D ao(pt, p);
	return pt + (perpdir * (ao * perpdir));
}
inline float InfinitePlane::sqDistToPlane(Point2D &p)
{
	Vector2D ao(pt, p);
	float d = ao * perpdir;
	return (ao * ao) - (d * d);
}
inline Vector2D InfinitePlane::getNormal() const
{ return dir; }
#define _INFINITEPLANE_H
#endif