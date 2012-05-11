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
#include "wMenu.h"

wMenuItem::wMenuItem(
	const std::string &path1, 
	const std::string &path2, 
	int type, 
	int group, 
	int id, 
	bool toogled, 
	bool locked, 
	bool itemOrientation) :
  wContener(0,0,0,0,true,-2,0,0,(path1 == "SE" && !itemOrientation)?wContener::alignTopBottom:wContener::alignLeftRight, wContener::wrapBoth),
  toogled(toogled),
  highlighted(false),
  locked(locked),
  orientation(itemOrientation),
  checkbox(AND_LETTER(type,'?')),
  greycolor(200),
  id(id),
  itemtype(type),
  checkgroup(group),
  userdata(0),
  sw(0),
  onItemClicked(0),
  onItemPushed(0),
  onItemReleased(0)
{
  if(AND_LETTER(type,';'))
    {
      wContener::addChild(
        new wImage(
          0,
          0,
          (path1 == "SE")?2 : 20,
          (path1 == "SE")?1 : 20,
          path1,
          (path1 == "SE")?(itemOrientation ? wImage::STRETCH_H : wImage::STRETCH_W) : wImage::STRETCH_NONE),
          false,
          (path1 == "SE")?0:4,
          (path1 == "SE")?0:AND_LETTER(type,':')? 9 : 4,
          (path1 == "SE")?0:4,
          (path1 == "SE")?0:4
        );
    }
  if(AND_LETTER(type,':'))
  {
      wContener::addChild(
        new wLabel(0, 0, AND_LETTER(type,';')?path2 : path1, 15),
        false,
        2, 2, 0, 0
      );
    }
  else
    {
      // TODO: custom item ?
    }
}

void wMenuItem::close()
{
  // TODO: free SFML's ressources
}

void wMenuItem::groupUnselect()
{
  if(toogled)
    {
      if(sw)
        sw->setVisible(false);
      toogled = false;
    }
}

void wMenuItem::expand(bool coll)
{
  if((!checkbox || coll)&& sw)
    sw->setVisible(coll);
  //toogled = coll;
}

bool wMenuItem::setSize(float w, float h)
{
  wContener::setSize(w, h);
  updateSubMenuLocation();
  return true;
}

bool wMenuItem::interpretEvent(sf::Event &ev, float mx, float my)
{
  if(locked)
    return false;
  if(!wContener::interpretEvent(ev, mx, my) && (!sw || !sw->getIsVisible() || !sw->interpretEvent(ev, mx, my)))
    {
      if(ev.type == sf::Event::MouseMoved)
        {
          if(!wWidget::contains(mx, my))
            {
              if(highlighted)
                highlighted= false;
            }
          else
            highlighted = true;
        }
      else if(ev.type == sf::Event::MouseButtonPressed)
        {
          if(wWidget::contains(mx, my))
            {
              if(!toogled)
                {
                  toogled = true;
                  if(onItemClicked)
                    onItemClicked(id, userdata);
                }
              else if(checkbox && !checkgroup)
                {
                  if(sw)
                    sw->setVisible(false);
                  toogled = false;
                  if(onItemReleased)
                    onItemReleased(id, userdata);
                }
              return true;
            }
          else if(!checkbox && !checkgroup)
            {
              if(toogled)
                {
                  toogled = false;
                  if(onItemReleased)
                    onItemReleased(id, userdata);
                }
            }
        }
      else if(ev.type == sf::Event::MouseButtonReleased)
        {
          if(!checkbox && !checkgroup && toogled)
            {
              toogled = false;
              if(onItemReleased)
                onItemReleased(id, userdata);
            }
        }
      return false;
    }
  else
    return true;
}

void wMenuItem::update(float)
{
  // TODO: lose focus ?
}

void wMenuItem::draw(sf::RenderWindow &rw)
{
  // draw back
  if(!locked)
    {
      if(toogled)
        greycolor = 10;
      else
        {
          if(highlighted)
            {
              greycolor-=40;
              if(greycolor < 60)
                greycolor = 60;
            }
          else
            {
              greycolor+=20;
              if (greycolor > 200)
                greycolor = 200;
            }
        }
      sf::RectangleShape rs (sf::Vector2f (wWidget::rect.width,
                                           wWidget::rect.height));
      rs.setPosition (wWidget::rect.left, wWidget::rect.top);
      rs.setOutlineThickness(2);
      rs.setOutlineColor(sf::Color(greycolor,greycolor,greycolor,100));
      rs.setFillColor(sf::Color(greycolor,greycolor,greycolor,200));
      rw.draw (rs);
    } // else do not draw the rounding rectangle but only the content
  // draw text & image
  if(sw && sw->getIsVisible())
    sw->draw(rw);
  wContener::draw(rw);
}

