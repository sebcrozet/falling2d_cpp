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
#ifndef SHAPE
#include "GeometryHelper.hh"
#include "AABB.hh"

namespace Falling
{
  struct Collision;
  class RigidBody;
  struct OBBtree;
  class FALLINGAPI Shape
  {
    private:
      int aabbid;
    public:
      virtual ~Shape();
    protected:
      RigidBody *parentBody;
      OBBtree *otree;
      GeometryHelper::Transformation2D t;
      GeometryHelper::Transformation2D t_old; // transform in the last frame
      Real aabb_xm, aabb_xM, aabb_ym, aabb_yM;
      bool fixedobj;
      bool moved;

      // Collision graph infos
      Collision *collisions,*collisionsTail; // <- CollisionTail is useless (juste for debug)
      int collisionStackLevel;
      unsigned int total_number_of_contacts;

      // TODO: remove, for debug
      bool deleting;
      // end TODO


    public:

      // TODO: remove, for debug
      inline bool isdeleting()
      { return deleting; }
      inline void setdeleting()
      { deleting = true; }
      inline Real get_aabb_xm()
      { return aabb_xm; }
      inline Real get_aabb_xM()
      { return aabb_xM; }
      inline Real get_aabb_ym()
      { return aabb_ym; }
      inline Real get_aabb_yM()
      { return aabb_yM; }
      // end TODO
      // Collision graph infos
      inline Collision *getCollisionList() const;
      inline void setCollisionList(Collision *,Collision *);
      inline void setStackLevel(int level);
      inline int getStackLevel() const;
      inline int get_total_number_of_contacts() const
      { return total_number_of_contacts; }
      inline void set_number_of_contacts(Real n_cnts)
      { total_number_of_contacts = n_cnts; }
      //
      virtual Real getSurface() const = 0;
      virtual Real getInertiaMomentum(Real m) const = 0;
      virtual int getShapeTypeID() const = 0;
      virtual void updateAABB() = 0;
      virtual bool containsPoint(const Point2D &) const = 0;

      inline RigidBody *getParent() const;
      inline void setParent(RigidBody *r);

      void updateAABB(EndPoint *xm, EndPoint *xM, EndPoint *ym, EndPoint *yM);
      inline void rotate(Real dteta);
      inline void translate(Vector2D du);

      inline OBBtree *getOtree() const;
      inline int getAABBid() const;
      inline void setAABBid(int id);
      inline bool AABBvsAABB(Real xm, Real xM, Real ym, Real yM);
      inline bool isFixed() const;
      inline void setFixed(bool fixed);
      inline Point2D *getLocalPoint() const;
      inline Vector2D getPos() const;
      inline void setPos(Vector2D &p);
      inline Real getTeta() const;
      inline void setTeta(Real nteta);
      inline bool getMoved() const;
      inline void setMoved(bool m);

      /*
       * transformations during the current frame
       */
      inline Vector2D toRotatedInv(const Vector2D &v) const;
      inline Vector2D toRotated(const Vector2D &v) const;
      inline Point2D toRotatedInv(const Point2D &v) const;
      inline Point2D toRotated(const Point2D &v) const;
      inline Vector2D toGlobal(const Vector2D &v) const;
      inline Point2D toGlobal(const Point2D &v) const;
      inline Vector2D toLocal(const Vector2D &p) const;
      inline Point2D toLocal(const Point2D &p) const;
      inline Vector2D toTranslated(const Vector2D &p) const;
      inline Point2D toTranslatedInv(const Point2D &p) const;
      inline Vector2D toTranslatedInv(const Vector2D &p) const;
      inline Point2D toTranslated(const Point2D &p) const;

      /*
       * transformations during the last frame
       */
      inline Vector2D toRotatedInv_old(const Vector2D &v) const;
      inline Vector2D toRotated_old(const Vector2D &v) const;
      inline Point2D toRotatedInv_old(const Point2D &v) const;
      inline Point2D toRotated_old(const Point2D &v) const;
      inline Vector2D toGlobal_old(const Vector2D &v) const;
      inline Point2D toGlobal_old(const Point2D &v) const;
      inline Vector2D toLocal_old(const Vector2D &p) const;
      inline Point2D toLocal_old(const Point2D &p) const;
      inline Vector2D toTranslated_old(const Vector2D &p) const;
      inline Point2D toTranslatedInv_old(const Point2D &p) const;
      inline Vector2D toTranslatedInv_old(const Vector2D &p) const;
      inline Point2D toTranslated_old(const Point2D &p) const;
  };

  /*
   * inline functions definitions
   */
  inline void Shape::setFixed(bool fixed)
  { fixedobj = fixed; }

  inline void Shape::setStackLevel(int level)
  { collisionStackLevel = level; }
  inline int Shape::getStackLevel() const
  { return collisionStackLevel; }

