#include "AABB.h"
#include "Shapes.h"

class AABB_disk : public AABB
{
public:
	AABB_disk(ImplicitShape *parent);
	void update();
};