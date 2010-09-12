#ifndef SHAPE
#include "GeometryHelper.h"
#include "AABB.h"

struct OBBtree;
class Shape
{
private:
	int aabbid;
protected:	
	OBBtree *otree;
	GeometryHelper::Transformation2D t;	
	float aabb_xm, aabb_xM, aabb_ym, aabb_yM;
	bool fixedobj;

public:
	virtual float getSurface() = 0;
	virtual float getInertiaMomentum(float m) = 0; 
	virtual int getShapeTypeID() = 0;
	virtual void updateAABB() = 0;

	void updateAABB(EndPoint *xm, EndPoint *xM, EndPoint *ym, EndPoint *yM);
	inline OBBtree *getOtree();
	inline int getAABBid();
	inline void setAABBid(int id);
	inline bool AABBvsAABB(float xm, float xM, float ym, float yM);
	inline bool isFixed();
	inline Vector2D toRotatedInv(Vector2D &v);
	inline Vector2D toRotated(Vector2D &v);
	inline Vector2D toGlobal(Vector2D &v);
	inline Point2D toGlobal(Point2D &v);
	inline Vector2D toLocal(Vector2D &p);
	inline Point2D toLocal(Point2D &p);
};   
inline bool Shape::isFixed()
{ return fixedobj; }
inline OBBtree *Shape::getOtree()
{ return otree; }
inline int Shape::getAABBid()
{ return aabbid; }
inline void Shape::setAABBid(int id)
{ aabbid = id; }
inline bool Shape::AABBvsAABB(float xm, float xM, float ym, float yM)
{
	if(xM < aabb_xm || xm > aabb_xM) return false;
	if(yM < aabb_ym || ym > aabb_yM) return false;
	return true;
}

inline Point2D Shape::toGlobal(Point2D &v)
{ return t.transform(v); }
inline Vector2D Shape::toGlobal(Vector2D &v)
{ return t.transform(v); }		
inline Point2D Shape::toLocal(Point2D &v)
{ return t.transforminv(v); }
inline Vector2D Shape::toLocal(Vector2D &v)
{ return t.transforminv(v); }		  
inline Vector2D Shape::toRotatedInv(Vector2D &v)
{ return t.rotateinv(v); }	  
inline Vector2D Shape::toRotated(Vector2D &v)
{ return t.rotate(v); }



class ImplicitShape
{
protected:	
	float margin;
	Shape *parent;
public:
	inline float getMargin();
	virtual float getBoundingSphereSqRadius() = 0;
	virtual Vector2D getCenter() = 0;
	virtual int getSupportPoint(Vector2D &d, Point2D *res) = 0;
	virtual int getSupportPoint(Vector2D &d, Point2D *res, int optimisationId) = 0;  // utile pour tous ou seulement les polygones?	 
	int getMarginedSupportPoint(Vector2D &d, Point2D *res);	 
	int getMarginedSupportPoint(Vector2D &d, Point2D *res, int optimisationId);	   
	void addMargin(Vector2D &v, Point2D *pt);
	
	inline Vector2D toRotatedInv(Vector2D &v);
	inline Vector2D toRotated(Vector2D &v);
	inline Vector2D toGlobal(Vector2D &v);
	inline Point2D toGlobal(Point2D &v);
	inline Vector2D toLocal(Vector2D &p);
	inline Point2D toLocal(Point2D &p);
};
inline float ImplicitShape::getMargin()
{ return margin; }

inline Point2D ImplicitShape::toGlobal(Point2D &v)
{ return parent->toGlobal(v); }
inline Vector2D ImplicitShape::toGlobal(Vector2D &v)
{ return parent->toGlobal(v); }	   
inline Point2D ImplicitShape::toLocal(Point2D &v)
{ return parent->toLocal(v); }
inline Vector2D ImplicitShape::toLocal(Vector2D &v)
{ return parent->toLocal(v); }
inline Vector2D ImplicitShape::toRotatedInv(Vector2D &v)
{ return parent->toRotatedInv(v); }
inline Vector2D ImplicitShape::toRotated(Vector2D &v)
{ return parent->toRotated(v); }
#define SHAPE
#endif

