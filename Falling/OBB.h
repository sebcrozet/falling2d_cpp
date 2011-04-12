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
#define DEBUGMOD
#ifndef _OBB
#include "TunningConstants.h"
#include "Shapes.h"
#include "InfinitePlane.h"

class OBB
{
#ifdef DEBUGMOD
public:
#else
private:
#endif
  ImplicitShape *parent;
  Vector2D pts[4];
  Vector2D axis[2];
  Real origin[2];
  Real aire;
  int obbid;

  bool intersects2axis(OBB *o);
public:
  OBB(Point2D a,Point2D b,Point2D c,Point2D d,ImplicitShape *parent, Real aire, int obbid);
  bool intersects(OBB *o);
  bool intersectsPlane(InfinitePlane *p);
  void getMedialAxis(Point2D *center, Point2D *refpt);
  Point2D getCenter() const;
  void translate(const Vector2D &);
  inline Real getAire() const;
  inline int getID() const;
  inline ImplicitShape *getParent() const;
};
inline Real OBB::getAire() const
{
  return aire;
}
inline int OBB::getID() const
{
  return obbid;
}
inline ImplicitShape *OBB::getParent() const
{
  return parent;
}

#define _OBB
#endif

