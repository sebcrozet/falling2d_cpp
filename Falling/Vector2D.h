#ifndef VECTOR
#include "TunningConstants.h"
#include "floatTests.h"
#include <stdio.h>
#define EPSILON	MACHINE_EPSILON
typedef struct Vector2D Vector2D;
struct FALLINGAPI Point2D;
struct FALLINGAPI Vector2D
{
private:
  Real x,y,z,padding;

public:
  Vector2D(const Vector2D &v);
  Vector2D(const Point2D &p);
  Vector2D(const Point2D &p1,const Point2D &p2);
  Vector2D(Real x = 0,Real y = 0, Real z = 0);

  inline void print();
  inline Real getX() const;
  inline Real getY() const;
  inline Real getZ() const;
  inline void setX(Real _x);
  inline void setY(Real _y);
  inline void setZ(Real _z);

  inline bool isNull();
  inline bool isCollinear(const Vector2D &v);
  inline bool isOrthogonal(const Vector2D &v);
  inline bool equals(const Vector2D &v);

  inline void reflect();
  inline void ortho();
  inline Real magnitude() const;
  inline Real normalise();

  inline Vector2D reflexion() const;
  inline Vector2D direction() const;

  inline Real dot(const Vector2D &v) const;
  inline Real perp(const Vector2D &v) const;
  inline Vector2D cross(const Vector2D &v) const;

  inline void operator+=(const Vector2D &u);
  inline void operator-=(const Vector2D &u);
  inline void operator/=(Real f);
  inline Vector2D operator+(const Vector2D &v) const;
  inline Vector2D operator-(const Vector2D &v) const;
  inline Vector2D operator^(const Vector2D &v) const;
  inline Real operator*(const Vector2D &v) const;
  inline Vector2D operator*(Real v) const;
  inline Vector2D operator/(Real f) const;
};

inline Real Vector2D::getX() const
{
  return x;
}
inline Real Vector2D::getY() const
{
  return y;
}
inline Real Vector2D::getZ() const
{
  return z;
}
inline void Vector2D::setX(Real _x)
{
  x=_x;
}
inline void Vector2D::setY(Real _y)
{
  y=_y;
}
inline void Vector2D::setZ(Real _z)
{
  z=_z;
}

inline void Vector2D::print()
{
	printf("%f %f %f\n", x, y, z);
}

inline bool Vector2D::isNull()
{
  return ABS(x) < EPSILON && ABS(y) < EPSILON;
}
inline bool Vector2D::isCollinear(const Vector2D &v)
{
  return cross(v).isNull();
}
inline bool Vector2D::isOrthogonal(const Vector2D &v)
{
  return dot(v) < EPSILON;
}

inline void Vector2D::reflect()
{
  y=-y;
  x=-x;
  z=-z;
}
inline Real Vector2D::normalise()
{
  Real m = magnitude();
  if(m < EPSILON)
    {
      x=0;
      y=0;
      z=0;
      return 0;
    }
  else
    {
      x/=m;
      y/=m;
      z/=m;
      return m;
    }
}
inline Real Vector2D::magnitude() const
{
  return sqrt(x*x+y*y+z*z);
}
inline void Vector2D::ortho()
{
  Real xx = x;
  x = -y;
  y = xx;
}
inline bool Vector2D::equals(const Vector2D &v)
{
  return (Float::equal(v.getX() , x) && Float::equal(v.getY() , y));
}

inline Vector2D Vector2D::cross(const Vector2D &v) const
{
  return Vector2D(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x);
}
inline Vector2D Vector2D::direction() const
{
  Real m = magnitude();
  return (m<EPSILON)? Vector2D(0,0):Vector2D(x/m,y/m,z/m);
}
inline Vector2D Vector2D::reflexion() const
{
  return Vector2D(-x,-y,-z);
}
inline Real Vector2D::dot(const Vector2D &v) const
{
  return x*v.x+y*v.y+z*v.z;
}
inline Real Vector2D::perp(const Vector2D &v) const
{
  return x*v.y-y*v.x;
}


inline void Vector2D::operator +=(const Vector2D &u)
{
  x+=u.x;
  y+=u.y;
  z+=u.z;
}
inline void Vector2D::operator -=(const Vector2D &u)
{
  x-=u.x;
  y-=u.y;
  z-=u.z;
}
inline void Vector2D::operator/=(Real f)
{
  x/=f;
  y/=f;
  z/=f;
}

inline Real Vector2D::operator *(const Vector2D &v) const
{
  return x*v.x+y*v.y+z*v.z;
}
inline Vector2D Vector2D::operator *(Real v) const
{
  return Vector2D(x*v, y*v,z*v);
}
inline Vector2D Vector2D::operator/(Real f) const
{
  return Vector2D(x / f, y / f, z / f);
}

inline Vector2D Vector2D::operator +(const Vector2D &v) const
{
  return Vector2D(x+v.x,y+v.y,z+v.z);
}
inline Vector2D Vector2D::operator -(const Vector2D &v) const
{
  return Vector2D(x-v.x,y-v.y,z-v.z);
}
inline Vector2D Vector2D::operator ^(const Vector2D &v) const
{
  return Vector2D(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x);
}

#define VECTOR
#endif
