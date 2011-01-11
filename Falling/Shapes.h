#define DEBUGMOD
#ifndef SHAPE
#include "GeometryHelper.h"
#include "AABB.h"

class Collision;
class RigidBody;
struct OBBtree;
class Shape
{
private:
	int aabbid;
#ifdef DEBUGMOD
public:
#else
protected:
#endif
	RigidBody *parentBody;
	OBBtree *otree;
	GeometryHelper::Transformation2D t;	
	float aabb_xm, aabb_xM, aabb_ym, aabb_yM;
	bool fixedobj;
	bool moved;

	// Collision graph infos
	Collision *collisions,*collisionsTail; // <- CollisionTail is useless (juste for debug)
	int collisionStackLevel;

public:
	// Collision graph infos
	inline Collision *getCollisionList();
	inline void setCollisionList(Collision *,Collision *);
	inline void setStackLevel(int level);
	inline int getStackLevel();
	//
	virtual float getSurface() = 0;
	virtual float getInertiaMomentum(float m) = 0; 
	virtual int getShapeTypeID() = 0;
	virtual void updateAABB() = 0;

	inline RigidBody *getParent();
	inline void setParent(RigidBody *r);

	void updateAABB(EndPoint *xm, EndPoint *xM, EndPoint *ym, EndPoint *yM);
	inline void rotate(float dteta);
	inline void translate(Vector2D du);

	inline OBBtree *getOtree();
	inline int getAABBid();
	inline void setAABBid(int id);
	inline bool AABBvsAABB(float xm, float xM, float ym, float yM);
	inline bool isFixed();
	inline Vector2D toRotatedInv(Vector2D &v);
	inline Vector2D toRotated(Vector2D &v);	 
	inline Point2D toRotatedInv(Point2D &v);
	inline Point2D toRotated(Point2D &v);
	inline Vector2D toGlobal(Vector2D &v);
	inline Point2D toGlobal(Point2D &v);
	inline Vector2D toLocal(Vector2D &p);
	inline Point2D toLocal(Point2D &p);
	inline Vector2D toTranslated(Vector2D &p);
	inline Point2D toTranslatedInv(Point2D &p);	 
	inline Vector2D toTranslatedInv(Vector2D &p);
	inline Point2D toTranslated(Point2D &p);
	inline Point2D *getLocalPoint() const;
	inline Vector2D getPos();
	inline void setPos(Vector2D &p);
	inline float getTeta();
	inline void setTeta(float nteta);
	inline bool getMoved();
	inline void setMoved(bool m);
};   

inline void Shape::setStackLevel(int level)
{ collisionStackLevel = level; }
inline int Shape::getStackLevel()
{ return collisionStackLevel; }

inline void Shape::setCollisionList(Collision *l, Collision *tail)
{ collisions = l; collisionsTail = tail; }
inline Collision *Shape::getCollisionList()
{ return collisions; }

inline RigidBody *Shape::getParent()
{ return parentBody; }
inline void Shape::setParent(RigidBody *p)
{ parentBody = p; }

inline Vector2D Shape::getPos()
{ return t.getU(); }
inline float Shape::getTeta()
{ return t.getTeta(); }
inline void Shape::setTeta(float nteta)
{ 
	setMoved(true);
	t.setTeta(nteta); 
}
inline void Shape::setPos(Vector2D &p)
{
	setMoved(true);
	t.setU(p); 
}
inline bool Shape::getMoved()
{ return moved; }
inline void Shape::setMoved(bool m)
{ moved = m; }

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

inline void Shape::rotate(float dteta)
{ 
	setMoved(true);
	t.addTeta(dteta);	
}
inline void Shape::translate(Vector2D du)
{ 
	setMoved(true);
	t.addU(du); 
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
inline Point2D Shape::toRotatedInv(Point2D &v)
{ return t.rotateinv(v); }	  
inline Point2D Shape::toRotated(Point2D &v)
{ return t.rotate(v); }		
inline Point2D Shape::toTranslatedInv(Point2D &v)
{ return t.translateinv(v); }	  
inline Point2D Shape::toTranslated(Point2D &v)
{ return t.translate(v); }	 
inline Vector2D Shape::toTranslated(Vector2D &v)
{ return t.translate(v); }	  
inline Vector2D Shape::toTranslatedInv(Vector2D &v)
{ return t.translateinv(v); }


class ImplicitShape
{
protected:	
	float margin;
	Shape *parent;
public:
	inline float getMargin();
	virtual float getBoundingSphereRadius() = 0;
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
	inline Vector2D toTranslated(Vector2D &p);
	inline Point2D toTranslatedInv(Point2D &p);	 
	inline Vector2D toTranslatedInv(Vector2D &p);
	inline Point2D toTranslated(Point2D &p);
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
inline Point2D ImplicitShape::toTranslatedInv(Point2D &v)
{ return parent->toTranslatedInv(v); }	  
inline Point2D ImplicitShape::toTranslated(Point2D &v)
{ return parent->toTranslated(v); }	 
inline Vector2D ImplicitShape::toTranslated(Vector2D &v)
{ return parent->toTranslated(v); }	  
inline Vector2D ImplicitShape::toTranslatedInv(Vector2D &v)
{ return parent->toTranslatedInv(v); }	   
#define SHAPE
#endif

