#include "stdafx.h"
#include "ContactBatch.h"

ContactBatch::~ContactBatch()
{
  eraseCashedContacts();
}

void ContactBatch::eraseCashedContacts()
{
  for(unsigned int i = 0; i < scol.size(); i++)
    delete scol[i];
  scol.clear();
}

void ContactBatch::addContact(const SubCollision &lastDeepestPen)
{

  bool pushNewContact = true;
  Vector2D n(lastDeepestPen.ptA, lastDeepestPen.ptB);
  Real depth = n.normalise();
  Point2D mid = Point2D::getMiddle(lastDeepestPen.ptA, lastDeepestPen.ptB);
  for(int i = 0; i < (int)scol.size(); i++)
    {
      ContactBackup *cb = (scol[i]);
      Point2D absA = A.toGlobal(cb->relPtA);
      Point2D absB = B.toGlobal(cb->relPtB);
      // update datas
      Vector2D ni(absA, absB);
      // calculate depth (don't change normal)
      cb->depth = ni * cb->normal;
      // see if scol[i] is deletable
      if(cb->depth > 2.0 * PROXIMITY_AWARENESS || (ni * ni - cb->depth * cb->depth) > 4.0 * PROXIMITY_AWARENESS * PROXIMITY_AWARENESS)
        {
          // disjoint objects, remove contact
          scol[i] = scol[scol.size() - 1];
          scol.pop_back();
          delete cb;
          i--;
          continue;
        }
      else
        {
          // see if scol[i] has to be replaced
          Vector2D test(Point2D::getMiddle(absA, absB), mid);
          if(test * test < /* TODO: use more reliable value here */ PROXIMITY_AWARENESS)
            {
              // replace
              cb->depth = depth;
              cb->relPtA = A.toLocal(lastDeepestPen.ptA);
              cb->relPtB = B.toLocal(lastDeepestPen.ptB);
              cb->normal = n;
              pushNewContact = false;
            }
        }
    }
  if(pushNewContact)
    scol.push_back(new ContactBackup(A.toLocal(lastDeepestPen.ptA), B.toLocal(lastDeepestPen.ptB),n, depth));
}
