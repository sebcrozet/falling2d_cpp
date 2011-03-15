#include "stdafx.h"
#include "wMenu.h"

wMenuItem::wMenuItem(std::string path1, std::string path2, int type) : 
    wContener(0,0,0,0,true,-2,0,0,wContener::alignLeftRight, wContener::wrapBoth), 
    toogled(false), highlighted(false), onItemClicked(0), onItemPushed(0), onItemReleased(0),
    sw(0), greycolor(200)
{ 
    if(AND(type,2))
    {
	wContener::addChild(
		new wImage(0,0, 20, 20, path1),
		false,
		4, (type == 3)?9:4, 4, 4
		);
    }
    if(AND(type,1))
    {
	wContener::addChild(
		new wLabel(0,0,(type == 3)?path2:path1,15),
		false,
		0, 0, 0, 0
		);
    }
    else
    {
	// TODO: custom item ?
    }
}

void wMenuItem::close()
{ 
    // TODO: free SFML's ressources
}

void wMenuItem::expand(bool coll)
{
    if(sw)
	sw->setVisible(coll);
    //toogled = coll;
}

bool wMenuItem::setSize(float w, float h)
{
    wContener::setSize(w, h);
    updateSubMenuLocation();
}

bool wMenuItem::interpretEvent(sf::Event &ev, float mx, float my)
{
    if(!wContener::interpretEvent(ev, mx, my) && (!sw || !sw->getIsVisible() || !sw->interpretEvent(ev, mx, my)))
    {
	if(ev.Type == sf::Event::MouseMoved)
	{
		if(!wWidget::contains(mx, my))
		{
		    if(highlighted)
			highlighted= false;
		}
		else
		    highlighted = true;
	}
	else if(ev.Type == sf::Event::MouseButtonPressed)
	{
	    if(wWidget::contains(mx, my))
	    {
		toogled = true;
		if(onItemPushed)
		    onItemPushed();
		return true;
	    }
	    else
	    {
		if(toogled)
		{
		    toogled = false;
		    if(onItemReleased)
			onItemReleased();
		}
	    }
	}
	else if(ev.Type == sf::Event::MouseButtonReleased)
	{
	    if(toogled)
	    {
		//toogled = false;
		if(onItemReleased)
		    onItemReleased();
	    }
	}
	return false;
    }
    else
	return true;
}

void wMenuItem::update(float dt)
{
    // TODO: lose focus ?
}

void wMenuItem::draw(sf::RenderWindow &rw)
{
    // draw back
    if(toogled)
	greycolor = 10;
    else
    {
	if(highlighted)
	{
	    greycolor-=40;
	    if(greycolor < 60)
		greycolor = 60;
	}
	else
	{
	    greycolor+=20;
	    if (greycolor > 200)
		greycolor = 200;
	}
    }
    rw.Draw(
	    sf::Shape::Rectangle(
		wWidget::rect.Left, 
		wWidget::rect.Top, 
		wWidget::rect.Right, 
		wWidget::rect.Bottom, 
		sf::Color(greycolor,greycolor,greycolor,100), 2, sf::Color(greycolor,greycolor,greycolor,200))
	    );
    // draw text & image
    if(sw && sw->getIsVisible())
	sw->draw(rw);
    wContener::draw(rw);
}

void wMenuItem::updateSubMenuLocation()
{
    // place the object at the good place
    if(sw)
    {
	if(!orientation)
	    sw->setCoords(rect.Left, rect.Bottom + 10);
	else
	    sw->setCoords(rect.Right + 10, rect.Top);
    }
}
void wMenuItem::setSubChildPopupMenu(bool orient, wWidget *mb)
{
    sw = mb;
    orientation = orient;
    updateSubMenuLocation();
    // TODO: insert arrow indicating the popup place
    if(orientation)
    {
	addChild
	    (
	     new wImage(0, 0, 10, 10, "TR"),
	     false,
	     10, 2, 0, 0
	    );
    }
    else
    {
	addChild
	    (
	     new wImage(0, 0, 10, 10, "TD"),
	     false,
	     10, 2, 0, 0
	    );
    }
}

