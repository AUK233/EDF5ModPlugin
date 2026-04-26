#include <windows.h>
#include <string>
#include <vector>
#include <format>
#include <stdexcept>
#include <list>
#include <cstdlib>

#include "g_DXresource.h"

extern "C" {
	uintptr_t pOutRenderColor1259640, vedf1259640; // EDF5.exe+1259640
}

void __fastcall XGS_DXresource_Initialize(PBYTE hmodEXE) {
	vedf1259640 = (uintptr_t)(hmodEXE + 0x1259640);
	pOutRenderColor1259640 = vedf1259640;
}
