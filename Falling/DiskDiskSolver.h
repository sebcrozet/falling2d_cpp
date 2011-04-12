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
// Specific solver for simple shapes (more efficient than always using GJK-EPA)
// ( Handle center-coincident disks special case (very poor performances whith EPA) )
#ifndef __DISK_DISK_SOLVER__
#include "CollisionDetector.h"
#include "Disk.h"

class DiskDiskSolver : public CollisionDetector
{
private:
  Disk *d1, *d2;
  ContactBackup lastBackup;

  bool _solve(std::vector<ContactBackup *> &res);
public:
  DiskDiskSolver(Disk *d1, Disk *d2);
  bool canDestroy();
};
#define __DISK_DISK_SOLVER__
#endif
