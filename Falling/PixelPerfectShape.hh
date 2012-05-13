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
#include "Shapes.hh"
#include "Point2D.hh"

namespace Falling
{
    class PixelPerfectShape : public Shape
    {
	private:
	    int dw, dh; // half-w / h
	    bool (*isSolid) (Real x, Real y);
	public:
	    PixelPerfectShape(int dw, int dx, bool (*isSolidPixel)(Real, Real));
	    Real getSurface();
	    Real getInertiaMomentum(Real m);
	    int getShapeTypeID();
	    void updateAABB();
	    inline bool isSolidPoint(int x, int y);
	    inline bool isSolidPoint(Point2D &pt);
	    inline bool isSolidPoint(Vector2D &v);
    };

    inline bool PixelPerfectShape::isSolidPoint(int x, int y)
    {
	return isSolid(x, y);
    }
    inline bool PixelPerfectShape::isSolidPoint(Vector2D &v)
    {
	return isSolid(v.getX(), v.getY());
    }
    inline bool PixelPerfectShape::isSolidPoint(Point2D &pt)
    {
	return isSolid(pt.getX(), pt.getY());
    }
}

