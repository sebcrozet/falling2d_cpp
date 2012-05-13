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
#include "stdafx.hh"
#include "Point2D.hh"

namespace Falling
{
    Vector2D::Vector2D(const Vector2D &v) : x(v.x), y(v.y), z(v.z)
    { }
    Vector2D::Vector2D(const Point2D &pt) : x(pt.getX()), y(pt.getY()), z(0)
    { }
    Vector2D::Vector2D(const Point2D &p1, const Point2D &p2) : x(p2.getX() - p1.getX()), y(p2.getY() - p1.getY()), z(0)
    { }
    Vector2D::Vector2D(Real x, Real y, Real z) : x(x), y(y), z(z)
    { }

}
