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
#define LCP
#include "stdafx.h"
// TODO: remove include (usefull only for debug)
#include "Island.h"
#include <iostream>
// end TODO

namespace Falling
{
  Island::Island()
    : nbrCtcts(0), total_contacts_number(0)
  { }

  Island::~Island()
  {
    while(!graphNodes.empty())
      graphNodes.pop();
    bodies_involved.clear();
  }

  void Island::batchIsland(Island *isl,Shape *coll) // coll must not be fixed
  {
    assert(!coll->isFixed());
    Collision *next;
    bool insertToOneLevel = false;
    next = coll->getCollisionList();
    assert(next->sa == coll); // ==> always true in the first sentinel
    assert(next->sa == next->sb); // must be a sentinel
    next = next->nexta; // skip the santinel
    while(next->sa != next->sb) // second sentinel reached if ==
    {
      assert(!next->sa->isdeleting() && !next->sb->isdeleting());
      assert(next->sa == coll ^ next->sb == coll);
      Shape *ssh;
      Collision *next_to_view;
      if(next->sa == coll)
      {
        ssh = next->sb; // ssh contains the node not equal to coll
        next_to_view = next->nexta;
      }
      else
      {
        ssh = next->sa;
        next_to_view = next->nextb;
      }
      /*
         do not propagate anything if there is no contact
         */
      if(next->c.size() && next->collisionStackLevel == -1)
      {
        isl->add_contact(next);
        next->collisionStackLevel = -2;
        isl->total_contacts_number += next->c.size();
        next->sa->set_number_of_contacts(next->sa->get_total_number_of_contacts() + next->c.size());
        next->sb->set_number_of_contacts(next->sb->get_total_number_of_contacts() + next->c.size());
        // if not already seen
        if(ssh->getStackLevel() == -1 && !ssh->isFixed())
        {
          isl->add_body(ssh->getParent());
          // another stack level invalid value (but != -1)
          // propagate in this item to find other fixed objects.
          ssh->setStackLevel(-2);
          batchIsland(isl,ssh);
        }
      }
      // go to next edge
      next = next_to_view;
    }
  }

  void Island::batchIslands(std::vector<Collision*> &colls, std::stack<Island*> &islands)
  {
    // Reinit
    for(std::vector<Collision*>::iterator i = colls.begin(); i != colls.end(); i++)
    {
      Collision *c = *i;
      c->collisionStackLevel = -1;
      /*
       * FIXME:
       * here we modify sa and sb.
       * However, it could be sheaper to iterate through 
       * each rigid body one by one an reset the attributes
       */
      c->sa->setStackLevel(-1);
      c->sb->setStackLevel(-1);
      c->sa->set_number_of_contacts(0);
      c->sb->set_number_of_contacts(0);
    }
    // Make a depth first search in the collision graph
    for(std::vector<Collision *>::iterator coll_ = colls.begin(); coll_!=colls.end(); coll_++)
    {
      Collision *coll = *coll_;
      /*
         if not already pushed to another island
         */
      if(coll->collisionStackLevel == -1)
      {
        // new island detected
        Island *isl = new Island();
        // recursive call with fixed object
        if(coll->sa->isFixed())
        {
          coll->sb->setStackLevel(-1);
          isl->add_body(coll->sb->getParent());
          batchIsland(isl,coll->sb);
          assert(!isl->isEmpty());
          // island will always contain one object
        }
        else
        {
          coll->sa->setStackLevel(-1);
          isl->add_body(coll->sa->getParent());
          batchIsland(isl,coll->sa);
          assert(!isl->isEmpty());
        }
        // push to islands stack
        islands.push(isl);
      }
    }
  }

  void Island::doit(Real dt)
  {
    /*
       first, see if all objects are sleeping
       */
    std::vector<RigidBody*>::iterator body = bodies_involved.begin();
    for(; body != bodies_involved.end() && (*body)->isSleeping(); body++) ;
    if(body == bodies_involved.end())
      return; // no resolution to do: the whole stack is sleeping (so lazy…)

    /*
     * pre-solve the stack to find the actives sub-stacks
     */
    solve_stack(dt, 1); // solve the stack with only two iterations!
    std::stack<Island *> sub_isls;
    /*
     * FIXME: if the whole stack is re-atived, we rebuild the whole (same)
     * stack… Find a way to avoid that!
     */
    /*
     * change the contact list, excluding fixed/sleeping and sleeping/sleeping
     * contacts.
     */
    std::vector<Collision *> active_collisions;
    for(std::vector<Collision *>::iterator coll = contacts_involved.begin();
        coll != contacts_involved.end();
        coll++)
    {
      Collision *ct = *coll;
      bool sa_concidered_fixed = ct->sa->isFixed() ||
                                 ct->sa->getParent()->isFakeSleeping(); // FIXME: need to test both?
      bool sb_concidered_fixed = ct->sb->isFixed() ||
                                 ct->sb->getParent()->isFakeSleeping(); // FIXME: need to test both?
      if(!sa_concidered_fixed || !sb_concidered_fixed)
        active_collisions.push_back(ct);
    }
    if(active_collisions.size()) // FIXME: is it really needed to test the non-zero-ness of the size?
    {
      Island::batchIslands_without_sleeping(active_collisions, sub_isls);
      while(!sub_isls.empty())
      {
        Island *sub_isl = sub_isls.top();
        sub_isl->solve_stack_without_sleeping(dt, MAX_LCP_ITERATIONS);
        //sub_isl->solve_positions_without_sleeping(dt, MAX_LCP_ITERATIONS);
        delete sub_isl;
        sub_isls.pop();
      }
    }
  }


