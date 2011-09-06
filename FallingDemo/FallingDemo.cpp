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
#include "FallingDemo.h"
#include <iostream>

int main(int, char**)
{
  sf::WindowSettings ws;
  ws.AntialiasingLevel = 0;
  sf::RenderWindow rw(
      sf::VideoMode(800,600,32),
      "Falling Demo v0.1",
      sf::Style::Close | sf::Style::Resize,
      ws
      );

  MachineState ms(rw);
  // pause the engine at the beginning
  ms.w.setPaused(true);
  ms.drawstate |= MachineState::DRAW_BORDERS;
  ms.drawstate |= MachineState::DRAW_SLEEPING_BLACK;
  ms.selectedObj = 0;
  UserInterface ui(ms);
  sf::Clock cl;
  sf::Clock pcl;
  while(rw.IsOpened())
  {
    sf::Event ev;
    cl.Reset();
    while(rw.GetEvent(ev))
      ::dispatchEvent(ev, ms,ui);
    float bph = cl.GetElapsedTime();
    ms.colls = ms.w.solve(0.016f);
    ms.elapsedPhysicsTime = cl.GetElapsedTime() - bph;
    draw(ms, ui);
    float elpstmr = cl.GetElapsedTime();
    if(elpstmr < 0.016)
      sf::Sleep(0.016 - elpstmr);
  }
  return 0;
}

void mouseMoved(MachineState &ms, float x, float y, float realx, float realy)
{
  if(ms.rbuttondown)
    ms.rwin.GetDefaultView().Move(ms.oldx - realx, ms.oldy - realy);
  else if(ms.selectedObj)
    return;
  else
  {
    switch(ms.buttonstate)
    {
      case MachineState::DRAW_POINTS:
        // do nothing
        break;
      case MachineState::DRAW_MOVE:
        /*
           if(ms.rwin.GetInput().IsMouseButtonDown(sf::Mouse::Left)
           || ms.rwin.GetInput().IsMouseButtonDown(sf::Mouse::Right))
           */
        if(ms.lbuttondown)
        {
          if(
              ms.vpts.size() == 0
              || !Falling::Point2D(x/SCALE, y/SCALE).exactEquals(
                ms.vpts[ms.vpts.size()-1]
                )
            )
          {
            ms.vpts.push_back(Falling::Point2D(x/SCALE,y/SCALE));
          }
        }
        break;
      case MachineState::DRAW_CIRCLE:
      case MachineState::DRAW_PLANE:
      case MachineState::DRAW_SQUARE:
        /*if(ms.rwin.GetInput().IsMouseButtonDown(sf::Mouse::Left)
          || ms.rwin.GetInput().IsMouseButtonDown(sf::Mouse::Right))
          */
        if(ms.lbuttondown)
        {
          if(ms.vpts.size() == 2)
            ms.vpts[1] = Falling::Point2D(x/SCALE, y/SCALE);
          else
            ms.vpts.push_back(Falling::Point2D(x/SCALE, y/SCALE));
        }
        break;
      case MachineState::DRAW_CIRCLE_CONTINUOUS:
        // create circle
        if(ms.lbuttondown)
        {
          ms.objs.push_back(
              new pObject(
                0,
                10,
                false,
                ms.w,
                pObject::O_CIRCLE,
                Falling::Point2D(x/SCALE, y/SCALE)
                ));
          ms.vpts.clear();
        }
        break;
      default:
        break;
    }
  }
}

void mousePushed(MachineState &ms, float x, float y)
{
  int i;

  for(i = ((int)ms.objs.size()) - 1; i >= 0; i--)
  {
    // scan in reverse order to touch topmost objects first.
    if(ms.objs[i]->rb->containsPoint(Falling::Point2D(x,y)))
    {
      //printf("Found!\n");
      ms.selectedObj = ms.objs[i];
      break;
    }
  }
  if(i == -1)
    ms.selectedObj = 0;
  else
  {
    ms.vpts.clear();
    return;
  }

  if(ms.lbuttondown)
  {
    switch(ms.buttonstate)
    {
      case MachineState::DRAW_MOVE:
      case MachineState::DRAW_CIRCLE:
      case MachineState::DRAW_PLANE:
      case MachineState::DRAW_SQUARE:
        ms.vpts.clear();
        // then add point
      case MachineState::DRAW_POINTS:
        ms.vpts.push_back(Falling::Point2D(x/SCALE, y/SCALE));
        break;
      default:
        break;
    }
  }
}

