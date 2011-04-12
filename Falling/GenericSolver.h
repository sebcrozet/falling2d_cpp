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
// Generic solver using hybrid GJK-EPA collision detection/penetration depth calculator
// Compute obb-tree intersection, and then apply hybrid distance solver to every sub-shape
//
#ifndef __GENERIC_SOLVER__
#include "CollisionDetector.h"
#include "PairManager.h"
#include "OBBtree.h"
#include "DistanceSolver.h"
#include "ContactBatch.h"
#define GS_IDBIGOFFSET 2063 // il ne devrait pas y avoir plus de GS_IDBIGOFFET OBB dans un arbre. GS_IDBIGOFFET est un nombre premier (réduction de collisions dans le pm?). 

class GenericSolver	: public CollisionDetector
{
private:
  Shape *s1, *s2;
  PairManager pm;	// cash all sub-shape collision couple

  static void deletePair(Pair &p);
  bool _solve(std::vector<ContactBackup *> &res);
  bool canDestroy();
public:
  GenericSolver(Shape *s1, Shape *s2);
};
#define __GENERIC_SOLVER__
#endif
