#ifndef _DISK_H
#include "Shapes.h"	
#include "AABB.h"
#include "OBBtree.h"

class Disk : public ImplicitShape, public Shape 
{
private:
	OBB *obb;
	Real radiuslessM, radius;
public:
	Disk(const Point2D &pt, Real radius, bool fixed);
	Real getSurface() const;
	Real getInertiaMomentum(Real m) const;
	Vector2D getCenter() const;
	int getSupportPoint(const Vector2D &d, Point2D *res) const;
	int getSupportPoint(const Vector2D &d, Point2D *res, int optimisationId) const;
	Real getBoundingSphereRadius() const;
	void updateAABB();
	inline int getShapeTypeID() const;
	inline Real getRadius() const;
};

inline Real Disk::getRadius() const
{ return radius; }
inline int Disk::getShapeTypeID() const
{ return 1; }
#define _DISK_H
#endif
