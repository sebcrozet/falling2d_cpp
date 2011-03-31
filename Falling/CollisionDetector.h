#ifndef _COLLISIONDETECTORS
#include "Shapes.h"
#include "SAPsolver.h"
#include "SubCollision.h"
#include "ContactBatch.h"
#include <vector>

class CollisionDetector
{
private :
  bool  isInactive;

  virtual bool _solve(std::vector<ContactBackup *> &res) = 0; // returns whether current class should be destroyed
public:
  CollisionDetector();
  bool solve(std::vector<ContactBackup *> &res); // returns whether current class should be destroyed
  inline void setInactive(bool inactive);
  inline bool getIsInactive();
  virtual bool canDestroy() = 0;
};
inline bool CollisionDetector::getIsInactive()
{
  return isInactive;
}
inline void CollisionDetector::setInactive(bool inactive)
{
  isInactive = inactive;
}

#define _COLLISIONDETECTORS
#endif