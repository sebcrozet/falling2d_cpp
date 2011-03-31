#include "stdafx.h"
#include "wImage.h"

wImage::wImage(int x, int y, int w, int h, std::string path, int stretch)
  : wWidget(x, y, w, h, true, -2),
    stretchMode(stretch)
{
  sf::Image imge;
  useShape = testPredefShape(path, w, h);
  if(!useShape)
    {
      if(!imge.LoadFromFile(path))
        {
          // TODO : draw error image
        }
      else
        {
          img.SetImage(imge);
        }
    }
}


bool wImage::interpretEvent(sf::Event &ev, float mx, float my)
{
  return false;
}

void wImage::update(float dt)
{ }

bool wImage::translate(float x, float y)
{
  if(useShape)
    {
      for(int i=0; i<shs.size(); i++)
        shs[i]->Move(x,y);
    }
  else
    img.Move(x,y);
  wWidget::translate(x, y);
  return true;
}

void wImage::draw(sf::RenderWindow &rw)
{
  if(useShape)
    for(int i=0; i<shs.size(); i++)
      rw.Draw(*shs[i]);
  else
    rw.Draw(img);
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
          for(int i = 0; i < shs.size(); i++)
            shs[i]->Scale(1, h / wWidget::rect.GetHeight());
        }
      else
        img.Scale(1, h / wWidget::rect.GetHeight());
      wWidget::setSize(-1, h);
      return true;
    }
  if(stretchMode & wImage::STRETCH_W)
    {
      if(useShape)
        {
          for(int i = 0; i < shs.size(); i++)
            shs[i]->Scale(w /  wWidget::rect.GetWidth(), 1);
        }
      else
        img.Scale(w /  wWidget::rect.GetWidth(), 1);
      wWidget::setSize(w, -1);
      return true;
    }
  return false;
}

bool wImage::testPredefShape(std::string descr, float w, float h)
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
  sf::Shape *sh = new sf::Shape();
  shs.push_back(sh);
  sh->AddPoint(w/2.f,0,sf::Color(222, 199, 158));
  sh->AddPoint(0, h,sf::Color(222, 199, 158));
  sh->AddPoint(w, h,sf::Color(222, 199, 158));
  // exclamation point (bar)
  sh = new sf::Shape();
  shs.push_back(sh);
  sh->AddPoint(w/2.f - w/20.f, 3.f * h/10.f,sf::Color(0, 0, 0));
  sh->AddPoint(w/2.f - w/20.f, 7.f * h/10.f,sf::Color(0, 0, 0));
  sh->AddPoint(w/2.f + w/20.f, 7.f * h/10.f,sf::Color(0, 0, 0));
  sh->AddPoint(w/2.f + w/20.f, 3.f * h/10.f,sf::Color(0, 0, 0));
  // exclamation point (point)
  sh = new sf::Shape();
  shs.push_back(sh);
  sh->AddPoint(w/2.f - w/20.f, 8.f * h/10.f,sf::Color(0, 0, 0));
  sh->AddPoint(w/2.f - w/20.f, 9.f * h/10.f,sf::Color(0, 0, 0));
  sh->AddPoint(w/2.f + w/20.f, 9.f * h/10.f,sf::Color(0, 0, 0));
  sh->AddPoint(w/2.f + w/20.f, 8.f * h/10.f,sf::Color(0, 0, 0));
}

void wImage::shapeError(float w, float h)
{
  // base rectangle
  sf::Shape *sh = new sf::Shape();
  shs.push_back(sh);
  *sh = sf::Shape::Rectangle(0, 0, w, h,sf::Color(255, 100, 100), 2, sf::Color(200, 0, 0));
  // X : / part
  sh = new sf::Shape();
  shs.push_back(sh);
  sh->AddPoint(w / 10.f, h / 10.f,sf::Color(0, 0, 0));
  sh->AddPoint(7.f * w / 10.f, 9.f * h / 10.f,sf::Color(0, 0, 0));
  sh->AddPoint(9.f * w / 10.f, 9.f * h / 10.f,sf::Color(0, 0, 0));
  sh->AddPoint(3.f * w / 10.f, h /10.f,sf::Color(0, 0, 0));
  // X : \ part
  sh = new sf::Shape();
  shs.push_back(sh);
  sh->AddPoint(w / 10.f, 9.0f * h / 10.f,sf::Color(0, 0, 0));
  sh->AddPoint(7.f * w / 10.f, h / 10.f,sf::Color(0, 0, 0));
  sh->AddPoint(9.f * w / 10.f, h / 10.f,sf::Color(0, 0, 0));
  sh->AddPoint(3.f * w / 10.f, 9.0f * h /10.f,sf::Color(0, 0, 0));
}

void wImage::shapePoly(float w, float h)
{
  // top part
  sf::Shape *sh = new sf::Shape();
  shs.push_back(sh);
  sh->AddPoint(1.f * w / 10.f, 1.f * h / 10.f, sf::Color(100,100,100));
  sh->AddPoint(2.f * w / 10.f, 4.f * h / 10.f, sf::Color(100,100,100));
  sh->AddPoint(w, 5.f * h / 10.f, sf::Color(100,100,100));
  sh->AddPoint(8.f * w / 10.f, 0, sf::Color(100,100,100));
  // bottom part
  sh = new sf::Shape();
  shs.push_back(sh);
  sh->AddPoint(2.f * w / 10.f, 4.f * h / 10.f, sf::Color(100,100,100));
  sh->AddPoint(0, 7.f * h / 10.f, sf::Color(100,100,100));
  sh->AddPoint(5.f * w / 10.f, h, sf::Color(100,100,100));
  sh->AddPoint(w, 5.f * h / 10.f, sf::Color(100,100,100));
}