  inline void Shape::setCollisionList(Collision *l, Collision *tail)
  {
    collisions = l;
    collisionsTail = tail;
  }

  inline Collision *Shape::getCollisionList() const
  { return collisions; }

  inline RigidBody *Shape::getParent() const
  { return parentBody; }

  inline void Shape::setParent(RigidBody *p)
  { parentBody = p; }

  inline Vector2D Shape::getPos() const
  { return t.getU(); }
  inline Real Shape::getTeta() const
  { return t.getTeta(); }

  inline void Shape::setTeta(Real nteta)
  {
    setMoved(true);
    t.setTeta(nteta);
  }
  inline void Shape::setPos(Vector2D &p)
  {
    setMoved(true);
    t.setU(p);
  }
  inline bool Shape::getMoved() const
  { return moved; }
  inline void Shape::setMoved(bool m)
  { moved = m; }

  inline bool Shape::isFixed() const
  { return fixedobj; }

  inline OBBtree *Shape::getOtree() const
  { return otree; }

  inline int Shape::getAABBid() const
  { return aabbid; }
  inline void Shape::setAABBid(int id)
  { aabbid = id; }
  inline bool Shape::AABBvsAABB(Real xm, Real xM, Real ym, Real yM)
  {
    if(xM < aabb_xm || xm > aabb_xM) return false;
    if(yM < aabb_ym || ym > aabb_yM) return false;
    return true;
  }

  inline void Shape::rotate(Real dteta)
  {
    setMoved(true);
    t.addTeta(dteta);
  }
  inline void Shape::translate(Vector2D du)
  {
    setMoved(true);
    t.addU(du);
  }

  inline Point2D Shape::toGlobal(const Point2D &v) const
  { return t.transform(v); }
  inline Vector2D Shape::toGlobal(const Vector2D &v) const
  { return t.transform(v); }
  inline Point2D Shape::toLocal(const Point2D &v) const
  { return t.transforminv(v); }
  inline Vector2D Shape::toLocal(const Vector2D &v) const
  { return t.transforminv(v); }
  inline Vector2D Shape::toRotatedInv(const Vector2D &v) const
  { return t.rotateinv(v); }
  inline Vector2D Shape::toRotated(const Vector2D &v) const
  { return t.rotate(v); }
  inline Point2D Shape::toRotatedInv(const Point2D &v) const
  { return t.rotateinv(v); }
  inline Point2D Shape::toRotated(const Point2D &v) const
  { return t.rotate(v); }
  inline Point2D Shape::toTranslatedInv(const Point2D &v) const
  { return t.translateinv(v); }
  inline Point2D Shape::toTranslated(const Point2D &v) const
  { return t.translate(v); }
  inline Vector2D Shape::toTranslated(const Vector2D &v) const
  { return t.translate(v); }
  inline Vector2D Shape::toTranslatedInv(const Vector2D &v) const
  { return t.translateinv(v); }

  inline Point2D Shape::toGlobal_old(const Point2D &v) const
  { return t_old.transform(v); }
  inline Vector2D Shape::toGlobal_old(const Vector2D &v) const
  { return t_old.transform(v); }
  inline Point2D Shape::toLocal_old(const Point2D &v) const
  { return t_old.transforminv(v); }
  inline Vector2D Shape::toLocal_old(const Vector2D &v) const
  { return t_old.transforminv(v); }
  inline Vector2D Shape::toRotatedInv_old(const Vector2D &v) const
  { return t_old.rotateinv(v); }
  inline Vector2D Shape::toRotated_old(const Vector2D &v) const
  { return t_old.rotate(v); }
  inline Point2D Shape::toRotatedInv_old(const Point2D &v) const
  { return t_old.rotateinv(v); }
  inline Point2D Shape::toRotated_old(const Point2D &v) const
  { return t_old.rotate(v); }
  inline Point2D Shape::toTranslatedInv_old(const Point2D &v) const
  { return t_old.translateinv(v); }
  inline Point2D Shape::toTranslated_old(const Point2D &v) const
  { return t_old.translate(v); }
  inline Vector2D Shape::toTranslated_old(const Vector2D &v) const
  { return t_old.translate(v); }
  inline Vector2D Shape::toTranslatedInv_old(const Vector2D &v) const
  { return t_old.translateinv(v); }
  /*
   * end of inline function definitons
   */


