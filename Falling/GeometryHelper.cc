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
#include "GeometryHelper.h"

namespace Falling
{
    GeometryHelper::Transformation2D::Transformation2D() : teta(0),coteta((Real)M_PI), siteta(0),  u(0,0)
    { }
    GeometryHelper::Transformation2D::Transformation2D(const Vector2D &u, Real radTeta) : u(u)
    {
	setTeta(radTeta);
    }
    GeometryHelper::Transformation2D::Transformation2D(const GeometryHelper::Transformation2D &t)
    {
	siteta = t.siteta;
	coteta = t.coteta;
	teta = t.teta;
	u = Vector2D(t.u);
    }
}
