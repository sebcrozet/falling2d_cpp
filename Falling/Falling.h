#include "math.h"
#include "Polygon2D.h"
#include "PenetrationSolver.h"
#include "ImpulseSolver.h"
#include "VitessSolver.h"

class World
{
private:
	CollisionArbiter ca;
	std::vector<RigidBody *> objs;
	std::stack<RigidBody *> removeWaitingQueue;
	std::stack<RigidBody *> addWaitingQueue;

	void dumpAddDelete();
	void checkSleeps(float dt);
public:
	void addObject(RigidBody *s);
	void removeObject(RigidBody *s);
	std::vector<Collision *> solve(float dt);

	void notifyObjectMoved(RigidBody *s);
};