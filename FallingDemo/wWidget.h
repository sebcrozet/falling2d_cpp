#ifndef __WWIDGET

#include "stdafx.h"
#include <vector>
typedef sf::Rect<float> Rect;

class wWidget
{
    public:
	static sf::View view;
    protected:
	int zOrder; // -1 => always behind -2 => top most
	bool isVisible;
	int id;

	Rect rect;

    protected:			   
	wWidget(
		int x, int y, int w, 
		int h, bool visible, 
		int zOrder); 
	virtual ~wWidget();


    public:
	virtual bool translate(float x, float y);
	virtual bool setSize(float w, float h);
	virtual bool setRect(float x, float y, float w, float h);
	bool setCoords(float x, float y);
	bool setX(float x);
	bool setY(float y);
	inline bool contains(float x, float y)
	{
	    return isVisible && rect.Contains(x, y);
	}
	inline Rect getRect() const
	{ return rect; }	  
	inline int getId() const
	{ return id; }
	inline bool getIsVisible() const
	{ return isVisible; }
	inline int getZOrder() const
	{ return zOrder; }
	inline void setId(int nid)
	{ id = nid; }

	inline void setVisible(bool visible)
	{ isVisible = visible; }

	virtual void close() = 0;
	virtual bool interpretEvent(sf::Event &ev, float mx, float my) = 0;
	virtual void update(float dt) = 0;
	virtual void draw(sf::RenderWindow &rw) = 0;
};

#define __WWIDGET
#endif