void wMenuItem::updateSubMenuLocation()
{
  // place the object at the good place
  if(sw)
    {
      if(!orientation)
        sw->setCoords(rect.left, rect.top + rect.height + 10);
      else
        sw->setCoords(rect.left + rect.width + 10, rect.top);
    }
}
void wMenuItem::setSubChildPopupMenu(bool orient, wWidget *mb)
{
  sw = mb;
  orientation = orient;
  updateSubMenuLocation();
  // TODO: insert arrow indicating the popup place
  if(orientation)
    {
      addChild
      (
        new wImage(0, 0, 10, 10, "TR", wImage::STRETCH_NONE),
        false,
        10, 2, 0, 0
      );
    }
  else
    {
      addChild
      (
        new wImage(0, 0, 10, 10, "TD", wImage::STRETCH_NONE),
        false,
        10, 2, 0, 0
      );
    }
}

bool wMenuItem::translate(float x, float y)
{
  bool res = wContener::translate(x, y);
  updateSubMenuLocation();
  return res;
}



/*
 * Menu bar definition
 */
wMenuBar::wMenuBar(int, int, bool orientation, bool visible)
  : 
    wContener(
      0, 0, 5, 5,
      visible,
      -2,
      3, 3,
      orientation?wContener::alignLeftRight:wContener::alignTopBottom,
      wContener::wrapBoth),
    orientation(orientation)
{ }

wMenuBar *wMenuBar::fromFileDescriptor(
  const std::string& ,
  bool ,
  void *,
  void (*)(void *,wMenuItem*))
{
  // FIXME
  /*
  std::string descr;
  // TODO: open file and extract the string descriptor
  // construct menu
  return wMenuBar::fromStringDescriptor(descr,orientation,userdata, itemCreated);
  */
  return 0;
}
/* Parses a string descripting a full menu bar
 ** Format:
 ** Example:
 "Item 1\n
 > [TIO] Subitem 1\n
 >> [TIO] Subsubitem 1\n
 >> [TIO] Subsubitem 2
 > [TIO] Subitem 2\n
 > [TIO] Subitem 3\n
 [TIO] Item 2\n
 [TIO] Item 3\n
 [TIO] Item 4\n"
 Use > without text decriptor to create empty item (without text or picture or
 custom widget).
 Otherwise, you must add a flag to indicate the content of the item:
 T => Text
 I => Image decoration. The rest of the string must define a valid image path
 O => Custom widget. The rest of the string is passed to the given callback
 TI (or IT) => Image + text. The rest of the string has two parts:
 <image path>|<text>
	 <image path> can contain spaces. Use '|' as the separator.
	 <text> begins with the first non-space character after the '|' separator,
	 	and ends with the end line marker \n.
 */
