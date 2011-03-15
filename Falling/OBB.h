#define DEBUGMOD
#ifndef _OBB	  
#include "TunningConstants.h"
#include "Shapes.h"
#include "InfinitePlane.h"

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
	Real origin[2];
	Real aire;
	int obbid;
			  
	bool intersects2axis(OBB *o);
public:
	OBB(Point2D a,Point2D b,Point2D c,Point2D d,ImplicitShape *parent, Real aire, int obbid);
	bool intersects(OBB *o);
	bool intersectsPlane(InfinitePlane *p);
	void getMedialAxis(Point2D *center, Point2D *refpt);
	Point2D getCenter() const;
	void translate(const Vector2D &);
	inline Real getAire() const;
	inline int getID() const;
	inline ImplicitShape *getParent() const;
};
inline Real OBB::getAire() const
{ return aire; }
inline int OBB::getID() const
{ return obbid; }
inline ImplicitShape *OBB::getParent() const
{ return parent; }

#define _OBB
#endif

