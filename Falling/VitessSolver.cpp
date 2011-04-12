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
#include "VitessSolver.h"

void VitessSolver::integrate(std::vector<RigidBody*> &r,Real dt)
{
  for(unsigned int i=0; i<r.size(); i++)
    {
      RigidBody * rb = r[i];
      if(rb->isSleeping()) continue;
      if(!rb->getShape()->isFixed())
        {
          //rb->updateSleepState(dt);
          //rb->getV().print();
          rb->setPos(rb->getPos()+rb->getV()*PIX_PER_METTER*dt+Vector2D(0, 0.5*G*dt*dt));
          rb->addV(Vector2D(0,G*dt,0));
          rb->multV(0.998);
          rb->setDeltaTeta(-PIX_PER_METTER*rb->getOmega()*dt);
          rb->setTeta(rb->getTeta()+rb->getDeltaTeta());
        }
    }
}