wMenuBar *wMenuBar::fromStringDescriptor(
  const std::string &descr,
  bool orientation,
  void *userdata,
  void (*itemCreated)(void *userdata, wMenuItem *))
{
  std::stack<wMenuBar *> stbars;
  int l = descr.size();
  int chgrp = 0;
  int subitemlevel = 0;
  int lastLevel = -1;
  int mode = 0;
  int itemtype = 0;
  int itemid = 0;
  int firstpathbegin = 0;
  int firstpathend = 0;
  int secondpathbegin = 0; // the 'second path' ends before the \n
  bool ignorespaces = true;
  bool error = false;
  wMenuItem *lastmi = 0;
  /* mode:
   * 0 > level detection mode
   * 1 > level detected, object flag mode
   * 2 > flag detected, content determination mode
   */
  for(int i = 0; !error && i < l; i++)
    {
      if(descr[i] == '\n')
        {
          // new line:
          // create sub-item (a menu bar containing one menu item...
          // create a new sub-menu bar
          if(subitemlevel > lastLevel)
            {
              wMenuBar *nmb = new wMenuBar
              (
                0,
                0,
                orientation && stbars.empty(),
                orientation && stbars.empty()
              );
              if(lastLevel > -1)
                {
                  ((wMenuItem *)stbars.top()->getLastChild())->setSubChildPopupMenu
                  (
                    !(orientation && subitemlevel == 1),
                    nmb
                  );
                  stbars.top()->updateSize();
                  stbars.top()->updatePositions();
                }
              stbars.push(nmb);
            }
          else
            {
              // return on an old  menu bar
              while(subitemlevel < lastLevel)
                {
                  stbars.pop();
                  lastLevel--;
                }
            }
          // add menu bar's child
          wMenuBar *mbar = stbars.top();
          lastmi =
            new wMenuItem(
            descr.substr(
              firstpathbegin,
              ((AND_LETTER(itemtype, (':')) && !AND_LETTER(itemtype, ':')) ? i - 1:firstpathend) - firstpathbegin + 1
            ),
            (AND_LETTER(itemtype, ':') && AND_LETTER(itemtype, ';')) ? descr.substr(secondpathbegin, i - secondpathbegin) : "",
            itemtype,
            chgrp,
            itemid,
            (AND_LETTER(itemtype, '@')),
            (AND_LETTER(itemtype, '=')),
            (orientation && subitemlevel == 0)
          );
          mbar->addChild(
            lastmi,false ,
            5, 5, 5, 5
          );
          if(itemCreated)
            itemCreated(userdata,lastmi);

          // and reinit search
          itemid = 0;
          ignorespaces = true;
          lastLevel = subitemlevel;
          chgrp = 0;
          subitemlevel = 0;
          mode = 0;
          itemtype = 0;
          firstpathbegin = 0;
          firstpathend = 0;
          secondpathbegin = 0;
        }
      else if(mode == 0)
        {
          if(descr[i] == '>')
            {
              subitemlevel++;
              ignorespaces = false;
            }
          else if(descr[i] == ' ')
            {
              if(ignorespaces)
                continue;
              // else quit level detection
              mode++;
              ignorespaces = true;
            }
          else
            {
              i--; // to not lost the first letter
              mode++;
              ignorespaces = true;
            }
        }
      else if(mode == 1)
        {
          if(descr[i] == '_')
            continue; // ignore underscores
          if(descr[i] == ' ')
            {
              if(ignorespaces)
                continue;
              // else quit flag mode
              mode++;
              ignorespaces = true;
            }
          else if(descr[i] >= ':' && descr[i] <= '@')
            {
              itemtype |= (1 << (descr[i] - ':'));
              ignorespaces = false;
            }
          else if (descr[i] >= 'A' && descr[i] <= 'Z')
            chgrp += descr[i];
          else if(descr[i] >= '0' && descr[i] <= '9')
            itemid = itemid * 10 + (descr[i] - '0');
          else
            error = true;
        }
      else if(mode == 2)
        {
          if(descr[i] == ' ')
            {
              if(ignorespaces)
                continue;
            }
          if(AND_LETTER(itemtype, ':') ^ AND_LETTER(itemtype, ';'))
            {
              if(descr[i] != ' ')
                {
                  if(ignorespaces)
                    {
                      firstpathbegin = i;
                      secondpathbegin = i;
                      ignorespaces = false;
                    }
                  firstpathend = i;
                }
            }
          else if(AND_LETTER(itemtype, ':') && AND_LETTER(itemtype, ';'))
            {
              if(descr[i] != ' ')
                {
                  if(ignorespaces)
                    {
                      if(!secondpathbegin)
                        firstpathbegin = i;
                      else
                        secondpathbegin = i;
                      ignorespaces = false;
                    }
                  if(!secondpathbegin && descr[i] == '|')
                    {
                      ignorespaces = true;
                      secondpathbegin = i + 1;
                    }
                  else if(!secondpathbegin)
                    firstpathend = i;
                }
            }
          else
            {
              // TODO: custom widget as menu item
            }
        }
    }
  if(error)
    printf("ERROR in menu string descriptor.\n");
  wMenuBar *res = 0;
  while(!stbars.empty())
    {
      res = stbars.top();
      stbars.pop();
    }
  return res;
}

void wMenuBar::close()
{
  // TODO: free sfml's ressources
}

void wMenuBar::draw(sf::RenderWindow &rw)
{
  sf::RectangleShape rs (sf::Vector2f (wWidget::rect.width,
                                       wWidget::rect.height));
  rs.setPosition (wWidget::rect.left, wWidget::rect.top);
  rs.setFillColor(sf::Color(20, 20, 20, 50));
  rw.draw (rs);
  wContener::draw(rw);
}

bool wMenuBar::interpretEvent(sf::Event &ev, float mx, float my)
{
  bool interpret = false;
  for(unsigned int i = 0; i < fils.size(); i++)
    {
      if(fils[i]->getIsVisible())
        {
          bool inter = fils[i]->interpretEvent(ev, mx, my);
          if(ev.type == sf::Event::MouseButtonPressed)
            {
              if(inter)
                {
                  interpret = true;
                  ((wMenuItem *)fils[i])->expand(true);
                  if(((wMenuItem *)fils[i])->getGroup())
                    {
                      for(unsigned int j = 0; j < fils.size(); j++)
                        {
                          if(j != i && ((wMenuItem *)fils[i])->getGroup() == ((wMenuItem *)fils[j])->getGroup())
                            ((wMenuItem *)fils[j])->groupUnselect();
                        }
                    }
                }
              else
                ((wMenuItem *)fils[i])->expand(false);
            }
        }
    }
  return interpret;
}

