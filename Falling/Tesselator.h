#include "Point2D.h"
#include "DoubleLinkedList.h"

static class Tesselator
{
private:
	struct Poly;
	struct Point
	{
		DoubleLinkedList<Poly *> *adjlist;
		void removeAdjascent(Poly *toRemove);
		void addAdjascent(Poly *toAdd);
	};
	struct Poly
	{
		DoubleLinkedList<Point *> *pts;
		// adjascent polygons.
		// i-nth item is the polygon apjascent to
		// (i-1)-nth | i-nth edge.
		DoubleLinkedList<Poly *> *adj; // size = pts.size()
		Poly();
																				   
		/*
		 * When manipulated as a triangle
		 * (pts and adj have size 3)
		 */
		// TODO: how about returning two DoubleLinkedList<Point *> * to further use by split and merge?
		Poly *getOppositeEdgeAndAdjascent(Point *refpoint, Point **opppt1, Point **opppt2);
		Point *getOppositePoint(Point *refpt1, Point *refpt2);
		// return value indicates if a swap occured
		bool testAndSwap(Poly *poly1, Poly *poly2, Point *p1, Point *p2);
		void split(Point *insertedPoint);
		void split(Point *insertedPoint, 


		/*
		 * When manipulated as a pseudo-polygon
		 */
		void triangulate();
		Poly *split(Point *p1,Point *p2);
		// merges this with next.
		// next is automaticly freed
		void merge(Poly *next, Point *p1,Point *p2);
	};
public:
};