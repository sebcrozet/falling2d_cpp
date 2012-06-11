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
#ifndef _INFINITEPLANE_H
# include "Shapes.hh"

namespace Falling
{
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
      virtual int getSupportPoint(const Vector2D& d,
                                  Point2D*        res,
                                  int             optimisationId) const;
      virtual int getSupportPoint(const Vector2D &d, Point2D *res) const;
      /*
         void getContinuousSupportPoint(const Vector2D &, Point2D *res, int *, int *);
         void getContinuousSupportPoint_with_opt_values(const Vector2D &, Point2D *res, int *, int *);
         */

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
    return pt + (dir * (PROXIMITY_AWARENESS + 0.5)) +
                 (perpdir * (ao * perpdir));
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
}
# define _INFINITEPLANE_H
#endif
