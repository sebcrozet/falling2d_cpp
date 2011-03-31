#include "stdafx.h"
#include "PlaneShapeSolver.h"

PlaneShapeSolver::PlaneShapeSolver(InfinitePlane *p, Shape *s)  : pm(pairDeleted), s(s), ip(p)
{ }

void PlaneShapeSolver::pairDeleted(Pair &p)
{
  delete (PlaneImplicitShapeDistanceSolver *)p.e;
}

bool PlaneShapeSolver::canDestroy()
{
  std::stack<Pair *> todel;
  Pair *ap = pm.getActivePairs();
  int n = pm.getNbActivePairs();
  for(int i = 0; i < n; i++)
    {
      if(((PlaneImplicitShapeDistanceSolver *)ap[i].e)->canDestroy())
        todel.push(ap);
    }
  while(!todel.empty())
    {
      pm.removePair(todel.top());
      todel.pop();
    }
  return pm.getNbActivePairs() == 0; // no valid cash datas
}

bool PlaneShapeSolver::_solve(std::vector<ContactBackup *> &res)
{
  std::vector<OBB *> oi;
  traverseTree(s->getOtree(),  oi);
  int size = (int)oi.size();
  for(int i = 0; i < size; i++)
    {
      OBB *oin = oi[i];
      Pair* p = pm.addPair(oin->getID(), 0);
      if(p->e) // pair already exists. Mark as still active
        ((PlaneImplicitShapeDistanceSolver *)p->e)->setInactive(false);
      else // new pair
        p->e = new PlaneImplicitShapeDistanceSolver(*ip, *oin->getParent());
      oi[i] = 0;
    }
  oi.clear();
  Pair *ap = pm.getActivePairs();
  int n = pm.getNbActivePairs();
  std::stack<Pair *> todel;
  for(int i = 0; i < n; i++)
    {
      if(((PlaneImplicitShapeDistanceSolver *)ap[i].e)->solve(res))
        todel.push(&ap[i]);
      else
        ((PlaneImplicitShapeDistanceSolver *)ap[i].e)->setInactive(true); // mark as inactive
    }
  while(!todel.empty())
    {
      pm.removePair(todel.top());
      todel.pop();
    }
  return pm.getNbActivePairs() == 0;
}

void PlaneShapeSolver::traverseTree(OBBtree *a, std::vector<OBB *> &res)
{
  std::stack<OBBtree *> s;

  while(true)
    {
      if(a->o->intersectsPlane(ip))
        {
          if(a->isLeaf())
            res.push_back(a->o);
          else
            {
              s.push(a->r);
              a = a->l;
              continue;
            }
        }
      if(s.empty())
        break;
      a = s.top();
      s.pop();
    }
}

PlaneImplicitShapeDistanceSolver::PlaneImplicitShapeDistanceSolver(InfinitePlane &ip, ImplicitShape &is) : is(is), p(ip)
{
  Point2D unusued;
  optid = is.getSupportPoint(ip.getNormal().reflexion(), &unusued);
}

bool PlaneImplicitShapeDistanceSolver::canDestroy()
{
  return p.sqDistToPlane(Point2D(is.getCenter()) - is.getBoundingSphereRadius()) > PS_DESTROYLIMIT;
}

bool PlaneImplicitShapeDistanceSolver::_solve(std::vector<ContactBackup *> &res)
{
  Point2D ppt;
  optid = is.getMarginedSupportPoint(p.getNormal().reflexion(), &ppt, optid);
  if(p.isInSolidHalfSpace(ppt))
    {
      // TODO: inherite also from Implicit Shape
      lastContactDatas.setDatas(p.getProjectedPt(ppt), ppt, &p, &is);
      res.push_back(&lastContactDatas);
      return false;
    }
  return canDestroy();
}