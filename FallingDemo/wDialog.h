#include "wContener.h"
#include "wLabel.h"
#define TITLEBARSIZE 20

/* Dialog box with title bar, optionnal buttons
 ** and methods to add internal widgets.
 ** Draggable window.
 */
class wDialog : public wContener
{
    private:
	wContener *corps;
	wLabel titlebar;
	//sf:: title;
	// drag and drop
	bool dragging;
	float lastx,lasty;
    public:
	wDialog(int x,int y,int w,int h, std::string title);
	virtual void draw(sf::RenderWindow &rw);
	virtual void close();
	virtual bool interpretEvent(sf::Event &ev, float mx, float my);
};
