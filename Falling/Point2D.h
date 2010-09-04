#ifndef POINT
#include "Vector2D.h"
			 
typedef struct Point2D Point2D;
struct Point2D
{
private:
	float x,y;

public:
	Point2D(float x = 0,float y = 0);

	inline float getX() const;
	inline void setX(float x);
	inline float getY() const;
	inline void setY(float y);

	inline float isLeftTo(const Point2D &p,const Point2D &p2);
	bool isInCWTriangle(const Point2D &p,const Point2D &p2, const Point2D &p3);
	bool isInCCWTriangle(const Point2D &p,const Point2D &p2, const Point2D &p3);   
	bool isInUnorientedTriangle(const Point2D &p,const Point2D &p2, const Point2D &p3);		   

	inline bool equals(const Point2D &p);
	inline void operator+=(const Point2D &p);
	inline void operator-=(const Point2D &p);
	inline void operator+=(Vector2D &p);
	inline void operator-=(Vector2D &p);	
	inline Point2D operator+(const Point2D &p);
	inline Point2D operator-(const Point2D &p);	
	inline Point2D operator+(Vector2D &v);
	inline Point2D operator-(Vector2D &v);
};


inline float Point2D::getX() const
{ return x; }
inline float Point2D::getY() const
{ return y; }
inline void Point2D::setX(float x)
{ this->x= x; }
inline void Point2D::setY(float y)
{ this->y= y; }

inline float Point2D::isLeftTo(const Point2D &p,const Point2D& p2)
{
	float px = p.getX(), py = p.getY();
	return (p2.getX() - px) * (y - py) - (p2.getY() - py) * (x - px);
}

inline bool Point2D::equals(const Point2D &p)
{ return p.x == x && p.y == y; }

inline Point2D Point2D::operator +(const Point2D &p)
{ return Point2D(x+p.x,y+p.y); }
inline Point2D Point2D::operator -(const Point2D &p)
{ return Point2D(x-p.x,y-p.y); }  
inline Point2D Point2D::operator +(Vector2D &v)
{ return Point2D(x+v.getX(),y+v.getY()); }
inline Point2D Point2D::operator -(Vector2D &v)
{ return Point2D(x-v.getX(),y-v.getY()); }

inline void Point2D::operator +=(const Point2D &p)
{ x+=p.x; y+=p.y; }
inline void Point2D::operator -=(const Point2D &p)
{ x-=p.x; y-=p.y; }		  
inline void Point2D::operator +=(Vector2D &v)
{ x+=v.getX(); y+=v.getY(); }
inline void Point2D::operator -=(Vector2D &v)
{ x-=v.getX(); y-=v.getY(); }
#define POINT
#endif
