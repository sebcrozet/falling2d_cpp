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
#include "wImage.h"

wImage::wImage(int x, int y, int w, int h,
               const std::string& path,
               int                stretch)
  : wWidget(x, y, w, h, true, -2),
    stretchMode(stretch)
{
  sf::Texture imge;
  useShape = testPredefShape(path, w, h);
  if(!useShape)
    {
      if(!imge.loadFromFile(path))
        {
          // TODO : draw error image
        }
      else
        {
          img.setTexture(imge);
        }
    }
}

wImage::~wImage()
{
	for(unsigned int i = 0; i < shs.size(); i++)
		delete shs[i];
}

bool wImage::interpretEvent(sf::Event &, float, float)
{
  return false;
}

void wImage::update(float)
{ }

bool wImage::translate(float x, float y)
{
  if(useShape)
    {
      for(unsigned int i=0; i<shs.size(); i++)
        shs[i]->move(x,y);
    }
  else
    img.move(x,y);
  wWidget::translate(x, y);
  return true;
}

void wImage::draw(sf::RenderWindow &rw)
{
  if(useShape)
    for(unsigned int i=0; i<shs.size(); i++)
      rw.draw(*shs[i]);
  else
    rw.draw(img);
}

void wImage::close()
{
  // TODO:
  // free sfml's resources
}

bool wImage::setSize(float w, float h)
{
  if(stretchMode & wImage::STRETCH_H)
    {
      if(useShape)
        {
          for(unsigned int i = 0; i < shs.size(); i++)
            shs[i]->scale(1, h / wWidget::rect.height);
        }
      else
        img.scale(1, h / wWidget::rect.height);
      wWidget::setSize(-1, h);
      return true;
    }
  if(stretchMode & wImage::STRETCH_W)
    {
      if(useShape)
        {
          for(unsigned int i = 0; i < shs.size(); i++)
            shs[i]->scale(w /  wWidget::rect.width, 1);
        }
      else
        img.scale(w /  wWidget::rect.width, 1);
      wWidget::setSize(w, -1);
      return true;
    }
  return false;
}

bool wImage::testPredefShape(const std::string &descr, float w, float h)
{
  if(descr == "TR")
    shapeTickR(w, h);
  else if(descr == "TD")
    shapeTickD(w, h);
  else if(descr == "ST")
    shapeStop(w, h);
  else if (descr == "PO")
    shapePause(w, h);
  else if(descr == "CI")
    shapeCircle(w, h);
  else if(descr == "RE")
    shapeRectangle(w, h);
  else if(descr == "LE")
    shapeLect(w, h);
  else if(descr == "ER")
    shapeError(w, h);
  else if(descr == "WA")
    shapeWarning(w, h);
  else if(descr == "LY")
    shapePoly(w, h);
  else if(descr == "LYPTS")
    shapePolyPoints(w, h);
  else if(descr == "SE")
    shapeSeparator(w, h);
  else
    return false;
  return true;
}

void wImage::shapeWarning(float w, float h)
{
  // base triangle
  sf::ConvexShape *sh = new sf::ConvexShape(3);
  shs.push_back(sh);
  sh->setPoint(0, sf::Vector2f (w / 2.f, 0));
  sh->setPoint(1, sf::Vector2f (0, h));
  sh->setPoint(2, sf::Vector2f (w, h));
  sh->setFillColor (sf::Color(222, 199, 158));
  // exclamation point (bar)
  sh = new sf::ConvexShape(4);
  shs.push_back(sh);
  sh->setPoint(0, sf::Vector2f (w / 2.f - w / 20.f, 3.f * h / 10.f));
  sh->setPoint(1, sf::Vector2f (w / 2.f - w / 20.f, 7.f * h / 10.f));
  sh->setPoint(2, sf::Vector2f (w / 2.f + w / 20.f, 7.f * h / 10.f));
  sh->setPoint(3, sf::Vector2f (w / 2.f + w / 20.f, 3.f * h / 10.f));
  sh->setFillColor (sf::Color(0, 0, 0));
  // exclamation point (point)
  sh = new sf::ConvexShape(4);
  shs.push_back(sh);
  sh->setPoint(0, sf::Vector2f (w / 2.f - w / 20.f, 8.f * h / 10.f));
  sh->setPoint(1, sf::Vector2f (w / 2.f - w / 20.f, 9.f * h / 10.f));
  sh->setPoint(2, sf::Vector2f (w / 2.f + w / 20.f, 9.f * h / 10.f));
  sh->setPoint(3, sf::Vector2f (w / 2.f + w / 20.f, 8.f * h / 10.f));
  sh->setFillColor (sf::Color(0, 0, 0));
}