  /*
   * Batching with fixed sleeping objects
   */
  void Island::batchIsland_without_sleeping(Island *isl,Shape *coll) // coll must not be fixed
  {
    assert(!coll->isFixed() && !coll->getParent()->isFakeSleeping());
    Collision *next;
    bool insertToOneLevel = false;
    next = coll->getCollisionList();
    assert(next->sa == coll); // ==> always true in the first sentinel
    assert(next->sa == next->sb); // must be a sentinel
    next = next->nexta; // skip the santinel
    while(next->sa != next->sb) // second sentinel reached if ==
    {
      assert(!next->sa->isdeleting() && !next->sb->isdeleting());
      assert(next->sa == coll ^ next->sb == coll);
      Shape *ssh;
      Collision *next_to_view;
      if(next->sa == coll)
      {
        ssh = next->sb; // ssh contains the node not equal to coll
        next_to_view = next->nexta;
      }
      else
      {
        ssh = next->sa;
        next_to_view = next->nextb;
      }
      /*
         do not propagate anything if there is no contact
         */
      if(next->c.size() && next->collisionStackLevel == -1)
      {
        isl->add_contact(next);
        next->collisionStackLevel = -2;
        isl->total_contacts_number += next->c.size();
        next->sa->set_number_of_contacts(next->sa->get_total_number_of_contacts() + next->c.size());
        next->sb->set_number_of_contacts(next->sb->get_total_number_of_contacts() + next->c.size());
        // if not already seen
        if(ssh->getStackLevel() == -1 && !ssh->isFixed() && !ssh->getParent()->isFakeSleeping())
        {
          isl->add_body(ssh->getParent());
          // another stack level invalid value (but != -1)
          // propagate in this item to find other fixed objects.
          ssh->setStackLevel(-2);
          batchIsland_without_sleeping(isl,ssh);
        }
      }
      // go to next edge
      next = next_to_view;
    }
  }

  void Island::batchIslands_without_sleeping(std::vector<Collision*> &colls, std::stack<Island*> &islands)
  {
    // Reinit
    for(std::vector<Collision*>::iterator i = colls.begin(); i != colls.end(); i++)
    {
      Collision *c = *i;
      c->collisionStackLevel = -1;
      /*
       * FIXME:
       * here we modify sa and sb.
       * However, it could be sheaper to iterate through 
       * each rigid body one by one an reset the attributes
       */
      c->sa->setStackLevel(-1);
      c->sb->setStackLevel(-1);
      c->sa->set_number_of_contacts(0);
      c->sb->set_number_of_contacts(0);
    }
    // Make a depth first search in the collision graph
    for(std::vector<Collision *>::iterator coll_ = colls.begin(); coll_!=colls.end(); coll_++)
    {
      Collision *coll = *coll_;
      /*
         if not already pushed to another island
         */
      if(coll->collisionStackLevel == -1)
      {
        // new island detected
        Island *isl = new Island();
        // recursive call with fixed object
        if(coll->sa->isFixed() || coll->sa->getParent()->isFakeSleeping())
        {
          coll->sb->setStackLevel(-1);
          isl->add_body(coll->sb->getParent());
          batchIsland_without_sleeping(isl,coll->sb);
          assert(!isl->isEmpty());
          // island will always contain one object
        }
        else
        {
          coll->sa->setStackLevel(-1);
          isl->add_body(coll->sa->getParent());
          batchIsland_without_sleeping(isl,coll->sa);
          assert(!isl->isEmpty());
        }
        // push to islands stack
        islands.push(isl);
      }
    }
  }

