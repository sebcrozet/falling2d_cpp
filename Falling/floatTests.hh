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
#ifndef __FALLING_FLOAT_TESTS
# include "stdafx.hh"
# include "TunningConstants.hh"
# include <stdarg.h>
# define ZERO_EPSILON Float::sqFloatEps

namespace Falling
{
  struct Float
  {
    static Real sqFloatEps;
    static inline bool equal(Real a, Real b)
    {
      Real aa = ABS(a), bb =  ABS(b);
      return ABS(a - b) <= MAX(aa, MAX(bb, 1.0)) * ZERO_EPSILON;
    }

    static inline bool negativeOrZero(Real a)
    {
      return a < MAX(a, 1.0) * ZERO_EPSILON;
    }

    static inline bool zero(Real a)
    {
      return negativeOrZero(ABS(a));
    }

    static int sumSign(int n, ...);
  };
}
# define __FALLING_FLOAT_TESTS
#endif
