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
#ifndef AABBs
# include "TunningConstants.hh"
# include "Primitives.hh"

namespace Falling
{
  class Shape;
  struct AABB;
  class EndPoint
  {
    private:
      // parentid & 0x00000001 == ismax ; parentid >> 1 == parent's id
      int parentid;
      Real value;
    public:
      EndPoint();
      EndPoint(Real v, bool isMax, int parentid);

      inline Real getValue() const;
      inline bool isMaxValue() const;
      inline int getParent() const;
      inline void setValue(Real v);
      inline void operator=(const EndPoint &ep);
  };

  inline int EndPoint::getParent() const
  {
    return parentid >> 1;
  }
  inline Real EndPoint::getValue() const
  {
    return value;
  }
  inline bool EndPoint::isMaxValue() const
  {
    return parentid & 1;
  }
  inline void EndPoint::setValue(Real v)
  {
    value = v;
  }
  inline void EndPoint::operator =(const EndPoint &ep)
  {
    value = ep.value;
    parentid = ep.parentid;
  }


  struct AABB
  {
    // [0] -> x // [1] -> y
    int mins[2];
    int maxs[2];
    Shape *parent;
    AABB();

    void update();
  };
}
# define AABBs
#endif
