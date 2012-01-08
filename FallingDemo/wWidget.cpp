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
#include "wWidget.h"

sf::View wWidget::view = sf::View(sf::FloatRect(0,0,800,600));
wWidget::wWidget(int x, int y, int w, int h, bool visible, int order)
  : rect(x, y, x + w, y + h), isVisible(visible), zOrder(order), id(0)
{ }

wWidget::~wWidget()
{ }


bool wWidget::setSize(float w, float h)
{
  if(h >= 0)
    rect.Bottom = rect.Top + h;
  if(w >= 0)
    rect.Right = rect.Left + w;
  return true;
}
bool wWidget::setCoords(float x, float y)
{
  return translate(x - rect.Left, y - rect.Top);
}

bool wWidget::setX(float x)
{
  return translate(x - rect.Left, 0);
}
bool wWidget::setY(float y)
{
  return translate(0,y - rect.Top);
}
bool wWidget::setRect(float x, float y, float w, float h)
{
  setCoords(x,y);
  setSize(w, h);
  return true;
}
bool wWidget::translate(float x, float y)
{
  rect.Offset(x,y);
  return true;
}
