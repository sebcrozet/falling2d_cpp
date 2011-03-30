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