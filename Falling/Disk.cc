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
#include "stdafx.hh"
#include "Disk.hh"

namespace Falling
{
  Disk::Disk(const Point2D &pt, Real dradius, bool fixed)
  {
    deleting = false;
    parent = this;
    fixedobj = fixed;
    radius = dradius + PROXIMITY_AWARENESS;
    margin = radius;//0.04;
    radiuslessM = 0;//radius - margin;
    t = GeometryHelper::Transformation2D(Vector2D (pt), 0);
    otree = new OBBtree(0, 0, new OBB(Point2D(-radius, radius),
                                      Point2D(radius, radius),
                                      Point2D(radius, -radius),
                                      Point2D(-radius, -radius),
                                      this,
                                      radius * radius,
                                      0));
  }

  void Disk::updateAABB()
  {
    Vector2D p = getCenter();
    aabb_xm = -radius + p.getX();
    aabb_xM = radius + p.getX();
    aabb_ym = -radius + p.getY();
    aabb_yM = radius + p.getY();
  }

  Real Disk::getBoundingSphereRadius() const
  {
    return radius;
  }

  Real Disk::getSurface() const
  {
    return (Real)M_PI * radius * radius;
  }

  Real Disk::getInertiaMomentum(Real m) const
  {
    return m * radius * radius / 2;
  }

  Vector2D Disk::getCenter() const
  {
    return t.getU();
  }

  int Disk::getSupportPoint(const Vector2D &, Point2D *res) const
  {
    Vector2D v = getCenter();
    //v += d.direction() * radiuslessM;
    *res =  Point2D(v.getX(), v.getY());
    return 0;
  }
  int Disk::getSupportPoint(const Vector2D &, Point2D *res, int) const
  {
    Vector2D v = getCenter();
    //v += d.direction() * radiuslessM;
    *res =  Point2D(v.getX(), v.getY());
    return 0;
  }

  bool Disk::containsPoint(const Point2D &pt) const
  {
    Vector2D v(getCenter(), pt);
    return  v * v <= radius * radius;
  }
}
