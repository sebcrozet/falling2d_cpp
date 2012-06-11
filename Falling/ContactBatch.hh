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
#ifndef __FALLING_CONTACT_BATCH
# include "stdafx.hh"
# include "TunningConstants.hh"
# include "floatTests.hh"
# include "SubCollision.hh"
# include "FakeShape.hh"
# include <vector>

namespace Falling
{
  typedef struct ContactBackup ContactBackup;
  struct ContactBackup
  {
    Point2D relPtA;
    Point2D relPtB;
    Real depth;
    Vector2D normal;
    Real lambda; // used to warm-start the solver
    Real frictionlambda;

    inline ContactBackup()
      :relPtA(Point2D()), relPtB(Point2D()), depth(0),
       normal(Vector2D()), lambda(0.), frictionlambda(0.)
    { }
    inline ContactBackup(
        const Point2D& pa,
        const Point2D& pb,
        ImplicitShape* A,
        ImplicitShape* B
        )
      : lambda(0.), frictionlambda(0.)
    {
      setDatas(pa, pb, A, B);
    }

    inline void setDatas(
        const Point2D &pa,
        const Point2D &pb,
        ImplicitShape* A,
        ImplicitShape* B
        )
    {
      normal = Vector2D(pa, pb);
      depth = normal.normalise();
      relPtA = A->toLocal(pa);
      relPtB = B->toLocal(pb);
    }

    inline ContactBackup(
        const Point2D &pa,
        const Point2D &pb,
        const Vector2D &n,
        Real depth)
      : relPtA(pa),
      relPtB(pb),
      depth(depth),
      normal(n),
      lambda(0.),
      frictionlambda(0.)
    { }
  };

  class ContactBatch
  {
    private:
      std::vector<ContactBackup* > scol;
      ImplicitShape &A;
      ImplicitShape &B;
    public:
      inline ContactBatch(ImplicitShape &A, ImplicitShape &B)
        : A(A), B(B)
      { }
      ~ContactBatch();
      void addContact(const SubCollision &lastDeepestColl);
      void eraseCashedContacts();

      inline int getNbrContacts() const
      {
        //printf("%i batched\n", scol.size());
        return scol.size();
      }
      inline ContactBackup* getContacts(int i) const
      {
        return scol[i];
      }
  };
}
# define __FALLING_CONTACT_BATCH
#endif