bool wMenuItem::translate(float x, float y)
{
    bool res = wContener::translate(x, y);
    updateSubMenuLocation();
    return res;
}



/*
 * Menu bar definition
 */
wMenuBar::wMenuBar(int x, int y, bool orientation, bool visible)
    : orientation(orientation),
    wContener(
	    0, 0, 5, 5, 
	    visible, 
	    -2, 
	    3, 3, 
	    orientation?wContener::alignLeftRight:wContener::alignTopBottom, 
	    wContener::wrapBoth)
{ }

wMenuBar *wMenuBar::fromFileDescriptor(
	std::string filePath, 
	bool orientation, 
	wContener *(*callback)(std::string))
{
    std::string descr;
    // open file and extract the string descriptor
    // construct menu
    return wMenuBar::fromStringDescriptor(descr,orientation,callback);
}
/* Parses a string descripting a full menu bar
 ** Format:
 ** Example:
 "Item 1\n
 > [TIO] Subitem 1\n
 >> [TIO] Subsubitem 1\n
 >> [TIO] Subsubitem 2
 > [TIO] Subitem 2\n
 > [TIO] Subitem 3\n
 [TIO] Item 2\n
 [TIO] Item 3\n
 [TIO] Item 4\n"
 Use > without text decriptor to create empty item (without text or picture or
 custom widget).
 Otherwise, you must add a flag to indicate the content of the item:
 T => Text
 I => Image decoration. The rest of the string must define a valid image path
 O => Custom widget. The rest of the string is passed to the given callback
 TI (or IT) => Image + text. The rest of the string has two parts:
 <image path>|<text>
	 <image path> can contain spaces. Use '|' as the separator.
	 <text> begins with the first non-space character after the '|' separator,
	 	and ends with the end line marker \n.
 */
