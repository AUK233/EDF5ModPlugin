#include <windows.h>
#include <string>
#include <vector>
#include <format>
#include <stdexcept>
#include <list>
#include <cstdlib>

#include "g_system.h"

extern "C" {
	uintptr_t vedf125AB68; // EDF5.exe+125AB68
	PXGS_System* pGameSystem125AB68;
}

void __fastcall XGS_SystemFunction_Initialize(PBYTE hmodEXE)
{
	vedf125AB68 = (uintptr_t)(hmodEXE + 0x125AB68);
	pGameSystem125AB68 = (PXGS_System*)vedf125AB68;
}

PXGS_System __fastcall XGS_GetXGSSystemPointer()
{
	return *pGameSystem125AB68;
}

PXGS_System_Player __fastcall XGS_GetSystemPlayerPointer(PXGS_System pSystem, int index)
{
	// If it's null, it just crashes the game.
	return &pSystem->player[index];
}
