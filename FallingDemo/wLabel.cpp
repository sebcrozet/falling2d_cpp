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
#include "wLabel.h"

wLabel::wLabel(int x, int y, const std::string &text, int fontSize)
  : wWidget(x, y, 0,0 , true, -2)
{
  dTitle = new sf::String(text, sf::Font::GetDefaultFont(), fontSize);
  dTitle->SetX(x);
  dTitle->SetY(y);
  wWidget::setSize(dTitle->GetRect().GetWidth(), dTitle->GetRect().GetHeight() + 2);
}

wLabel::~wLabel()
{ delete dTitle; }

void wLabel::draw(sf::RenderWindow &rw)
{
  rw.Draw(*dTitle);
}

void wLabel::setText(const std::string &s, int r, int g, int b)
{
  dTitle->SetText(sf::Unicode::Text(s));
  dTitle->SetColor(sf::Color(r,g,b));
  wWidget::setSize(dTitle->GetRect().GetWidth(), dTitle->GetRect().GetHeight() + 2);
}

bool wLabel::setSize(float, float)
{
  return false;
}

bool wLabel::translate(float x, float y)
{
  dTitle->Move(x, y);
  return wWidget::translate(x, y);
}

bool wLabel::interpretEvent(sf::Event &, float, float)
{
  return false;
}

void wLabel::update(float)
{ }

void wLabel::close()
{
  // TODO: free sfml's ressources
}
