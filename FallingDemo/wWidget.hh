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
#ifndef __WWIDGET

#include "stdafx.hh"
#include <vector>
typedef sf::Rect<float> Rect;

class wWidget
{
public:
  static sf::View view;
protected:
  Rect rect;
  bool isVisible;
  int zOrder; // -1 => always behind -2 => top most
  int id;


protected:
  wWidget(
    int x, int y, int w,
    int h, bool visible,
    int zOrder);

public:
  virtual ~wWidget();
  virtual bool translate(float x, float y);
  virtual bool setSize(float w, float h);
  virtual bool setRect(float x, float y, float w, float h);
  bool setCoords(float x, float y);
  bool setX(float x);
  bool setY(float y);
  inline bool contains(float x, float y)
  {
    return isVisible && rect.contains(x, y);
  }
  inline Rect getRect() const
  {
    return rect;
  }
  inline int getId() const
  {
    return id;
  }
  inline bool getIsVisible() const
  {
    return isVisible;
  }
  inline int getZOrder() const
  {
    return zOrder;
  }
  inline void setId(int nid)
  {
    id = nid;
  }

  inline void setVisible(bool visible)
  {
    isVisible = visible;
  }

  virtual void close() = 0;
  virtual bool interpretEvent(sf::Event &ev, float mx, float my) = 0;
  virtual void update(float dt) = 0;
  virtual void draw(sf::RenderWindow &rw) = 0;
};

#define __WWIDGET
#endif
