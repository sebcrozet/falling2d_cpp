#include "stdafx.h"
#include "wLabel.h"

wLabel::wLabel(int x, int y, std::string text, int fontSize)
  : wWidget(x, y, 0,0 , true, -2)
{
  dTitle = sf::String(text, sf::Font::GetDefaultFont(), fontSize);
  dTitle.SetX(x);
  dTitle.SetY(y);
  wWidget::setSize(dTitle.GetRect().GetWidth(), dTitle.GetRect().GetHeight() + 2);
}

void wLabel::draw(sf::RenderWindow &rw)
{
  rw.Draw(dTitle);
}

bool wLabel::setSize(float w, float h)
{
  return false;
}

bool wLabel::translate(float x, float y)
{
  dTitle.Move(x, y);
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
