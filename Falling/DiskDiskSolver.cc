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
#include "DiskDiskSolver.h"

namespace Falling
{
    DiskDiskSolver::DiskDiskSolver(Disk *d1, Disk *d2) : d1(d1), d2(d2)
    { }

    bool DiskDiskSolver::canDestroy()
    {
	return true;
    }

    bool DiskDiskSolver::_solve(std::vector<ContactBackup *> &resl)
    {
	// ignore OBB tree, direct result is available
	Vector2D p1 = d1->getCenter(), p2 = d2->getCenter();
	Real r1 = d1->getRadius(), r2 = d2->getRadius();
	Real sr = r1 + r2;
	Vector2D dp = p2 - p1;
	Real sqd = dp * dp;
	if(sqd > sr * sr)
	    return canDestroy(); // no collision
	else
	{
	    if(dp.isNull())
		dp.setY(1);
	    else
		dp.normalise();
	    Vector2D res = p1 + (dp * r1);
	    Point2D ptA = Point2D(res.getX(), res.getY());
	    res = p2 - (dp * r2);
	    Point2D ptB = Point2D(res.getX(), res.getY());
	    lastBackup.setDatas(ptA, ptB, d1, d2);
	    resl.push_back(&lastBackup);
	    return false;
	}
    }
}
