/* Copyright (C) 2011 CROZET Sébastien

 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */
#ifndef POINT
#include "TunningConstants.h"
#include "floatTests.h"
#include "Vector2D.h"

namespace Falling
{
    typedef struct Point2D Point2D;
    struct FALLINGAPI Point2D
    {
	private:
	    Real x,y;

	public:
	    Point2D(Real x = 0,Real y = 0);
	    Point2D(const Vector2D &v);

	    inline Real getX() const;
	    inline void setX(Real x);
	    inline Real getY() const;
	    inline void setY(Real y);

	    inline Real isLeftTo(const Point2D &p,const Point2D &p2) const;
	    inline Real isLeftTo(const Point2D &p,const Point2D &p2, int onsegmentvalue) const;
	    inline Real isInLine(const Point2D &p,const Point2D& p2) const;
	    static Real intersectSegments(const Point2D &p, const Point2D &p2, const Point2D &p3, const Point2D &p4, Point2D *res, Real *bparam2);
	    static bool intersectLines(const Point2D &p, const Point2D &p2, const Point2D &p3, const Point2D &p4, Point2D *res);
	    bool isInCWTriangle(const Point2D &p,const Point2D &p2, const Point2D &p3) const;
	    bool isInCCWTriangle(const Point2D &p,const Point2D &p2, const Point2D &p3) const;
	    bool isInUnorientedTriangle(const Point2D &p,const Point2D &p2, const Point2D &p3) const;

	    //inline bool equals(const Point2D &p) const;
	    inline bool exactEquals(const Point2D &p) const;
	    inline bool errorEquals(const Point2D &p) const;
	    inline void operator+=(const Point2D &p);
	    inline void operator-=(const Point2D &p);
	    inline void operator+=(const Vector2D &p);
	    inline void operator-=(const Vector2D &p);
	    inline Point2D operator+(const Point2D &p) const;
	    inline Point2D operator-(const Point2D &p) const;
	    inline Point2D operator+(const Vector2D &v) const;
	    inline Point2D operator-(const Vector2D &v) const;
	    inline Point2D operator-(Real v) const;
	    inline Point2D operator*(const Real &f) const
	    {
		return Point2D(x*f,y*f);
	    }

	    static inline Point2D getMiddle(const Point2D &pa,const Point2D &pb);
	    // determine if a point is included in a polygon
	    static bool pointInPolygon(const Point2D &pt, Point2D *pts, int nbpts);
    };


    inline Real Point2D::getX() const
    {
	return x;
    }
    inline Real Point2D::getY() const
    {
	return y;
    }
    inline void Point2D::setX(Real x)
    {
	this->x= x;
    }
    inline void Point2D::setY(Real y)
    {
	this->y= y;
    }

    inline Real Point2D::isLeftTo(const Point2D &p,const Point2D& p2) const
    {
	Real px = p.getX(), py = p.getY();
	if(this->isInLine(p,p2))
	    return 0.0;
	return (p2.getX() - px) * (y - py) - (p2.getY() - py) * (x - px);
    }

    inline Real Point2D::isInLine(const Point2D &p,const Point2D& p2) const
    {
	Real px = p.getX(), py = p.getY();
	Real a,b;
	a = (p2.getX() - px) * (y - py);
	b = (p2.getY() - py) * (x - px);
	return Float::equal(a , b);
	/*
	   Vector2D pp2(p,p2);
	   Vector2D ppo(p,*this);
	   Real dot = (pp2 * ppo);
	   dot *= dot;
	   dot /= pp2 * pp2;
	   return (ppo * ppo - dot) <= POINT_EQUALS_ERROR;
	   */



    }
    inline Real Point2D::isLeftTo(const Point2D &p,const Point2D& p2, int onsegmentvalue) const
    {
	Vector2D cp(p,*this), pp2(p,p2);
	Real res = pp2.perp(cp);
	if(res == 0)
	{
	    Real dot = cp * cp;
	    if(dot >= 0 && dot <= pp2 * pp2)
		res = onsegmentvalue;
	}
	return res;
    }

    //inline bool Point2D::equals(const Point2D &p) const
    //{ return (ABS(p.x - x)<0.000001) && (ABS(p.y - y)<0.000001); }

    inline bool Point2D::errorEquals(const Point2D &p) const
    {
	return (p.x - x) * (p.x - x) + (p.y - y) * (p.y - y) <= POINT_EQUALS_ERROR;
    }

    inline bool Point2D::exactEquals(const Point2D &p) const
    {
	return p.x == x && p.y == y;
    }

    inline Point2D Point2D::operator +(const Point2D &p) const
    {
	return Point2D(x+p.x,y+p.y);
    }
    inline Point2D Point2D::operator -(const Point2D &p) const
    {
	return Point2D(x-p.x,y-p.y);
    }
    inline Point2D Point2D::operator +(const Vector2D &v) const
    {
	return Point2D(x+v.getX(),y+v.getY());
    }
    inline Point2D Point2D::operator -(const Vector2D &v) const
    {
	return Point2D(x-v.getX(),y-v.getY());
    }
    inline Point2D Point2D::operator -(Real p) const
    {
	return Point2D(x-p,y-p);
    }

    inline void Point2D::operator +=(const Point2D &p)
    {
	x+=p.x;
	y+=p.y;
    }
    inline void Point2D::operator -=(const Point2D &p)
    {
	x-=p.x;
	y-=p.y;
    }
    inline void Point2D::operator +=(const Vector2D &v)
    {
	x+=v.getX();
	y+=v.getY();
    }
    inline void Point2D::operator -=(const Vector2D &v)
    {
	x-=v.getX();
	y-=v.getY();
    }
    inline Point2D Point2D::getMiddle(const Point2D &pa, const Point2D &pb)
    {
	return Point2D((pa.x+pb.x)/2.0,(pa.y+pb.y)/2.0);
    }
}
#define POINT
#endif
