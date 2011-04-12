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
#ifndef __FALLING_USER_INTERFACE_
#include "stdafx.h"
#include "wMenu.h"
#include "State.h"
#include "wDialog.h"
#include "pObject.h"


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
