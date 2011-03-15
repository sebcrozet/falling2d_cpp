#include "Shapes.h"
#include "Point2D.h"



class PixelPerfectShape : public Shape
{
private:
	int dw, dh; // demi-w / h
	bool (*isSolid) (Real x, Real y);
public:		  
	PixelPerfectShape(int dw, int dx, bool (*isSolidPixel)(Real, Real));
	Real getSurface();
	Real getInertiaMomentum(Real m); 
	int getShapeTypeID();
	void updateAABB();
	inline bool isSolidPoint(int x, int y);
	inline bool isSolidPoint(Point2D &pt);
	inline bool isSolidPoint(Vector2D &v); 
};

inline bool PixelPerfectShape::isSolidPoint(int x, int y)
{ return isSolid(x, y); }
inline bool PixelPerfectShape::isSolidPoint(Vector2D &v) 
{ return isSolid(v.getX(), v.getY()); }
inline bool PixelPerfectShape::isSolidPoint(Point2D &pt)
{ return isSolid(pt.getX(), pt.getY()); }

