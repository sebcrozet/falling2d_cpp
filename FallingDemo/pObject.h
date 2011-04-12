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
#ifndef __FALLING_DEMO_POBJECT
#include "stdafx.h"
#include "State.h"

#define SCALE 1.0f

typedef struct pObject pObject;
struct pObject
{
public:
  enum ObjectType
  {
    O_CIRCLE,
    O_POLY,
    O_PLANE
  };
public:
  Falling::Tesselator *t;
  Falling::Vector2D u;
  Falling::Polygon2D * p;
  Falling::Disk * d;
  Falling::RigidBody *rb;
  Falling::Point2D * pts;
  int nb, support, r, g, b;
  ObjectType otype;
  int drawLimit;
  Falling::Point2D diskcenter;
  static sf::RenderWindow *rwin;
  static int icall;
  static float polx,poly;

  pObject(
	  Falling::Point2D * pts,
	  int n,
	  bool iscircle, 
	  Falling::World &coll, 
	  ObjectType type,
	  Falling::Point2D center = Falling::Point2D()
	  );
  void draw(const MachineState &ms);
  void toogleFixed();
  inline void incrementLimit(int inc)
  {
    drawLimit += inc;
    if(drawLimit > nb)
      drawLimit = nb;
    else if(drawLimit < 1)
      drawLimit = 1;
  }
};

#define __FALLING_DEMO_POBJECT
#endif