void wImage::shapeError(float w, float h)
{
  /* FIXME: use SFML 2.0 functions…
  // base rectangle
  sf::ConvexShape *sh = new sf::Shape();
  shs.push_back(sh);
  *sh = sf::Rectangle(0, 0, w, h,sf::Color(255, 100, 100), 2, sf::Color(200, 0, 0));
  // X : / part
  sh = new sf::ConvexShape();
  shs.push_back(sh);
  sh->setPoint(0, sf::Vector2f (w / 10.f, h / 10.f,sf::Color(0, 0, 0));
  sh->setPoint(0, sf::Vector2f (7.f * w / 10.f, 9.f * h / 10.f,sf::Color(0, 0, 0));
  sh->setPoint(0, sf::Vector2f (9.f * w / 10.f, 9.f * h / 10.f,sf::Color(0, 0, 0));
  sh->setPoint(0, sf::Vector2f (3.f * w / 10.f, h /10.f,sf::Color(0, 0, 0));
  // X : \ part
  sh = new sf::ConvexShape();
  shs.push_back(sh);
  sh->setPoint(0, sf::Vector2f (w / 10.f, 9.0f * h / 10.f,sf::Color(0, 0, 0));
  sh->setPoint(0, sf::Vector2f (7.f * w / 10.f, h / 10.f,sf::Color(0, 0, 0));
  sh->setPoint(0, sf::Vector2f (9.f * w / 10.f, h / 10.f,sf::Color(0, 0, 0));
  sh->setPoint(0, sf::Vector2f (3.f * w / 10.f, 9.0f * h /10.f,sf::Color(0, 0, 0));
  */
}

void wImage::shapePoly(float w, float h)
{
  // top part
  sf::ConvexShape *sh = new sf::ConvexShape(4);
  shs.push_back(sh);
  sh->setPoint(0, sf::Vector2f (1.f * w / 10.f, 1.f * h / 10.f));
  sh->setPoint(1, sf::Vector2f (2.f * w / 10.f, 4.f * h / 10.f));
  sh->setPoint(2, sf::Vector2f (w, 5.f * h / 10.f));
  sh->setPoint(3, sf::Vector2f (8.f * w / 10.f, 0));
  sh->setFillColor (sf::Color(100,100,100));
  // bottom part
  sh = new sf::ConvexShape(4);
  shs.push_back(sh);
  sh->setPoint(0, sf::Vector2f (2.f * w / 10.f, 4.f * h / 10.f));
  sh->setPoint(1, sf::Vector2f (0, 7.f * h / 10.f));
  sh->setPoint(2, sf::Vector2f (5.f * w / 10.f, h));
  sh->setPoint(3, sf::Vector2f (w, 5.f * h / 10.f));
  sh->setFillColor (sf::Color(100,100,100));
}

void wImage::shapePolyPoints(float w, float h)
{
  sf::CircleShape *sh;
  sh = new sf::CircleShape(2);
  sh->setOrigin(2, 2);
  sh->setPosition (1.f * w / 10.f, 1.f * h / 10.f);
  sh->setFillColor (sf::Color(100,100,100));
  shs.push_back(sh);

  sh = new sf::CircleShape(2);
  sh->setOrigin(2, 2);
  sh->setPosition (2.f * w / 10.f, 4.f * h / 10.f);
  sh->setFillColor (sf::Color(100,100,100));
  shs.push_back(sh);

  sh = new sf::CircleShape(2);
  sh->setOrigin(2, 2);
  sh->setPosition (w, 5.f * h / 10.f);
  sh->setFillColor (sf::Color(100,100,100));
  shs.push_back(sh);

  sh = new sf::CircleShape(2);
  sh->setOrigin(2, 2);
  sh->setPosition (8.f * w / 10.f, 0);
  sh->setFillColor (sf::Color(100,100,100));
  shs.push_back(sh);

  // bottom part
  sh = new sf::CircleShape(2);
  sh->setOrigin(2, 2);
  sh->setPosition (7.f * h / 10.f, h);
  sh->setFillColor (sf::Color(100,100,100));
  shs.push_back(sh);

  sh = new sf::CircleShape(2);
  sh->setOrigin(2, 2);
  sh->setPosition (5.f * w / 10.f, h);
  sh->setFillColor (sf::Color(100,100,100));
  shs.push_back(sh);
}

