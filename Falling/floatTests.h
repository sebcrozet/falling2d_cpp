#include "stdafx.h"
#include "TunningConstants.h"
#include <stdarg.h>
#define ZERO_EPSILON Float::sqFloatEps 

static struct Float
{
	static float sqFloatEps;
	static inline bool equal(float a, float b)
	{
		float aa = ABS(a), bb =  ABS(b);
		return aa - bb <= MAX(aa, MAX(bb, 1.f)) * ZERO_EPSILON;
	}

	static inline bool negativeOrZero(float a)
	{ return a < MAX(a, 1.f) * ZERO_EPSILON; }

	static inline bool zero(float a)
	{ return negativeOrZero(ABS(a)); }

	static int sumSign(int n, ...);
};