void makePolyFromvptsList(MachineState &ms)
{
  int n = ms.vpts.size();
  if(n>=3)
  {
    Falling::Point2D *pts = new Falling::Point2D[n];
    for(int i=0; i<n; i++)
      pts[i] = ms.vpts[i];
    ms.objs.push_back(
        new pObject(
          pts,
          n,
          false,
          ms.w,
          pObject::O_POLY));
  }
  ms.vpts.clear();
}

void mouseReleased(MachineState &ms, float x, float y)
{
  Falling::Point2D ul;
  Falling::Point2D dr;
  Falling::Point2D *optvect;

  if(ms.selectedObj || ms.vpts.size() < 2)
  {
    if(ms.buttonstate != MachineState::DRAW_POINTS)
      ms.vpts.clear();
    return;
  }
  if(ms.lbuttondown)
  {
    switch(ms.buttonstate)
    {
      case MachineState::DRAW_POINTS:
        // nothing to do...
        break;
      case MachineState::DRAW_SQUARE:
      case MachineState::DRAW_MOVE:
        if(ms.buttonstate == MachineState::DRAW_MOVE)
        {
          if(ms.vpts.size() < 3)
          {
            ms.vpts.clear();
            return;
          }
          ms.vpts.push_back(Falling::Point2D(x/SCALE,y/SCALE));
        }
        else
        {
          // create a rectangle adding the two missing points
          ul = ms.vpts[0];
          dr = ms.vpts[1];
          ms.vpts.clear();
          ms.vpts.push_back(ul);
          ms.vpts.push_back(Falling::Point2D(ul.getX(), dr.getY()));
          ms.vpts.push_back(dr);
          ms.vpts.push_back(Falling::Point2D(dr.getX(), ul.getY()));
        }
        makePolyFromvptsList(ms);
        break;
      case MachineState::DRAW_CIRCLE:
        // create circle
        ms.objs.push_back(
            new pObject(
              0,
              Falling::Vector2D(ms.vpts[0], ms.vpts[1]).magnitude(),
              false,
              ms.w,
              pObject::O_CIRCLE,
              ms.vpts[0])
            );
        ms.vpts.clear();
        break;
      case MachineState::DRAW_PLANE:
        optvect = new Falling::Point2D(ms.vpts[1]);
        ms.objs.push_back(
            new pObject(
              optvect,
              1,
              true,
              ms.w,
              pObject::O_PLANE,
              ms.vpts[0])
            );
        ms.vpts.clear();
        break;
      default:
        ms.vpts.clear();
        break;
    }
  }
}

