#ifndef __FALLING_DEMO_STATE
#include "stdafx.h"
#include "Falling.h"
#include "DistanceSolver.h"
#include "Disk.h"
#include "PairManager.h"
#include "Tesselator.h"
#include <ctime>
#include <cstdlib>

struct pObject;
typedef struct MachineState MachineState;
struct MachineState
{
  enum LeftButtonState
  {
    SELECTION,
    JOINT_FIX,
    DRAW_POINTS,
    DRAW_MOVE,
    DRAW_CIRCLE,
    DRAW_SQUARE,
    DRAW_PLANE
  };
  enum DrawState
  {
    DRAW_COMPONENTS = 1,
    DRAW_BORDERS = 2
  };

  inline MachineState(sf::RenderWindow &renderWin)
    : rwin(renderWin), buttonstate(MachineState::DRAW_MOVE), lbuttondown(false), rbuttondown(false)
  { }


  bool running;
  // physics engine
  World w;
  // objects in the world
  sf::RenderWindow &rwin;
  std::vector<pObject *> objs;
  std::vector<Point2D> vpts;
  // logical state of the engine
  LeftButtonState buttonstate;
  int drawstate;
  bool lbuttondown;
  bool rbuttondown;
  // environment dragging
  float oldx;
  float oldy;
  //
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
