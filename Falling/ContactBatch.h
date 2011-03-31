#ifndef __FALLING_CONTACT_BATCH
#include "stdafx.h"
#include "TunningConstants.h"
#include "floatTests.h"
#include "SubCollision.h"
#include "FakeShape.h"
#include <vector>

typedef struct ContactBackup ContactBackup;
struct ContactBackup
{
  Point2D relPtA;
  Point2D relPtB;
  Real depth;
  Vector2D normal;

  inline ContactBackup()
    :relPtA(Point2D()), relPtB(Point2D()), depth(0), normal(Vector2D())
  { }
  inline ContactBackup(
    const Point2D &pa,
    const Point2D &pb,
    ImplicitShape *A,
    ImplicitShape *B
  )
  {
    setDatas(pa, pb, A, B);
  }

  inline void setDatas(
    const Point2D &pa,
    const Point2D &pb,
    ImplicitShape *A,
    ImplicitShape *B
  )
  {
    normal = Vector2D(pa, pb);
    depth = normal.normalise();
    relPtA = A->toLocal(pa);
    relPtB = B->toLocal(pb);
  }

  inline ContactBackup(
    const Point2D &pa,
    const Point2D &pb,
    const Vector2D &n,
    Real depth)
    : relPtA(pa),
      relPtB(pb),
      depth(depth),
      normal(n)
  { }
};

class ContactBatch
{
private:
  std::vector<ContactBackup *> scol;
  ImplicitShape &A;
  ImplicitShape &B;
public:
  inline ContactBatch(ImplicitShape &A, ImplicitShape &B)
    : A(A), B(B)
  { }
  ~ContactBatch();
  void addContact(const SubCollision &lastDeepestColl);
  void eraseCashedContacts();

  inline int getNbrContacts() const
  {
    //printf("%i batched\n", scol.size());
    return scol.size();
  }
  inline ContactBackup * getContacts(int i) const
  {
    return scol[i];
  }
};
#define __FALLING_CONTACT_BATCH
#endif
