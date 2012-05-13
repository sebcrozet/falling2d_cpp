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
#include "PixelPerfectShape.h"

namespace Falling
{
    PixelPerfectShape::PixelPerfectShape(int dw, int dx, bool (*isSolidPixel)(Real, Real))
	: dw(dw), dh(dx), isSolid(isSolidPixel)
    {
	fixedobj = true;
    }

    Real PixelPerfectShape::getSurface()
    {
	return dh * dw * 4;
    }
    Real PixelPerfectShape::getInertiaMomentum(Real)
    {
	return 0;
    }
    int PixelPerfectShape::getShapeTypeID()
    {
	return 20;
    }
    void PixelPerfectShape::updateAABB()
    {
	Vector2D u = t.getU();
	aabb_xM = u.getX() + dw;
	aabb_xm = u.getX() - dw;
	aabb_xm = u.getY() - dh;
	aabb_yM = u.getY() + dh;
    }
}