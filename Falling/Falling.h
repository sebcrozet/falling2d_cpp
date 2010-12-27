#include "math.h"
#include "Polygon2D.h"
#include "CollisionArbiter.h"

class World
{
private:
	CollisionArbiter ca;
	std::stack<Shape *> removeWaitingQueue;
	std::stack<Shape *> addWaitingQueue;

	void dumpAddDelete();
public:
	void addObject(Shape *s);
	void removeObject(Shape *s);
	std::vector<Collision *> solve(float dt);

	void notifyObjectMoved(Shape *s);
};