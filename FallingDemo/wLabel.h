#ifndef __WLABEL
#include "stdafx.h"
#include "wWidget.h"

class wLabel : public wWidget
{
    private:
	sf::String dTitle;
    public:
	wLabel(int x, int y, std::string text, int fontSize);
	void draw(sf::RenderWindow &rw);
	void close();

	virtual bool translate(float x, float y);
	virtual bool interpretEvent(sf::Event &ev, float mx, float my);
	virtual void update(float f);
	virtual bool setSize(float w, float h);
};
#define __WLABEL
#endif
