#ifndef _INFINITEPLANE_H
#include "Shapes.h"

class InfinitePlane : public ImplicitShape, public Shape
{
private:
  Point2D pt, spt;
  Vector2D dir;
  Vector2D perpdir;
public:
  InfinitePlane(const Point2D &p, const Vector2D &normal);

  inline Point2D getProjectedPt(const Point2D &pt) const;
  inline bool isInSolidHalfSpace(const Point2D &pt) const;
  inline Real sqDistToPlane(const Point2D &pt) const;
  inline Vector2D getNormal() const;


  // Implicit shape methods
  virtual Real getBoundingSphereRadius() const;
  virtual Vector2D getCenter() const;
  virtual int getSupportPoint(const Vector2D &d, Point2D *res, int optimisationId) const;
  virtual int getSupportPoint(const Vector2D &d, Point2D *res) const;

  // Shape methods
  virtual inline int getShapeTypeID() const;
  virtual Real getSurface() const;
  virtual Real getInertiaMomentum(Real m) const;
  virtual void updateAABB();
  virtual bool containsPoint(const Point2D &pt) const;
};
inline bool InfinitePlane::isInSolidHalfSpace(const Point2D &ptToTest) const
{
  return ptToTest.isLeftTo(pt, spt) >= 0;
}
inline Point2D InfinitePlane::getProjectedPt(const Point2D &p) const
{
  Vector2D ao(pt, p);
  return pt + (perpdir * (ao * perpdir));
}
inline Real InfinitePlane::sqDistToPlane(const Point2D &p) const
{
  Vector2D ao(pt, p);
  Real d = ao * perpdir;
  return (ao * ao) - (d * d);
}
inline Vector2D InfinitePlane::getNormal() const
{
  return dir;
}
#define _INFINITEPLANE_H
#endif
