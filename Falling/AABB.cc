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
#include "AABB.hh"

namespace Falling
{
  AABB::AABB() : parent(0)
  { }

  void AABB::update()
  {
  }

  EndPoint::EndPoint() : parentid(-1), value(0)
  { }

  EndPoint::EndPoint(Real v, bool isMax, int parent) : value(v)
  {
    parentid = (parent << 1) | (isMax & 1);
  }
}
