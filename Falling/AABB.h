#ifndef AABBs
#include "Primitives.h"
#include "Shapes.h"

class AABB;
class EndPoint
{
private:	   
	ImplicitShape *parent;
	bool isMax;
	float value;
public:
	EndPoint();
	EndPoint(float v, bool isMax, ImplicitShape *parent);

	inline float getValue() const;
	inline bool isMaxValue() const;
	inline ImplicitShape *getParent() const;
	inline void setValue(float v);
	inline void operator=(EndPoint &ep);
};

inline ImplicitShape *EndPoint::getParent() const
{ return parent; }
inline float EndPoint::getValue() const
{ return value; }
inline bool EndPoint::isMaxValue() const
{ return isMax; }
inline void EndPoint::setValue(float v)
{ value = v; }
inline void EndPoint::operator =(EndPoint &ep)
{
	isMax = ep.isMax;
	value = ep.value;
	parent = ep.parent;
}

				   
class AABB
{
protected:
	EndPoint xmin, xmax, ymin, ymax;
	AABB();
public:	
	virtual void update() = 0;
};
#define AABBs
#endif
