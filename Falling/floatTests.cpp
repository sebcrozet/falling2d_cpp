#include "stdafx.h"
#include "floatTests.h"

Real Float::sqFloatEps = sqrt(MACHINE_EPSILON);

int Float::sumSign(int n, ...)
{
	va_list fs;
	va_start(fs, n);
	// use variable arguments list
	// end of variable arguments list
	va_end(fs);
	return 0;
}
