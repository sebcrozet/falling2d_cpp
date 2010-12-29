#include <vector>
#include "ContactGenerator.h"
#include "Shapes.h"

class PenetrationSolver
{
private:
public:
	static void solve(std::vector<Contact *> scs);
	static void applyPositionChange(Contact *c,float *ch);
};
