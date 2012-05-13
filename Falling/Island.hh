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
#include "CollisionArbiter.hh"
#include "ContactGenerator.hh"

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
        std::queue<Shape *> graphNodes; // first graph's nodes for breadth first search.
        /*
         for lcp
         */
        unsigned int total_contacts_number;
        /*
         * FIXME: do we really need both?
         */
        std::vector<RigidBody *> bodies_involved;
        std::vector<Collision *> contacts_involved;
        /*
         end: for lcp
         */
        
        static void batchIsland(Island *isl,Shape *coll); // recursive call
        static void batchIsland_without_sleeping(Island *isl,Shape *coll); // recursive call
        
    public:
        Island();
        ~Island();
        void calculateStackLevels();
        inline
        void add_contact(Collision *c)
        { contacts_involved.push_back(c); }
        inline
          void add_body(RigidBody *rb)
          { bodies_involved.push_back(rb); }
        
        inline void insertToLevelOne(Shape *c)
        { graphNodes.push(c); }
        inline bool isEmpty()
        { return false; }//return graphNodes.empty(); }
        inline int getNbrCtcts()
        { return nbrCtcts; }
        
        
        static void batchIslands(std::vector<Collision *> &colls,std::stack<Island *> &islands);
        static void batchIslands_without_sleeping(std::vector<Collision *> &colls,std::stack<Island *> &islands);
        
        /* 
         for lcp
         */
        void solve_stack(Real doit, unsigned int iterations_number);
        void solve_stack_without_sleeping(Real doit, unsigned int iterations_number);
        void solve_positions_without_sleeping(Real dt, unsigned int iterations_number);
        void doit(Real doit);
        void solve(Real *J, Real *B, Real *nu, Real *lambda, Real *bounds, Real *a, int *idx, unsigned int s, unsigned int n, unsigned int iterations_number);
        /*
         end: for lcp
         */
    };
}
#define __ISLANDS__
#endif
