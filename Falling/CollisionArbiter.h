#ifndef __COLLISION_ARBITER__
#include "CollisionDetector.h"
#include "DiskDiskSolver.h"
#include "GenericSolver.h"
#include "PlaneShapeSolver.h"
#include "FakeShape.h"

struct Contact;
struct Collision
{
	Shape *sa, *sb;
	std::vector<SubCollision> c;
	Contact **cnts; // Contact *[]
	CollisionDetector *cd;	 // Specific collision solver
	Collision *preva,*prevb,*nexta,*nextb;
	Collision *nextlvlptr,*prevlvlptr;
	Contact *worstContact;
	int collisionStackLevel;

	Collision(Shape *sa, Shape *sb);
	Collision(Shape *s);
	~Collision();
	void removeFromList();
	void autoInsert();
	void insertInLevel(Collision *);
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