void wImage::shapeStop(float w, float h)
{
  sf::ConvexShape* sh = new sf::ConvexShape(4);
  sh->setPoint(0, sf::Vector2f (0, 0));
  sh->setPoint(1, sf::Vector2f (0, h));
  sh->setPoint(2, sf::Vector2f (w, h));
  sh->setPoint(3, sf::Vector2f (w, 0));
  sh->setFillColor (sf::Color(255, 255, 255));
  shs.push_back(sh);
}
void wImage::shapePause(float w, float h)
{
  sf::ConvexShape *sh = new sf::ConvexShape(4);
  shs.push_back(sh);
  sh->setPoint(0, sf::Vector2f (0, 0));
  sh->setPoint(1, sf::Vector2f (0, h));
  sh->setPoint(2, sf::Vector2f (w / 2.f, h));
  sh->setPoint(3, sf::Vector2f (w / 2.f, 0));
  sh->setOutlineThickness(1);
  sh->setFillColor (sf::Color (255, 255, 255));
  sh->setOutlineColor (sf::Color (100, 100, 100));

  sh = new sf::ConvexShape(4);
  shs.push_back(sh);
  sh->setPoint(0, sf::Vector2f (w / 2.f, 0));
  sh->setPoint(1, sf::Vector2f (w / 2.f, h));
  sh->setPoint(2, sf::Vector2f (w, h));
  sh->setPoint(3, sf::Vector2f (w, 0));
  sh->setOutlineThickness(1);
  sh->setFillColor (sf::Color (255, 255, 255));
  sh->setOutlineColor (sf::Color (100, 100, 100));
}
void wImage::shapeLect(float w, float h)
{
  // Same as shapetickR ?
  sf::ConvexShape *sh = new sf::ConvexShape(3);
  shs.push_back(sh);
  sh->setPoint(0, sf::Vector2f (0,0));
  sh->setPoint(1, sf::Vector2f (0,h));
  sh->setPoint(2, sf::Vector2f (w,h/2.f));
  sh->setOutlineThickness(1);
  sh->setFillColor (sf::Color (255, 255, 255));
  sh->setOutlineColor (sf::Color (100, 100, 100));
}
void wImage::shapeTickR(float w, float h)
{
  sf::ConvexShape *sh = new sf::ConvexShape(3);
  shs.push_back(sh);
  sh->setPoint(0, sf::Vector2f (0,0));
  sh->setPoint(1, sf::Vector2f (0,h));
  sh->setPoint(2, sf::Vector2f (w,h/2.f));
  sh->setFillColor (sf::Color (100, 100, 100));
}
void wImage::shapeTickD(float w, float h)
{
  sf::ConvexShape *sh = new sf::ConvexShape(3);
  shs.push_back(sh);
  sh->setPoint(0, sf::Vector2f (0, 0));
  sh->setPoint(1, sf::Vector2f (w / 2.f, h));
  sh->setPoint(2, sf::Vector2f (w, 0));
  sh->setOutlineThickness(1);
  sh->setFillColor (sf::Color (10, 10, 10));
  sh->setOutlineColor (sf::Color (100, 100, 100));
}

void wImage::shapeRectangle(float w, float h)
{
  sf::Shape *sh = new sf::RectangleShape(sf::Vector2f (w, h));
  sh->setOutlineThickness(1.f);
  sh->setFillColor (sf::Color (200, 200, 200));
  sh->setOutlineColor(sf::Color (100, 100, 100));
  shs.push_back(sh);
}

void wImage::shapeCircle(float w, float h)
{
  sf::CircleShape *sh = new sf::CircleShape(w / 2.f);
  sh->setOrigin(w / 2.f, w / 2.f);
  sh->setPosition(h / 2.f, w / 2.f);
  sh->setOutlineThickness(1.f);
  sh->setFillColor(sf::Color (200, 200, 200));
  sh->setOutlineColor(sf::Color (100, 100, 100));
  shs.push_back(sh);
}

void wImage::shapeDelete(float, float)
{
    // FIXME
}

void wImage::shapeSeparator(float w, float h)
{
  sf::RectangleShape *sh = new sf::RectangleShape(sf::Vector2f (w, h));
  sh->setFillColor (sf::Color(200, 200, 200));
  shs.push_back(sh);
}
