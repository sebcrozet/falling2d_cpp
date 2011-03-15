#ifndef POLYGON	    
#define DEBUGMOD
#include "Primitives.h"
#include "BoundingVolumes.h"
#include "Shapes.h"
#include "GeometryHelper.h"
#include "OBBtree.h"
#include "AABB.h"
#include "Tesselator.h"
#include <vector>

class ImplicitPolygon2D;
class FALLINGAPI Polygon2D	: public Shape
{	  
#ifdef DEBUGMOD
public:
#else
private:
#endif
	int nbrPts, nbrSubShapes;
	ImplicitPolygon2D **subShapes;
	ImplicitPolygon2D *chull;
	Point2D *points;

	
	int ixm, ixM, iym, iyM; //AABB update (hill climbing) optimisation datas

	void buildOBBtree();
	void buildOBBtree(OBBtree **o, std::vector<ImplicitPolygon2D*> &polyset, int &i);
public:												   
	Polygon2D(
		Point2D p[], 
		int nbpts, 
		Point2D *holespts[], 
		int nbrholes, 
		int holesnbrpts[], 
		int mergetype, 
		Vector2D position, 
		bool useCentroid, 
		Real orientation, 
		bool fixed
		);
	~Polygon2D();

	static void scalepts(Point2D *in, int n, Real scalefactor);
	static int simplify(Point2D * in, int n, Point2D **out, Real tolerence);
	static int simplifyToProper(Point2D * in, int n, Point2D **out);
	void update(Real rotationAngle);
	Real getInertiaMomentum(Real density) const;
	Real getSurface() const;
	void updateAABB();
	inline Vector2D getCentroid() const;
	inline int getShapeTypeID() const;

	static Point2D getCentroid(Point2D * in, int n);
	static Point2D getCentroid(Point2D * in, int n, Real aire);
	static Real getSurface(Point2D *in, int n);  
	static Real getUnitInertiaMomentum(Point2D *pts, int n, const Vector2D &axistranslate);
	static int buildConvexHull(Point2D *pts, int nbPts, Point2D ** outHull);
	static int getUncrossedPolygon(Point2D * pts, int nb, Point2D **res);
};	  

class FALLINGAPI ImplicitPolygon2D	: public ImplicitShape
{
#ifdef DEBUGMOD
public:
#else
private:
#endif
	Point2D *pts, center;
	int nbrPts;
	OBB *obb;
	Real radius;
	Real surface;
	Real unitInertia;

	int naiveClimb(int ibase, int imax,Vector2D &v) const;
	Real _getBoundingSphereSqRadius();
public:		   

	ImplicitPolygon2D(Point2D * ptsId, int n, Polygon2D *parent, int id);
	Point2D rightTgtPt(Point2D &ref);
	int getSupportPoint(const Vector2D &d, Point2D * res) const;
	int getSupportPoint(const Vector2D &d, Point2D * res, int o)const;
	Real getBoundingSphereRadius() const;
	void translateCentroid(const Vector2D &);
	inline Vector2D getCenter() const;  
	inline Vector2D getCentroid() const;
	inline OBB *getOBB() const;
	inline Real getUnitInertiaMomentum() const;
	inline Real getSurface() const;
	inline int getNbrPts()
	{ return nbrPts; }
	inline Point2D *getPts()
	{ return pts; }

	static OBB *buildOBB (Point2D *pts, int n, ImplicitPolygon2D *p, int id);
};

inline int Polygon2D::getShapeTypeID() const
{ return 2; }
inline Vector2D Polygon2D::getCentroid() const
{ return t.getU(); }

inline Real ImplicitPolygon2D::getSurface() const
{ return surface; }
inline Real ImplicitPolygon2D::getUnitInertiaMomentum() const
{ return unitInertia + surface * (center.getX() * center.getX() + center.getY() * center.getY()); }
inline Vector2D ImplicitPolygon2D::getCenter() const
{ return toGlobal(center); }
inline Vector2D ImplicitPolygon2D::getCentroid() const
{ return center; }
				  
inline OBB *ImplicitPolygon2D::getOBB() const
{ return obb; }

#define ilow(a) a--; if (a < 0) a = nbrPts - 1;
#define ihig(a) a++; if (a == nbrPts) a = 0; 
#define tlow(a) ((a - 1 < 0)? nbrPts - 1 : a)
#define thig(a) ((a + 1 == nbrPts)? 0 : a)
#define tmod(a, m) ((a >= m)? a - m : a)	
#define tmodinv(a, m) ((a < 0)? a + m : a) 
#define POLYGON
#endif
