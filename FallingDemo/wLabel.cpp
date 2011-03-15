#include "stdafx.h"
#include "wLabel.h"

sf::Font wLabel::f = sf::Font();
bool wLabel::fontinited = false;


wLabel::wLabel(int x, int y, std::string text, int fontSize)
    : wWidget(x, y, 0,0 , true, -2)
{
    printf("\n");
    printf("\n");
    printf("String TITLE: ");
    printf("%s", text.c_str());
    printf("\n");
    printf("\n");
    if(!wLabel::fontinited)
    {
	wLabel::fontinited = true;
	wLabel::f.LoadFromFile("Inconsolata.otf", fontSize);
    }
    dTitle = sf::String(text, wLabel::f, fontSize); 
    dTitle.SetX(x);
    dTitle.SetY(y);
    wWidget::setSize(dTitle.GetRect().GetWidth(), dTitle.GetRect().GetHeight() * 1.5f);
}

void wLabel::draw(sf::RenderWindow &rw)
{
    rw.Draw(dTitle);
}

bool wLabel::setSize(float w, float h)
{ return false; }

bool wLabel::translate(float x, float y)
{
    dTitle.Move(x, y);
    return wWidget::translate(x, y);
}

bool wLabel::interpretEvent(sf::Event &, float, float)
{ return false; }

void wLabel::update(float)
{ }

void wLabel::close()
{
    // TODO: free sfml's ressources
}
