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

Real pObject::poly = 0;
Real pObject::polx = 0;
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
      diskcenter = center;
      u = Falling::Vector2D(center, pts[0]);
      u.normalise();
      rb = Falling::RigidBody::build_planarBody(center, u);
      u = Falling::Vector2D(u.getY(), -u.getX());
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
             rb->isSleeping() && (ms.drawstate & MachineState::DRAW_SLEEPING_BLACK) ? sf::Color(0,0,0) : sf::Color(r,g,b),
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

      if(p->getNbrSubShapes())
        {
          if(!rb->isFixed() || (ms.drawstate & MachineState::DRAW_COMPONENTS))
            {
              for(int j = 0; j < p->getNbrSubShapes(); j++)
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
		  else if(rb->isSleeping() && (ms.drawstate & MachineState::DRAW_SLEEPING_BLACK))
		  {
		      rrand = grand = brand = 0;
		  }
		  else
                    {
                      rrand = r;
                      grand = g;
                      brand = b;
                    }
                  for(int i = 0; i < p->getSubShape(j)->getNbrPts(); i++)
                    {
                      Falling::Point2D ptsi = p->toGlobal(p->getSubShape(j)->getPts()[i]);
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
              for(int j = 0; j < p->getNbrSubShapes(); j++)
                {
                  sf::Shape sh;
                  for(int i = 0; i < p->getSubShape(j)->getNbrPts(); i++)
                    {
                      Falling::Point2D ptsi = p->toGlobal(p->getSubShape(j)->getPts()[i]);
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
      // build a list of all points on the screen
      // and insert the first point at the end of
      // the list also
      Falling::Point2D screenpts[5];
      sf::Vector2f cvpt;
      Falling::Point2D inter1;
      Falling::Point2D inter2;
      
      cvpt = ms.rwin.ConvertCoords(0, 0, &ms.rwin.GetDefaultView());
      screenpts[0] = Falling::Point2D(cvpt.x, cvpt.y);
      cvpt = ms.rwin.ConvertCoords(ms.rwin.GetWidth(), 0, &ms.rwin.GetDefaultView());
      screenpts[1] = Falling::Point2D(cvpt.x, cvpt.y);
      cvpt = ms.rwin.ConvertCoords(ms.rwin.GetWidth(), ms.rwin.GetHeight(), &ms.rwin.GetDefaultView());
      screenpts[2] = Falling::Point2D(cvpt.x, cvpt.y);
      cvpt = ms.rwin.ConvertCoords(0, ms.rwin.GetHeight(), &ms.rwin.GetDefaultView());
      screenpts[3] = Falling::Point2D(cvpt.x, cvpt.y);
      screenpts[4] = screenpts[0];

      int transition = 0;
      int inters = 0;
      sf::Shape polyshape;
      for(unsigned i = 0; i < 5; i++)
      {
	  if(rb->containsPoint(screenpts[i]))
	  {
	      if(transition == -1)
	      {
		  // transition not contained -> contained
		  // determine the plane's intersection with the corresponding segment
		  if(Falling::Point2D::intersectLines(screenpts[i - 1], screenpts[i], diskcenter + u, diskcenter, &inter1))
		  {
		      // add the point on the list
		      polyshape.AddPoint(
			      inter1.getX(),
			      inter1.getY(),
			      sf::Color(200, 200, 200)
			      );
		      inters++;
		  }
	      }
	      polyshape.AddPoint(
		      screenpts[i].getX(),
		      screenpts[i].getY(),
		      sf::Color(200, 200, 200)
		      );
	      //add the point
	      transition = 1;
	  }
	  else 
	  {
	      // point not contained
	      if(transition == 1)
	      {
		  // transition contained -> not contained
		  // determine the plane's intersection with the corresponding segment
		  if(Falling::Point2D::intersectLines(screenpts[i - 1], screenpts[i], diskcenter + u, diskcenter, &inter2))
		  {
		      // add the point on the list
		      polyshape.AddPoint(
			      inter2.getX(),
			      inter2.getY(),
			      sf::Color(200, 200, 200)
			      );
		      inters++;
		  }
	      }
	      transition = -1;
	  }
      }
      ms.rwin.Draw(polyshape);
      if(inters == 2)
      {
	  ms.rwin.Draw(
		  sf::Shape::Line(
		      inter1.getX(),
		      inter1.getY(),
		      inter2.getX(),
		      inter2.getY(),
		      ms.selectedObj == this? 4.0f : 3.f,
		      ms.selectedObj == this? sf::Color(255, 255, 255) : sf::Color(100, 100, 100) 
		      )
		  );
      }
      break;
    }
}
