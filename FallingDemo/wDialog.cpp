#include "stdafx.h"
#include "wDialog.h"


wDialog::wDialog(int x, int y, int w, int h, std::string title) 
: wContener(x,y,x+w,y+h+TITLEBARSIZE,true,5,5,-2,wContener::alignLeftRight, wContener::wrapBoth),
    titlebar(x,y,title,19),
    dragging(false)
{
    setCoords(x,y);
    setSize(w,TITLEBARSIZE + h);
    this->addChild(&(this->titlebar), false, 0,0,0,0);
    this->addChild((new wLabel(0,0, "blblbl", 20)), false, 0,0,0,0);
    this->addChild((new wLabel(0,0, "blauiersauieeeeeerteetetetettblbl", 10)), false, 0,0,0,0);
}

bool wDialog::interpretEvent(sf::Event &ev, float mx, float my)
{
    if(!wContener::interpretEvent(ev, mx, my))
    {
	if(ev.Type == sf::Event::MouseMoved)
	{					
	    if(dragging)
	    {
		printf("translating this \n");
		float dx = mx - lastx;
		float dy = my - lasty;
		if(rect.Left + dx < 0)
		    dx = -rect.Left;
		if(rect.Top + dy < 0)
		    dy = -rect.Top;
		wContener::translate(dx, dy);
		lastx = mx;
		lasty = my;
		return true;
	    }
	}
	else if(ev.Type == sf::Event::MouseButtonPressed)
	{

	    if(wWidget::contains(mx , my))
	    {
		dragging = true;
		lastx = mx;
		lasty = my;
	    }
	}
	else if(ev.Type == sf::Event::MouseButtonReleased)
	{
	    if(dragging)
	    {
		dragging = false;
		return true;
	    }
	}
	return false;
    }
    else
	return false;
}

void wDialog::draw(sf::RenderWindow &rw)
{
    rw.Draw(sf::Shape::Rectangle(wWidget::rect.Left, wWidget::rect.Top, wWidget::rect.Right, wWidget::rect.Bottom, sf::Color(20,20,20,10)));
    wContener::draw(rw);
}

void wDialog::close()
{
    // TODO: free sfml's ressources
}
