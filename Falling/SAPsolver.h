#ifndef _SAPSOLVER_H
#include "PairManager.h"
#include "Shapes.h"
#include "AABB.h"
#include <stack>

class SAPsolver
{
private:
	PairManager pm;
	std::vector<AABB> aabbs;
	std::vector<EndPoint> epx;
	std::vector<EndPoint> epy; 
	std::vector<int> updateids;
	std::stack<int> emptyaabbs;
	void (*cb_addobj)(Pair *, Shape *, Shape *);
	bool (*cb_removeobj)(Pair *);
	void (*cb_deleteobj)(Pair &);

	void updateBoxCollisionPairs(AABB &b);
	void updateAddedBoxCollisionPairs(AABB &);
	void updateEndPointMax(std::vector<EndPoint> &list,int *im, float xm, float xM, float ym, float yM, int xyid);
	void updateEndPointMin(std::vector<EndPoint> &list,int *im, float xm, float xM, float ym, float yM, bool sens, int xyid);
public:
	SAPsolver(void (*cb_addobj)(Pair *, Shape *, Shape *), bool (*cb_removeobj)(Pair *), void (*cb_deleteobj)(Pair &));
	~SAPsolver();
	void addObject(Shape *s);
	void removeObject(Shape *s);
	void notifyBoxMoved(Shape *s);
	Pair *solve(int *nbres);
	void removePair(Pair *);
};
#define _SAPSOLVER_H
#endif