#ifndef __COLLISION_ARBITER__	  
#include "TunningConstants.h"
#include "CollisionDetector.h"
#include "DiskDiskSolver.h"
#include "GenericSolver.h"
#include "PlaneShapeSolver.h"
#include "FakeShape.h"
#include "RigidBody.h"

struct Contact;
struct QuarterSpace;
struct Collision
{
	Shape *sa, *sb;
	std::vector<SubCollision> c;
	Contact **cnts; // Contact *[]
	CollisionDetector *cd;	 // Specific collision solver
	Collision *preva,*prevb,*nexta,*nextb;
	Collision *nextlvlptr,*prevlvlptr;
	Contact *worstContact;
	Real worstPenetrationAmount;
	int collisionStackLevel;   
	QuarterSpace *liberty1,*liberty2;

	Collision(Shape *sa, Shape *sb);
	Collision(Shape *s);
	~Collision();
	void removeFromList();
	void autoInsert();
	void insertInLevel(Collision *);

	static Collision *inPlaceSortList(Collision *);
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
#define __COLLISION_ARBITER__
#endif
