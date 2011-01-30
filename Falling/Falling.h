#include "TunningConstants.h"
#include "math.h"
#include "Polygon2D.h"
#include "PenetrationSolver.h"
#include "ImpulseSolver.h"
#include "VitessSolver.h"
#include "Island.h"

class FALLINGAPI World
{
private:
	CollisionArbiter ca;
	std::vector<RigidBody *> objs;
	std::stack<RigidBody *> removeWaitingQueue;
	std::stack<RigidBody *> addWaitingQueue;

	void dumpAddDelete();
	void checkSleeps(float dt);
public:	
	World();
	~World();
	// for debug
	bool penAlgorithm;
	// end: for debug
	void addObject(RigidBody *s);
	void removeObject(RigidBody *s);
	void solve(float dt);

	void notifyObjectMoved(RigidBody *s);
};