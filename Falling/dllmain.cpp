// dllmain.cpp : Définit le point d'entrée pour l'application DLL.
#include "stdafx.h"
#include "TunningConstants.h"

#ifdef Falling_WINDOWS
BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
  switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
      break;
    }
  return TRUE;
}
#else
void __attribute__((constructor)) DLLload(void)
{ }
#endif

