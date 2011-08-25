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
#include "stdafx.h"
#include "ImpulseSolver.h"

namespace Falling
{
  /*
   * Version with islands
   *
   */
  void ImpulseSolver::solve(Island *isl,bool implode, int maxiter, Real dt)
  {
    int nbstack = isl->stackLevels.size();
    for(int currlevel = 0;
        currlevel < nbstack;
        currlevel++)
    {
      // next level
      Collision *curr = isl->stackLevels[currlevel];
      // sort list
      curr = Collision::inPlaceSortList_impulsion(curr);
      Collision *begining = curr;
      //assert(curr->c.size());
      for(int id = 0; id < maxiter; id++)
      {
        Vector2D vch[2];
        Vector2D rch[2];
        Contact *worst = 0;
        worst = curr->worstVelocityContact;
        if(worst)
        {
          if(implode)
            applyVelocityChange(worst,rch,vch);
            //applyPositionChange(worst,trchange,rchange);
          else
            applyVelocityChangePerLevel(worst,rch,vch);
            //applyPositionChangePerLevel(worst, trchange, rchange, false);
          // adjust other penetrations
          if(!worst->s2 || implode || worst->s1->getStackLevel() >= worst->s2->getStackLevel())
          {
            // Correct attached nodes
            // iterate through graph's edges:
            Collision * cl = worst->s1->getCollisionList();
            //assert(cl->nexta == cl->nextb || !cl->nextb);
            cl = cl->nexta; // skip sentinel
            while(cl->nexta != cl->nextb)	// while other sentinel not reached; do
            {
              Contact **scs = cl->cnts; // scs[] <==> Contact *[]
              cl->worstVelocityContact = 0;
              cl->worstVelocityChangeAmount = 0.01;
              for(unsigned int j = 0; j < cl->c.size(); j++)
              {
                // update and save worst contact
                Vector2D cp;
                if(scs[j]->s1 == worst->s1)
                {
                  cp = rch[0] ^ scs[j]->relContactPoint[0];
                  cp = cp + vch[0];
                  scs[j]->closingVelocity = scs[j]->closingVelocity + scs[j]->toLocal(cp);
                  scs[j]->updateVelChange(dt);
                }
                else// if(scs[j]->s2 == worst->s1)
                {
                  //assert(scs[j]->s2 == worst->s1);
                  cp = rch[0] ^ scs[j]->relContactPoint[1];
                  cp = cp + vch[0];
                  scs[j]->closingVelocity = scs[j]->closingVelocity - scs[j]->toLocal(cp);
                  scs[j]->updateVelChange(dt);
                }
                if(scs[j]->getPenetration() >= -0.5 && scs[j]->desiredVelocityChange > cl->worstVelocityChangeAmount)
                {
                  cl->worstVelocityContact = scs[j];
                  cl->worstVelocityChangeAmount = scs[j]->desiredVelocityChange;
                }
              }
              if(cl->sa == worst->s1)
                cl = cl->nexta;
              else
                cl = cl->nextb;
            }
          }
          if(worst->s2 && (implode || worst->s1->getStackLevel() <= worst->s2->getStackLevel()))
          {
            // Correct attached nodes
            // iterate through graph's edges:
            Collision * cl = worst->s2->getCollisionList();
            //assert(cl->nexta == cl->nextb || !cl->nextb);
            cl = cl->nexta; // skip sentinel
            while(cl->nexta != cl->nextb)	// while other sentinel not reached; do
            {
              Contact **scs = cl->cnts; // scs[] -> Contact *[]
              cl->worstVelocityContact = 0;
              cl->worstVelocityChangeAmount = 0.01;
              for(unsigned int j = 0; j < cl->c.size(); j++)
              {
                // update and save worst contact
                Vector2D cp;
                if(scs[j]->s1 == worst->s2)
                {
                  cp = rch[1] ^ scs[j]->relContactPoint[0];
                  cp = cp + vch[1];
                  scs[j]->closingVelocity = scs[j]->closingVelocity + scs[j]->toLocal(cp);
                  scs[j]->updateVelChange(dt);
                }
                else// if(scs[j]->s2 == worst->s2)
                {
                  //assert(scs[j]->s2 == worst->s2);
                  cp = rch[1] ^ scs[j]->relContactPoint[1];
                  cp = cp + vch[1];
                  scs[j]->closingVelocity = scs[j]->closingVelocity - scs[j]->toLocal(cp);
                  scs[j]->updateVelChange(dt);
                }
                if(scs[j]->getPenetration() >= -0.5 && scs[j]->desiredVelocityChange > cl->worstVelocityChangeAmount)
                {
                  cl->worstVelocityContact = scs[j];
                  cl->worstVelocityChangeAmount = scs[j]->desiredVelocityChange;
                }
              }
              if(cl->sa == worst->s2)
                cl = cl->nexta;
              else
                cl = cl->nextb;
            }
          }
          // sort list again
          curr = Collision::inPlaceSortList_impulsion(begining);
          begining = curr;

          // TODO: register which node has to be sorted (to avoid useless tests on already sorted elements)
          /*if(worst->s2)
            {
            }	*/
        }
        else break;
      }
    }
  }
  /*
   * end of version with island
   */
  void ImpulseSolver::solve(std::vector<Contact *> &scs,Real dt)
  {
    /*
       printf("%i \n",scs.size());
       for(unsigned int j = 0; j<scs.size(); j++)
       printf(">> %f\n", scs[j]->getPenetration());
       */
    unsigned int i = 0;
    for(; i < scs.size() * 4; i++)
    {
      Vector2D rch[2];
      Vector2D vch[2];
      Contact *worst = 0;
      Real worstV = 0.01;
      for(unsigned int j=0; j<scs.size(); j++)
      {
        if(scs[j]->getPenetration() > -0.5/*Float::sqFloatEps*/ && scs[j]->desiredVelocityChange > worstV)
        {
          worstV = scs[j]->desiredVelocityChange;
          worst = scs[j];
        }
      }
      if(worst)
      {
        applyVelocityChange(worst,rch,vch);

        // adjust other velocities
        for(unsigned int j=0; j<scs.size(); j++)
        {
          Vector2D cp;
          if(scs[j]->s1 == worst->s1)
          {
            cp = rch[0] ^ scs[j]->relContactPoint[0];
            cp = cp + vch[0];
            scs[j]->closingVelocity = scs[j]->closingVelocity + scs[j]->toLocal(cp);
            scs[j]->updateVelChange(dt);
          }
          else if(scs[j]->s2 == worst->s1)
          {
            cp = rch[0] ^ scs[j]->relContactPoint[1];
            cp = cp + vch[0];
            scs[j]->closingVelocity = scs[j]->closingVelocity - scs[j]->toLocal(cp);
            scs[j]->updateVelChange(dt);
          }
          if(worst->s2)
          {
            if(scs[j]->s1 == worst->s2)
            {
              cp = rch[1] ^ scs[j]->relContactPoint[0];
              cp = cp + vch[1];
              scs[j]->closingVelocity = scs[j]->closingVelocity + scs[j]->toLocal(cp);
              scs[j]->updateVelChange(dt);
            }
            else if(scs[j]->s2 == worst->s2)
            {
              cp = rch[1] ^ scs[j]->relContactPoint[1];
              cp = cp + vch[1];
              scs[j]->closingVelocity = scs[j]->closingVelocity - scs[j]->toLocal(cp);
              scs[j]->updateVelChange(dt);
            }
          }
        }
      }
      else break;
    }
    //printf("%i iterations...\n", (int) i);
  }

