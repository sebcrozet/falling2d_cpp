/* Copyright (C) 2011 CROZET Sébastien

 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */
#include "Point2D.hh"
#include "DoubleLinkedList.hh"
#include <stack>
#include <queue>
#ifndef __TESSELATOR_DEFINED__

namespace Falling
{
  struct Poly;
  struct Edge
  {
    Poly *p1, *p2;

    Edge(Poly *poly1, Poly *poly2);

    inline bool isnull()
    {
      return !p1 && !p2;
    }

    inline Poly *getOther(Poly *p)
    {
      return (p1 == p)? p2 : p1;
    }
    inline void replace(Poly *p,Poly *np)
    {
      if(p1 == p)
        p1 = np;
      else if(p2 == p)
        p2 = np;
          if(isnull())
            delete this;
      // else: no p in this edge
    }

  };
  struct Point
  {
    Point2D pt;		   
    DoubleLinkedList<Poly *> *adjlist;
    bool marked;
    Point(const Point2D &pt);
    Point();
    ~Point();
    void removeAdjascent(Poly *toRemove);
    void addAdjascent(Poly *toAdd);
    void replace(Poly *p, Poly *np);
    static bool isInCircumcircle(Point *a, Point *b, Point *c, Point *dt);
  };
  struct Poly
  {
    DoubleLinkedList<Point *> *pts;
    // adjascent polygons.
    // i-nth item is the polygon apjascent to
    // (i-1)-nth | i-nth edge.
    DoubleLinkedList<Edge *> *adj; // size = pts.size()
    // TODO remove:
    int parckey;
    // end tode
    bool parcstate;
    Poly();
    ~Poly();

    // TODO: remove this test
    void testAdj();
    // end TODO
    /*
     * When manipulated as a triangle
     * (pts and adj have size 3)
     */
    void removeFromGraphAndDelete();
    Edge *getOppositeEdgeAndAdjascent(Point *refpoint, Point **opppt1, Point **opppt2);
    Point *getOppositePoint(Point *refpt1, Point *refpt2);
    Poly *testAndSwap(Point *p1);
    bool isInCircumcircle(Point *dt);
    // splits in three parts
    void split(Point *insertedPoint, Poly **p2,Poly **p3);
    // splits in two parts
    Poly *split(Point *insertedPoint,Point *pt1e, Point *pt2e, Poly **adjsplit);

    /*
     * When manipulated as a pseudo-polygon
     */
    void triangulate(Point *);
    Poly *split(Point *p1,Point *p2);
    // merges this with next.
    // next is automaticly freed
    void merge(Poly *next, Point *p1,Point *p2);
    void mergeNoDelete(Poly *next, Point *p1,Point *p2);
  };
  class Tesselator
  {
    public:
    enum WiningNumberMode
    {
      ODD_IS_SOLID = 1,
      EVEN_IS_SOLID = 2,
      ALL_ADDITION = 4,
      ALL_SUBSTRACTION = 8,
      INTERSECTION = 16,
      COMPL_INTERSECTION = 32
    };
    private:
            Poly *currpoly;
            Point *currpt;
            Poly *triangleMarching(Point *begin, Point2D &end, Point **graphpt);
            bool insertPoint(Point2D &pt, Point **gpoints);
            void insertEdge(Point *r, Point *l);
            /*
             * removeMode:
             *	1 -> odd is solid
             *	2 -> even is solid
             *	4 -> all addition
             *	8 -> all substaction
             *	16 -> intersection
             *	32 -> all complementary intersection
             * Note: Points included in no polygon (hole or not)
             * are always deleted
             */
            void removeExternalTriangles(
                int removeMode,
                Point2D *pts,
                int nbpts,
                Point2D *holespts[],
                int nbholes,
                int nbptsholes[],
                std::stack<Poly *> &remainingPolygons);
            int batchTriangles(std::stack<Poly *> &toparc, std::stack<Poly *> &respolys);
            void point2DListFromPointList(Poly *);
            Point *** ptgraphlist;
            Point2D ***subPolys;
            int **nbptsSubPolys;
            int ipoly;

    public:
            void parcgraph(void(*print)(Poly *));
            int initAndRun(int removeMode, Point2D *pts, int nbpts, Point2D *holespts[], int nbholes, int nbptsholes[], Point2D ***subPolys, int **nbptssubPolys);
  };
}
#define __TESSELATOR_DEFINED__
#endif