  void Island::solve_stack(Real dt, unsigned int iterations_number)
  {
    Real invdt = 1.0 / dt;
    total_contacts_number *= 2; // normal constraint + friction constraint
    /*
     * build matrices
     */
    // {vx vy w} * 2 * n 
    Real *J = new Real[2 * 3 * total_contacts_number];
    // {vx vy w} * 2 * n. It's the same matrix as the jacobian's but each term
    // is multiplied by M^-1
    Real *B = new Real[2 * 3 * total_contacts_number];
    // sparce matrix indices
    int *idx = new int[2 * total_contacts_number];
    // work done by constraints: zeta = JV
    Real *zeta = new Real[total_contacts_number];
    // nu = invdt * zeta - J (invdt * V1 + M^-1 * Fext)
    Real *nu = new Real[total_contacts_number];
    // unknowns' matrix
    Real *lambda = new Real[total_contacts_number];
    // constraints bounds
    Real *bounds = new Real[total_contacts_number * 2];
    /*
     * Here, some characteristics are copied off the rigid bodies: v, omega, m,
     * I.  I dont know if it's really faster than accessing these datas using
     * RigidBodies' pointers.  Here, we mais think that copying is better
     * because of porcessor's cash… So: FIXME: verify that it's true (comparing
     * times).
     */
    Real *vomega = new Real[bodies_involved.size() * 3];
    // contains the mass and the inertia
    Real *mi = new Real[bodies_involved.size() * 3];
    /*
     * initialize shapes' indices
     */
    for(unsigned int i = 0; i < bodies_involved.size(); i++)
    {
      RigidBody *rb = bodies_involved[i];
      vomega[i * 3] = rb->getV().getX();
      vomega[i * 3 + 1] = rb->getV().getY();
      vomega[i * 3 + 2] = rb->getOmega();
      mi[i * 2] = rb->getInvM();
      mi[i * 2 + 1] = rb->getInvI();
      rb->setIslandIndex(i);
    }
    /*
       fill matrices
       */
    //### lambda: FIXME: remove this if the engine is warm-started
    std::fill(lambda, lambda + total_contacts_number, 0);
    //### J: contact constraints. Assemble the matrix calling the perparation
    //method in each contacts.
    //### bounds
    //### idx
    //### zeta
    /*
     *  indexes
     */
    Real *i_bounds = bounds;
    Real *i_J = J;
    int *i_idx = idx;
    Real *i_zeta = zeta;
    Real *i_lambda = lambda;
    for(std::vector<Collision*>::iterator  j = contacts_involved.begin();
        j != contacts_involved.end();
        j++)
    {
      Collision *curr_col = *j;
      ContactGenerator::PrepareContactDatasInMatrix(
        dt,
        curr_col,
        i_J,
        i_bounds,
        i_zeta,
        i_lambda,
        i_idx);
    }

    //### nu: FIXME: for now, we assume that Fext = mG.
    // Thus: nu = - J (invdt * {vx vy w, vx' vy' w', …} + {0 G 0, 0 G 0, …})
    //          = - J (invdt * {vx (vy + G) w ; vx' (vy' + G) w' ; …})
    // FIXME: optimize using pointers' arichmetic
    for(unsigned int i = 0; i < total_contacts_number; i++)
    {
      Real vax1 = vomega[idx[i * 2] * 3] * invdt;
      Real vay1 = vomega[idx[i * 2] * 3 + 1] * invdt + G;
      Real w1 = vomega[idx[i * 2] * 3 + 2] * invdt;
      unsigned i23 = i * 2 * 3;
      nu[i] = zeta[i] - (J[i23] * vax1 + J[i23 + 1] * vay1 + J[i23 + 2] * w1);
      if(idx[i * 2 + 1] >= 0)
      {
        Real vax2 = idx[i * 2 + 1] >= 0 ?
                    vomega[idx[i * 2 + 1] * 3] * invdt
                    : 0.;
        Real vay2 = idx[i * 2 + 1] >= 0 ?
                    vomega[idx[i * 2 + 1] * 3 + 1] * invdt + G
                    : 0.;
        Real w2 = idx[i * 2 + 1] >= 0 ?
                  vomega[idx[i * 2 + 1] * 3 + 2] * invdt
                  : 0.;

        unsigned i23p3 = i * 2 * 3 + 3;
        nu[i] -= J[i23p3] * vax2 + J[i23p3 + 1] * vay2 + J[i23p3 + 2] * w2;
      }

    }
    //### B:
    // FIXME: use arithmetic on pointers
    for(unsigned int i = 0; i < total_contacts_number; i++)
    {
      Real im1 = mi[idx[i * 2] * 2];
      Real ii1 = mi[idx[i * 2] * 2 + 1];

      B[i * 3] = J[i * 6] * im1;
      B[i * 3 + 1] = J[i * 6 + 1] * im1;
      B[i * 3 + 2] = J[i * 6 + 2] * ii1;

      if(idx[i * 2 + 1] >= 0)
      {
        Real im2 = mi[idx[i * 2 + 1] * 2];
        Real ii2 = mi[idx[i * 2 + 1] * 2 + 1];
        // go to next line because we work with the transpose matrix
        B[(i + total_contacts_number) * 3] = J[i * 6 + 3] * im2;
        B[(i + total_contacts_number) * 3 + 1] = J[i * 6 + 4] * im2;
        B[(i + total_contacts_number) * 3 + 2] = J[i * 6 + 5] * ii2;
      }
      else
      {
        B[(i + total_contacts_number) * 3] = 0;
        B[(i + total_contacts_number) * 3 + 1] = 0;
        B[(i + total_contacts_number) * 3 + 2] = 0;
      }

    }

    /*
       solve the system
       */
    Real *a = new Real[3 * bodies_involved.size()]; // B * lambda: actually, the lambda buffer is less useful than this one for the integration step!
    solve(J, B, nu, lambda, bounds, a, idx, total_contacts_number,
          bodies_involved.size(), iterations_number);

    /*
       copy back the lambda buffer for warm starting the engine:
       */
    i_lambda = lambda;
    for(std::vector<Collision*>::iterator  j = contacts_involved.begin();
        j != contacts_involved.end();
        j++)
    {
      for(std::vector<ContactBackup*>::iterator ctct = (*j)->c.begin();
          ctct != (*j)->c.end();
          ctct++)
      {
        (*ctct)->lambda = *(i_lambda++);
        (*ctct)->frictionlambda = *(i_lambda++);
      }
    }
    /*
       last, integrate:
       V_2 = V_1 + dt (M^-1 * J^t * lambda + M^-1 * F_ext)
       = V_1 + dt (a + M^-1 * F_ext)
FIXME: once again, it is assumed that the only F_ext is the G gravity on the y axis.
*/
    for (unsigned int i = 0; i < bodies_involved.size(); i++) 
    {
      Real d_vx = dt * a[i * 3];
      Real d_vy = dt * a[i * 3 + 1];// + G;
      Real d_omega = dt * a[i * 3 + 2];
      /*
       * when we compute the energy to determine if the object must be awaken,
       * the effect of gravity (or, actually, any external force) must be
       * removed from the velocity correction since it is the amount of
       * velocity needed to kill the gravity which will be introduced in the
       * next integration!
       */
      Real d_vy_without_acceleration = d_vy + dt * G;
      /*
         see if we need to awake some bodies
         */
      RigidBody *rb = bodies_involved[i]; 

      if(rb->isSleeping())
      {
        if(d_vx * d_vx + d_vy_without_acceleration * d_vy_without_acceleration
           + d_omega * d_omega > SLEEPLIMIT)
        {
          rb->set_fake_sleep_state(false);
          /*
             rb->addV(Vector2D(d_vx, d_vy, 0));
             rb->addRV(d_omega);
             */
        }
        else
          rb->set_fake_sleep_state(false);//true);
      }
      else
      {
        rb->set_fake_sleep_state(false);
        /*
           rb->addV(Vector2D(d_vx, d_vy, 0));
           rb->addRV(d_omega);
           */
      }


      //std::cout << d_vx << " and " << d_vy << " ando " << d_omega << std::endl;
      /*
         rb->setPos(rb->getPos()+rb->getV()*PIX_PER_METTER*dt);
         rb->setDeltaTeta(-PIX_PER_METTER*rb->getOmega()*dt);
         rb->setTeta(rb->getTeta()+rb->getDeltaTeta());
         */
    }
    //std::cout << "loop" << std::endl;

    /*
       free memory
       */
    delete[] J;
    delete[] B;
    delete[] idx;
    delete[] nu;
    delete[] lambda;
    delete[] bounds;
    delete[] vomega;
    delete[] mi;
    delete[] a;
    delete[] zeta;
  }

