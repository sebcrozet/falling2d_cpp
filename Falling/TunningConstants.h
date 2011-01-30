#ifndef _DEBUG
	#ifdef Falling_EXPORTS
		#define FALLINGAPI __declspec(dllexport)
	#else
		#define FALLINGAPI __declspec(dllimport)
	#endif	
#else
	#define FALLINGAPI
#endif
