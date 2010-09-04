#include "Shapes.h"	
#include "AABB_disk.h"
#include "OBBtree.h"

class Disk : public ImplicitShape, public Shape 
{
private:
	OBB *obb;
	OBBtree *otree;
	float radiuslessM, radius;
public:
	Disk(Point2D &pt, float radius);
	float getSurface();
	float getInertiaMomentum(float m);	   
	Vector2D getCenter();
	int getSupportPoint(Vector2D &d, Point2D *res);
	int getSupportPoint(Vector2D &d, Point2D *res, int optimisationId);
};