#include <vector>
#include "ContactGenerator.h"
#include "Shapes.h"

class ImpulseSolver
{
private:
public:
	static void solve(std::vector<Contact *> scs,float dt);
	static void applyVelocityChange(Contact *c,Vector2D *rch,Vector2D *vch);
};
