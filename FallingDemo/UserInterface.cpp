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
#include "UserInterface.h"
#include <sstream>

void removeObject(int, void *userdata)
{
#ifndef _USE_BOX_2D_
  if(((MachineState *)userdata)->selectedObj)
  {
    pObject *po = ((MachineState *)userdata)->selectedObj;
    ((MachineState *)userdata)->selectedObj = 0;
    ((MachineState *)userdata)->w.removeObject(po->rb);
    ((MachineState *)userdata)->objs.erase
      (
       std::remove
       (
        ((MachineState *)userdata)->objs.begin(),
        ((MachineState *)userdata)->objs.end(),
        po
       ), 
       ((MachineState *)userdata)->objs.end()
      );
    delete po;
  }
#endif
  // else, nothing to delete!
}

void toogledFixedItem(int, void *userdata)
{
  if(((MachineState *)userdata)->selectedObj != 0)
  {
    ((MachineState *)userdata)->selectedObj->toogleFixed();
  }
  // else, nothing to (un)fix
}

void exitApplication(int, void *userdata)
{
  ((MachineState *)userdata)->rwin.Close();
}

void pauseEngine(int, void *userdata)
{
#ifndef _USE_BOX_2D_
  ((MachineState *)userdata)->w.setPaused(!((MachineState *)userdata)->w.getPaused());
#endif
  ((MachineState *)userdata)->running = !((MachineState *)userdata)->running;
}

void switchFlagOn(int id, void *userdata)
{
  if(id == 2001)
    ((MachineState *)userdata)->drawstate |= MachineState::DRAW_BORDERS;
  else if(id == 2002)
    ((MachineState *)userdata)->drawstate |= MachineState::DRAW_COMPONENTS;
  else if(id == 2003)
    ((MachineState *)userdata)->drawstate |= MachineState::DRAW_AABBS;
  else if(id == 2004)
    ((MachineState *)userdata)->drawstate |= MachineState::DRAW_OBB_TREE;
  else if(id == 2005)
    ((MachineState *)userdata)->drawstate |= MachineState::DRAW_SLEEPING_BLACK;
  else if(id == 2006)
    ((MachineState *)userdata)->drawstate |= MachineState::DRAW_OBB_LEAVES;
  else if(id == 2007)
    ((MachineState *)userdata)->drawstate |= MachineState::DRAW_COLLISION_COUPLES;
}

void switchFlagOff(int id, void *userdata)
{
  if(id == 2001)
    ((MachineState *)userdata)->drawstate &= ~MachineState::DRAW_BORDERS;
  else if(id == 2002)
    ((MachineState *)userdata)->drawstate &= ~MachineState::DRAW_COMPONENTS;
  else if(id == 2003)
    ((MachineState *)userdata)->drawstate &= ~MachineState::DRAW_AABBS;
  else if(id == 2004)
    ((MachineState *)userdata)->drawstate &= ~MachineState::DRAW_OBB_TREE;
  else if(id == 2005)
    ((MachineState *)userdata)->drawstate &= ~MachineState::DRAW_SLEEPING_BLACK;
  else if(id == 2006)
    ((MachineState *)userdata)->drawstate &= ~MachineState::DRAW_OBB_LEAVES;
  else if(id == 2007)
    ((MachineState *)userdata)->drawstate &= ~MachineState::DRAW_COLLISION_COUPLES;
}

