#ifndef _DISK_H
#include "Shapes.h"	
#include "AABB.h"
#include "OBBtree.h"

class Disk : public ImplicitShape, public Shape 
{
private:
	OBB *obb;
	float radiuslessM, radius;
public:
	Disk(const Point2D &pt, float radius, bool fixed);
	float getSurface() const;
	float getInertiaMomentum(float m) const;
	Vector2D getCenter() const;
	int getSupportPoint(const Vector2D &d, Point2D *res) const;
	int getSupportPoint(const Vector2D &d, Point2D *res, int optimisationId) const;
	float getBoundingSphereRadius() const;
	void updateAABB();
	inline int getShapeTypeID() const;
	inline float getRadius() const;
};

inline float Disk::getRadius() const
{ return radius; }
inline int Disk::getShapeTypeID() const
{ return 1; }
#define _DISK_H
#endif
