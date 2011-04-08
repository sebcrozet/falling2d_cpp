#include "stdafx.h"
#include "FallingDemo.h"

int drawLimit = 0;
int main(int argc, char* argv[])
{
  sf::WindowSettings ws;
  ws.AntialiasingLevel = 8;
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
  UserInterface ui(ms);
  sf::Clock cl;
  sf::Clock pcl;
  while(rw.IsOpened())
    {
      sf::Event ev;
	  cl.Reset();
      while(rw.GetEvent(ev))
        dispatchEvent(ev, ms,ui);
	  float bph = cl.GetElapsedTime();
      ms.w.solve(0.016f);
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
                || !Point2D(x/SCALE, y/SCALE).exactEquals(
                  ms.vpts[ms.vpts.size()-1]
                )
              )
                {
                  ms.vpts.push_back(Point2D(x/SCALE,y/SCALE));
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
                ms.vpts[1] = Point2D(x/SCALE, y/SCALE);
              else
                ms.vpts.push_back(Point2D(x/SCALE, y/SCALE));
            }
          break;
        }
    }
}

void mousePushed(MachineState &ms, float x, float y)
{
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
          ms.vpts.push_back(Point2D(x/SCALE, y/SCALE));
          break;
        case MachineState::JOINT_FIX:
          for(int i = 0; i < ms.objs.size(); i++)
            {
              if(ms.objs[i]->rb->containsPoint(Point2D(x,y)))
                ms.objs[i]->toogleFixed();
            }
          break;
        }
    }
}

void makePolyFromvptsList(MachineState &ms)
{
  int n = ms.vpts.size();
  if(n>=3)
    {
      Point2D *pts = new Point2D[n];
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
  int n;
  Point2D *pts;
  Point2D ul;
  Point2D dr;
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
            ms.vpts.push_back(Point2D(x/SCALE,y/SCALE));
          else
            {
              // create a rectangle adding the two missing points
              ul = ms.vpts[0];
              dr = ms.vpts[1];
              ms.vpts.clear();
              ms.vpts.push_back(ul);
              ms.vpts.push_back(Point2D(ul.getX(), dr.getY()));
              ms.vpts.push_back(dr);
              ms.vpts.push_back(Point2D(dr.getX(), ul.getY()));
            }
          makePolyFromvptsList(ms);
          break;
        case MachineState::DRAW_CIRCLE:
          // create circle
          ms.objs.push_back(
            new pObject(
              0,
              20,//Vector2D(ms.vpts[0], ms.vpts[1]).magnitude(),
              false,
              ms.w,
              pObject::O_CIRCLE,
              ms.vpts[0])
          );
          ms.vpts.clear();
          break;
        case MachineState::DRAW_PLANE:
          Point2D *optvect = new Point2D(ms.vpts[1]);
          ms.objs.push_back(
            new pObject(
              optvect,
              1,
              true,
              ms.w,
              pObject::O_PLANE,
              ms.vpts[0])
          );
          delete optvect;
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
      for(int i = 0; i < ms.objs.size(); i++)
        ms.objs[i]->incrementLimit(inc);
      drawLimit+=inc;

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


void exploreOBBtree(sf::RenderWindow &screen,OBBtree *t, Vector2D v, int deepness, int curr)
{
  if(t==0)
    return;
  float ux = v.getX();
  float uy = v.getY();
  //if(curr == deepness)
  //{
  screen.Draw(
    sf::Shape::Line(
      ux + t->o->pts[0].getX(),
      uy + t->o->pts[0].getY(),
      ux + t->o->pts[1].getX(),
      uy + t->o->pts[1].getY(),
      1,
      sf::Color(0,0,0)));
  screen.Draw(
    sf::Shape::Line(
      ux + t->o->pts[1].getX(),
      uy + t->o->pts[1].getY(),
      ux + t->o->pts[2].getX(),
      uy + t->o->pts[2].getY(),
      1,
      sf::Color(0,0,0)));
  screen.Draw(
    sf::Shape::Line(
      ux + t->o->pts[2].getX(),
      uy + t->o->pts[2].getY(),
      ux + t->o->pts[3].getX(),
      uy + t->o->pts[3].getY(),
      1,
      sf::Color(0,0,0))
  );
  screen.Draw(
    sf::Shape::Line(
      ux + t->o->pts[3].getX(),
      uy + t->o->pts[3].getY(),
      ux + t->o->pts[0].getX(),
      uy + t->o->pts[0].getY(),
      1,
      sf::Color(0,0,0))
  );
  exploreOBBtree(screen,t->r, v, deepness, curr+1);
  exploreOBBtree(screen,t->l, v, deepness, curr+1);
}

void drawDrawingShape(MachineState &ms, UserInterface &ui)
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
                     Vector2D(ms.vpts[0], ms.vpts[1]).magnitude(),
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
    }
}

void draw(MachineState &ms, UserInterface &ui)
{
  int sens = -1;
  int currR = 0;

  ms.rwin.Clear(sf::Color(200,191,231));
  ms.rwin.SetView(ms.rwin.GetDefaultView());
  drawDrawingShape(ms, ui);

  for(unsigned int i = 0; i < ms.objs.size(); i++)
    {
      ms.objs[i]->draw(ms);
      /*
         win.Draw(sf::Shape::Line(objs[i]->p->aabb_xm,objs[i]->p->aabb_ym, objs[i]->p->aabb_xM, objs[i]->p->aabb_ym,1,sf::Color(0,0,0)));
         win.Draw(sf::Shape::Line(objs[i]->p->aabb_xM,objs[i]->p->aabb_yM, objs[i]->p->aabb_xM, objs[i]->p->aabb_ym,1,sf::Color(0,0,0)));
         win.Draw(sf::Shape::Line(objs[i]->p->aabb_xm,objs[i]->p->aabb_ym, objs[i]->p->aabb_xm, objs[i]->p->aabb_yM,1,sf::Color(0,0,0)));
         win.Draw(sf::Shape::Line(objs[i]->p->aabb_xM,objs[i]->p->aabb_yM, objs[i]->p->aabb_xm, objs[i]->p->aabb_yM,1,sf::Color(0,0,0)));
         Vector2D poss= objs[i]->p->getPos();
         OBB *o = objs[i]->p->chull->obb;//getOtree()->o;
         win.Draw(sf::Shape::Line(poss.getX() + o->pts[0].getX(), poss.getY() + o->pts[0].getY(), poss.getX() + o->pts[1].getX(), poss.getY() + o->pts[1].getY(),1,sf::Color(0,0,0)));
         win.Draw(sf::Shape::Line(poss.getX() + o->pts[1].getX(), poss.getY() + o->pts[1].getY(), poss.getX() + o->pts[2].getX(), poss.getY() + o->pts[2].getY(),1,sf::Color(0,0,0)));
         win.Draw(sf::Shape::Line(poss.getX() + o->pts[2].getX(), poss.getY() + o->pts[2].getY(), poss.getX() + o->pts[3].getX(), poss.getY() + o->pts[3].getY(),1,sf::Color(0,0,0)));
         win.Draw(sf::Shape::Line(poss.getX() + o->pts[3].getX(), poss.getY() + o->pts[3].getY(), poss.getX() + o->pts[0].getX(), poss.getY() + o->pts[0].getY(),1,sf::Color(0,0,0)));
         */
    }
  ms.rwin.SetView(wWidget::view);
  ui.draw(ms);
  ms.rwin.Display();
}
