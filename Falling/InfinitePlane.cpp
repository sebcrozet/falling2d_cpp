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
#include "stdafx.h"
#include "InfinitePlane.h"

namespace Falling
{
    InfinitePlane::InfinitePlane(const Point2D &p, const Vector2D &normal)
    {
		otree = 0;
	deleting = false;
	parent = this;
	margin = 0.0;
	pt = p;
	dir = normal.direction();
	perpdir = Vector2D(-dir.getY(), dir.getX());
	spt = p + perpdir;
	fixedobj = true;
    }

    Real InfinitePlane::getBoundingSphereRadius() const
    {
	return MACHINE_EPSILON / 2;
    }
    Vector2D InfinitePlane::getCenter() const
    {
	return pt;
    }
    int InfinitePlane::getSupportPoint(const Vector2D &, Point2D *res) const
    {
	*res = pt;
	return 0;
    }
    int InfinitePlane::getSupportPoint(const Vector2D &, Point2D *res, int) const
    {
	*res = pt;
	return 0;
    }


    int InfinitePlane::getShapeTypeID() const
    {
	return 100;
    }
    Real InfinitePlane::getInertiaMomentum(Real) const
    {
	return MACHINE_MAX;
    }
    Real InfinitePlane::getSurface() const
    {
	return MACHINE_MAX;
    }
    void InfinitePlane::updateAABB()
    {
	if(dir.getX() == 0)
	{
	    if(dir.getY() < 0)
	    {
		aabb_ym = pt.getY();
		aabb_yM = MACHINE_MAX / 2;
	    }
	    else
	    {
		aabb_yM = pt.getY();
		aabb_ym = -MACHINE_MAX / 2;
	    }
	    aabb_xm = - MACHINE_MAX / 2;
	    aabb_xM = MACHINE_MAX / 2;
	}
	else if(dir.getY() == 0)
	{
	    if(dir.getX() < 0)
	    {
		aabb_xm = pt.getX();
		aabb_xM = MACHINE_MAX / 2;
	    }
	    else
	    {
		aabb_xM = pt.getX();
		aabb_xm = - MACHINE_MAX / 2;
	    }
	    aabb_ym = - MACHINE_MAX / 2;
	    aabb_yM = MACHINE_MAX / 2;
	}
	else
	{
	    aabb_ym = - MACHINE_MAX / 2;
	    aabb_yM = MACHINE_MAX / 2;
	    aabb_xm = - MACHINE_MAX / 2;
	    aabb_xM = MACHINE_MAX / 2;
	}
    }

    bool InfinitePlane::containsPoint(const Point2D &testpt) const
    {
	return testpt.isLeftTo(pt, spt) >= 0;
    }
}