void dispatchEvent(sf::Event &ev, MachineState &ms, UserInterface &ui)
{
  // do not continue if gui catches the event
  if(ui.dispatchEvent(ev, ms))
    return;

  ///////////////////////////
  // handle non-gui events //
  ///////////////////////////
  ms.rwin.SetView(ms.rwin.GetDefaultView());
  if(ev.Type == sf::Event::Closed)
    ms.rwin.Close();
  else if(ev.Type == sf::Event::MouseButtonReleased)
  {
    sf::Vector2f pos =
      ms.rwin.ConvertCoords(
          MAX(ev.MouseButton.X,0),
          MAX(ev.MouseButton.Y,0),
          &ms.rwin.GetDefaultView()
          );
    mouseReleased(ms, pos.x, pos.y);
    if(ev.MouseButton.Button == sf::Mouse::Left)
      ms.lbuttondown = false;
    else if(ev.MouseButton.Button == sf::Mouse::Right)
      ms.rbuttondown = false;
  }
  else if(ev.Type == sf::Event::MouseButtonPressed)
  {
    if(ev.MouseButton.Button == sf::Mouse::Left)
      ms.lbuttondown = true;
    else if(ev.MouseButton.Button == sf::Mouse::Right)
      ms.rbuttondown = true;
    sf::Vector2f pos =
      ms.rwin.ConvertCoords(
          MAX(ev.MouseButton.X,0),
          MAX(ev.MouseButton.Y,0),
          &ms.rwin.GetDefaultView());
    ms.oldx = ev.MouseButton.X;
    ms.oldy = ev.MouseButton.Y;
    mousePushed(ms, pos.x, pos.y);
  }
  else if(ev.Type == sf::Event::MouseMoved)
  {
    //ms.lbuttondown = ms.rwin.GetInput().IsMouseButtonDown(sf::Mouse::Left);
    //ms.rbuttondown =  ms.rwin.GetInput().IsMouseButtonDown(sf::Mouse::Right);
    sf::Vector2f pos =
      ms.rwin.ConvertCoords(
          MAX(ev.MouseMove.X,0),
          MAX(ev.MouseMove.Y,0),
          &ms.rwin.GetDefaultView()
          );
    mouseMoved(ms, pos.x, pos.y, ev.MouseMove.X, ev.MouseMove.Y);
    ms.oldx = ev.MouseMove.X;
    ms.oldy = ev.MouseMove.Y;
  }
  else if(ev.Type == sf::Event::KeyPressed)
  {
    int inc;
    if(ev.Key.Code == sf::Key::Return)
    {
      if(ms.buttonstate == MachineState::DRAW_POINTS)
        makePolyFromvptsList(ms);
      inc = 1;
    }
    else
      inc = -1;
    for(unsigned int i = 0; i < ms.objs.size(); i++)
      ms.objs[i]->incrementLimit(inc);

  }
  else if(ev.Type == sf::Event::Resized)
  {
    ms.rwin.GetDefaultView().SetHalfSize(
        ev.Size.Width / 2.f,
        ev.Size.Height / 2.f
        );
    wWidget::view.SetHalfSize(
        ev.Size.Width / 2.f,
        ev.Size.Height / 2.f
        );
    wWidget::view.SetCenter(
        ev.Size.Width / 2.f,
        ev.Size.Height / 2.f
        );
  }
}


   void exploreOBBtree(sf::RenderWindow &screen, Falling::OBBtree *t, Falling::Vector2D v, int deepness, int curr, Falling::Shape *csh, bool leaves_only)
   {
   if(!t)
     return;
   float ux = v.getX();
   float uy = v.getY();
//if(curr == deepness)
//{

   Falling::OBB *o = t->o;
   Falling::Vector2D obb_pts[] = {csh->toRotated(o->get_pt(0)), csh->toRotated(o->get_pt(1)), csh->toRotated(o->get_pt(2)), csh->toRotated(o->get_pt(3))};
   if(!leaves_only || (!t->r && !t->l))//t->isLeaf());
   {
     screen.Draw(
         sf::Shape::Line(
           ux + obb_pts[0].getX(),
           uy + obb_pts[0].getY(),
           ux + obb_pts[1].getX(),
           uy + obb_pts[1].getY(),
           3,
           sf::Color(0,0,0)));
     screen.Draw(
         sf::Shape::Line(
           ux + obb_pts[1].getX(),
           uy + obb_pts[1].getY(),
           ux + obb_pts[2].getX(),
           uy + obb_pts[2].getY(),
           3,
           sf::Color(0,0,0)));
     screen.Draw(
         sf::Shape::Line(
           ux + obb_pts[2].getX(),
           uy + obb_pts[2].getY(),
           ux + obb_pts[3].getX(),
           uy + obb_pts[3].getY(),
           3,
           sf::Color(0,0,0))
         );
     screen.Draw(
         sf::Shape::Line(
           ux + obb_pts[3].getX(),
           uy + obb_pts[3].getY(),
           ux + obb_pts[0].getX(),
           uy + obb_pts[0].getY(),
           3,
           sf::Color(0,0,0))
         );
   }
   exploreOBBtree(screen,t->r, v, deepness, curr+1, csh, leaves_only);
   exploreOBBtree(screen,t->l, v, deepness, curr+1, csh, leaves_only);
   }