  void Island::solve_stack_without_sleeping(Real dt, unsigned int iterations_number)
  {
    Real invdt = 1.0 / dt;
    total_contacts_number *= 2; // normal constraint + friction constraint
    /*
       build the matrices
       */
    Real *J = new Real[2 * 3 * total_contacts_number];   // {vx vy w} * 2 * n 
    Real *B = new Real[2 * 3 * total_contacts_number];    // {vx vy w} * 2 * n. It's the same matrix as the jacobian's but each term is multiplied by M^-1
    int *idx = new int[2 * total_contacts_number]; //RigidBody **idx = new RigidBody*[2 * total_contacts_number]; // sparce matrix indices
    Real *zeta = new Real[total_contacts_number]; // work done by constraints: zeta = JV
    Real *nu = new Real[total_contacts_number];          // nu = invdt * zeta - J (invdt * V1 + M^-1 * Fext)
    Real *lambda = new Real[total_contacts_number];      // unknowns' matrix
    Real *bounds = new Real[total_contacts_number * 2];  // constraints bounds
    /*
     * Here, some characteristics are copied off the rigid bodies: v, omega, m,
     * I.  I dont know if it's really faster than accessing these datas using
     * RigidBodies' pointers.  Here, we may think that copying is better
     * because of porcessor's cash… So: FIXME: verify that it's true (comparing
     * times).
     */
    Real *vomega = new Real[bodies_involved.size() * 3];
    Real *mi = new Real[bodies_involved.size() * 3]; // contains the mass and the inertia
    /*
     * initialize shapes' indices
     */
    for(unsigned int i = 0; i < bodies_involved.size(); i++)
    {
      RigidBody *rb = bodies_involved[i];
      vomega[i * 3] = rb->getV().getX();
      vomega[i * 3 + 1] = rb->getV().getY();
      vomega[i * 3 + 2] = rb->getOmega();
      mi[i * 2] = rb->getInvM();
      mi[i * 2 + 1] = rb->getInvI();
      rb->setIslandIndex(i);
    }
    /*
       fill matrices
       */
    //### lambda: FIXME: optimize with lambda cash. For now, fill with zeroes.
    std::fill(lambda, lambda + total_contacts_number, 0);
    //### J: contact constraints. Assemble the matrix calling the perparation method in each contacts.
    //### bounds
    //### idx
    //### zeta
    /*
     *  indexes
     */
    Real *i_bounds = bounds;
    Real *i_J = J;
    int *i_idx = idx;
    Real *i_zeta = zeta;
    Real *i_lambda = lambda;
    for(std::vector<Collision*>::iterator  j = contacts_involved.begin(); j != contacts_involved.end(); j++)
    {
      Collision *curr_col = *j;
      ContactGenerator::PrepareContactDatasInMatrix_without_sleeping(dt, curr_col, i_J, i_bounds, i_zeta, i_lambda, i_idx);
    }

    //### nu: FIXME: for now, we assume that Fext = mG. 
    // Thus: nu = - J (invdt * {vx vy w, vx' vy' w', …} + {0 G 0, 0 G 0, …})
    //          = - J (invdt * {vx (vy + G) w ; vx' (vy' + G) w' ; …})
    // FIXME: optimize using pointers' arichmetic
    for(unsigned int i = 0; i < total_contacts_number; i++)
    {
      Real vax1 = vomega[idx[i * 2] * 3] * invdt;
      Real vay1 = vomega[idx[i * 2] * 3 + 1] * invdt + G;
      Real w1 = vomega[idx[i * 2] * 3 + 2] * invdt;
      unsigned i23 = i * 2 * 3;
      nu[i] = zeta[i] - (J[i23] * vax1 + J[i23 + 1] * vay1 + J[i23 + 2] * w1);
      if(idx[i*2+1] >= 0)
      {
        Real vax2 =idx[i*2+1] >= 0 ?  vomega[idx[i * 2 + 1] * 3] * invdt :0.;
        Real vay2 =idx[i*2+1] >= 0 ?  vomega[idx[i * 2 + 1] * 3 + 1] * invdt + G:0.;
        Real w2 =idx[i*2+1] >= 0 ?  vomega[idx[i * 2 + 1] * 3 + 2] * invdt:0.;

        unsigned i23p3 = i * 2 * 3 + 3;
        nu[i] -= J[i23p3] * vax2 + J[i23p3 + 1] * vay2 + J[i23p3 + 2] * w2;
      }

    }
    //### B:
    // FIXME: use arithmetic on pointers
    for(unsigned int i = 0; i < total_contacts_number; i++)
    {
      Real im1 = mi[idx[i * 2] * 2];
      Real ii1 = mi[idx[i * 2] * 2 + 1];

      B[i * 3] = J[i * 6] * im1;
      B[i * 3 + 1] = J[i * 6 + 1] * im1;
      B[i * 3 + 2] = J[i * 6 + 2] * ii1;

      if(idx[i * 2 + 1] >= 0)
      {
        Real im2 = mi[idx[i * 2 + 1] * 2];
        Real ii2 = mi[idx[i * 2 + 1] * 2 + 1];
        // go to next line because we work with the transpose matrix
        B[(i + total_contacts_number) * 3] = J[i * 6 + 3] * im2;
        B[(i + total_contacts_number) * 3 + 1] = J[i * 6 + 4] * im2;
        B[(i + total_contacts_number) * 3 + 2] = J[i * 6 + 5] * ii2;
      }
      else
      {
        B[(i + total_contacts_number) * 3] = 0;
        B[(i + total_contacts_number) * 3 + 1] = 0;
        B[(i + total_contacts_number) * 3 + 2] = 0;
      }

    }

    /*
       solve the system
       */
    Real *a = new Real[3 * bodies_involved.size()]; // B * lambda: actually, the lambda buffer is less useful than this one for the integration step!
    solve(J, B, nu, lambda, bounds, a, idx, total_contacts_number, bodies_involved.size(), iterations_number);

    /*
       copy back the lambda buffer for warm starting the engine:
       */
    /*
       i_lambda = lambda;
       for(std::vector< std::vector<Collision*> >::iterator i = stackLevels_lcp.begin(); i != stackLevels_lcp.end(); i++)
       {
       for(std::vector<Collision*>::iterator  j = i->begin(); j != i->end(); j++)
       {
       for(std::vector<ContactBackup*>::iterator ctct = (*j)->c.begin(); ctct != (*j)->c.end(); ctct++)
       {
       (*ctct)->lambda = *(i_lambda++);
       (*ctct)->frictionlambda = *(i_lambda++);
       }
       }
       }
       */
    /*
     * last, integrate: V_2 = V_1 + dt (M^-1 * J^t * lambda + M^-1 * F_ext) =
     * V_1 + dt (a + M^-1 * F_ext) FIXME: once again, it is assumed that the
     * only F_ext is the G gravity on the y axis.
     */
    for (unsigned int i = 0; i < bodies_involved.size(); i++) 
    {
      Real d_vx = dt * a[i * 3];
      Real d_vy = dt * a[i * 3 + 1];// + G;
      Real d_omega = dt * a[i * 3 + 2];
      /*
       * when we compute the energy to determine if the object must be awaken,
       * the effect of gravity (or, actually, any external force) must be
       * removed from the velocity correction since it is the amount of
       * velocity needed to kill the gravity which will be introduced in the
       * next integration!
       */
      Real d_vy_without_acceleration = d_vy + dt * G;
      /*
         see if we need to awake some bodies
         */
      RigidBody *rb = bodies_involved[i]; 

      if(rb->isSleeping())
      {
        if(d_vx * d_vx +
           d_vy_without_acceleration * d_vy_without_acceleration +
           d_omega * d_omega > SLEEPLIMIT)
        {
          rb->setAwake(true);
          rb->addV(Vector2D(d_vx, d_vy, 0));
          rb->addRV(d_omega);
        }
      }
      else
      {
        rb->addV(Vector2D(d_vx, d_vy, 0));
        rb->addRV(d_omega);
      }


      //std::cout << d_vx << " and " << d_vy << " ando " << d_omega << std::endl;
      /*
         rb->setPos(rb->getPos()+rb->getV()*PIX_PER_METTER*dt);
         rb->setDeltaTeta(-PIX_PER_METTER*rb->getOmega()*dt);
         rb->setTeta(rb->getTeta()+rb->getDeltaTeta());
         */
    }
    //std::cout << "loop" << std::endl;

    /*
       free memory
       */
    delete[] J;
    delete[] B;
    delete[] idx;
    delete[] nu;
    delete[] lambda;
    delete[] bounds;
    delete[] vomega;
    delete[] mi;
    delete[] a;
    delete[] zeta;
  }

