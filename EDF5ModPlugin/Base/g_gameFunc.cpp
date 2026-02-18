#include <windows.h>
#include <string>
#include <vector>
#include <format>
#include <stdexcept>
#include <list>
#include <cstdlib>
#include "g_system.h"

#include "g_gameFunc.h"

extern "C" {
	uintptr_t _F_IsOnlineMode_; // EDF5.exe+3944B0
}

void __fastcall XGS_GetGameFunction_Initialize(PBYTE hmodEXE)
{
	_F_IsOnlineMode_ = (uintptr_t)(hmodEXE + 0x3944B0);
}

bool __fastcall Game_IsOnlineMode()
{
	typedef bool(__fastcall* CallFunc)();
	auto fn = (CallFunc)_F_IsOnlineMode_;
	return fn();
}
