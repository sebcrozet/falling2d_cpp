#ifndef GHELP
#include "TunningConstants.h"
#include "Primitives.h"


namespace GeometryHelper
{
class Transformation2D
{
private:
  Real teta, coteta, siteta;
  Vector2D u;

public:
  Transformation2D();
  Transformation2D(const Transformation2D &t);
  Transformation2D(const Vector2D &u, Real teta);

  inline void setTeta(Real t);
  inline void addTeta(Real t);
  inline Real getTeta() const;
  inline void setU(const Vector2D &nu);
  inline void addU(const Vector2D &nu);
  inline Vector2D getU() const;

  inline Vector2D transform(const Vector2D &x) const;
  inline Point2D transform(const Point2D &x) const;
  inline Vector2D transforminv(const Vector2D &x) const;
  inline Point2D transforminv(const Point2D &x) const;

  inline Vector2D rotate(const Vector2D &x) const;
  inline Point2D rotate(const Point2D &x) const;
  inline Vector2D rotateinv(const Vector2D &x) const;
  inline Point2D rotateinv(const Point2D &x) const;

  inline Vector2D translate(const Vector2D &x) const;
  inline Point2D translate(const Point2D &x) const;
  inline Vector2D translateinv(const Vector2D &x) const;
  inline Point2D translateinv(const Point2D &x) const;
};
}
inline void GeometryHelper::Transformation2D::setTeta(Real t)
{
  teta = t;
  coteta = cos(t);
  siteta = sin(t);
}
inline void GeometryHelper::Transformation2D::addTeta(Real t)
{
  setTeta(teta + t);
}
inline Real GeometryHelper::Transformation2D::getTeta() const
{
  return teta;
}
inline void GeometryHelper::Transformation2D::setU(const Vector2D &v)
{
  u = v;
}
inline void GeometryHelper::Transformation2D::addU(const Vector2D &v)
{
  u += v;
}
inline Vector2D GeometryHelper::Transformation2D::getU() const
{
  return u;
}

inline Vector2D GeometryHelper::Transformation2D::translate(const Vector2D &x) const
{
  return x + u;
}
inline Point2D GeometryHelper::Transformation2D::translate(const Point2D &x) const
{
  return x + u;
}
inline Vector2D GeometryHelper::Transformation2D::translateinv(const Vector2D &x) const
{
  return x - u;
}
inline Point2D GeometryHelper::Transformation2D::translateinv(const Point2D &x) const
{
  return x - u;
}
inline Vector2D GeometryHelper::Transformation2D::rotate(const Vector2D &x) const
{
  Real X = x.getX(), Y = x.getY();
  return Vector2D(X * coteta + Y * siteta, Y * coteta - X * siteta);
}
inline Point2D GeometryHelper::Transformation2D::rotate(const Point2D &x) const
{
  Real X = x.getX(), Y = x.getY();
  return Point2D(X * coteta + Y * siteta, Y * coteta - X * siteta);
}
inline Vector2D GeometryHelper::Transformation2D::rotateinv(const Vector2D &x) const
{
  Real X = x.getX(), Y = x.getY();
  return Vector2D(X * coteta - Y * siteta, Y * coteta + X * siteta);
}
inline Point2D GeometryHelper::Transformation2D::rotateinv(const Point2D &x) const
{
  Real X = x.getX(), Y = x.getY();
  return Point2D(X * coteta - Y * siteta, Y * coteta + X * siteta);
}
inline Vector2D GeometryHelper::Transformation2D::transform(const Vector2D &x) const
{
  Real X = x.getX(), Y = x.getY();
  return Vector2D(X * coteta + Y * siteta + u.getX(), Y * coteta - X * siteta  + u.getY());
}
inline Point2D GeometryHelper::Transformation2D::transform(const Point2D &x) const
{
  Real X = x.getX(), Y = x.getY();
  return Point2D(X * coteta + Y * siteta + u.getX(), Y * coteta - X * siteta  + u.getY());
}
inline Vector2D GeometryHelper::Transformation2D::transforminv(const Vector2D &x) const
{
  Real X = x.getX() - u.getX(), Y = x.getY() - u.getY();
  return Vector2D(X * coteta - Y * siteta, Y * coteta + X * siteta);
}
inline Point2D GeometryHelper::Transformation2D::transforminv(const Point2D &x) const
{
  Real X = x.getX() - u.getX(), Y = x.getY()  - u.getY();
  return Point2D(X * coteta - Y * siteta, Y * coteta + X * siteta);
}
#define GHELP
#endif


