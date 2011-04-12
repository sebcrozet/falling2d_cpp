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
#include "stdafx.h"
#include "pObject.h"

#define SCALE 1.0f

float pObject::poly = 0;
float pObject::polx = 0;
int pObject::icall = 0;

pObject::pObject(Falling::Point2D *pts,int n,bool iscircle, Falling::World &w, ObjectType cir,Falling::Point2D center)  : pts(pts), support(0), diskcenter(center)
{
  otype = cir;

  nb = n;
  drawLimit = nb;
  switch (otype)
    {
    case pObject::O_POLY:
      rb = Falling::RigidBody::build_polygonalBody(
             pts,
             n,
             iscircle,
             10.f,
             Falling::Vector2D(),
             0.f
           );
      u = rb->getPos();
      for(int i=0; i<n; i++)
        pts[i] -= u;
      p = (Falling::Polygon2D *)(rb->getShape());
      w.addObject(rb);
      break;
    case pObject::O_CIRCLE:
      rb = Falling::RigidBody::build_circularBody(
             center,
             nb,
             iscircle,
             nb,
             Falling::Vector2D(center),
             0.f
           );
      d = (Falling::Disk *)(rb->getShape());
      w.addObject(rb);
      break;
    case pObject::O_PLANE:
      rb = Falling::RigidBody::build_planarBody(center, Falling::Vector2D(center, pts[0]));
      w.addObject(rb);
      break;
    }

  srand(time(0));
  if(rb->isFixed())
    {
      r = 180;
      g = 180;
      b = 180;
    }
  else
    {
      r = rand()%176 + 80;
      g = rand()%176 + 80;
      b = rand()%176 + 80;
    }
}

void pObject::toogleFixed()
{
  rb->setFixed(!rb->isFixed());
  srand(time(0));
  if(rb->isFixed())
    {
      r = 180;
      g = 180;
      b = 180;
    }
  else
    {
      r = rand()%176 + 80;
      g = rand()%176 + 80;
      b = rand()%176 + 80;
    }
}

void pObject::draw(const MachineState &ms)
{
  sf::Shape sh;
  Falling::Vector2D pos;

  icall = 0;
  poly = rb->getPos().getY();
  polx = rb->getPos().getX();
  switch(otype)
    {
    case pObject::O_CIRCLE:
      pos = rb->getPos();
      sh = sf::Shape::Circle(
             pos.getX()*SCALE,
             pos.getY()*SCALE,
             nb,
             sf::Color(r,g,b),
             ms.selectedObj == this ? 4.f : (ms.drawstate & MachineState::DRAW_BORDERS) ? 3.f : 0.f,
             ms.selectedObj == this ? sf::Color(255, 255, 255) : sf::Color(r - 70, g - 70,b - 70)
           );
      ms.rwin.Draw(sh);
      ms.rwin.Draw(
        sf::Shape::Circle(
          pos.getX(),
          pos.getY(),
          3, sf::Color(r - 70, g - 70, b - 70), 2
        )
      );
      break;
    case pObject::O_POLY:
      if(p->nbrSubShapes)
        {
          if(!rb->isFixed() || (ms.drawstate & MachineState::DRAW_COMPONENTS))
            {
              for(int j = 0; j < p->nbrSubShapes; j++)
                {
                  sf::Shape sh;
                  int rrand;
                  int grand;
                  int brand;
                  if(ms.drawstate & MachineState::DRAW_COMPONENTS)
                    {
                      srand(r+j);
                      rrand = rand()%255;
                      grand = rand()%255;
                      brand = rand()%255;
                    }
		  else if(rb->isSleeping())
		  {
		      rrand = grand = brand = 0;
		  }
		  else
                    {
                      rrand = r;
                      grand = g;
                      brand = b;
                    }
                  for(int i = 0; i < p->subShapes[j]->nbrPts; i++)
                    {
                      Falling::Point2D ptsi = p->toGlobal(p->subShapes[j]->pts[i]);
                      sh.AddPoint(
                        ptsi.getX()*SCALE,
                        ptsi.getY()*SCALE,
                        sf::Color(rrand,grand,brand)
                      );
                    }
                  ms.rwin.Draw(sh);
                }
            }
          else
            {
              for(int j = 0; j < p->nbrSubShapes; j++)
                {
                  sf::Shape sh;
                  for(int i = 0; i < p->subShapes[j]->nbrPts; i++)
                    {
                      Falling::Point2D ptsi = p->toGlobal(p->subShapes[j]->pts[i]);
                      sh.AddPoint(
                        ptsi.getX()*SCALE,
                        ptsi.getY()*SCALE,
                        sf::Color(r,g,b)
                      );
                    }
                  ms.rwin.Draw(sh);
                }
            }
        }
      {
        int j = nb-1;
        if(ms.drawstate & MachineState::DRAW_BORDERS || ms.selectedObj == this)
          {

            for(int i = 0; i < nb; j = i,i++)
              {
                Falling::Point2D vptsi = p->toGlobal(pts[j]);
                Falling::Point2D vptsi1 = p->toGlobal(pts[i]);
                ms.rwin.Draw(
                  sf::Shape::Line(
                    vptsi.getX()*SCALE,
                    vptsi.getY()*SCALE,
                    vptsi1.getX()*SCALE,
                    vptsi1.getY()*SCALE,
                    ms.selectedObj == this ? 4.f : 3.f,
                    ms.selectedObj == this ? sf::Color(255, 255, 255) : sf::Color(r - 70, g - 70,b - 70)
                  )
                );
              }
          }
      }
      ms.rwin.Draw(
        sf::Shape::Circle(
          p->toGlobal(Falling::Point2D(0,0)).getX(),
          p->toGlobal(Falling::Point2D(0,0)).getY(),
          1, sf::Color(r - 70, g - 70, b - 70),2
        )
      );
      break;
    case pObject::O_PLANE:
      break;
    }
}
