#include "CollisionDetector.h"
#include "DiskDiskSolver.h"
#include "GenericSolver.h"
#include "PlaneShapeSolver.h"

struct Collision
{
	Shape *sa, *sb;
	std::vector<SubCollision> c;
	CollisionDetector *cd;	 // Specific collision solver

	Collision(Shape *sa, Shape *sb);
	~Collision();
};


class CollisionArbiter
{	   
private:
	std::vector<Shape *> s;
	SAPsolver sap;

	static void addP(Pair *p, Shape *s, Shape *s2);
	static void deleteP(Pair &p);
	static bool removeP(Pair *p);
public:
	CollisionArbiter();
	~CollisionArbiter();
	void addObject(Shape *s);
	void deleteObject(Shape *s);
	void solve(std::vector<Collision *> &res);
	void notifyObjectMoved(Shape *s);
};