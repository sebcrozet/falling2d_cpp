#include "Shapes.h"



class PixelPerfectShape : public Shape
{
private:
	bool (*isSolid) (float x, float y);
public:
};