#ifndef AABBs
#include "TunningConstants.h"
#include "Primitives.h"

class Shape;
struct AABB;
class EndPoint
{
private:
  int parentid; // parentid & 0x00000001 == ismax ; parentid >> 1 == parent's id
  Real value;
public:
  EndPoint();
  EndPoint(Real v, bool isMax, int parentid);

  inline Real getValue() const;
  inline bool isMaxValue() const;
  inline int getParent() const;
  inline void setValue(Real v);
  inline void operator=(const EndPoint &ep);
};

inline int EndPoint::getParent() const
{
  return parentid >> 1;
}
inline Real EndPoint::getValue() const
{
  return value;
}
inline bool EndPoint::isMaxValue() const
{
  return parentid & 1;
}
inline void EndPoint::setValue(Real v)
{
  value = v;
}
inline void EndPoint::operator =(const EndPoint &ep)
{
  value = ep.value;
  parentid = ep.parentid;
}


struct AABB
{
  // [0] -> x // [1] -> y
  int mins[2];
  int maxs[2];
  Shape *parent;
  AABB();

  void update();
};
#define AABBs
#endif
