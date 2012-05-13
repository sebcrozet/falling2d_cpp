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
#include "wContener.h"

wContener::wContener(
  int x,
  int y,
  int w,
  int h,
  bool visible,
  int order,
  int prl,
  int pud,
  wContener::InsertMode imode,
  wContener::WrapMode wmode
)
  : wWidget(x, y, w, h, visible, order),
    paddingRL(prl),
    paddingUD(pud),
    im(imode), wm(wmode)
{ }

wContener::~wContener()
{
	for(unsigned int i = 0; i < fils.size(); i++)
		delete fils[i];
	for(unsigned int i = 0; i < filsgrid.size(); i++)
		delete filsgrid[i];
}

wWidget *wContener::getLastChild()
{
  return fils.size() ? fils[fils.size() -1] : 0;
}

bool wContener::setSize(float w, float h)
{
  if(h > 0)// && im == wContener::alignLeftRight)
    rect.height = h;
  if(w > 0)// && im == wContener::alignTopBottom)
    rect.width = w;
  //updateSize();
  updatePositions();
  return false;
}
bool wContener::setCoords(float x, float y)
{
  translate(x - rect.left, y - rect.top);
  return true;
}
bool wContener::translate(float x, float y)
{
  if(im!=wContener::absolute)
    {
      for(unsigned int i = 0; i < fils.size(); i++)
        fils[i]->translate(x, y);
    }
  rect.left += x;
  rect.top += y;
  return true;
}

void wContener::addChild(
  wWidget *w,
  bool addFront,
  int mL,
  int mR,
  int mU,
  int mD
)
{
  // insert the object at the good place
  // according to its zOrder
  if(w->getZOrder() == -1)
    {
      // push front
      fils.push_back(w); // to allocate one more cell
      int i = fils.size() - 2;
      for(; i >= 0; i--)
        {
          fils[i]->setId(i + 1);
          fils[i + 1] = fils[i];
        }
      fils[0] =  w;
      w->setId(0);

    }
  else if(w->getZOrder() == -2)
    {
      // just push back
      w->setId(fils.size());
      fils.push_back(w);
    }
  else
    {
      // insert behind all top most windows
      fils.push_back(w); // to allocate one more cell
      int i = fils.size() - 2;
      for(; i >= 0; i--)
        {
          if(fils[i]->getZOrder() == -2)
            {
              fils[i + 1] = fils[i];
              fils[i + 1]->setId(i + 1);
            }
          else
            break;
        }
      fils[i + 1] =  w;
      w->setId(i + 1);
    }

  // create apropriate structure
  PositionWrapper *pw = new PositionWrapper(w, mL, mR, mU, mD);
  // according to the object's alignment
  if(addFront)
    {
      // push front
      filsgrid.push_back(pw); // to allocate one more cell
      int i = filsgrid.size() - 2;
      for(; i >= 0; i--)
        filsgrid[i + 1] = filsgrid[i];
      filsgrid[0] =  pw;
    }
  else
    {
      // just push back
      filsgrid.push_back(pw);
    }
  updateSize();
  updatePositions();
  // TODO?: notify the parent ?
}

void wContener::bringToTop(int id)
{
  wWidget *curr = fils[id];
  if(curr->getZOrder() == -1)
    return;
  unsigned int i;
  for(i = id + 1; i < fils.size(); i++)
    {
      if(fils[i]->getZOrder() != -2) // not top most
        {
          fils[i]->setId(i - 1);
          fils[i - 1] = fils[i];
        }
      else if (curr->getZOrder() != -2)
        break;
    }
  fils[i - 1] = curr;
  curr->setId(i - 1);
}

