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
	Disk(Point2D &pt, float radius, bool fixed);
	float getSurface();
	float getInertiaMomentum(float m);	   
	Vector2D getCenter();
	int getSupportPoint(Vector2D &d, Point2D *res);
	int getSupportPoint(Vector2D &d, Point2D *res, int optimisationId);
	float getBoundingSphereSqRadius();
	void updateAABB();
	inline int getShapeTypeID();

	inline float getRadius() const;
};

inline float Disk::getRadius() const
{ return radius; }
inline int Disk::getShapeTypeID()
{ return 1; }
#define _DISK_H
#endif