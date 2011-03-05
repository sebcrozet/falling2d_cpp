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
	inline Point2D getProjectedPt(const Point2D &pt) const;
	inline bool isInSolidHalfSpace(const Point2D &pt) const;
	inline float sqDistToPlane(const Point2D &pt) const;
	inline Vector2D getNormal() const;
};
inline bool InfinitePlane::isInSolidHalfSpace(const Point2D &pt) const
{ return pt.isLeftTo(pt, spt) >= 0; }
inline Point2D InfinitePlane::getProjectedPt(const Point2D &p) const
{
	Vector2D ao(pt, p);
	return pt + (perpdir * (ao * perpdir));
}
inline float InfinitePlane::sqDistToPlane(const Point2D &p) const
{
	Vector2D ao(pt, p);
	float d = ao * perpdir;
	return (ao * ao) - (d * d);
}
inline Vector2D InfinitePlane::getNormal() const
{ return dir; }
#define _INFINITEPLANE_H
#endif
