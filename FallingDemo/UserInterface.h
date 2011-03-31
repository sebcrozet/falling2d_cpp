#ifndef __FALLING_USER_INTERFACE_
#include "stdafx.h"
#include "wMenu.h"
#include "State.h"
#include "wDialog.h"


class UserInterface
{
  std::vector<wWidget *> ui;
  bool absorbMouse;

public:
  UserInterface(MachineState &ms);
  bool dispatchEvent(sf::Event ev, MachineState &ms);
  void draw(MachineState &ms);
};
#define __FALLING_USER_INTERFACE_
#endif
