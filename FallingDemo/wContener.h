#ifndef __WCONTENER
#include "stdafx.h"
#include <vector>
#include "wWidget.h"
#define AND(var,mask) ((var & mask) != 0) 
struct PositionWrapper
{
    wWidget *w;
    int mL, mR, mU, mD;

    PositionWrapper(
	    wWidget *w, 
	    int mL, int mR, int mU, int mD
	    );
};

class wContener : public wWidget
{
    public:
	enum InsertMode
	{
	    absolute = 0,
	    alignLeftRight = 1,
	    alignTopBottom = 2
	};
	enum WrapMode
	{
	    wrapW = 1,
	    wrapH = 2,
	    wrapBoth = 3, 
	    noWrap = 0
	};
    protected:
	std::vector<wWidget *>fils; 
    private:				  
	std::vector<PositionWrapper *>filsgrid;
	wContener::InsertMode im;
	wContener::WrapMode wm;
	int paddingRL, paddingUD;


    private:			   
	void bringToTop(int id);

    protected:			   

	virtual bool setSize(float w, float h);
	virtual bool setCoords(float x, float y);
	virtual bool translate(float x, float y);

    public:		   
	wContener
	    (
	     int x, int y, int w, 
	     int h, bool visible, 
	     int prl, int pud,
	     int zOrder, 
	     wContener::InsertMode im,
	     wContener::WrapMode wm
	    );
	virtual ~wContener();

	void updatePositions();
	void updateSize();
	void addChild
	    (
	     wWidget *widget, 
	     bool addfront,
	     int mL,
	     int mR,
	     int mU,
	     int mD
	    ); 
	wWidget *getLastChild();
	virtual void close();
	virtual bool interpretEvent(sf::Event &ev, float mx, float my);
	virtual void update(float dt);
	virtual void draw(sf::RenderWindow &rw);
};


#define __WCONTENER
#endif
