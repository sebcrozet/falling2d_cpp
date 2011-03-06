#ifndef VECTOR
#include "TunningConstants.h"
#include <float.h>
#include <stdio.h>
#define EPSILON	 FLT_EPSILON
typedef struct Vector2D Vector2D;
struct FALLINGAPI Point2D;
struct FALLINGAPI Vector2D
{
private:
	float x,y,z,padding;

public:
	Vector2D(const Vector2D &v);
	Vector2D(const Point2D &p);
	Vector2D(const Point2D &p1,const Point2D &p2);
	Vector2D(float x = 0,float y = 0, float z = 0);

	inline void print();
	inline float getX() const;
	inline float getY() const;
	inline float getZ() const;
	inline void setX(float _x);
	inline void setY(float _y);
	inline void setZ(float _z);

	inline bool isNull();
	inline bool isCollinear(const Vector2D &v);
	inline bool isOrthogonal(const Vector2D &v);
	inline bool equals(const Vector2D &v);

	inline void reflect();	 
	inline void ortho();
	inline float magnitude() const;
	inline float normalise();

	inline Vector2D reflexion() const;
	inline Vector2D direction() const;

	inline float dot(const Vector2D &v) const;
	inline float perp(const Vector2D &v) const;
	inline Vector2D cross(const Vector2D &v) const;		

	inline void operator+=(const Vector2D &u);
	inline void operator-=(const Vector2D &u);	
	inline void operator/=(float f);
	inline Vector2D operator+(const Vector2D &v) const;
	inline Vector2D operator-(const Vector2D &v) const;
	inline Vector2D operator^(const Vector2D &v) const;
	inline float operator*(const Vector2D &v) const;   
	inline Vector2D operator*(float v) const;	 
	inline Vector2D operator/(float f) const;
};

inline float Vector2D::getX() const
{ return x; }
inline float Vector2D::getY() const
{ return y; }
inline float Vector2D::getZ() const
{ return z; }
inline void Vector2D::setX(float _x)
{ x=_x; }	 
inline void Vector2D::setY(float _y)
{ y=_y; }
inline void Vector2D::setZ(float _z)
{ z=_z; }

inline void Vector2D::print()
{
	printf("%f",x);
	printf("  ");
	printf("%f",y);
	printf("  ");
	printf("%f",z);
	printf("\n");
}

inline bool Vector2D::isNull()
{ return ABS(x) < EPSILON && ABS(y) < EPSILON; }
inline bool Vector2D::isCollinear(const Vector2D &v)
{ return cross(v).isNull(); }
inline bool Vector2D::isOrthogonal(const Vector2D &v)
{ return dot(v) < EPSILON; }

inline void Vector2D::reflect()
{ y=-y;	x=-x; z=-z; }
inline float Vector2D::normalise()
{
	float m = magnitude();
	if(m < EPSILON)
	{ x=0; y=0; z=0; return 0;	}
	else
	{ x/=m;	y/=m; z/=m; return m; }
}
inline float Vector2D::magnitude() const
{ return sqrt(x*x+y*y+z*z); }
inline void Vector2D::ortho()
{
	float xx = x;
	x = -y;
	y = xx;
}
inline bool Vector2D::equals(const Vector2D &v)
{ return ((ABS(v.getX() - x))<=0.001 && (ABS(v.getY() - y))<=0.001); }

inline Vector2D Vector2D::cross(const Vector2D &v) const
{ return Vector2D(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x); }
inline Vector2D Vector2D::direction() const
{																  
	float m = magnitude();
	return (m<EPSILON)? Vector2D(0,0):Vector2D(x/m,y/m,z/m);
}
inline Vector2D Vector2D::reflexion() const
{ return Vector2D(-x,-y,-z); }
inline float Vector2D::dot(const Vector2D &v) const
{ return x*v.x+y*v.y+z*v.z; }
inline float Vector2D::perp(const Vector2D &v) const
{ return x*v.y-y*v.x; }


inline void Vector2D::operator +=(const Vector2D &u)
{ x+=u.x; y+=u.y; z+=u.z; }	
inline void Vector2D::operator -=(const Vector2D &u)
{ x-=u.x; y-=u.y; z-=u.z; }
inline void Vector2D::operator/=(float f)
{ x/=f; y/=f; z/=f; }

inline float Vector2D::operator *(const Vector2D &v) const
{ return x*v.x+y*v.y+z*v.z; }
inline Vector2D Vector2D::operator *(float v) const
{ return Vector2D(x*v, y*v,z*v); }	
inline Vector2D Vector2D::operator/(float f) const
{ return Vector2D(x / f, y / f, z / f); }

inline Vector2D Vector2D::operator +(const Vector2D &v) const
{ return Vector2D(x+v.x,y+v.y,z+v.z); }
inline Vector2D Vector2D::operator -(const Vector2D &v) const
{ return Vector2D(x-v.x,y-v.y,z-v.z); }	  
inline Vector2D Vector2D::operator ^(const Vector2D &v) const
{ return Vector2D(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x); }

#define VECTOR
#endif
