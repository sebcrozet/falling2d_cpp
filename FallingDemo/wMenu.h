#ifndef __FALLING_DEMO_WMENUITEM
#include "stdafx.h"
#include <stack>
#include "wContener.h"
#include "wLabel.h"
#include "wImage.h"
#define _B(mask) (1 << (mask - ':'))
#define AND_LETTER(var,mask) ((var & (_B(mask))) != 0)


class wMenuItem : public wContener
{
    private: 
	bool locked;
	bool highlighted;
	bool toogled;
	bool orientation;
	int greycolor;
	int id;
	int itemtype;
	wWidget *sw;
	void *userdata;
	bool checkbox;
	int checkgroup;

	void (*onItemClicked)(int,void*);
	bool (*onItemPushed)(int,void*);
	void (*onItemReleased)(int,void*);
    public:
	wMenuItem(
		std::string path1, 
		std::string path2, 
		int itemtype, 
		int group, 
		int id, 
		bool checked, 
		bool locked, 
		bool orientation
		);
	inline void setOnItemClicked(void (*callback)(int,void *))
	{ onItemClicked = callback; }
	inline void setOnItemPushed(bool (*callback)(int,void *))
	{ onItemPushed = callback; }
	inline void setOnItemReleased(void (*callback)(int,void *))
	{ onItemReleased = callback; }
	inline void setUserDatas(void *data)
	{ userdata = data; }
	inline void *getUserDatas() const
	{ return userdata; }
	inline int getGroup() const
	{ return checkgroup; }
	inline int getId() const
	{ return id; }

	void setSubChildPopupMenu(bool orientation, wWidget *mb);
	void expand(bool b);
	void updateSubMenuLocation();
	void groupUnselect();

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

	static wMenuBar *fromFileDescriptor(
		std::string filePath, 
		bool orientation,
		void *userdata,
		void (*itemCreated)(void *,wMenuItem*)
		);
	static wMenuBar *fromStringDescriptor(
		std::string descr, 
		bool orientation,
		void *userdata,
		void (*itemCreated)(void *,wMenuItem*)
		);

	// Useless ? : wMenuItem *getMenuItem(int *indices);
	virtual void close();
	virtual void draw(sf::RenderWindow &rw);
	virtual bool interpretEvent(sf::Event &ev, float mx, float my);
};
#define __FALLING_DEMO_WMENUITEM
#endif
