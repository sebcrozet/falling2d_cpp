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
#ifndef CGEN
#include "CollisionArbiter.h"
#include "RigidBody.h"
#include <vector>

namespace Falling
{
    struct Contact
    {
        /*
         Datas for LCP solving (jacobian).
         */
        Real jacobian[6]; // v1.x v1.y w1 v2.x v2.y w2
        Real jacobian_indices[6]; // indices of objects
        Real bounds[2]; // bounds for lambda
        /*
         End of datas for LCP solving.
         */
	// TODO: see if there are no ureless datas here (is dvely still useful?)
	Shape *s1;
	Shape *s2;
	private:
	Real penetration;
	public:
	Point2D absoluteContactPoint;
	Vector2D relContactPoint[2];
	Vector2D normal, tangeant;
	Vector2D closingVelocity;
	Real desiredVelocityChange;
	Real totalInertia;
	Real dvel,dvely;
	Real lin1,lin2;
	Real angin[2];
	Real linin[2];
	Real unitangmov[2];
	Real unitlinmov[2];

	inline Vector2D toLocal(const Vector2D &v) const;
	inline Vector2D toGlobal(const Vector2D &v) const;
	void updateVelChange(Real dt);
	void awakeIfNeeded();
	// TODO: for debug
	inline void setPenetration(Real p)
	{
	    penetration = p;
	    assert(penetration == penetration);
	}
	inline Real getPenetration()
	{
	    return penetration;
	}
	//
    };
    inline Vector2D Contact::toLocal(const Vector2D &v) const
    {
	return Vector2D(v.dot(normal),v.dot(tangeant));
    }
    inline Vector2D Contact::toGlobal(const Vector2D &v) const
    {
	return Vector2D(v.getX()*normal.getX()+v.getY()*tangeant.getX(),v.getX()*normal.getY()+v.getY()*tangeant.getY());
    }

    class ContactGenerator
    {
	private:
	public:
	    static void DeduceContactsDatas(std::vector<Collision *> &collisions, std::vector<Contact *> &concacts,Real dt);
        static void PrepareContactDatasForImpulseSolver(std::vector<Collision *> &collisions,Real dt);
        static void PrepareContactDatasInMatrix(Real dt, Collision *c, Real *J, Real *bounds, Real *zeta, int *idx, unsigned int&i_bounds, unsigned int &i_J, unsigned int &i_idx, unsigned int &i_zeta);
    };
}
#define CGEN
#endif
