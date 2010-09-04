#ifndef SHAPE
#include "GeometryHelper.h"
class AABB;
class Shape
{
protected:					 
	AABB *aabb;
	GeometryHelper::Transformation2D *t;	
public:
	virtual float getSurface() = 0;
	virtual float getInertiaMomentum(float m) = 0;
};   


class ImplicitShape
{
protected:	
	float margin;
public:
	inline float getMargin();
	virtual Vector2D getCenter() = 0;
	virtual int getSupportPoint(Vector2D &d, Point2D *res) = 0;
	virtual int getSupportPoint(Vector2D &d, Point2D *res, int optimisationId) = 0;  // utile pour tous ou seulement les polygones?	  
	int getMarginedSupportPoint(Vector2D &d, Point2D *res);	 
	int getMarginedSupportPoint(Vector2D &d, Point2D *res, int optimisationId);	   
	void addMargin(Vector2D &v, Point2D *pt);
};
inline float ImplicitShape::getMargin()
{ return margin; }
#define SHAPE
#endif

