#include "stdafx.h"
#include "wWidget.h"

class wImage : public wWidget 
{
    sf::Sprite img;
    std::vector<sf::Shape *> shs;

    sf::Color col;
    bool useShape;

    public:
    wImage(int x, int y, int w, int h, std::string path);
    bool testPredefShape(std::string descr, float w, float h);
    // Predefine shapes creation
    void shapeWarning(float w, float h);
    void shapeStop(float w, float h);
    void shapePause(float w, float h);
    void shapeLect(float w, float h);
    void shapeTickR(float w, float h);
    void shapeTickD(float w, float h);
    void shapePolyDraw(float w, float h);
    void shapePolyPointDraw(float w, float h);
    void shapeDelete(float w, float h);
    // End of predefined shapes

    virtual bool setSize(float w, float h);
    virtual void close();
    virtual bool translate(float x, float y);
    virtual bool interpretEvent(sf::Event &ev, float mx, float my);
    virtual void update(float dt);
    virtual void draw(sf::RenderWindow &rw);
};