void wContener::updatePositions()
{
  // updates sub-objects'positions
  if(im == wContener::alignLeftRight)
    {
      int currx = rect.left + paddingRL;
      for(unsigned int i = 0; i < filsgrid.size(); i++)
        {
          wWidget *ww = filsgrid[i]->w;
          currx += filsgrid[i]->mL;
          ww->setCoords(currx, rect.top + paddingUD);
          currx += ww->getRect().width + filsgrid[i]->mR;
        }
    }
  else if(im == wContener::alignTopBottom)
    {
      int curry = rect.top + paddingUD;
      for(unsigned int i = 0; i < filsgrid.size(); i++)
        {
          wWidget *ww = filsgrid[i]->w;
          curry += filsgrid[i]->mU;
          ww->setCoords(rect.left + paddingRL, curry);
          curry += ww->getRect().height + filsgrid[i]->mD;
        }
    }
  // adapt sub objects' size if needed
  if(im == wContener::alignTopBottom)
    {
      for(unsigned int i = 0; i < filsgrid.size(); i++)
        {
          wWidget *wdg = filsgrid[i]->w;
          wdg->setSize(
            rect.width - 2 * paddingRL - filsgrid[i]->mR - filsgrid[i]->mL,
            -1
          );
          Rect rc = wdg->getRect();
          wdg->setX(
            rect.left
            + (rect.width - 2 * paddingRL - filsgrid[i]->mR - filsgrid[i]->mL)/2.f
            - rc.width / 2.f + paddingRL + filsgrid[i]->mL
          );
        }
    }
  if(im == wContener::alignLeftRight)
    {
      for(unsigned int i = 0; i < filsgrid.size(); i++)
        {
          wWidget *wdg = filsgrid[i]->w;
          wdg->setSize(
            -1 ,
            rect.height - 2 * paddingUD - filsgrid[i]->mU - filsgrid[i]->mD
          );
          Rect rc = wdg->getRect();
          wdg->setY(rect.top + (rect.height - 2 * paddingUD - filsgrid[i]->mU - filsgrid[i]->mD) / 2.f - rc.height / 2.f + paddingUD + filsgrid[i]->mU);
        }
    }
}

void wContener::updateSize()
{
  // update current size according to this object wrapping mode
  float  neww = 0;
  float newh = 0;
  if(AND(wm, wContener::wrapW))
    {
      for(unsigned int i = 0; i < filsgrid.size(); i++)
        {
          float w = filsgrid[i]->w->getRect().width
                    + filsgrid[i]->mR
                    + filsgrid[i]->mL;
          if(im == wContener::alignLeftRight)
            neww += w;
          else
            {
              // find max width
              if(neww < w)
                neww = w;
            }
        }
      neww += 2*paddingRL;
      wWidget::setSize(neww, -1);
    }
  if(AND(wm, wContener::wrapH))
    {
      for(unsigned int i = 0; i < filsgrid.size(); i++)
        {
          int h = filsgrid[i]->w->getRect().height
                  + filsgrid[i]->mU
                  + filsgrid[i]->mD;
          if(im == wContener::alignTopBottom)
            newh += h;
          else
            {
              if(newh < h)
                newh = h;
            }
        }
      newh += 2*paddingUD;
      wWidget::setSize(-1, newh);
    }

}

void wContener::draw(sf::RenderWindow &rw)
{
  for(unsigned int i = 0; i < fils.size(); i++)
    if(fils[i]->getIsVisible())
      fils[i]->draw(rw);
}

void wContener::update(float dt)
{
  for(unsigned int i = 0; i < fils.size(); i++)
    fils[i]->update(dt);
}

bool wContener::interpretEvent(sf::Event &ev, float mx, float my)
{
  for(unsigned int i = 0; i < fils.size(); i++)
    {
      if(fils[i]->interpretEvent(ev, mx, my))
        return true;
    }
  return false;
}

void wContener::close()
{
  for(unsigned int i = 0; i < fils.size(); i++)
    fils[i]->close();
  // TODO: free sfml's resources for this object

}


// Position wrapper
PositionWrapper::PositionWrapper(wWidget *w, int mL, int mR, int mU, int mD)
  : w(w), mL(mL), mR(mR), mU(mU), mD(mD)
{ }
