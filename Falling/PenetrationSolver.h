#include <vector>
#include "ContactGenerator.h"
#include "Shapes.h"

class PenetrationSolver
{
private:
public:
	static void solve(std::vector<Contact *> scs, Shape *s1, Shape *s2);
};
