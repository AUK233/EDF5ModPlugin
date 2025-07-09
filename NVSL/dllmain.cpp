#include <vector>
#include <cstdio>
#include <Windows.h>
#include <string.h>

#include "nvsl.h"

extern "C" void __declspec(dllexport) __fastcall InitializeDLL(PBYTE hmodEXE)
{
	InstallNVIDIAdlss(hmodEXE);
}

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

