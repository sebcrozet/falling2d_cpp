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

void ImpulseSolver::solve(std::vector<Contact *> &scs,Real dt)
{
  /*
  printf("%i \n",scs.size());
  for(unsigned int j = 0; j<scs.size(); j++)
  	printf(">> %f\n", scs[j]->getPenetration());
  	*/
  unsigned int i = 0;
  for(; i < scs.size()*4; i++)
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
  float ximp =c->desiredVelocityChange/(c->dvel);
  float yimp = -c->closingVelocity.getY() / c->dvely;
  if(ABS(yimp) > ABS(ximp) * 0.7)
  {
	  yimp = (yimp < 0 ? -0.7:0.7)*ximp;
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
