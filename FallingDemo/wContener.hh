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
#ifndef __WCONTENER
#include "stdafx.hh"
#include <vector>
#include "wWidget.hh"
#define AND(var,mask) ((var & mask) != 0)
struct PositionWrapper
{
  wWidget *w;
  int mL, mR, mU, mD;

  PositionWrapper(
    wWidget *w,
    int mL, int mR, int mU, int mD
  );
};

class wContener : public wWidget
{
public:
  enum InsertMode
  {
    absolute = 0,
    alignLeftRight = 1,
    alignTopBottom = 2
  };
  enum WrapMode
  {
    wrapW = 1,
    wrapH = 2,
    wrapBoth = 3,
    noWrap = 0
  };
protected:
  std::vector<wWidget *>fils;
private:
  int paddingRL, paddingUD;
  wContener::InsertMode im;
  wContener::WrapMode wm;
  std::vector<PositionWrapper *>filsgrid;


private:
  void bringToTop(int id);

protected:

  virtual bool setSize(float w, float h);
  virtual bool setCoords(float x, float y);
  virtual bool translate(float x, float y);

public:
  wContener
  (
    int x, int y, int w,
    int h, bool visible,
    int prl, int pud,
    int zOrder,
    wContener::InsertMode im,
    wContener::WrapMode wm
  );
  virtual ~wContener();

  void updatePositions();
  void updateSize();
  void addChild
  (
    wWidget *widget,
    bool addfront,
    int mL,
    int mR,
    int mU,
    int mD
  );
  wWidget *getLastChild();
  virtual void close();
  virtual bool interpretEvent(sf::Event &ev, float mx, float my);
  virtual void update(float dt);
  virtual void draw(sf::RenderWindow &rw);
};


#define __WCONTENER
#endif
