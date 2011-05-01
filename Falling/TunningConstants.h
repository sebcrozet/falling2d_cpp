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
#include <vld.h>
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
#include <float.h>
// Define Floating point accuracy
#define DOUBLE
#ifdef DOUBLE
#	define Real double
#	define MACHINE_EPSILON DBL_EPSILON
#	define MACHINE_MAX DBL_MAX
#	define MACHINE_MIN DBL_MIN
#	define MAX(a,b) std::max((a),(b))
#	define MIN(a,b) std::min((a),(b))
#	define ABS(a) std::abs((a))
#	define POW(a,b) (pow((a), (b)))
#else
#	define Real float
#	define MACHINE_EPSILON FLT_EPSILON
#	define MACHINE_MAX FLT_MAX
#	define MACHINE_MIN FLT_MIN
#	define MAX(a,b) ((float)std::max((float)(a),(float)(b)))
#	define MIN(a,b) ((float)std::min((float)(a),(float)(b)))
#	define ABS(a) ((float)std::abs((float)(a)))
#	define POW(a,b) ((float)pow((float)(a), (float)(b)))
#endif
// Define comparaison functions

#define G (50.0 * 9.81)
#define PIX_PER_METTER 1.0
#define POINT_EQUALS_ERROR (MACHINE_EPSILON)
#define PROXIMITY_AWARENESS 8.0
//
#define Falling_CONFIG
#endif
