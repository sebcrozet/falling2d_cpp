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
#ifndef __COLLISION_ARBITER__
#include "TunningConstants.h"
#include "CollisionDetector.h"
#include "DiskDiskSolver.h"
#include "GenericSolver.h"
#include "PlaneShapeSolver.h"
#include "FakeShape.h"
#include "RigidBody.h"
#include "ContactBatch.h"

namespace Falling
{
  struct Contact;
  struct QuarterSpace;
  struct Collision
  {
    Shape *sa, *sb;
    std::vector<ContactBackup *> c;
    Contact **cnts; // Contact *[]
    CollisionDetector *cd;	 // Specific collision solver
    Collision *preva,*prevb,*nexta,*nextb;
    int collisionStackLevel;

    Collision(Shape *sa, Shape *sb);
    Collision(Shape *s);
    ~Collision();
    void removeFromList();
    void autoInsert();
    void clearContacts();
  };


  class CollisionArbiter
  {
    private:
      std::vector<Shape *> s;
      SAPsolver sap;

      static void addP(Pair *p, Shape *s, Shape *s2);
      static void deleteP(Pair &p);
      static bool removeP(Pair *p);
    public:
      CollisionArbiter();
      ~CollisionArbiter();
      void addObject(Shape *s);
      void deleteObject(Shape *s);
      void solve(std::vector<Collision *> &res);
      void notifyObjectMoved(Shape *s);
  };
}
#define __COLLISION_ARBITER__
#endif