  void ImpulseSolver::applyVelocityChange(Contact *c,Vector2D *rch,Vector2D *vch)
  {
    Real ximp =c->desiredVelocityChange/(c->dvel);
    Real yimp = -c->closingVelocity.getY() / c->dvely;
    if(ABS(yimp) > ABS(ximp) * 0.1)
    {
      yimp = (yimp < 0 ? -0.1:0.1)*ximp;
    }
    Vector2D impulse = c->toGlobal(Vector2D(ximp,yimp,0));
    vch[0] = (impulse) * c->s1->getParent()->getInvM();
    rch[0] = ((c->relContactPoint[0] ^ impulse) * c->s1->getParent()->getInvI());
    c->s1->getParent()->addV(vch[0]);
    c->s1->getParent()->addRV(rch[0].getZ());
    if(c->s2)
    {
      vch[1] = impulse * (- c->s2->getParent()->getInvM());
      rch[1] = (c->relContactPoint[1] ^ impulse) * -(c->s2->getParent()->getInvI());
      c->s2->getParent()->addV(vch[1]);
      c->s2->getParent()->addRV(rch[1].getZ());
    }
  }

  void ImpulseSolver::applyVelocityChangePerLevel(Contact *c,Vector2D *rch,Vector2D *vch)
  {
    Real ximp =c->desiredVelocityChange/(c->dvel);
    Real yimp = -c->closingVelocity.getY() / c->dvely;
    if(ABS(yimp) > ABS(ximp) * 0.1)
    {
      yimp = (yimp < 0 ? -0.1:0.1)*ximp;
    }
    Vector2D impulse = c->toGlobal(Vector2D(ximp,yimp,0));
    if(!c->s2 || c->s1->getStackLevel() >= c->s2->getStackLevel())
    {
      vch[0] = (impulse) * c->s1->getParent()->getInvM();
      rch[0] = ((c->relContactPoint[0] ^ impulse) * c->s1->getParent()->getInvI());
      c->s1->getParent()->addV(vch[0]);
      c->s1->getParent()->addRV(rch[0].getZ());
    }
    if(c->s2 && c->s1->getStackLevel() <= c->s2->getStackLevel())
    {
      vch[1] = impulse * (- c->s2->getParent()->getInvM());
      rch[1] = (c->relContactPoint[1] ^ impulse) * -(c->s2->getParent()->getInvI());
      c->s2->getParent()->addV(vch[1]);
      c->s2->getParent()->addRV(rch[1].getZ());
    }
  }
}
