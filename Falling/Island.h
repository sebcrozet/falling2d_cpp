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
#ifndef __ISLANDS__
#include "CollisionArbiter.h"
#include "ContactGenerator.h"

namespace Falling
{
    class PenetrationSolver;
    class World;
    class Island
    {
        friend class PenetrationSolver;
        friend class ImpulseSolver;
        // TODO: remove this friend statement
        friend class World;
        // end TODO
	public:
        // TODO: remove (test)
        bool isnonfix;
        // end TODO
	private:
        int nbrCtcts;
        std::vector<Collision *> stackLevels;
        std::queue<Shape *> graphNodes; // first graph's nodes for breadth first search.
        /*
         for lcp
         */
        unsigned int total_contacts_number;
        std::vector<RigidBody *> bodies_involved;
        std::vector< std::vector<Collision *> > stackLevels_lcp;
        /*
         end: for lcp
         */
        
        static void batchIsland(Island *isl,Shape *coll); // recursive call
        
    public:
        Island();
        ~Island();
        void calculateStackLevels();
        void pushToLevelOneChain(Collision *c);
        
        // TODO: remove test
        static void verifyLvlPtrChain(Collision *ptr);
        // end TODO
        
        inline void insertToLevelOne(Shape *c)
        { graphNodes.push(c); }
        inline bool isEmpty()
        { return graphNodes.empty(); }
        inline int getNbrCtcts()
        { return nbrCtcts; }
        
        
        static void batchIslands(std::vector<Collision *> &colls,std::stack<Island *> &islands);
        
        /* 
         for lcp
         */
        void doit(Real doit);
        void solve(Real *J, Real *B, Real *nu, Real *lambda, Real *bounds, Real *a, int *idx, unsigned int s, unsigned int n);
        /*
         end: for lcp
         */
    };
}
#define __ISLANDS__
#endif
