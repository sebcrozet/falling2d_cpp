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
#include "wContener.h"
#include "wLabel.h"
#include "wImage.h"
#include "wMenu.h"
#define TITLEBARSIZE 20

/* Dialog box with title bar, optionnal buttons
 ** and methods to add internal widgets.
 ** Draggable window.
 */
class wDialog : public wContener
{
public:
  enum ButtonType
  {
    BUTTON_YES,
    BUTTON_NO,
    BUTTON_OK,
    BUTTON_RETRY,
    BUTTON_APPLY,
    BUTTON_CANCEL
  };
private:
  wContener corp;
  wContener buttonArea;
  wImage separator;
  wLabel titlebar;
  //sf:: title;
  // drag and drop
  bool dragging;
  float lastx,lasty;
  // dialogs with buttons
public:
  wDialog(float x,float y, const std::string &title);
  virtual void draw(sf::RenderWindow &rw);
  virtual void close();
  virtual bool interpretEvent(sf::Event &ev, float mx, float my);
  void addCorpChild(wWidget *w);
  void addButton(ButtonType type, void *userdata, void (*itempushed)(int, void *));

  static wDialog *getErrorDialog(float x, float y, const std::string &text);
  static wDialog *getWarningDialog(float x, float y, const std::string &text);
};
