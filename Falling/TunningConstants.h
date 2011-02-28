#ifndef _DEBUG
	#ifdef Falling_EXPORTS
		#define FALLINGAPI __declspec(dllexport)
	#else
		#define FALLINGAPI __declspec(dllimport)
	#endif	
#else
	#define FALLINGAPI
#endif
#define max(a,b) ((a > b)?a:b)
#define G 9.81f
#define PIX_PER_METTER 10.f
