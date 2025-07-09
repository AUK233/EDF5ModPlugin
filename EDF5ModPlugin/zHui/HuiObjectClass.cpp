#include <windows.h>
#include <psapi.h>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <format>
#include <stdexcept>
#include <list>
#include <cstdlib>

#include "utiliy.h"
#include "commonNOP.h"
#include "HuiObjectClass.h"

extern "C" {
	LPCWSTR huiObjectClassSGOName[4] = {
		L"base",  // index 0
		L"base_B",// index 1
		L"base_C",// index 2
		L"base_D" // index 3
	};
	// common
	void __fastcall ASMhuiCommonUIClassFuncP8();
	uintptr_t huiCommonUIClassFuncP8RetAddr;
}

void module_UpdateHuiObjectClass(PBYTE hmodEXE)
{
	// EDF5.exe+4AE217
	hookGameBlockWithInt3((void*)(hmodEXE + 0x4AE217), (uintptr_t)ASMhuiCommonUIClassFuncP8);
	WriteHookToProcess((void*)(hmodEXE + 0x4AE217 + 15), (void*)&nop4, 4U);
	huiCommonUIClassFuncP8RetAddr = (uintptr_t)(hmodEXE + 0x4AE22A);

	// EDF5.exe+4AFFFD, other, but now not use
}