  void Island::solve_positions_without_sleeping(Real dt, unsigned int iterations_number)
  {
    Real invdt = 1.0 / dt;
    total_contacts_number /= 2; // FIXME: weird

    /*
       build the matrices
       */
    Real *J = new Real[2 * 3 * total_contacts_number];   // {vx vy w} * 2 * n 
    Real *B = new Real[2 * 3 * total_contacts_number];    // {vx vy w} * 2 * n. It's the same matrix as the jacobian's but each term is multiplied by M^-1
    int *idx = new int[2 * total_contacts_number]; //RigidBody **idx = new RigidBody*[2 * total_contacts_number]; // sparce matrix indices
    Real *zeta = new Real[total_contacts_number]; // work done by constraints: zeta = JV
    Real *nu = new Real[total_contacts_number];          // nu = invdt * zeta - J (invdt * V1 + M^-1 * Fext)
    Real *lambda = new Real[total_contacts_number];      // unknowns' matrix
    Real *bounds = new Real[total_contacts_number * 2];  // constraints bounds
    /*
     * Here, some characteristics are copied off the rigid bodies: v, omega, m, I.
     * I dont know if it's really faster than accessing these datas using RigidBodies' pointers.
     * Here, we mais think that copying is better because of porcessor's cash…
     * So: FIXME: verify that it's true (comparing times).
     */
    Real *vomega = new Real[bodies_involved.size() * 3];
    Real *mi = new Real[bodies_involved.size() * 3]; // contains the mass and the inertia
    std::cout << "nbr " << total_contacts_number
              << " involv " << bodies_involved.size() << std::endl;
    /*
     * initialize shapes' indices
     */
    for(unsigned int i = 0; i < bodies_involved.size(); i++)
    {
      RigidBody *rb = bodies_involved[i];
      vomega[i * 3] = rb->getV().getX();
      vomega[i * 3 + 1] = rb->getV().getY();
      vomega[i * 3 + 2] = rb->getOmega();
      mi[i * 2] = rb->getInvM();
      mi[i * 2 + 1] = rb->getInvI();
      rb->setIslandIndex(i);
    }
    /*
       fill matrices
       */
    //### lambda: FIXME: optimize with lambda cash. For now, fill with zeroes.
    std::fill(lambda, lambda + total_contacts_number, 0);
    //### J: contact constraints. Assemble the matrix calling the perparation
    //method in each contacts.
    //### bounds
    //### idx
    //### zeta
    /*
     *  indexes
     */
    Real *i_bounds = bounds;
    Real *i_J = J;
    int *i_idx = idx;
    Real *i_zeta = zeta;
    Real *i_lambda = lambda;
    for(std::vector<Collision*>::iterator  j = contacts_involved.begin();
        j != contacts_involved.end();
        j++)
    {
      Collision* curr_col = *j;
      ContactGenerator::PrepareContactDatasInMatrix_position_without_sleeping(
        dt,
        curr_col,
        i_J,
        i_bounds,
        i_zeta,
        i_lambda,
        i_idx);
    }

    //### nu: FIXME: for now, we assume that Fext = mG. 
    // Thus: nu = - J (invdt * {vx vy w, vx' vy' w', …} + {0 G 0, 0 G 0, …})
    //          = - J (invdt * {vx (vy + G) w ; vx' (vy' + G) w' ; …})
    // FIXME: optimize using pointers' arichmetic
    for(unsigned int i = 0; i < total_contacts_number; i++)
    {
      Real vax1 = vomega[idx[i * 2] * 3] * invdt;
      Real vay1 = vomega[idx[i * 2] * 3 + 1] * invdt + G;
      Real w1 = vomega[idx[i * 2] * 3 + 2] * invdt;
      unsigned i23 = i * 2 * 3;
      nu[i] = zeta[i] - (J[i23] * vax1 + J[i23 + 1] * vay1 + J[i23 + 2] * w1);
      if(idx[i*2+1] >= 0)
      {
        Real vax2 = idx[i * 2 + 1] >= 0 ?
                    vomega[idx[i * 2 + 1] * 3] * invdt
                    : 0.;
        Real vay2 = idx[i * 2 + 1] >= 0 ?
                    vomega[idx[i * 2 + 1] * 3 + 1] * invdt + G
                    : 0.;
        Real w2 = idx[i * 2 + 1] >= 0 ?
                  vomega[idx[i * 2 + 1] * 3 + 2] * invdt
                  : 0.;

        unsigned i23p3 = i * 2 * 3 + 3;
        nu[i] -= J[i23p3] * vax2 + J[i23p3 + 1] * vay2 + J[i23p3 + 2] * w2;
      }

    }
    //### B:
    // FIXME: use arithmetic on pointers
    for(unsigned int i = 0; i < total_contacts_number; i++)
    {
      Real im1 = mi[idx[i * 2] * 2];
      Real ii1 = mi[idx[i * 2] * 2 + 1];

      B[i * 3] = J[i * 6] * im1;
      B[i * 3 + 1] = J[i * 6 + 1] * im1;
      B[i * 3 + 2] = J[i * 6 + 2] * ii1;

      if (idx[i * 2 + 1] >= 0)
      {
        Real im2 = mi[idx[i * 2 + 1] * 2];
        Real ii2 = mi[idx[i * 2 + 1] * 2 + 1];
        // go to next line because we work with the transpose matrix
        B[(i + total_contacts_number) * 3] = J[i * 6 + 3] * im2;
        B[(i + total_contacts_number) * 3 + 1] = J[i * 6 + 4] * im2;
        B[(i + total_contacts_number) * 3 + 2] = J[i * 6 + 5] * ii2;
      }
      else
      {
        B[(i + total_contacts_number) * 3] = 0;
        B[(i + total_contacts_number) * 3 + 1] = 0;
        B[(i + total_contacts_number) * 3 + 2] = 0;
      }
    }

    /*
       solve the system
       */
    Real *a = new Real[3 * bodies_involved.size()]; // B * lambda: actually, the lambda buffer is less useful than this one for the integration step!
    solve(J, B, zeta, lambda, bounds, a, idx, total_contacts_number,
          bodies_involved.size(), iterations_number);

    /*
       copy back the lambda buffer for warm starting the engine:
       */
    /*
       last, integrate:
       V_2 = V_1 + dt (M^-1 * J^t * lambda + M^-1 * F_ext)
       = V_1 + dt (a + M^-1 * F_ext)
FIXME: once again, it is assumed that the only F_ext is the G gravity on the y axis.
*/
    for (unsigned int i = 0; i < bodies_involved.size(); i++) 
    {
      Real d_vx = a[i * 3];
      Real d_vy = a[i * 3 + 1];// + G;
      Real d_omega = a[i * 3 + 2];
      /*
         when we compute the energy to determine if the object must be awaken,
         the effect of gravity (or, actually, any external force) must be removed
         from the velocity correction since it is the amount of velocity needed to kill
         the gravity which will be introduced in the next integration!
         */
      Real d_vy_without_acceleration = d_vy + dt * G;
      /*
         see if we need to awake some bodies
         */
      RigidBody *rb = bodies_involved[i]; 

      if(rb->isSleeping())
      {
        if(d_vx * d_vx +
           d_vy_without_acceleration * d_vy_without_acceleration +
           d_omega * d_omega > SLEEPLIMIT)
        {
          rb->setAwake(true);
          rb->setPos(rb->getPos() +
                     Vector2D(d_vx * PIX_PER_METTER,
                              d_vy * PIX_PER_METTER,
                              0));
          rb->setDeltaTeta(-PIX_PER_METTER*d_omega);
        }
      }
      else
      {
        rb->setPos(rb->getPos() +
                   Vector2D(d_vx * PIX_PER_METTER,
                            d_vy * PIX_PER_METTER, 0));
          rb->setDeltaTeta(-PIX_PER_METTER * d_omega);
      }
    }
    /*
     * free memory
     */
    delete[] J;
    delete[] B;
    delete[] idx;
    delete[] nu;
    delete[] lambda;
    delete[] bounds;
    delete[] vomega;
    delete[] mi;
    delete[] a;
    delete[] zeta;
  }


