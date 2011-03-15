#include "stdafx.h"
#include <stack>
#include "wContener.h"
#include "wLabel.h"
#include "wImage.h"


class wMenuItem : public wContener
{
    private: 
	bool highlighted;
	bool toogled;
	bool orientation;
	int greycolor;
	wWidget *sw;

	void (*onItemClicked)();
	bool (*onItemPushed)();
	void (*onItemReleased)();
    public:
	wMenuItem(std::string path1, std::string path2, int itemtype);
	inline void setOnItemClicked(void (*callback)())
	{ onItemClicked = callback; }
	inline void setOnItemPushed(bool (*callback)())
	{ onItemPushed = callback; }
	inline void setOnItemReleased(void (*callback)())
	{ onItemReleased = callback; }
	void setSubChildPopupMenu(bool orientation, wWidget *mb);
	void expand(bool b);
	void updateSubMenuLocation();

	virtual void close();
	virtual bool interpretEvent(sf::Event &ev, float mx, float my);
	virtual void update(float dt);
	virtual void draw(sf::RenderWindow &rw);
	virtual bool translate(float x, float y);
	virtual bool setSize(float w, float y);
};

class wMenuBar : public wContener
{
    private:
	bool orientation; // true => vertical // false => horizontal

    public:
	wMenuBar(int x, int y, bool orientation, bool visible);

	static wMenuBar *fromFileDescriptor(std::string filePath, bool orientation,wContener *(*callback)(std::string));
	static wMenuBar *fromStringDescriptor(std::string descr, bool orientation,wContener *(*callback)(std::string));

	// Useless ? : wMenuItem *getMenuItem(int *indices);
	virtual void close();
	virtual void draw(sf::RenderWindow &rw);
	virtual bool interpretEvent(sf::Event &ev, float mx, float my);
};
