#ifndef AABBp
#include "primitives.h"
#include "AABB.h"

class AABB_polygon : public AABB 
{	
private:
	int ixm, ixM, iym, iyM;
public:		  
	AABB_polygon(ImplicitShape *parent);
	void update();
};
#define AABBp
#endif

