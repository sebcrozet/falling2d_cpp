#include "stdafx.h"
#include "UserInterface.h"
#include <sstream>

void exitApplication(int, void *userdata)
{
  ((MachineState *)userdata)->rwin.Close();
}

void pauseEngine(int id, void *userdata)
{
  ((MachineState *)userdata)->w.setPaused(!((MachineState *)userdata)->w.getPaused());
}

void switchFlagOn(int id, void *userdata)
{
  if(id == 1001)
    ((MachineState *)userdata)->drawstate |= MachineState::DRAW_BORDERS;
  else if(id == 1002)
    ((MachineState *)userdata)->drawstate |= MachineState::DRAW_COMPONENTS;
}

void switchFlagOff(int id, void *userdata)
{
  if(id == 1001)
    ((MachineState *)userdata)->drawstate &= ~MachineState::DRAW_BORDERS;
  else if(id == 1002)
    ((MachineState *)userdata)->drawstate &= ~MachineState::DRAW_COMPONENTS;
}

void stepIntegrateSolveEngine(int id, void *userdata)
{
	if(id == 201)
		((MachineState *)userdata)->w.integrate(0.016);
	else
		((MachineState *)userdata)->w.solvePenetrationsAndImpulse(0.016);
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
    ((MachineState *)userdata)->buttonstate = MachineState::JOINT_FIX;
  else if(id == 1009)
    ((MachineState *)userdata)->buttonstate = MachineState::DRAW_PLANE;
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
  else if(mi->getId() > 200 && mi->getId() <= 202)
  {
	mi->setUserDatas(userdata);
	mi->setOnItemClicked(stepIntegrateSolveEngine);
  }
  else if(mi->getId() >= 1001 && mi->getId() <= 1002)
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
}



UserInterface::UserInterface(MachineState &ms)
  : absorbMouse(false)
{
  ui.push_back(new wLabel(10, 40, "", 20));
  ui.push_back(wMenuBar::fromStringDescriptor(
                 ": Files\n"
                 "> :_1 Quit Falling Demo\n"
                 ": Options\n"
                 "> : Draw options\n"
                 ">> :_?@_1001 Draw borders\n"
                 ">> ;_=_ SE\n"
                 ">> :_?_1002 Show components\n"
                 ";_=_ SE\n"
                 ";_?_200 LE\n"
				 ":_201 Integrate\n"
				 ":_202 Solve\n"
                 ";_=_ SE\n"
                 ";_?@_A_1004 LY\n"
                 ";_?_A_1005 LYPTS\n"
                 ";_?_A_1006 CI\n"
                 ";_?_A_1007 RE\n"
                 ":_?_A_1009 Plane\n"
                 ":_?_A_1008 Fix\n"
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
	std::ostringstream o;
	o << ms.elapsedPhysicsTime;
	if(ms.elapsedPhysicsTime < 0.016)
		((wLabel *)ui[0])->setText(o.str(), 0, 0, 0);
	else
		((wLabel *)ui[0])->setText(o.str(), 255, 0, 0);
  for(unsigned int i = 0; i < ui.size(); i++)
    {
      ui[i]->draw(ms.rwin);
    }
}
