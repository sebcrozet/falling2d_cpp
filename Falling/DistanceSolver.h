#ifndef _GJKSOLVER_H
#include "Shapes.h"
#include "queue"
#include "CollisionDetector.h"
#define GJK_DESTROYLIMIT 20

class EPAsolver;
class GJKsolver : public CollisionDetector
{
	friend EPAsolver;
private:
	float msum, sqmsum;	// margin sum
	ImplicitShape &A,&B;
	int optidsA[3],
		optidsB[3];
	Point2D ptsA[3],
			ptsB[3];
	Vector2D ptsC[3],
			 dirs[3];
	Vector2D satlastdir;
	int simplexSize;
	
	bool ISAsolve();
	float solveDist(float* bparam);
	void recomputeSimplex();
	void updateClosestFeatureSimplexDatas(Vector2D &p, float * barycentricParam);
	void gjk_buildMarginedSimplexWithOrigin();

	bool _solve(std::vector<SubCollision> &res);
	inline void swapPts(Vector2D &a, Vector2D &b);
public:
	GJKsolver(ImplicitShape &a,ImplicitShape &b);
	float getSeparatingDistance();
	float getClosestPoints(Point2D *pA, Point2D *pB);
	float getPenDepth(Point2D *pA, Point2D *pB);
	bool canDestroy();
};

inline void GJKsolver::swapPts(Vector2D &a, Vector2D &b)
{ Vector2D c = a; a = b; b = c; }

class SimplexSeg
{
private:
	float bCoord;
	float dist;
	Point2D ptA1, ptB1, ptC1, ptA2, ptB2, ptC2;
	Vector2D v;
public:
	inline float getdist() const;

	SimplexSeg(Point2D &pA1, Point2D &ptB1, Point2D &ptC1, Point2D &ptA2, Point2D &ptB2, Point2D &ptC2);
	bool isValid();
	SimplexSeg *cut(Point2D &ptA3,Point2D &ptB3,Point2D &ptC3);
	bool operator<(SimplexSeg &s2);			   
	Vector2D getSupportVect();
	void getABpoints(Point2D *a,Point2D *b);
	inline Point2D getC1();
	inline Point2D getC2();
};

inline Point2D SimplexSeg::getC1()
{ return ptC1; }
inline Point2D SimplexSeg::getC2()
{ return ptC2; }

struct simplexSeg_less : std::binary_function<SimplexSeg *, SimplexSeg *,bool>
{
	bool operator()(SimplexSeg *a, SimplexSeg *b) const;
};

class EPAsolver 
{
private:
	GJKsolver &gsolv;
	std::priority_queue<SimplexSeg *,std::vector<SimplexSeg *>,simplexSeg_less> sd;
	bool distnull;
public:
	EPAsolver(GJKsolver &simplex);
	Vector2D getPenetrationDepth(Point2D *pA, Point2D *pB);
};
#define EPSILON_ FLT_EPSILON * 1000
#define EPSILON_1 (1 + EPSILON_)  * (1 + EPSILON_)

inline float SimplexSeg::getdist() const
{ return dist; }
#define _GJKSOLVER_H
#endif