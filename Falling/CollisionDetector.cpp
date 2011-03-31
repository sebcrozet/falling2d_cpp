#include "stdafx.h"
#include "CollisionDetector.h"
#include "PenetrationSolver.h"
#include "ContactBatch.h"


CollisionDetector::CollisionDetector() : isInactive(false)
{ }

bool CollisionDetector::solve(std::vector<ContactBackup *> &res)
{
  if(!isInactive)
    return _solve(res);
  else
    return canDestroy();
}
