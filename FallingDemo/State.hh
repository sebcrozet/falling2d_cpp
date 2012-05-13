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
#ifndef __FALLING_DEMO_STATE
#include "stdafx.hh"
#include "Falling.hh"
#include "DistanceSolver.hh"
#include "Disk.hh"
#include "PairManager.hh"
#include "Tesselator.hh"
#include <ctime>
#include <cstdlib>

struct pObject;
typedef struct MachineState MachineState;
struct MachineState
{
  enum LeftButtonState
  {
    SELECTION,
    DRAW_POINTS,
    DRAW_MOVE,
    DRAW_CIRCLE,
    DRAW_CIRCLE_CONTINUOUS,
    DRAW_SQUARE,
    DRAW_PLANE
  };
  enum DrawState
  {
    DRAW_COMPONENTS = 1,
    DRAW_BORDERS = 2,
    DRAW_AABBS = 4,
    DRAW_OBB_TREE = 8,
    DRAW_SLEEPING_BLACK = 16,
    DRAW_OBB_LEAVES = 32,
    DRAW_COLLISION_COUPLES = 64
  };

  inline MachineState(sf::RenderWindow &renderWin)
    : rwin(renderWin), 
      buttonstate(MachineState::DRAW_MOVE), 
      drawstate(0),
      lbuttondown(false), 
      rbuttondown(false)
  { }


  bool running;
  // physics engine
  Falling::World w;
  // objects in the world
  sf::RenderWindow &rwin;
  std::vector<pObject *> objs;
  std::vector<Falling::Point2D> vpts;
  // logical state of the engine
  LeftButtonState buttonstate;
  int drawstate;
  bool lbuttondown;
  bool rbuttondown;
  // object selected (-1 if no object selected)
  pObject *selectedObj;
  // environment dragging
  float oldx;
  float oldy;
  // 
  float elapsedPhysicsTime;
  //
  std::vector<Falling::Collision *> colls;
};

class UserInterface;
typedef struct WholeState WholeState;
struct WholeState
{
  MachineState &ms;
  UserInterface &ui;
  inline WholeState(MachineState &machine, UserInterface &interface)
    : ms(machine), ui(interface)
  { }
};
#define __FALLING_DEMO_STATE
#endif
