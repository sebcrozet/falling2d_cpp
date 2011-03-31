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
  Tesselator *t;
  Vector2D u;
  Polygon2D * p;
  Disk * d;
  RigidBody *rb;
  Point2D * pts;
  int nb, support, r, g, b;
  ObjectType otype;
  int drawLimit;
  Point2D diskcenter;
  static sf::RenderWindow *rwin;
  static int icall;

  pObject(Point2D * pts,int n,bool iscircle, World &coll, ObjectType type,Point2D center = Point2D());
  void draw(const MachineState &ms);
  void toogleFixed();
  // TODO: remove
  static void drawp(sf::RenderWindow &rw, Poly *pol);
  static float polx,poly;
  // end todo
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
