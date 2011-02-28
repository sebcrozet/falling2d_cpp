#ifndef POINT
#include "TunningConstants.h"
#include "Vector2D.h"
			 
typedef struct Point2D Point2D;
struct FALLINGAPI Point2D
{
private:
	float x,y;

public:
	Point2D(float x = 0,float y = 0);
	Point2D(Vector2D &v);

	inline float getX() const;
	inline void setX(float x);
	inline float getY() const;
	inline void setY(float y);

	inline float isLeftTo(const Point2D &p,const Point2D &p2);   
	inline float isLeftTo(const Point2D &p,const Point2D &p2, int onsegmentvalue);
	inline float isInLine(const Point2D &p,const Point2D& p2);
	static float intersectSegments(Point2D &p,Point2D &p2,Point2D &p3,Point2D &p4, Point2D *res, float *bparam2);
	bool isInCWTriangle(const Point2D &p,const Point2D &p2, const Point2D &p3);
	bool isInCCWTriangle(const Point2D &p,const Point2D &p2, const Point2D &p3);   
	bool isInUnorientedTriangle(const Point2D &p,const Point2D &p2, const Point2D &p3);		   

	inline bool equals(const Point2D &p);
	inline bool exactEquals(const Point2D &p);
	inline void operator+=(const Point2D &p);
	inline void operator-=(const Point2D &p);
	inline void operator+=(Vector2D &p);
	inline void operator-=(Vector2D &p);	
	inline Point2D operator+(const Point2D &p);
	inline Point2D operator-(const Point2D &p);	
	inline Point2D operator+(Vector2D &v);
	inline Point2D operator-(Vector2D &v);
	inline Point2D operator*(const float &f)
	{ return Point2D(x*f,y*f); }

	static inline Point2D getMiddle(const Point2D &pa,const Point2D &pb);
	// determine if a point is included in a polygon
	static bool pointInPolygon(Point2D pt, Point2D *pts, int nbpts);
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

inline float Point2D::isInLine(const Point2D &p,const Point2D& p2)
{
	float px = p.getX(), py = p.getY();
	float xx = abs(p.getX() - p2.getX());
	float yy = abs(p.getY() - p2.getY());
	float a,b;
	if(x - px == 0)
	{
		a = p2.getX();
		b = x;
	}
	else if(p2.getX() - px == 0)
	{
		a = x;
		b = px;
	}
	else
	{
		a = /*(p2.getY() - py)/(p2.getX() - px);*/(p2.getX() - px) * (y - py);
		b = /*(y-py)/(x-px);*/(p2.getY() - py) * (x - px);
	}
	printf("%f vs %f\n",sqrt((double)FLT_EPSILON) * max(abs(a),max(abs(b),1.0)), abs(a-b));
	return abs(a - b) <= sqrt(FLT_EPSILON) * max(abs(a),max(abs(b),1.0));
}						
inline float Point2D::isLeftTo(const Point2D &p,const Point2D& p2, int onsegmentvalue)
{
	float px = p.getX(), py = p.getY();
	Vector2D cp(p,*this), pp2(p,p2); 
	float res = pp2.perp(cp);
	if(res == 0)
	{
		float dot = cp * cp;
		if(dot >= 0 && dot <= pp2 * pp2)
			res = onsegmentvalue;
	}
	return res;
}

inline bool Point2D::equals(const Point2D &p)
{ return (abs(p.x - x)<0.000001) && (abs(p.y - y)<0.000001); }

inline bool Point2D::exactEquals(const Point2D &p)
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
inline Point2D Point2D::getMiddle(const Point2D &pa, const Point2D &pb)
{ return Point2D((pa.x+pb.x)/2.0f,(pa.y+pb.y)/2.0f); }
#define POINT
#endif
