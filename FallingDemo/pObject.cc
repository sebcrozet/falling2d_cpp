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

sf::View pObject::view = sf::View();
Real pObject::poly = 0;
Real pObject::polx = 0;
int pObject::icall = 0;

pObject::pObject(Falling::Point2D *pts,int n,bool iscircle, Falling::World &w,
                 ObjectType cir,Falling::Point2D center)
  : pts(pts), support(0), diskcenter(center)
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
  icall = 0;
  poly = rb->getPos().getY();
  polx = rb->getPos().getX();
  switch(otype)
    {
    case pObject::O_CIRCLE:
      {
        Falling::Vector2D pos = rb->getPos();
        sf::CircleShape sh(nb);
        sh.setOrigin(nb, nb);
        sh.setPosition(pos.getX() * SCALE, pos.getY() * SCALE);
        sh.setFillColor(rb->isSleeping() &&
                        (ms.drawstate & MachineState::DRAW_SLEEPING_BLACK) ?
                        sf::Color(0,0,0) : sf::Color(r,g,b));
        sh.setOutlineThickness(ms.selectedObj == this ? 1.f :
                               (ms.drawstate & MachineState::DRAW_BORDERS) ?
                               1.f : 0.f);
        sh.setOutlineColor(ms.selectedObj == this ?
                           sf::Color(255, 255, 255) :
                           sf::Color(r - 70, g - 70,b - 70));
        ms.rwin.draw(sh);

        sh = sf::CircleShape(3);
        sh.setOrigin(3, 3);
        sh.setOutlineThickness(1);
        sh.setFillColor(sf::Color(r - 70, g - 70, b - 70));
        sh.setPosition(pos.getX(), pos.getY());
        ms.rwin.draw(sh);
      }
      break;
    case pObject::O_POLY:
      {
        if(p->getNbrSubShapes())
        {
          if(!rb->isFixed() || (ms.drawstate & MachineState::DRAW_COMPONENTS))
          {
            for(int j = 0; j < p->getNbrSubShapes(); j++)
            {
              sf::ConvexShape sh(p->getSubShape(j)->getNbrPts());
              int rrand;
              int grand;
              int brand;
              if(ms.drawstate & MachineState::DRAW_COMPONENTS)
              {
                srand(r + j);
                rrand = rand() % 255;
                grand = rand() % 255;
                brand = rand() % 255;
              }
              else if(rb->isSleeping() &&
                  (ms.drawstate & MachineState::DRAW_SLEEPING_BLACK))
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
                Falling::Point2D ptsi = p->toGlobal(
                    p->getSubShape(j)->getPts()[i]
                    );
                sh.setPoint(i, sf::Vector2f(ptsi.getX() * SCALE,
                      ptsi.getY() * SCALE));
              }
              sh.setFillColor(sf::Color(rrand, grand, brand));
              ms.rwin.draw(sh);
            }
          }
          else
          {
            for(int j = 0; j < p->getNbrSubShapes(); j++)
            {
              sf::ConvexShape sh(p->getSubShape(j)->getNbrPts());
              for(int i = 0; i < p->getSubShape(j)->getNbrPts(); i++)
              {
                Falling::Point2D ptsi = p->toGlobal(
                                          p->getSubShape(j)->getPts()[i]
                                        );
                sh.setPoint(i, sf::Vector2f(ptsi.getX()*SCALE,
                                            ptsi.getY()*SCALE));
              }
              sh.setFillColor(sf::Color(r,g,b));
              ms.rwin.draw(sh);
            }
          }
        }
      }
      {
        int j = nb - 1;
        if(ms.drawstate & MachineState::DRAW_BORDERS || ms.selectedObj == this)
        {
          for(int i = 0; i < nb; j = i, ++i)
          {
            Falling::Point2D vptsi = p->toGlobal(pts[j]);
            Falling::Point2D vptsi1 = p->toGlobal(pts[i]);

            sf::Vertex vertices[2];
            sf::Color c = ms.selectedObj == this ? sf::Color(255, 255, 255) :
                                                   sf::Color(r-70, g-70, b-70);
            vertices[0] = sf::Vertex(sf::Vector2f(vptsi.getX() * SCALE,
                                                  vptsi.getY() * SCALE), c);
            vertices[1] = sf::Vertex(sf::Vector2f(vptsi1.getX() * SCALE,
                                                  vptsi1.getY() * SCALE), c);
            ms.rwin.draw (vertices, 2, sf::LinesStrip);
            // FIXME: ms.selectedObj == this ? 4.f : 3.f,
          }
        }
      }
      {
        sf::CircleShape sf(1);
        sf.setPosition(p->toGlobal(Falling::Point2D(0,0)).getX(),
                       p->toGlobal(Falling::Point2D(0,0)).getY());
        sf.setFillColor(sf::Color(r - 70, g - 70, b - 70));
        ms.rwin.draw(sf);
      }
      break;
    case pObject::O_PLANE:
      // build a list of all points on the screen
      // and insert the first point at the end of
      // the list also
      Falling::Point2D screenpts[5];
      sf::Vector2f cvpt;
      Falling::Point2D inter1;
      Falling::Point2D inter2;

      cvpt = ms.rwin.convertCoords(sf::Vector2i(0, 0),
                                   pObject::view);
      screenpts[0] = Falling::Point2D(cvpt.x, cvpt.y);
      cvpt = ms.rwin.convertCoords(sf::Vector2i(ms.rwin.getSize().x, 0),
                                   pObject::view);
      screenpts[1] = Falling::Point2D(cvpt.x, cvpt.y);
      cvpt = ms.rwin.convertCoords(sf::Vector2i(ms.rwin.getSize().x,
                                                ms.rwin.getSize().y),
                                   pObject::view);
      screenpts[2] = Falling::Point2D(cvpt.x, cvpt.y);
      cvpt = ms.rwin.convertCoords(sf::Vector2i(0, ms.rwin.getSize().y),
                                   pObject::view);
      screenpts[3] = Falling::Point2D(cvpt.x, cvpt.y);
      screenpts[4] = screenpts[0];

      int transition = 0;
      int inters = 0;
      std::vector<Falling::Point2D> pts;
      for(unsigned i = 0; i < 5; i++)
      {
        if(rb->containsPoint(screenpts[i]))
        {
          if(transition == -1)
          {
            // transition not contained -> contained
            // determine the plane's intersection with the corresponding segment
            if(Falling::Point2D::intersectLines(screenpts[i - 1],
                                                screenpts[i],
                                                diskcenter + u,
                                                diskcenter,
                                                &inter1))
            {
              // add the point on the list
              pts.push_back (inter1);
              inters++;
            }
          }
          pts.push_back (screenpts[i]);
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
            if(Falling::Point2D::intersectLines(screenpts[i - 1],
                                                screenpts[i],
                                                diskcenter + u,
                                                diskcenter,
                                                &inter2))
            {
              // add the point on the list
              pts.push_back (inter2);
              inters++;
            }
          }
          transition = -1;
        }
      }

      sf::ConvexShape polyshape(pts.size());
      for (int i = 0; i < pts.size(); ++i) {
        polyshape.setPoint(i, sf::Vector2f(pts[i].getX(), pts[i].getY()));
      }
      ms.rwin.draw(polyshape);
      if(inters == 2)
      {
        sf::Vertex vertices[2];
        sf::Color c (200, 200, 200);
        vertices[0] = sf::Vertex(sf::Vector2f(inter1.getX(),
                                              inter1.getY()), c);
        vertices[1] = sf::Vertex(sf::Vector2f(inter2.getX(),
                                              inter2.getY()), c);
        ms.rwin.draw (vertices, 2, sf::LinesStrip);
        //FIXME ms.selectedObj == this? 4.0f : 3.f,
        //FIXME ms.selectedObj == this? sf::Color(255, 255, 255) : sf::Color(100, 100, 100)
      }
      break;
    }
}
