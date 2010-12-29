#include <vector>
#include "RigidBody.h"

class VitessSolver
{
public:
	static void integrate(std::vector <RigidBody *> r,float dt);
};