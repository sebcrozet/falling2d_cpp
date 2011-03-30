#include "CollisionDetector.h"
#include "Shapes.h"
#include "PixelPerfectShape.h"

class PixelPerfectSolver : CollisionDetector
{
private:
	PixelPerfectShape *pps;
	Shape * s;

	bool _solve(std::vector<ContactBackup *> &res);
public:
	bool canDestroy();
	PixelPerfectSolver(PixelPerfectShape *ps, Shape *s);
};