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
#include "ContactGenerator.h"
#include <iostream>

namespace Falling
{
    void ContactGenerator::PrepareContactDatasInMatrix(Real dt, Collision *c, Real *&J, Real *&bounds, Real *&zeta, Real *&lambda, int *&idx)
    {
        for(std::vector<ContactBackup*>::iterator j = c->c.begin(); j != c->c.end(); j++)
        {
            ContactBackup *cb = *j;
            Shape *a = c->sa;
            Shape *b = c->sb;
            Point2D middle = Point2D::getMiddle(a->toGlobal(cb->relPtA), b->toGlobal(cb->relPtB));
            Vector2D norm = cb->normal;
            
            
            if(a->isFixed())
            {
                a = b;
                b = 0;
                norm.reflect();
            }
            else if(b->isFixed())
                b = 0;
            // get tangeant for friction
            Vector2D tangeant = Vector2D(-norm.getY(),norm.getX());
            // now calculate relative points of contact.
            Vector2D relp1 = a->toTranslatedInv(middle);
            Real relative_velocity = -(a->getParent()->getV() * norm - relp1.cross(Vector2D(0,0,a->getParent()->getOmega())) * norm);
            Vector2D relp2;
            if(b)
            {    
                relp2 = b->toTranslatedInv(middle);
                relative_velocity += b->getParent()->getV() * norm - relp2.cross(Vector2D(0,0,b->getParent()->getOmega())) * norm;
            }
            
            /*
             Prepare contact for the LCP solver.
             This is a normal constsraint: J = (-n -(r_1 * n) n (r_2 * n))
             */
            *(J++) = norm.getX();
            *(J++) = norm.getY();
            *(J++) = relp1.perp(norm); // perpendicular product to keep the z component only
            *(bounds++) = 0;
            *(bounds++) = MACHINE_MAX; // infinite
            *(idx++) = a->getParent()->getIslandIndex();
            if(b)
            {
                *(idx++) = b->getParent()->getIslandIndex();
                *(J++) = -norm.getX();
                *(J++) = -norm.getY();
                *(J++) = -relp2.perp(norm);
            }
            else
            {
                *(idx++) = -1;
                *(J++) = 0;
                *(J++) = 0;
                *(J++) = 0;
            }
            /*
             Coefficient to correct the penetration.
             Don't correct anything when it's visibly unnoticeable (<= 2 * PROXIMITY_AWARENESS)
             */
            Real extra_v = 0;
            if(cb->depth > 2 * PROXIMITY_AWARENESS)
                extra_v += 0.8 / dt * cb->depth; // apply an artificial force proportional to the penetration depth
            if(relative_velocity * relative_velocity > 2.0f * SLEEPLIMIT) // use a coefficient of restitution of 0 when the closing velocity is too small => better stability in stacks.
                extra_v += 1 / dt * relative_velocity * 0.5; // 0.5 = coefficient of restitution.
            *(zeta++) = extra_v;
            *(J++) = -tangeant.getX();
            *(J++) = -tangeant.getY();
            *(J++) = -relp1.perp(tangeant); // perpendicular product to keep the z component only
            
            *(bounds++) = -0.5 * (a->getParent()->getM() / a->get_total_number_of_contacts() + (b ? b->getParent()->getM() / b->get_total_number_of_contacts() : 0.0)) * G;
            *(bounds++) = 0.5 * (a->getParent()->getM() / a->get_total_number_of_contacts() + (b ? b->getParent()->getM() / b->get_total_number_of_contacts() : 0.0)) * G;
            *(idx++) = a->getParent()->getIslandIndex();
            if(b)
            {
                *(idx++) = b->getParent()->getIslandIndex();
                *(J++) = tangeant.getX();
                *(J++) = tangeant.getY();
                *(J++) = relp2.perp(tangeant);
            }
            else
            {
                *(idx++) = -1;
                *(J++) = 0;
                *(J++) = 0;
                *(J++) = 0;
            }
            *(zeta++) = 0.; // friction doesn't work.
            *(lambda++) = cb->lambda; // warm start the solver
            *(lambda++) = cb->frictionlambda; // warm start the solver
        }
    }
}
