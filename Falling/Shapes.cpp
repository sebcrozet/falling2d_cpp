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
#include "Shapes.h"

namespace Falling
{

	Shape::~Shape()
	{
		// FIXME: obbtree deletion
		//if(otree)
		//delete otree;
	}

    void ImplicitShape::addMargin(const Vector2D &d, Point2D *pt) const
    {
	*pt += d * margin;
    }

    int ImplicitShape::getMarginedSupportPoint(const Vector2D &d, Point2D *res) const
    {
	int opt = getSupportPoint(d, res);
	addMargin(d, res);
	return opt;
    }

    int ImplicitShape::getMarginedSupportPoint(const Vector2D &d, Point2D *res, int optimisationId) const
    {
	int opt = getSupportPoint(d, res, optimisationId);
	addMargin(d, res);
	return opt;
    }

    void Shape::updateAABB(EndPoint *xm, EndPoint *xM, EndPoint *ym, EndPoint *yM)
    {
	xm->setValue(aabb_xm);
	xM->setValue(aabb_xM);
	ym->setValue(aabb_ym);
	yM->setValue(aabb_yM);
    }
}
