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

#include "stdafx.hh"
#include "ContactGenerator.hh"
#include "CollisionArbiter.hh"
#include "Island.hh"

namespace Falling
{
  Collision::Collision(Shape* s)
  {
    sa = s;
    sb = s;
    cd = 0;
    nexta = 0; // if 0, 'this' is not in a list
    nextb = 0;
    preva = 0;
    prevb = 0;
  }

  Collision::Collision(Shape* s, Shape* s2)
  {
    nexta = 0;
    nextb = 0;
    preva = 0;
    prevb = 0;
    // choose correct collision detector and order shapes
    int idsum = s->getShapeTypeID() + s2->getShapeTypeID();
    if(idsum == 2)
    {
      // disk-disk
      cd = new DiskDiskSolver((Disk* )s, (Disk* )s2);
      sa = s;
      sb = s2;
    }
    else if(idsum > 10)
    {
      // plane-shape
      if(s->getShapeTypeID() == 100)
      {
        cd = new PlaneShapeSolver((InfinitePlane* )s, (Shape* )s2);
        sa = s;
        sb = s2;
      }
      else
      {
        cd = new PlaneShapeSolver((InfinitePlane* )s2, (Shape* )s);
        sa = s2;
        sb = s;
      }
    }
    else
    {
      // nothing special
      cd = new GenericSolver(s, s2);
      sa = s;
      sb = s2;
    }
  }

  Collision::~Collision()
  {
    clearContacts();
    if(cd) // test cd's validity (not detector is allocated for sentinels!)
      delete cd;
  }

  void Collision::clearContacts()
  {
    c.clear();
  }

  void Collision::removeFromList()
  {
    bool sentinel = (preva->nexta == preva->nextb || !preva->nextb);
    assert(preva->sa == sa || preva->sb == sa);
    assert(nexta->sa == sa || nexta->sb == sa);
    assert(prevb->sa == sb || prevb->sb == sb);
    assert(nextb->sa == sb || nextb->sb == sb);
    if(preva->sa == sa)
      preva->nexta = nexta;
    else
    {
      assert(preva->sb == sa);
      preva->nextb = nexta;
    }

    if(nexta->sa == sa)
      nexta->preva = preva;
    else
    {
      assert(nexta->sb == sa);
      nexta->prevb = preva;
    }

    if(prevb->sa == sb)
      prevb->nexta = nextb;
    else
    {
      assert(prevb->sb == sb);
      prevb->nextb = nextb;
    }

    if(nextb->sa == sb)
      nextb->preva = prevb;
    else
    {
      assert(nextb->sb == sb);
      nextb->prevb = prevb;
    }

    if(sentinel)
      assert(preva->nexta == preva->nextb || !preva->nextb);
    nexta = 0; // mark as not in any list
    nextb = 0;
    preva = 0;
    prevb = 0;
  }
  void Collision::autoInsert()
  {
    assert(nexta == 0);
    Collision* ca = sa->getCollisionList();
    Collision* cb = sb->getCollisionList();
    assert(ca);
    assert(cb);
    assert(ca->nexta == ca->nextb || !ca->nextb);
    assert(cb->nexta == cb->nextb || !cb->nextb);
    assert(ca->sa == ca->sb);
    assert(cb->sa == cb->sb);
    preva = ca;
    prevb = cb;

    if(sa == ca->sa)
    {
      nexta = ca->nexta;
      if(nexta->sa == sa)
        nexta->preva = this;
      else
        nexta->prevb = this;
      ca->nexta = this;
    }
    else
    {
      nexta = ca->nextb;
      if(nexta->sa == sa)
        nexta->preva = this;
      else
        nexta->prevb = this;
      ca->nextb = this;
    }
    // Insert in b list
    if(sb == cb->sa)
    {
      nextb = cb->nexta;
      if(cb->nexta->sa == sb)
        cb->nexta->preva = this;
      else
        cb->nexta->prevb = this;
      cb->nexta = this;
    }
    else
    {
      nextb = cb->nextb;
      if(cb->nextb->sa == sb)
        cb->nextb->preva = this;
      else
        cb->nextb->prevb = this;
      cb->nextb = this;
    }
    assert(!ca->nextb);
    assert(!cb->nextb);
  }

  // Collision arbiter
  CollisionArbiter::CollisionArbiter() : sap(addP,removeP,deleteP)
  { }

