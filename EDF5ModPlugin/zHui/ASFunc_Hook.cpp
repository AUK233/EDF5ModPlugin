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

#include "DLSS/1SetPostProcess.h"
#include "ASFunc_Hook.h"

extern D3D::PD3DPostProcess pD3DPostProcess;

extern "C" {
	void __fastcall ASMResetControllerAssign();
}

void __fastcall ASFunc_Hook_Initialize(PBYTE hmodEXE) {
	// edf5.exe+3E4E68
	hookGameBlockRAXWithInt3((void*)(hmodEXE + 0x3E4E68), (uintptr_t)ASMResetControllerAssign);
	WriteHookToProcess((void*)(hmodEXE + 0x3E4E68 + 15), (void*)&nop1, 1U);
}

void __fastcall ASFunc_Hook_ResetControllerAssign() {
	static int initialized = 0;
	if (initialized) return;

	initialized = 0x55;

	if (pD3DPostProcess) pD3DPostProcess->LoadLUTBuffer();
}
