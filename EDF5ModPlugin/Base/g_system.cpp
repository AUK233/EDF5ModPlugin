#include <windows.h>
#include <string>
#include <vector>
#include <format>
#include <stdexcept>
#include <list>
#include <cstdlib>

#include "g_system.h"

extern "C" {
	uintptr_t _F_XGS_System_Pointer_; // EDF5.exe+125AB68
}

void __fastcall XGS_SystemFunction_Initialize(PBYTE hmodEXE)
{
	_F_XGS_System_Pointer_ = (uintptr_t)(hmodEXE + 0x125AB68);
}

PXGS_System __fastcall XGS_GetXGSSystemPointer()
{
	return *(PXGS_System*)_F_XGS_System_Pointer_;
}

PXGS_System_Player __fastcall XGS_GetSystemPlayerPointer(PXGS_System pSystem, int index)
{
	// If it's null, it just crashes the game.
	return &pSystem->player[index];
}
