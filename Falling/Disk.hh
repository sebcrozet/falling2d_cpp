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
#ifndef _DISK_H
# include "Shapes.hh"
# include "AABB.hh"
# include "OBBtree.hh"

namespace Falling
{
  class Disk : public ImplicitShape, public Shape
  {
    private:
      Real radiuslessM;
      Real radius;
    public:
      Disk(const Point2D& pt, Real radius, bool fixed);
      inline Real getRadius() const;

      // Implicit shape methods
      virtual Real getBoundingSphereRadius() const;
      virtual Vector2D getCenter() const;
      virtual int getSupportPoint(
          const Vector2D& d,
          Point2D* res,
          int optimisationId) const;
      virtual int getSupportPoint(const Vector2D& d, Point2D* res) const;

      // Shape methods
      virtual inline int getShapeTypeID() const;
      virtual Real getSurface() const;
      virtual Real getInertiaMomentum(Real m) const;
      virtual void updateAABB();
      virtual bool containsPoint(const Point2D& pt) const;
  };

  inline Real Disk::getRadius() const
  {
    return radius;
  }
  inline int Disk::getShapeTypeID() const
  {
    return 1;
  }
}
# define _DISK_H
#endif