  class FALLINGAPI ImplicitShape
  {
    protected:
      Real margin;
      Shape *parent;
    public:
      inline Real getMargin() const;
      virtual Real getBoundingSphereRadius() const = 0;
      virtual Vector2D getCenter() const = 0;
      virtual int getSupportPoint(const Vector2D &d, Point2D *res) const = 0;
      virtual int getSupportPoint(const Vector2D &d, Point2D *res, int optimisationId) const = 0;  // utile pour tous ou seulement les polygones?
      /*
      virtual void getContinuousSupportPoint(const Vector2D &od, Point2D *res, int *opt, int *opt_old) const = 0; //FIXME: decomment the const = 0
      virtual void getContinuousSupportPoint_with_opt_values(const Vector2D &od, Point2D *res, int *opt, int *opt_old) const = 0;
      */
      int getMarginedSupportPoint(const Vector2D &d, Point2D *res) const;
      int getMarginedSupportPoint(const Vector2D &d, Point2D *res, int optimisationId) const;
      int getMarginedContinuousSupportPoint(const Vector2D &d, Point2D *res) const;
      int getMarginedContinuousSupportPoint(const Vector2D &d, Point2D *res, int optimisationId) const;
      void addMargin(const Vector2D &v, Point2D *pt) const;

      /*
       * transformations at current frame
       */
      inline Vector2D toRotatedInv(const Vector2D &v) const;
      inline Vector2D toRotated(const Vector2D &v) const;
      inline Vector2D toGlobal(const Vector2D &v) const;
      inline Point2D toGlobal(const Point2D &v) const;
      inline Vector2D toLocal(const Vector2D &p) const;
      inline Point2D toLocal(const Point2D &p) const;
      inline Vector2D toTranslated(const Vector2D &p) const;
      inline Point2D toTranslatedInv(const Point2D &p) const;
      inline Vector2D toTranslatedInv(const Vector2D &p) const;
      inline Point2D toTranslated(const Point2D &p) const;

      /*
       * transformations during the last frame
       */
      inline Vector2D toRotatedInv_old(const Vector2D &v) const;
      inline Vector2D toRotated_old(const Vector2D &v) const;
      inline Vector2D toGlobal_old(const Vector2D &v) const;
      inline Point2D toGlobal_old(const Point2D &v) const;
      inline Vector2D toLocal_old(const Vector2D &p) const;
      inline Point2D toLocal_old(const Point2D &p) const;
      inline Vector2D toTranslated_old(const Vector2D &p) const;
      inline Point2D toTranslatedInv_old(const Point2D &p) const;
      inline Vector2D toTranslatedInv_old(const Vector2D &p) const;
      inline Point2D toTranslated_old(const Point2D &p) const;
  };
  /*
   * inline functions definitions
   */
  inline Real ImplicitShape::getMargin() const
  { return margin; }

  inline Point2D ImplicitShape::toGlobal(const Point2D &v) const
  { return parent->toGlobal(v); }
  inline Vector2D ImplicitShape::toGlobal(const Vector2D &v) const
  { return parent->toGlobal(v); }
  inline Point2D ImplicitShape::toLocal(const Point2D &v) const
  { return parent->toLocal(v); }
  inline Vector2D ImplicitShape::toLocal(const Vector2D &v) const
  { return parent->toLocal(v); }
  inline Vector2D ImplicitShape::toRotatedInv(const Vector2D &v) const
  { return parent->toRotatedInv(v); }
  inline Vector2D ImplicitShape::toRotated(const Vector2D &v) const
  { return parent->toRotated(v); }
  inline Point2D ImplicitShape::toTranslatedInv(const Point2D &v) const
  { return parent->toTranslatedInv(v); }
  inline Point2D ImplicitShape::toTranslated(const Point2D &v) const
  { return parent->toTranslated(v); }
  inline Vector2D ImplicitShape::toTranslated(const Vector2D &v) const
  { return parent->toTranslated(v); }
  inline Vector2D ImplicitShape::toTranslatedInv(const Vector2D &v) const
  { return parent->toTranslatedInv(v); }


  inline Point2D ImplicitShape::toGlobal_old(const Point2D &v) const
  { return parent->toGlobal(v); }
  inline Vector2D ImplicitShape::toGlobal_old(const Vector2D &v) const
  { return parent->toGlobal(v); }
  inline Point2D ImplicitShape::toLocal_old(const Point2D &v) const
  { return parent->toLocal(v); }
  inline Vector2D ImplicitShape::toLocal_old(const Vector2D &v) const
  { return parent->toLocal(v); }
  inline Vector2D ImplicitShape::toRotatedInv_old(const Vector2D &v) const
  { return parent->toRotatedInv(v); }
  inline Vector2D ImplicitShape::toRotated_old(const Vector2D &v) const
  { return parent->toRotated(v); }
  inline Point2D ImplicitShape::toTranslatedInv_old(const Point2D &v) const
  { return parent->toTranslatedInv(v); }
  inline Point2D ImplicitShape::toTranslated_old(const Point2D &v) const
  { return parent->toTranslated(v); }
  inline Vector2D ImplicitShape::toTranslated_old(const Vector2D &v) const
  { return parent->toTranslated(v); }
  inline Vector2D ImplicitShape::toTranslatedInv_old(const Vector2D &v) const
  { return parent->toTranslatedInv(v); }
  /*
   * end of inline functions definitions
   */
#define SHAPE
}
#endif
