#ifndef __FALLING_FLOAT_TESTS

#include "stdafx.h"
#include "TunningConstants.h"
#include <stdarg.h>
#define ZERO_EPSILON Float::sqFloatEps 

struct Float
{
	static Real sqFloatEps;
	static inline bool equal(Real a, Real b)
	{
		Real aa = ABS(a), bb =  ABS(b);
		return ABS(a - b) <= MAX(aa, MAX(bb, 1.0)) * ZERO_EPSILON;
	}

	static inline bool negativeOrZero(Real a)
	{ return a < MAX(a, 1.0) * ZERO_EPSILON; }

	static inline bool zero(Real a)
	{ return negativeOrZero(ABS(a)); }

	static int sumSign(int n, ...);
};
#define __FALLING_FLOAT_TESTS
#endif
