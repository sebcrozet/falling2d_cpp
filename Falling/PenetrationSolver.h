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
#include <vector>
#include "ContactGenerator.h"
#include "Shapes.h"
#include "Island.h"

namespace Falling
{
    class PenetrationSolver
    {
	private:
	public:
	    static void solve(std::vector<Contact *> &scs);
	    static void solveRelax(std::vector<Contact *> &scs, Real relaxRate);
	    static void solve(Island *isl, bool implode, int maxiter);
	    static void applyPositionChange(Contact *c,Real *ch,Real *ah);
	    static void applyPositionChangePerLevel(Contact *c,Real *ch,Real *ah, bool implode);
	    static void solveRelax(std::vector<Contact *> &scs);
	    static void applyPositionChangeRelax(Contact *c,Real *ch,Real *ah,Real relaxRate);
    };
}