void changeDrawMode(int id, void *userdata)
{
  if(id == 1004)
    ((MachineState *)userdata)->buttonstate = MachineState::DRAW_MOVE;
  else if(id == 1005)
    ((MachineState *)userdata)->buttonstate  = MachineState::DRAW_POINTS;
  else if(id == 1006)
    ((MachineState *)userdata)->buttonstate = MachineState::DRAW_CIRCLE;
  else if(id == 1007)
    ((MachineState *)userdata)->buttonstate = MachineState::DRAW_SQUARE;
  else if(id == 1008)
    ((MachineState *)userdata)->buttonstate = MachineState::DRAW_PLANE;
  else if(id == 1009)
    ((MachineState *)userdata)->buttonstate = MachineState::DRAW_CIRCLE_CONTINUOUS;
}
void initParseMenuCallback(void *userdata, wMenuItem *mi)
{
  if(mi->getId() == 1)
  {
    mi->setUserDatas(userdata);
    mi->setOnItemClicked(exitApplication);
  }
  else if(mi->getId() == 200)
  {
    mi->setUserDatas(userdata);
    mi->setOnItemClicked(pauseEngine);
    mi->setOnItemReleased(pauseEngine);
  }
  else if(mi->getId() >= 2001 && mi->getId() <= 2007)
  {
    mi->setUserDatas(userdata);
    mi->setOnItemClicked(switchFlagOn);
    mi->setOnItemReleased(switchFlagOff);
  }
  else if(mi->getId() >= 1004 && mi->getId() <= 1009)
  {
    mi->setUserDatas(userdata);
    mi->setOnItemClicked(changeDrawMode);
  }
  else if(mi->getId() == 1010)
  {
    mi->setUserDatas(userdata);
    mi->setOnItemClicked(toogledFixedItem);
  }
  else if(mi->getId() == 1011)
  {
    mi->setUserDatas(userdata);
    mi->setOnItemClicked(removeObject);
  }
}


UserInterface::~UserInterface()
{
  for(unsigned int i = 0; i < ui.size(); i++)
    delete ui[i];
}

  UserInterface::UserInterface(MachineState &ms)
: absorbMouse(false)
{
  ui.push_back(new wLabel(10, 40, "", 20));
  ui.push_back(new wLabel(10, 80, "", 20));
  ui.push_back(wMenuBar::fromStringDescriptor(
        ": Files\n"
        "> :_1 Quit Falling Demo\n"
        ": Options\n"
        "> : Draw options\n"
        ">> :_?@_2001 Draw borders\n"
        ">> ;_=_ SE\n"
        ">> :_?_2002 Show components\n"
        "> : Draw options (debug)\n"
        ">> :_?_2003 Draw AABBs\n"
        ">> :_?@_2005 Draw sleeping objs. in black\n"
        ">> :_?_2007 Draw collision pairs\n"
        ">> : Draw OBB-tree\n"
        ">>> :_?_2004 Draw the whole tree\n"
        ">>> :_?_2006 Draw leaves only\n"
        ";_=_ SE\n"
        ";_?_200 LE\n"
        ";_=_ SE\n"
        ";_?@_A_1004 LY\n"
        ";_?_A_1005 LYPTS\n"
        ";_?_A_1006 CI\n"
        ";_?_A_1007 RE\n"
        ":_?_A_1008 Plane\n"
        ":_?_A_1009 Circles\n"
        ";_=_ SE\n"
        ":_1010 Fix\n"
        ":_1011 Del\n"
        ";_=_ SE\n"
        ":_=_ Falling Demo v0.0\n"
        ,
    true,
    &ms,
    initParseMenuCallback
      ));

}

bool UserInterface::dispatchEvent(sf::Event ev, MachineState &ms)
{
  for(unsigned int i = 0; i < ui.size(); i++)
  {
    ms.rwin.SetView(wWidget::view);
    sf::Vector2f mp = ms.rwin.ConvertCoords(
        MAX(ms.rwin.GetInput().GetMouseX(),0),
        MAX(ms.rwin.GetInput().GetMouseY(),0));
    if(ui[i]->interpretEvent(ev, mp.x, mp.y))
    {
      if(ev.Type == sf::Event::MouseButtonPressed)
        absorbMouse = true;
      return true;
    }
  }
  if(absorbMouse && ev.Type == sf::Event::MouseButtonReleased)
  {
    absorbMouse = false;
    return true;
  }
  return (ev.Type == sf::Event::MouseMoved && absorbMouse);
}

void UserInterface::draw(MachineState &ms)
{
  std::ostringstream o, n;
  o << ms.elapsedPhysicsTime;
  if(ms.elapsedPhysicsTime < 0.016)
    ((wLabel *)ui[0])->setText(o.str(), 0, 0, 0);
  else
    ((wLabel *)ui[0])->setText(o.str(), 255, 0, 0);
  n << ms.objs.size();
  ((wLabel *)ui[1])->setText(n.str(), 0, 0, 0);
  for(unsigned int i = 0; i < ui.size(); i++)
    ui[i]->draw(ms.rwin);
}
