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
#ifndef _GJKSOLVER_H
# include "Shapes.hh"
# include "queue"
# include "CollisionDetector.hh"
# include "ContactBatch.hh"

# define GJK_DESTROYLIMIT 20
# define EPSILON_ MACHINE_EPSILON    // * 1000
# define EPSILON_1 (1 + EPSILON_)  * (1 + EPSILON_)

namespace Falling
{
  class EPAsolver;
  class GJKsolver : public CollisionDetector
  {
    friend class EPAsolver;
    private:
    ContactBatch cash;
    Real msum, sqmsum;	// margin sum
    Real minbRadius;
    ImplicitShape& A;
    ImplicitShape& B;
    int optidsA[3],
        optidsB[3];
    Point2D ptsA[3],
            ptsB[3];
    Vector2D ptsC[3],
             dirs[3];
    Vector2D satlastdir;
    int simplexSize;

    bool ISAsolve();
    Real solveDist(Real* bparam);
    void recomputeSimplex();
    void updateClosestFeatureSimplexDatas(Vector2D& p,
                                          Real*     barycentricParam);
    bool gjk_buildMarginedSimplexWithOrigin();

    bool _solve(std::vector<ContactBackup*>& res);
    inline void swapPts(Vector2D& a, Vector2D& b);
    public:
    GJKsolver(ImplicitShape& a,ImplicitShape& b);
    Real getSeparatingDistance();
    Real getClosestPoints(Point2D* pA, Point2D* pB);
    Real getPenDepth(Point2D* pA, Point2D* pB);
    bool canDestroy();
  };

  inline void GJKsolver::swapPts(Vector2D& a, Vector2D& b)
  {
    Vector2D c = a;
    a = b;
    b = c;
  }

  class SimplexSeg
  {
    private:
      Real bCoord;
      Real dist;
      Point2D ptA1, ptB1, ptC1, ptA2, ptB2, ptC2;
      Vector2D v;
    public:
      inline Real getdist() const;

      SimplexSeg(
          const Point2D& pA1,
          const Point2D& ptB1,
          const Point2D& ptC1,
          const Point2D& ptA2,
          const Point2D& ptB2,
          const Point2D& ptC2);
      bool isValid();
      SimplexSeg* cut(
          const Point2D& ptA3,
          const Point2D& ptB3,
          const Point2D& ptC3);
      bool operator<(SimplexSeg& s2);
      Vector2D getSupportVect();
      void getABpoints(Point2D* a,Point2D* b);
      inline Point2D getC1();
      inline Point2D getC2();
  };

  inline Point2D SimplexSeg::getC1()
  {
    return ptC1;
  }
  inline Point2D SimplexSeg::getC2()
  {
    return ptC2;
  }

  struct simplexSeg_less : std::binary_function<SimplexSeg*, SimplexSeg*, bool>
  {
    bool operator()(SimplexSeg* a, SimplexSeg* b) const;
  };

  class EPAsolver
  {
    private:
      GJKsolver& gsolv;
      std::priority_queue<SimplexSeg* ,std::vector<SimplexSeg*>,simplexSeg_less> sd;
      bool distnull;
    public:
      EPAsolver(GJKsolver& simplex);
      Vector2D getPenetrationDepth(Point2D* pA, Point2D* pB);
  };

  inline Real SimplexSeg::getdist() const
  {
    return dist;
  }
}
# define _GJKSOLVER_H
#endif