  CollisionArbiter::~CollisionArbiter()
  {
    s.clear();
  }

  bool CollisionArbiter::removeP(Pair* p)
  {
    if(!((Collision*)p->e)->cd->getIsInactive()) //To avoid deletion redondency problems
      ((Collision*)p->e)->removeFromList(); // Remove this from collision graph
    if(!((Collision*)p->e)->cd->canDestroy())
    {
      ((Collision*)p->e)->cd->setInactive(true); // mark as inactive but keep it in cash
      return false;
    }
    return true;
  }

  void CollisionArbiter::deleteP(Pair &p)
  {
    Collision* coll = (Collision*)p.e;
    if(coll->nexta) // if still in the list remove it
    {
      //printf("Removing an in-list pair!\n");
      coll->removeFromList();
    }
    /*
       else
       printf("Removing an not-in-list pair!\n");
       */
    delete coll;
  }

  void CollisionArbiter::addP(Pair* p, Shape* s, Shape* s2)
  {
    if(p->e)
    {
      if(((Collision*)p->e)->cd->getIsInactive())
        ((Collision*)p->e)->cd->setInactive(false); // mark as active
      else
        return; // avoid redundency in collision deletion
    }
    else
      p->e = new Collision(s, s2);
    ((Collision*)p->e)->autoInsert(); // Auto instert in collision graph
  }

  void CollisionArbiter::addObject(Shape* s)
  {
    // Prepare object's collision list sentinels
    Collision* head = new Collision(s);
    Collision* tail = new Collision(s);
    head->nexta = tail;
    tail->preva = head;
    // other pointers are useless for sentinels, so don't initialize them
    s->setCollisionList(head,tail);
    ////
    sap.addObject(s);
  }

  void CollisionArbiter::deleteObject(Shape* s)
  {
    /*
     * first, awake every objects in contact with this one
     */
    Collision* c = s->getCollisionList()->nexta;
    while(c->sa != c->sb) // sentinel not reached
    {
      if(c->sa == s)
      {
        if(c->sb->getParent()->isSleeping())
          c->sb->getParent()->setAwake(true);
        c = c->nexta;
      }
      else
      {
        assert(c->sb == s);
        if(c->sa->getParent()->isSleeping())
          c->sa->getParent()->setAwake(true);
        c = c->nexta;
      }
    }

    /*
     * Then, we can remove safely
     * from the sap
     */
    sap.removeObject(s);
    // Free object's collision list sentinels
    // All other collision should have been deleed by SAP,
    // so, head = first collision / tail = second collision
    c = s->getCollisionList();
    // invalidate the collision list
    assert(c->sa == c->sb);
    assert(c->nexta->sa == c->nexta->sb);
    s->setCollisionList(0, 0);
    // must be sentinels
    // delete it
    delete c->nexta;
    delete c;
  }

  void CollisionArbiter::notifyObjectMoved(Shape* s)
  {
    sap.notifyBoxMoved(s);
  }

  void CollisionArbiter::solve(std::vector<Collision*> &res)
  {
    int   n;
    Pair* p;
    std::stack<Pair* > todel;

    n = 0;
    p = sap.solve(&n);

    for(int i = 0; i < n; i ++)
    {
      assert(!(((Collision*)p[i].e)->sa->isdeleting()));
      assert(!(((Collision*)p[i].e)->sb->isdeleting()));
      bool collisionLost = false;
      if(
          !((Collision*)p[i].e)->sa->getParent()->isSleeping() 
          || !((Collision*)p[i].e)->sb->getParent()->isSleeping()
        )
      {
        collisionLost = ((Collision*)p[i].e)->c.size() > 0;
        ((Collision*)p[i].e)->clearContacts();
        /*if(*/
        ((Collision*)p[i].e)->cd->solve(((Collision*)p[i].e)->c);//)
        //	;//todel.push(&p[i]);
        //else
      }
      if(((Collision*)p[i].e)->c.size())
        res.push_back((Collision*)p[i].e);
      else if(collisionLost)
      {
        // Wake up objects

        //printf("Collision lost!\n");
        ((Collision*)p[i].e)->sa->getParent()->setAwake(true);
        ((Collision*)p[i].e)->sb->getParent()->setAwake(true);

      }
    }
    /*while(!todel.empty())
      {
      sap.removePair(todel.top());
      todel.pop();
      }*/
  }
}