  /*
   * Solve the linear problem using PGS (Projected Gauss Seidel) iterative
   * solver.  FIXME: move this procedure out of the island file (maybe to a
   * "LCP" file?).
   */
  void Island::solve(Real *J, Real *B, Real *nu, Real *lambda, Real *bounds,
                     Real *a, int *idx, unsigned int s, unsigned int n,
                     unsigned int iterations_number)
  {
    // a = B * lambda
    Real *d = new Real[s];     // JB matrix’s diagonal.
    /*
       compute a = B * lambda_0
       */
    std::fill(a, a + 3 * n, 0.0);
    for (unsigned int i = 0; i < s; i++) 
    {
      int b1 = idx[i * 2];
      int b2 = idx[i * 2 + 1];
      for(unsigned j = 0; j < 3; j++)
      {
        a[b1 * 3 + j] += B[i * 3 + j] * lambda[i];
        if(b2 >= 0)
          a[b2 * 3 + j] += B[i * 3 + s * 3 + j] * lambda[i];
      }
    }
    /*
     * init JB's diagonal
     */
    for(unsigned i = 0; i < s; i++)
    {
      /*
       * we don't use idx here becaues J and B^t have the same sparcity (so,
       * there is no need to use indexing).
       */

      d[i] = J[i * 6] * B[i * 3] +
             J[i * 6 + 3] * B[s * 3 + i * 3] +
             J[i * 6 + 1] * B[i * 3 + 1] +
             J[i * 6 + 1 + 3] * B[s * 3 + i * 3 + 1] +
             J[i * 6 + 2] * B[i * 3 + 2] +
             J[i * 6 + 2 + 3] * B[s * 3 + i * 3 + 2];
    }
    /*
     * solve the system
     */
    unsigned iter;
    for(iter = 0; iter < iterations_number; iter++)
    {
      /*
       * measure the variation of all lambdas over one iteration.
       * allow to exit the solver sooner.
       * Is it worth it?
       */
      //Real d_lambda_accu = 0.0;
      for(unsigned i = 0; i < s; i++)
      {
        int b1 = idx[i * 2] * 3;
        int b2 = idx[i * 2 + 1] * 3;

        Real d_lambda_i = nu[i];
        d_lambda_i -= J[6 * i] * a[b1] +
                      J[6 * i + 1] * a[b1 + 1] +
                      J[6 * i + 2] * a[b1 + 2];
        if(b2 >= 0)
          d_lambda_i -= J[6 * i + 3] * a[b2] +
                        J[6 * i + 3 + 1] * a[b2 + 1] +
                        J[6 * i + 3 + 2] * a[b2 + 2];
        d_lambda_i /= d[i];
        /*
         * clamp the value such that: lambda- <= lambda <= lambda+
         */
        Real lambda_i_0 = lambda[i];
        lambda[i] = MAX(bounds[i * 2],
                        MIN(lambda_i_0 + d_lambda_i, bounds[i * 2 + 1]));
        d_lambda_i = lambda[i] - lambda_i_0;

        for(unsigned j = 0; j < 3; j++)
        {
          Real da = d_lambda_i * B[i * 3 + j];
          a[b1 + j] += da;
          //d_lambda_accu += ABS(da);
          if(b2 >= 0)
          {
            da = d_lambda_i * B[i * 3 + s * 3 + j];
            a[b2 + j] += da;
            //d_lambda_accu += ABS(da);

          }
        }
      }
      /*
       * if(d_lambda_accu < 6 * s)
       * break;
       */
    }
    delete[] d;
  }
}
