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
#ifndef __FALLING_WORLD
#include "TunningConstants.h"
#include "math.h"
#include "Polygon2D.h"
#include "VitessSolver.h"
#include "Island.h"

namespace Falling
{
    class FALLINGAPI World
    {
	private:
	    bool paused;
	    CollisionArbiter ca;
	    std::vector<RigidBody *> objs;
	    std::stack<RigidBody *> removeWaitingQueue;
	    std::stack<RigidBody *> addWaitingQueue;

	    void dumpAddDelete();
	    void checkSleeps(Real dt);
	public:
	    World();
	    ~World();
	    // for debug
	    bool penAlgorithm;
	    // end: for debug
	    inline void setPaused(bool enginePaused)
	    {
		paused = enginePaused;
	    }
	    inline bool getPaused()
	    {
		return paused;
	    }
	    void addObject(RigidBody *s);
	    void removeObject(RigidBody *s);
        std::vector<Collision *> solve(Real dt);
	    void integrate(Real dt);
        std::vector<Collision *> solvePenetrationsAndImpulseWithLCP(Real dt);

	    void notifyObjectMoved(RigidBody *s);
    };
}
#define __FALLING_WORLD
#endif