wMenuBar *wMenuBar::fromStringDescriptor(
	std::string descr,
	bool orientation, 
	wContener *(*callback)(std::string))
{
    std::stack<wMenuBar *> stbars;
    int l = descr.size();
    int subitemlevel = 0;
    int lastLevel = -1;
    int mode = 0;
    int itemtype = 0;
    int firstpathbegin = 0;
    int firstpathend = 0;
    int secondpathbegin = 0; // the 'second path' ends before the \n
    bool ignorespaces = true;
    bool error = false;
    wMenuItem *lastmi = 0;
    /* mode:
     * 0 > level detection mode
     * 1 > level detected, object flag mode
     * 2 > flag detected, content determination mode
     */
    for(int i = 0;!error && i < l; i++)
    {
	if(descr[i] == '\n')
	{
	    // new line:
	    // create sub-item (a menu bar containing one menu item...
	    // create a new sub-menu bar
	    printf("%i ... %i \n", subitemlevel, lastLevel);
	    if(subitemlevel > lastLevel)
	    {
		wMenuBar *nmb = new wMenuBar(0, 0, orientation && stbars.empty(), orientation && stbars.empty());
		if(lastLevel > -1)
		{
		    ((wMenuItem *)stbars.top()->getLastChild())->setSubChildPopupMenu
			(
			 !(orientation && subitemlevel == 1),
			 nmb
			);
		    stbars.top()->updateSize();
		    stbars.top()->updatePositions();
		}
		stbars.push(nmb);
	    }
	    else
	    {
		// return on an old  menu bar
		while(subitemlevel < lastLevel)
		{
		    stbars.pop();
		    lastLevel--;
		}
	    }
	    // add menu bar's child
	    wMenuBar *mbar = stbars.top();
	    lastmi = 
		    new wMenuItem(
			descr.substr(firstpathbegin, (itemtype == 1 ? i - 1:firstpathend)-firstpathbegin + 1),
			itemtype == 3 ? descr.substr(secondpathbegin, i - secondpathbegin) : "",
			itemtype
			);
	    mbar->addChild(
		    lastmi,false ,
		    5, 5, 5, 5
		    );

	    // and reinit search
	    ignorespaces = true;
	    lastLevel = subitemlevel;
	    subitemlevel = 0;
	    mode = 0;
	    itemtype = 0;
	    firstpathbegin = 0;
	    firstpathend = 0;
	    secondpathbegin = 0;
	}
	else if(mode == 0)
	{ 
	    if(descr[i] == '>')
	    {
		subitemlevel++;
		ignorespaces = false;
	    }
	    else if(descr[i] == ' ')
	    {
		if(ignorespaces)
		    continue;
		// else quit level detection
		mode++;
		ignorespaces = true;
	    }
	    else
	    {
		i--; // to not lost the first letter
		mode++;
		ignorespaces = true;
	    }
	}
	else if(mode == 1)
	{
	    if(descr[i] == ' ')
	    {
		if(ignorespaces)
		    continue;
		// else quit flag mode
		mode++;
		ignorespaces = true;
	    }
	    else if(descr[i] == 'T' || descr[i] == 't')
	    {
		if(itemtype == 2)
		    itemtype = 3;
		else if(itemtype)
		    error = true;
		else
		    itemtype = 1;
		ignorespaces = false;
	    }
	    else if(descr[i] == 'I' || descr[i] == 'i')
	    {
		if(itemtype == 1)
		    itemtype = 3;
		else if(itemtype)
		    error = true;
		else
		    itemtype = 2;
		ignorespaces = false;
	    }
	    else if(descr[i] == 'O' || descr[i] == 'o')
	    {
		if(itemtype)
		    error = true;
		else
		    itemtype = 4;
		ignorespaces = false;
	    }
	    else
		error = true;
	}
	else if(mode == 2)
	{
	    if(descr[i] == ' ')
	    {
		if(ignorespaces)
		    continue;
	    }
	    if(itemtype == 1 || itemtype == 2)
	    {
		if(descr[i] != ' ')
		{
		    if(ignorespaces)
		    {
			firstpathbegin = i;
			secondpathbegin = i;
			ignorespaces = false;
		    }
		    firstpathend = i;
		}
	    }
	    else if(itemtype == 3)
	    {
		if(descr[i] != ' ')
		{
		    if(ignorespaces)
		    {
			if(!secondpathbegin)
			    firstpathbegin = i;
			else
			    secondpathbegin = i;
			ignorespaces = false;
		    }
		    if(!secondpathbegin && descr[i] == '|')
		    {
			ignorespaces = true;
			secondpathbegin = i + 1;
		    }
		    else if(!secondpathbegin)
			firstpathend = i;
		}
	    }
	    else
	    {
		// TODO: custom widget as menu item
	    }
	}
    }
    if(error)
	printf("ERROR in menu string descriptor.\n");
    wMenuBar *res = 0;
    while(!stbars.empty())
    {
	res = stbars.top();
	stbars.pop();
    }
    return res;
}

void wMenuBar::close()
{
    // TODO: free sfml's ressources
}

void wMenuBar::draw(sf::RenderWindow &rw)
{
    rw.Draw(sf::Shape::Rectangle(wWidget::rect.Left, wWidget::rect.Top, wWidget::rect.Right, wWidget::rect.Bottom, sf::Color(20,20,20,50)));
    wContener::draw(rw);
}

bool wMenuBar::interpretEvent(sf::Event &ev, float mx, float my)
{
    bool interpret = false;
    for(int i = 0; i < fils.size(); i++)
    {
	if(fils[i]->getIsVisible())
	{
	    bool inter = fils[i]->interpretEvent(ev, mx, my);
	    if(ev.Type == sf::Event::MouseButtonPressed)
	    {
		if(inter)
		{
		    interpret = true;
		    ((wMenuItem *)fils[i])->expand(true);
		}
		else
		    ((wMenuItem *)fils[i])->expand(false);
	    }
	}
    }
    return interpret;
}
