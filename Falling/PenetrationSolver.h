#include <vector>
#include "ContactGenerator.h"
#include "Shapes.h"
#include "island.h"

class PenetrationSolver
{
private:
public:
	static void solve(std::vector<Contact *> &scs);
	static void solve(Island *isl);
	static void applyPositionChange(Contact *c,float *ch,float *ah);
	static void perLevelApplyPositionChange(Contact *c,float *ch,float *ah);
	static void solveRelax(std::vector<Contact *> &scs);
	static void applyPositionChangeRelax(Contact *c,float *ch);
};
