#include "wContener.h"
#include "wLabel.h"
#include "wImage.h"
#include "wMenu.h"
#define TITLEBARSIZE 20

/* Dialog box with title bar, optionnal buttons
 ** and methods to add internal widgets.
 ** Draggable window.
 */
class wDialog : public wContener
{
    public:
	enum ButtonType
	{
	    BUTTON_YES,
	    BUTTON_NO,
	    BUTTON_OK,
	    BUTTON_RETRY,
	    BUTTON_APPLY,
	    BUTTON_CANCEL
	};
    private:
	wContener corp;
	wLabel titlebar;
	wImage separator;
	wContener buttonArea;
	//sf:: title;
	// drag and drop
	bool dragging;
	float lastx,lasty;
	// dialogs with buttons
    public:
	wDialog(float x,float y, std::string title);
	virtual void draw(sf::RenderWindow &rw);
	virtual void close();
	virtual bool interpretEvent(sf::Event &ev, float mx, float my);
	void addCorpChild(wWidget *w);
	void addButton(ButtonType type, void *userdata, void (*itempushed)(int, void *));

	static wDialog *getErrorDialog(float x, float y, std::string text);
	static wDialog *getWarningDialog(float x, float y, std::string text);
};
