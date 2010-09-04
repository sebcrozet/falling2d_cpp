#define DEBUGMOD
#include "Shapes.h"

#ifndef _OBB
class OBB
{  
#ifdef DEBUGMOD
public:
#else
private:
#endif	  
	ImplicitShape *parent;
	Vector2D pts[4];
	Vector2D axis[2];
	float origin[2];
	float aire;
			  
	bool intersects2axis(OBB *o);
public:
	OBB(Point2D a,Point2D b,Point2D c,Point2D d,ImplicitShape *parent, float aire);
	bool intersects(OBB *o);
	void getMedialAxis(Point2D *center, Point2D *refpt);
	Point2D getCenter() const;
	inline float getAire() const;
};
inline float OBB::getAire() const
{ return aire; }

#define _OBB
#endif

