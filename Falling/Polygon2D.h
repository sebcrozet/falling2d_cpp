#ifndef POLYGON	    
#define DEBUGMOD
#include "Primitives.h"
#include "BoundingVolumes.h"
#include "Shapes.h"
#include "GeometryHelper.h"
#include "OBBtree.h"
#include "AABB_polygon.h"
#include <vector>

class ImplicitPolygon2D;
class Polygon2D	: public Shape
{	  
#ifdef DEBUGMOD
public:
#else
private:
#endif
	GeometryHelper::Transformation2D t;
	int nbrPts, nbrSubShapes, nbrSubShapes2;
	ImplicitPolygon2D *chull, **subShapes, **subShapes2;
	Point2D *points;
	OBBtree *otree;

	void buildOBBtree();
	void buildOBBtree(OBBtree **o, std::vector<ImplicitPolygon2D*> &polyset);
	int simplify(Point2D * in, int n, Point2D **out, int tolerence);
	void tesselate();
	void tesselate2();
public:
	Polygon2D(Point2D *pts,GeometryHelper::Transformation2D &tr, int n);
	~Polygon2D();

	void update(float rotationAngle);
	float getInertiaMomentum(float m);
	float getSurface();

	inline Point2D toGlobal(Point2D &p);
	inline Vector2D toGlobal(Vector2D &p);
	inline Vector2D toLocal(Vector2D &p);
	inline Point2D toLocal(Point2D &p);
	inline Vector2D toRotatedInv(Vector2D &p);
	inline Vector2D getCentroid();

	static Point2D getCentroid(Point2D * in, int n);
	static Point2D getCentroid(Point2D * in, int n, float aire);
	static float getSurface(Point2D *in, int n);  
	static int buildConvexHull(Point2D *pts, int nbPts, Point2D ** outHull);
};	  

class ImplicitPolygon2D	: public ImplicitShape
{
#ifdef DEBUGMOD
public:
#else
private:
#endif
	Polygon2D &parent;
	Point2D *pts, center;
	int nbrPts;
	OBB *obb;

	int naiveClimb(int ibase, int imax,Vector2D &v);
	
	inline Vector2D toRotatedInv(Vector2D &v);
	inline Vector2D toGlobal(Vector2D &v);
	inline Point2D toGlobal(Point2D &v);
public:		   
	inline Vector2D toLocal(Vector2D &p);
	inline Point2D toLocal(Point2D &p);

	ImplicitPolygon2D(Point2D * ptsId, int n, Polygon2D &parent);
	Point2D rightTgtPt(Point2D &ref);
	int getSupportPoint(Vector2D &d, Point2D * res);
	int getSupportPoint(Vector2D &d, Point2D * res, int o);	 
	inline Vector2D getCenter();  
	inline OBB *getOBB();

	static OBB *buildOBB (Point2D *pts, int n, ImplicitPolygon2D *p);
};

inline Vector2D Polygon2D::getCentroid()
{ return t.getU(); }

inline Point2D Polygon2D::toGlobal(Point2D &v)
{ return t.transform(v); }
inline Vector2D Polygon2D::toGlobal(Vector2D &v)
{ return t.transform(v); }		
inline Point2D Polygon2D::toLocal(Point2D &v)
{ return t.transforminv(v); }
inline Vector2D Polygon2D::toLocal(Vector2D &v)
{ return t.transforminv(v); }		  
inline Vector2D Polygon2D::toRotatedInv(Vector2D &v)
{ return t.rotateinv(v); }

								  
inline Vector2D ImplicitPolygon2D::getCenter()
{ return center; }
inline Point2D ImplicitPolygon2D::toGlobal(Point2D &v)
{ return parent.toGlobal(v); }
inline Vector2D ImplicitPolygon2D::toGlobal(Vector2D &v)
{ return parent.toGlobal(v); }	   
inline Point2D ImplicitPolygon2D::toLocal(Point2D &v)
{ return parent.toLocal(v); }
inline Vector2D ImplicitPolygon2D::toLocal(Vector2D &v)
{ return parent.toLocal(v); }
inline Vector2D ImplicitPolygon2D::toRotatedInv(Vector2D &v)
{ return parent.toRotatedInv(v); }
				  
inline OBB *ImplicitPolygon2D::getOBB()
{ return obb; }

#define ilow(a) a--; if (a < 0) a = nbrPts - 1;
#define ihig(a) a++; if (a == nbrPts) a = 0; 
#define tlow(a) ((a - 1 < 0)? nbrPts - 1 : a)
#define thig(a) ((a + 1 == nbrPts)? 0 : a)
#define tmod(a, m) ((a >= m)? a - m : a)	
#define tmodinv(a, m) ((a < 0)? a + m : a) 
#define POLYGON
#endif