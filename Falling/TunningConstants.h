#ifndef Falling_CONFIG
//////////////////
#if defined(_WIN32) || defined(__WIN32)
#	define Falling_WINDOWS
#elif defined(linux) || defined(__linux)
#	define Falling_LINUX
#elif defined(__APPLE__) || defined(MACOSX) || defined(Macintosh) || defined(macintosh)
#	define Falling_MACOS
#elif defined(__FreeBSD__) || defined(__FreeBSD_kernel__)
#	define Falling_FREEBSD
#else
#	error Operating system not suported by the Falling physics library
#endif


#if defined(Falling_WINDOWS)
#	ifndef _DEBUG
#		ifdef Falling_EXPORTS
#			define FALLINGAPI __declspec(dllexport)
#		else
#			define FALLINGAPI __declspec(dllimport)
#		endif	
#	else
#		define FALLINGAPI 
#	endif
#else
#	define FALLINGAPI
#endif

// TODO: diferenciate compiler instead of platforms
#if defined(Falling_WINDOWS)
#	include <windows.h>
#	undef min
#	undef max
#else
#	include <cmath>
#	include <string.h>
#endif
#include <stdlib.h>
#include <algorithm>
// Define Floating point accuracy
#define DOUBLE
#	ifdef DOUBLE
#		define Real double
#		define MACHINE_EPSILON DBL_EPSILON
#		define MACHINE_MAX DBL_MAX
#		define MACHINE_MIN DBL_MIN
#	else
#		define Real float
#		define MACHINE_EPSILON FLT_EPSILON
#		define MACHINE_MAX FLT_MAX
#		define MACHINE_MIN FLT_MIN
#	endif
// Define comparaison functions
#define MAX(a,b) std::max((a),(b))
#define MIN(a,b) std::min((a),(b))
#define ABS(a) std::abs((a))

#define G 10.0*9.81
#define PIX_PER_METTER 10.0
#define POINT_EQUALS_ERROR (MACHINE_EPSILON)
//
#define Falling_CONFIG
#endif

