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
#include "PenetrationSolver.h"

namespace Falling
{
  void PenetrationSolver::solve(Island *isl,bool implode, int maxiter)
  {
    int nbstack = isl->stackLevels.size();
    for(int currlevel = implode?nbstack-1:0;
        implode?currlevel >= 0: currlevel < nbstack;
        implode?currlevel--:currlevel++)
    {
      // next level
      Collision *curr = isl->stackLevels[currlevel];
      // sort list
      curr = Collision::inPlaceSortList(curr);
      Collision *begining = curr;
      //assert(curr->c.size());
      for(int id = 0; id < maxiter; id++)
      {
        Real trchange[2];
        Real rchange[2];
        Contact *worst = 0;
        worst = curr->worstContact;
        if(worst)
        {
          worst->awakeIfNeeded();
          if(implode)
            applyPositionChange(worst,trchange,rchange);
          else
            applyPositionChangePerLevel(worst, trchange, rchange, false);
          // adjust other penetrations
          if(!worst->s2 ||
              (implode ? worst->s1->getStackLevel() <= worst->s2->getStackLevel()
               : worst->s1->getStackLevel() >= worst->s2->getStackLevel()))
          {
            // Correct attached nodes
            // iterate through graph's edges:
            Collision * cl = worst->s1->getCollisionList();
            //assert(cl->nexta == cl->nextb || !cl->nextb);
            cl = cl->nexta; // skip sentinel
            while(cl->nexta != cl->nextb)	// while other sentinel not reached; do
            {
              Contact **scs = cl->cnts; // scs[] <==> Contact *[]
              cl->worstContact = 0;
              cl->worstPenetrationAmount = 0.01;
              for(unsigned int j = 0; j < cl->c.size(); j++)
              {
                // update and save worst contact
                if(scs[j]->s1 == worst->s1)
                {
                  Real m = (worst->normal*trchange[0])*scs[j]->normal + (Vector2D(0,0,rchange[0])^scs[j]->relContactPoint[0])*scs[j]->normal;
                  scs[j]->setPenetration(scs[j]->getPenetration() - m);
                }
                else// if(scs[j]->s2 == worst->s1)
                {
                  //assert(scs[j]->s2 == worst->s1);
                  scs[j]->setPenetration(scs[j]->getPenetration() + (worst->normal*trchange[0])*scs[j]->normal);
                  scs[j]->setPenetration(scs[j]->getPenetration() + (Vector2D(0,0,rchange[0])^scs[j]->relContactPoint[0])*scs[j]->normal);
                }
                if(scs[j]->getPenetration() > cl->worstPenetrationAmount)
                {
                  cl->worstContact = scs[j];
                  cl->worstPenetrationAmount = scs[j]->getPenetration();
                }
              }
              if(cl->sa == worst->s1)
                cl = cl->nexta;
              else
                cl = cl->nextb;
            }
          }
          if(worst->s2 &&	(implode ? worst->s1->getStackLevel() >= worst->s2->getStackLevel()
                : worst->s1->getStackLevel() <= worst->s2->getStackLevel()))
          {
            // Correct attached nodes
            // iterate through graph's edges:
            Collision * cl = worst->s2->getCollisionList();
            //assert(cl->nexta == cl->nextb || !cl->nextb);
            cl = cl->nexta; // skip sentinel
            while(cl->nexta != cl->nextb)	// while other sentinel not reached; do
            {
              Contact **scs = cl->cnts; // scs[] -> Contact *[]
              cl->worstContact = 0;
              cl->worstPenetrationAmount = 0.01;
              for(unsigned int j = 0; j < cl->c.size(); j++)
              {
                // update and save worst contact
                if(scs[j]->s1 == worst->s2)
                {
                  Real m = (worst->normal*trchange[1])*scs[j]->normal + (Vector2D(0,0,rchange[1])^scs[j]->relContactPoint[1])*scs[j]->normal;
                  scs[j]->setPenetration(scs[j]->getPenetration() - m);
                }
                else// if(scs[j]->s2 == worst->s2)
                {
                  //assert(scs[j]->s2 == worst->s2);
                  scs[j]->setPenetration(
                      scs[j]->getPenetration() +
                      (worst->normal*trchange[1])*scs[j]->normal +
                      (Vector2D(0,0,rchange[1])^scs[j]->relContactPoint[1])*scs[j]->normal);
                }
                if(scs[j]->getPenetration() > cl->worstPenetrationAmount)
                {
                  cl->worstContact = scs[j];
                  cl->worstPenetrationAmount = scs[j]->getPenetration();
                }
              }
              if(cl->sa == worst->s2)
                cl = cl->nexta;
              else
                cl = cl->nextb;
            }
          }
          // sort list again
          curr = Collision::inPlaceSortList(begining);
          begining = curr;

          // TOTO: register which node has to be sorted (to avoid useless tests on already sorted elements)
          /*if(worst->s2)
            {
            }	*/
        }
        else break;
      }
    }
  }