void drawDrawingShape(MachineState &ms, UserInterface &)
{
  if(ms.vpts.size() < 2)
    return;
  switch(ms.buttonstate)
  {
    case MachineState::DRAW_POINTS:
    case MachineState::DRAW_MOVE:
      for(unsigned int i = 0; i + 1 < ms.vpts.size(); i++)
      {
        ms.rwin.Draw(
            sf::Shape::Line(
              ms.vpts[i].getX() * SCALE,
              ms.vpts[i].getY() * SCALE,
              ms.vpts[i+1].getX() * SCALE,
              ms.vpts[i+1].getY() * SCALE,
              3.f,
              sf::Color(95,95,95)
              )
            );
      }
      if(ms.vpts.size() > 2)
        ms.rwin.Draw(
            sf::Shape::Line(
              ms.vpts[0].getX() * SCALE,
              ms.vpts[0].getY() * SCALE,
              ms.vpts[ms.vpts.size()-1].getX() * SCALE,
              ms.vpts[ms.vpts.size()-1].getY() * SCALE,
              3.f,
              sf::Color(130,130,130)
              )
            );
      break;
    case MachineState::DRAW_CIRCLE:
      // only two points to describe the circle
      ms.rwin.Draw(sf::Shape::Circle(
            ms.vpts[0].getX(),
            ms.vpts[0].getY(),
            Falling::Vector2D(ms.vpts[0], ms.vpts[1]).magnitude(),
            sf::Color(95,95,95,200), 3, sf::Color(130,130,130)));
      break;
    case MachineState::DRAW_SQUARE:
      ms.rwin.Draw(sf::Shape::Rectangle(
            ms.vpts[0].getX(),
            ms.vpts[0].getY(),
            ms.vpts[1].getX(),
            ms.vpts[1].getY(),
            sf::Color(95,95,95,200), 3, sf::Color(130,130,130)));
      break;
    case MachineState::DRAW_PLANE:
      ms.rwin.Draw(sf::Shape::Line(
            ms.vpts[0].getX(),
            ms.vpts[0].getY(),
            ms.vpts[1].getX(),
            ms.vpts[1].getY(),
            2,
            sf::Color(95,95,95,200), 3, sf::Color(130,130,130)));
      break;
    default:
      break;
  }
}

void draw(MachineState &ms, UserInterface &ui)
{

  ms.rwin.Clear(sf::Color(200,191,231));
  ms.rwin.SetView(ms.rwin.GetDefaultView());
  if(!ms.selectedObj)
    drawDrawingShape(ms, ui);

  for(unsigned int i = 0; i < ms.objs.size(); i++)
  {
    ms.objs[i]->draw(ms);
    Falling::Shape *csh = ms.objs[i]->rb->getShape();
    if(csh->getShapeTypeID() == 100)
        continue; // don't try to draw an infinite plane's aabb…
    if(ms.drawstate & MachineState::DRAW_AABBS)
    {
      ms.rwin.Draw(sf::Shape::Line(csh->get_aabb_xm(),csh->get_aabb_ym(), csh->get_aabb_xM(), csh->get_aabb_ym(),1,sf::Color(0,0,0)));
      ms.rwin.Draw(sf::Shape::Line(csh->get_aabb_xM(),csh->get_aabb_yM(), csh->get_aabb_xM(), csh->get_aabb_ym(),1,sf::Color(0,0,0)));
      ms.rwin.Draw(sf::Shape::Line(csh->get_aabb_xm(),csh->get_aabb_ym(), csh->get_aabb_xm(), csh->get_aabb_yM(),1,sf::Color(0,0,0)));
      ms.rwin.Draw(sf::Shape::Line(csh->get_aabb_xM(),csh->get_aabb_yM(), csh->get_aabb_xm(), csh->get_aabb_yM(),1,sf::Color(0,0,0)));
    }

    if(ms.drawstate & MachineState::DRAW_OBB_TREE || ms.drawstate & MachineState::DRAW_OBB_LEAVES)
       exploreOBBtree(ms.rwin, csh->getOtree(), csh->getPos(), 10, 0, csh, (ms.drawstate & MachineState::DRAW_OBB_LEAVES) && !(ms.drawstate & MachineState::DRAW_OBB_TREE));
  }
  /*
   * draw collisions
   */
  if(ms.drawstate & MachineState::DRAW_COLLISION_COUPLES)
  {
    for(std::vector<Falling::Collision *>::iterator coll = ms.colls.begin(); coll != ms.colls.end(); coll++)
    {
      Falling::Collision *c = *coll;
      ms.rwin.Draw(sf::Shape::Line(c->sa->getPos().getX(), c->sa->getPos().getY(),c->sb->getPos().getX(), c->sb->getPos().getY(),1,sf::Color(255,0,0)));
    }
  }
  /*
   */
  ms.rwin.SetView(wWidget::view);
  ui.draw(ms);
  ms.rwin.Display();
}
