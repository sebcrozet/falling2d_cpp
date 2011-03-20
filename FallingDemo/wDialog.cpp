#include "stdafx.h"
#include "wDialog.h"


wDialog::wDialog(float x, float y, std::string title) 
: wContener(x,y,10,10,true,2,2,-2,wContener::alignTopBottom, wContener::wrapBoth),
    corp(0,0, 200,200, true, -2, 2, 2,wContener::alignLeftRight, wContener::wrapBoth),
    buttonArea(0, 0, 200, 50, true, -2, 2, 2, wContener::alignLeftRight, wContener::wrapBoth),
    separator(0, 0, 2, 2, "SE", wImage::STRETCH_W),
    titlebar(x,y,title,19),
    dragging(false)
{
    setCoords(x,y);
    addChild(&titlebar, false, 0, 0, 0, 0);
    addChild(&separator, false, 0, 0, 0, 0);
    addChild(&corp, false, 0,0,0,0);
    addChild(&buttonArea, false, 0, 0, 0, 0);
    updateSize();
}

bool wDialog::interpretEvent(sf::Event &ev, float mx, float my)
{
    if(!wContener::interpretEvent(ev, mx, my))
    {
	if(ev.Type == sf::Event::MouseMoved)
	{					
	    if(dragging)
	    {
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

void wDialog::addCorpChild(wWidget *w)
{ corp.addChild(w, false, 1, 1, 1, 1); }

void wDialog::addButton(ButtonType type, void *userdata, void (*itempushed)(int id, void *userdata))
{
    wMenuItem *mi;
    switch(type)
    {
	case wDialog::BUTTON_YES:
	    mi = new wMenuItem("Yes", "", 1, 0,0, false, false, true);
	    break;
	case wDialog::BUTTON_NO:
	    mi = new wMenuItem("No", "", 1, 0,0, false, false, true);
	    break;
	case wDialog::BUTTON_OK:
	    mi = new wMenuItem("OK", "", 1, 0,0, false, false, true);
	    break;
	case wDialog::BUTTON_APPLY:
	    mi = new wMenuItem("Apply", "", 1, 0,0, false, false, true);
	    break;
	case wDialog::BUTTON_RETRY:
	    mi = new wMenuItem("Retry", "", 1, 0,0, false, false, true);
	    break;
	case wDialog::BUTTON_CANCEL:
	    mi = new wMenuItem("Cancel", "", 1, 0,0, false, false, true);
	    break;
    }
    mi->setUserDatas(userdata);
    mi->setOnItemReleased(itempushed);
    buttonArea.addChild(mi, false, 4, 4, 1, 1);
}

void wDialog::close()
{
    // TODO: free sfml's ressources
}

/*
 * static mechods
 */
wDialog *wDialog::getErrorDialog(float x, float y, std::string text)
{
    wDialog *res = new wDialog(x, y, "ERROR");
    res->addCorpChild(new wImage(0,0, 50, 50, "ER", wImage::STRETCH_NONE));
    res->addCorpChild(new wLabel(0,0, text, 10));
    return res;
}

wDialog *wDialog::getWarningDialog(float x, float y, std::string text)
{
    wDialog *res = new wDialog(x, y, "ERROR");
    res->addCorpChild(new wImage(0,0, 50, 50, "WA", wImage::STRETCH_NONE));
    res->addCorpChild(new wLabel(0,0, text, 10));
    return res;
}
