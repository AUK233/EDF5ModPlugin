#include <windows.h>
#include <string>
#include <vector>
#include <format>
#include <stdexcept>
#include <list>
#include <cstdlib>

#include "utiliy.h"
#include "commonNOP.h"
#include "g_system.h"

#include "base_baseObject.h"

void __fastcall XGS_BaseObject_Initialize(PBYTE hmodEXE)
{
	// EDF5.exe+617563
	BYTE InitializeF34[9] = {
		0x4C, 0x89, 0x71, 0x30, // mov qword ptr [rcx+30], r14
		0x0F, 0x1F, 0x44, 0x00, 0x00
	};
	WriteHookToProcess((void*)(hmodEXE + 0x617563), &InitializeF34, 9U);
}
