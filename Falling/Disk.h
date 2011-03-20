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
	inline Real getRadius() const;

	// Implicit shape methods
	virtual Real getBoundingSphereRadius() const;
	virtual Vector2D getCenter() const;
	virtual int getSupportPoint(const Vector2D &d, Point2D *res, int optimisationId) const;
	virtual int getSupportPoint(const Vector2D &d, Point2D *res) const;

	// Shape methods
	virtual inline int getShapeTypeID() const;
	virtual Real getSurface() const;
	virtual Real getInertiaMomentum(Real m) const;
	virtual void updateAABB();
	virtual bool containsPoint(const Point2D &pt) const;
};

inline Real Disk::getRadius() const
{ return radius; }
inline int Disk::getShapeTypeID() const
{ return 1; }
#define _DISK_H
#endif
