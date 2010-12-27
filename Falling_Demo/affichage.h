#include "stdafx.h"
#include <vector>
#include "Falling.h"
#include "DistanceSolver.h"
#include "Disk.h"	 
#include "PairManager.h"
#include <ctime>
#include <cstdlib>

typedef struct pObject pObject;
struct pObject
{
public:
   Vector2D u;
   Polygon2D * p;
   Disk * d;
   Point2D * pts;
   int nb, support, r, g, b;
   bool isdisk;

   pObject(Point2D * pts,int n,bool iscircle, World &coll, Point2D center = Point2D()); 
};

void initSDL();
void update(SDL_Surface *screen, std::vector<pObject *> ps, int depth, int depth2, Vector2D &sp);