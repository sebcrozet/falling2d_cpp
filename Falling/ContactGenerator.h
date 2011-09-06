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
#ifndef CGEN
#include "CollisionArbiter.h"
#include "RigidBody.h"
#include <vector>

namespace Falling
{
  struct Contact
  {
    /*
       Datas for LCP solving (jacobian).
       */
    Real jacobian[6]; // v1.x v1.y w1 v2.x v2.y w2
    Real jacobian_indices[6]; // indices of objects
    Real bounds[2]; // bounds for lambda
  };

  class ContactGenerator
  {
    private:
    public:
      static void PrepareContactDatasInMatrix(Real dt, Collision *c, Real *&J, Real *&bounds, Real *&zeta, Real *&lambda, int *&idx);
      static void PrepareContactDatasInMatrix_without_sleeping(Real dt, Collision *c, Real *&J, Real *&bounds, Real *&zeta, Real *&lambda, int *&idx);
  };
}
#define CGEN
#endif
