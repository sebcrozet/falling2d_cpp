#include "stdafx.h"
#include "wImage.h"

wImage::wImage(int x, int y, int w, int h, std::string path)
    : wWidget(x, y, w, h, true, -2)
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
	for(int i=0;i<shs.size();i++)
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
	for(int i=0;i<shs.size();i++)
	    rw.Draw(*shs[i]);
    else
	rw.Draw(img);
}

void wImage::close()
{
    // TODO:
    // free sfml's resources
}

bool wImage::setSize(float w, float y)
{
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
    else
	return false;
    return true;
}

void wImage::shapeWarning(float w, float h)
{
    // base triangle
    sf::Shape *sh = new sf::Shape();
    shs.push_back(sh);


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
void wImage::shapePolyDraw(float w, float h)
{
}
void wImage::shapePolyPointDraw(float w, float h)
{
}
void wImage::shapeDelete(float w, float h)
{
}
