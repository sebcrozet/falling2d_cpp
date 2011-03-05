#ifndef Falling_CONFIG
//////////////////
#if defined(_WIN32) || defined(__WIN32)
	#define Falling_WINDOWS
#elif defined(linux) || defined(__linux)
	#define Falling_LINUX
#elif defined(__APPLE__) || defined(MACOSX) || defined(Macintosh) || defined(macintosh)
	#define Falling_MACOS
#elif defined(__FreeBSD__) || defined(__FreeBSD_kernel__)
	#define Falling_FREEBSD
#else
	#error Operating system not suported by the Falling physics library
#endif


#if defined(Falling_WINDOWS)
	#ifndef _DEBUG
		#ifdef Falling_EXPORTS
			#define FALLINGAPI __declspec(dllexport)
		#else
			#define FALLINGAPI __declspec(dllimport)
		#endif	
	#else
		#define FALLINGAPI 
	#endif
#else
	#define FALLINGAPI
#endif

// TODO: diferenciate compiler instead of platforms
#include <stdlib.h>
#if defined(Falling_WINDOWS)
	#include <windows.h>
	#define MAX(a,b) max((a),(b))
	#define MIN(a,b) min((a),(b))
	#define ABS(a) abs((a))
#else
	#include <cmath>
	#include <algorithm>
	#include <string.h>
	#define MAX(a,b) std::max((a),(b))
	#define MIN(a,b) std::min((a),(b))
	#define ABS(a) std::abs((a))
#endif

#define G 9.81f
#define PIX_PER_METTER 10.f
#define POINT_EQUALS_ERROR (sqrt(FLT_EPSILON))
//
#define Falling_CONFIG
#endif
