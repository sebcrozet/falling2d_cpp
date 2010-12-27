#ifndef _COLLISIONDETECTORS
#include "Shapes.h"
#include "SAPsolver.h"
#include "SubCollision.h"
#include <vector>

class CollisionDetector
{
private :
	bool  isInactive;

	virtual bool _solve(std::vector<SubCollision> &res) = 0; // returns whether current class should be destroyed
public:	
	CollisionDetector();
	bool solve(std::vector<SubCollision> &res); // returns whether current class should be destroyed
	inline void setInactive(bool inactive);
	virtual bool canDestroy() = 0;
};	

inline void CollisionDetector::setInactive(bool inactive)
{ isInactive = inactive; }

#define _COLLISIONDETECTORS
#endif