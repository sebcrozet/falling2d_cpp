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
#ifndef __WLABEL
#include "stdafx.h"
#include "wWidget.h"

class wLabel : public wWidget
{
private:
  sf::Text *dTitle;
public:
  wLabel(int x, int y, const std::string &text, int fontSize);
  ~wLabel();
  void draw(sf::RenderWindow &rw);
  void close();
  void setText(const std::string &text,int r, int g, int b);

  virtual bool translate(float x, float y);
  virtual bool interpretEvent(sf::Event &ev, float mx, float my);
  virtual void update(float f);
  virtual bool setSize(float w, float h);
};
#define __WLABEL
#endif
