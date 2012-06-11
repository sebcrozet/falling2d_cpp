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
#ifndef _COLLISIONDETECTORS
# include "Shapes.hh"
# include "SAPsolver.hh"
# include "SubCollision.hh"
# include "ContactBatch.hh"
# include <vector>

namespace Falling
{
  class CollisionDetector
  {
    private :
      bool  isInactive;

      // returns whether current class should be destroyed
      virtual bool _solve(std::vector<ContactBackup*> &res) = 0;
    public:
      CollisionDetector();
      virtual inline ~CollisionDetector()
      {
        // std::cout << "removing...\n";
      }
      // returns whether current class should be destroyed
      bool solve(std::vector<ContactBackup*> &res);
      inline void setInactive(bool inactive);
      inline bool getIsInactive();
      virtual bool canDestroy() = 0;
  };
  inline bool CollisionDetector::getIsInactive()
  {
    return isInactive;
  }
  inline void CollisionDetector::setInactive(bool inactive)
  {
    isInactive = inactive;
  }
}
# define _COLLISIONDETECTORS
#endif