  void PenetrationSolver::solve(std::vector<Contact *> &scs)
  {
    for(unsigned int id = 0; id < scs.size() * 4; id++)
    {
      Real trchange[2];
      Real rchange[2];
      Contact *worst = 0;
      Real worstP = 0.01;
      for(unsigned int j=0; j<scs.size(); j++)
      {
        if(scs[j]->getPenetration() > worstP)
        {
          worstP = scs[j]->getPenetration();
          worst = scs[j];
        }
      }
      if(worst)
      {
        worst->awakeIfNeeded();
        applyPositionChange(worst,trchange,rchange);
        // adjust other penetrations
        for(unsigned int j=0; j<scs.size(); j++)
        {
          if(scs[j]->s1 == worst->s1)
          {
            Real m = (worst->normal*trchange[0])*scs[j]->normal + (Vector2D(0,0,rchange[0])^scs[j]->relContactPoint[0])*scs[j]->normal;
            scs[j]->setPenetration(scs[j]->getPenetration() - m);
          }
          else if(scs[j]->s2 == worst->s1)
          {
            scs[j]->setPenetration(scs[j]->getPenetration() + (worst->normal*trchange[0])*scs[j]->normal);
            scs[j]->setPenetration(scs[j]->getPenetration() + (Vector2D(0,0,rchange[0])^scs[j]->relContactPoint[0])*scs[j]->normal);
          }
          if(worst->s2)
          {
            if(scs[j]->s1 == worst->s2)
            {
              Real m = (worst->normal*trchange[1])*scs[j]->normal + (Vector2D(0,0,rchange[1])^scs[j]->relContactPoint[1])*scs[j]->normal;
              scs[j]->setPenetration(scs[j]->getPenetration() - m);
            }
            else if(scs[j]->s2 == worst->s2)
            {
              Real m = (worst->normal*trchange[1])*scs[j]->normal + (Vector2D(0,0,rchange[1])^scs[j]->relContactPoint[1])*scs[j]->normal;
              scs[j]->setPenetration(scs[j]->getPenetration() + m);
            }
          }
        }
      }
      else
        break;
    }
  }

  void PenetrationSolver::applyPositionChangePerLevel(Contact *c,Real *ch,Real *ah, bool implode)
  {
    Real limita;
    Real pivot;
    Real dch;
    bool lvlequ = !c->s2 || c->s1->getStackLevel() == c->s2->getStackLevel();
    if(!c->s2 || (implode ? c->s1->getStackLevel() <= c->s2->getStackLevel()
          : c->s1->getStackLevel() >= c->s2->getStackLevel()))
    {
      Real factor = (!lvlequ)?1.0/((c->linin[0] + c->angin[0])*c->totalInertia):1.0;
      ah[0] = c->getPenetration()*c->unitangmov[0]*factor;
      ch[0] = c->getPenetration()*c->unitlinmov[0]*factor;
      limita = c->s1->getParent()->getDeltaTeta();
      pivot = c->s1->getParent()->getConsumedDeltaTeta();
      dch = 0;
      if(limita < 0)
      {
        if(ah[0] + pivot > 0)
        {
          dch = ((Vector2D(0,0,(ah[0] + pivot)) ^ c->relContactPoint[0])*c->normal);
          ah[0] = -pivot;
        }
        else if(ah[0] + pivot < limita)
        {
          dch = ((Vector2D(0,0,(ah[0] - limita + pivot)) ^ c->relContactPoint[0])*c->normal);
          ah[0] = limita - pivot;
        }
        //else ok
      }
      else // limita >= 0 // same thing
      {
        if(ah[0] + pivot < 0)
        {
          dch = ((Vector2D(0,0,(ah[0] + pivot)) ^ c->relContactPoint[0])*c->normal);
          ah[0] = -pivot;
        }
        else if(ah[0] + pivot > limita)
        {
          dch = ((Vector2D(0,0,(ah[0] - limita + pivot)) ^ c->relContactPoint[0])*c->normal);
          ah[0] = limita - pivot;
        }
        //else ok
      }
      c->s1->getParent()->addConsumedTeta(ah[0]);
      ch[0] += dch;
      c->s1->translate(c->normal*ch[0]);
      c->s1->rotate(-ah[0]);
    }
    if(c->s2 && (implode ? c->s1->getStackLevel() >= c->s2->getStackLevel()
          : c->s1->getStackLevel() <= c->s2->getStackLevel()))
    {
      Real factor = (!lvlequ)?1.0/((c->linin[1] + c->angin[1])*c->totalInertia):1.0;
      ch[1] = -c->getPenetration()*c->unitlinmov[1]*factor;
      ah[1] = -c->getPenetration()*c->unitangmov[1]*factor;

      limita = c->s2->getParent()->getDeltaTeta();
      pivot = c->s2->getParent()->getConsumedDeltaTeta();
      dch = 0;
      if(limita < 0)
      {
        if(ah[1] + pivot > 0)
        {
          dch = ((Vector2D(0,0,(ah[1] + pivot)) ^ c->relContactPoint[1])*c->normal);
          ah[1] = -pivot;
        }
        else if(ah[1] + pivot < limita)
        {
          dch = ((Vector2D(0,0,(ah[1] - limita + pivot)) ^ c->relContactPoint[1])*c->normal);
          ah[1] = limita - pivot;
        }
        //else ok
      }
      else // limita >= 0 // same thing
      {
        if(ah[1] + pivot < 0)
        {
          dch = ((Vector2D(0,0,(ah[1] + pivot)) ^ c->relContactPoint[1])*c->normal);
          ah[1] = -pivot;
        }
        else if(ah[1] + pivot > limita)
        {
          dch = ((Vector2D(0,0,(ah[1] - limita + pivot)) ^ c->relContactPoint[1])*c->normal);
          ah[1] = limita - pivot;
        }
        //else ok
      }
      c->s2->getParent()->addConsumedTeta(ah[1]);
      ch[1] += dch;
      c->s2->translate(c->normal*ch[1]);
      c->s2->rotate(-ah[1]);
    }
  }

