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

namespace Falling
{
    void VitessSolver::integrate(std::vector<RigidBody*> &r,Real dt)
    {
        for(std::vector<RigidBody*>::iterator i = r.begin(); i != r.end(); i++)
        {
            RigidBody * rb = *i;
            if(rb->isSleeping()) continue;
            if(!rb->getShape()->isFixed())
            {
                //rb->updateSleepState(dt);
                //rb->getV().print();
                rb->addV(Vector2D(0,G*dt,0));
                rb->setPos(rb->getPos()+rb->getV()*PIX_PER_METTER*dt);// + 0.5 * dt * dt * G);
                rb->multV(0.998); // damping
                rb->setDeltaTeta(-PIX_PER_METTER*rb->getOmega()*dt);
                rb->setTeta(rb->getTeta()+rb->getDeltaTeta());
            }
        }
    }
    
    void VitessSolver::integrate_positions(std::vector<RigidBody*> &r, Real dt)
    {
        for(std::vector<RigidBody*>::iterator i = r.begin(); i != r.end(); i++)
        {
            RigidBody * rb = *i;
            if(rb->isSleeping()) continue;
            if(!rb->getShape()->isFixed())
            {
                //rb->updateSleepState(dt);
                //rb->getV().print();
                rb->setPos(rb->getPos()+rb->getV()*PIX_PER_METTER*dt+Vector2D(0, 0.5*G*dt*dt));
                rb->setDeltaTeta(-PIX_PER_METTER*rb->getOmega()*dt);
                rb->setTeta(rb->getTeta()+rb->getDeltaTeta());
            }
        }
    }
    void VitessSolver::integrate_velocities(std::vector<RigidBody*> &r, Real dt)
    {
        for(std::vector<RigidBody*>::iterator i = r.begin(); i != r.end(); i++)
        {
            RigidBody * rb = *i;
            if(rb->isSleeping()) continue;
            if(!rb->getShape()->isFixed())
            {
                rb->addV(Vector2D(0,G*dt,0));
                rb->multV(0.998);
            }
        }
    }
}