void wImage::shapePolyPoints(float w, float h)
{
  sf::Shape *sh = new sf::Shape();
  shs.push_back(sh);
  *sh = sf::Shape::Circle(1.f * w / 10.f, 1.f * h / 10.f, 2, sf::Color(100,100,100));
  sh = new sf::Shape();
  shs.push_back(sh);
  *sh = sf::Shape::Circle(2.f * w / 10.f, 4.f * h / 10.f,2, sf::Color(100,100,100));
  sh = new sf::Shape();
  shs.push_back(sh);
  *sh = sf::Shape::Circle(w, 5.f * h / 10.f, 2,sf::Color(100,100,100));
  sh = new sf::Shape();
  shs.push_back(sh);
  *sh = sf::Shape::Circle(8.f * w / 10.f, 0, 2,sf::Color(100,100,100));
  // bottom part
  sh = new sf::Shape();
  shs.push_back(sh);
  *sh = sf::Shape::Circle(0, 7.f * h / 10.f, 2,sf::Color(100,100,100));
  sh = new sf::Shape();
  shs.push_back(sh);
  *sh = sf::Shape::Circle(5.f * w / 10.f, h, 2,sf::Color(100,100,100));
}

void wImage::shapeStop(float w, float h)
{
  sf::Shape *sh = new sf::Shape();
  shs.push_back(sh);
  sh->AddPoint(0,0,sf::Color(255, 255, 255));
  sh->AddPoint(0,h,sf::Color(255, 255, 255));
  sh->AddPoint(w,h,sf::Color(255, 255, 255));
  sh->AddPoint(w,0,sf::Color(255, 255, 255));
}
void wImage::shapePause(float w, float h)
{
  sf::Shape *sh = new sf::Shape();
  shs.push_back(sh);
  sh->AddPoint(0,0,sf::Color(255, 255, 255), sf::Color(100,100,100));
  sh->AddPoint(0,h,sf::Color(255, 255, 255), sf::Color(100,100,100));
  sh->AddPoint(w/2.f,h,sf::Color(255, 255, 255), sf::Color(100,100,100));
  sh->AddPoint(w/2.f,0,sf::Color(255, 255, 255), sf::Color(100,100,100));
  sh->SetOutlineWidth(2.f);

  sh = new sf::Shape();
  shs.push_back(sh);
  sh->AddPoint(w/2.f,0,sf::Color(255, 255, 255), sf::Color(100,100,100));
  sh->AddPoint(w/2.f,h,sf::Color(255, 255, 255), sf::Color(100,100,100));
  sh->AddPoint(w,h,sf::Color(255, 255, 255), sf::Color(100,100,100));
  sh->AddPoint(w,0,sf::Color(255, 255, 255), sf::Color(100,100,100));
  sh->SetOutlineWidth(2.f);
}
void wImage::shapeLect(float w, float h)
{
  // Same as shapetickR ?
  sf::Shape *sh = new sf::Shape();
  shs.push_back(sh);
  sh->AddPoint(0,0,sf::Color(255, 255, 255), sf::Color(100,100,100));
  sh->AddPoint(0,h,sf::Color(255, 255, 255), sf::Color(100,100,100));
  sh->AddPoint(h,h/2.f,sf::Color(255, 255, 255), sf::Color(100,100,100));
  sh->SetOutlineWidth(2.f);
}
void wImage::shapeTickR(float w, float h)
{
  sf::Shape *sh = new sf::Shape();
  shs.push_back(sh);
  sh->AddPoint(0,0,sf::Color(100,100,100));
  sh->AddPoint(0,h,sf::Color(100,100,100));
  sh->AddPoint(h,h/2.f,sf::Color(100,100,100));
}
void wImage::shapeTickD(float w, float h)
{
  sf::Shape *sh = new sf::Shape();
  shs.push_back(sh);
  sh->AddPoint(0,0,sf::Color(100,100,100),sf::Color(10,10,10));
  sh->AddPoint(w/2.f,h,sf::Color(100,100,100),sf::Color(10,10,10));
  sh->AddPoint(w,0,sf::Color(100,100,100),sf::Color(10,10,10));
}

void wImage::shapeRectangle(float w, float h)
{
  sf::Shape *sh = new sf::Shape();
  shs.push_back(sh);
  *sh = sf::Shape::Rectangle(0, 0, w, h,sf::Color(100, 100, 100), 2, sf::Color(200, 200, 200));
}

void wImage::shapeCircle(float w, float h)
{
  sf::Shape *sh = new sf::Shape();
  shs.push_back(sh);
  *sh = sf::Shape::Circle(w/2.f, h/2.f, w/2.f,sf::Color(100, 100, 100), 2, sf::Color(200, 200, 200));
}

void wImage::shapeDelete(float w, float h)
{
}

void wImage::shapeSeparator(float w, float h)
{
  sf::Shape *sh = new sf::Shape();
  shs.push_back(sh);
  *sh = sf::Shape::Rectangle(0, 0, w, h,sf::Color(200, 200, 200));
}