  void PenetrationSolver::applyPositionChange(Contact *c,Real *ch,Real *ah)
  {
    ah[0] = c->getPenetration()*c->unitangmov[0];
    ch[0] = c->getPenetration()*c->unitlinmov[0];
    Real limita = c->s1->getParent()->getDeltaTeta();
    Real pivot = c->s1->getParent()->getConsumedDeltaTeta();
    Real dch = 0;
    if(limita < 0)
    {
      if(ah[0] + pivot > 0)
      {
        dch = ((Vector2D(0,0,(ah[0] + pivot)) ^ c->relContactPoint[0])*c->normal);
        ah[0] = -pivot;
      }
      else if(ah[0] + pivot < limita)
      {
        dch = ((Vector2D(0,0,(ah[0] - limita + pivot)) ^ c->relContactPoint[0])*c->normal);
        ah[0] = limita - pivot;
      }
      //else ok
    }
    else // limita >= 0 // same thing
    {
      if(ah[0] + pivot < 0)
      {
        dch = ((Vector2D(0,0,(ah[0] + pivot)) ^ c->relContactPoint[0])*c->normal);
        ah[0] = -pivot;
      }
      else if(ah[0] + pivot > limita)
      {
        dch = ((Vector2D(0,0,(ah[0] - limita + pivot)) ^ c->relContactPoint[0])*c->normal);
        ah[0] = limita - pivot;
      }
      //else ok
    }
    c->s1->getParent()->addConsumedTeta(ah[0]);
    ch[0] += dch;
    c->s1->translate(c->normal*ch[0]);
    c->s1->rotate(-ah[0]);
    if(c->s2)
    {
      ch[1] = -c->getPenetration()*c->unitlinmov[1];
      ah[1] = -c->getPenetration()*c->unitangmov[1];

      limita = c->s2->getParent()->getDeltaTeta();
      pivot = c->s2->getParent()->getConsumedDeltaTeta();
      dch = 0;
      if(limita < 0)
      {
        if(ah[1] + pivot > 0)
        {
          dch = ((Vector2D(0,0,(ah[1] + pivot)) ^ c->relContactPoint[1])*c->normal);
          ah[1] = -pivot;
        }
        else if(ah[1] + pivot < limita)
        {
          dch = ((Vector2D(0,0,(ah[1] - limita + pivot)) ^ c->relContactPoint[1])*c->normal);
          ah[1] = limita - pivot;
        }
        //else ok
      }
      else // limita >= 0 // same thing
      {
        if(ah[1] + pivot < 0)
        {
          dch = ((Vector2D(0,0,(ah[1] + pivot)) ^ c->relContactPoint[1])*c->normal);
          ah[1] = -pivot;
        }
        else if(ah[1] + pivot > limita)
        {
          dch = ((Vector2D(0,0,(ah[1] - limita + pivot)) ^ c->relContactPoint[1])*c->normal);
          ah[1] = limita - pivot;
        }
        //else ok
      }
      c->s2->getParent()->addConsumedTeta(ah[1]);
      ch[1] += dch;
      c->s2->translate(c->normal*ch[1]);
      c->s2->rotate(-ah[1]);
    }
  }
}
