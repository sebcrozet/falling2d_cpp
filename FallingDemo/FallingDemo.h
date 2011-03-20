#include "stdafx.h"
#include "pObject.h"
#include "State.h"
#include "UserInterface.h"
#include "wMenu.h"

void draw(MachineState &st, UserInterface &ui);
void dispatchEvent(sf::Event &ev, MachineState &st, UserInterface &ui);
