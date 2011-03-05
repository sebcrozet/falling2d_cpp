#include "stdafx.h"
#include "PixelPerfectSolver.h"

PixelPerfectSolver::PixelPerfectSolver(PixelPerfectShape *ps, Shape *s) : pps(ps), s(s)
{ }

bool PixelPerfectSolver::_solve(std::vector<SubCollision> &)
{
	// naive climb shape vertices
	return canDestroy();	
}

bool PixelPerfectSolver::canDestroy()
{
	return true; // no cash data for now
}
