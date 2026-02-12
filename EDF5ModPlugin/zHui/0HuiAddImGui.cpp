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
#include "ToGui/0GetDXGI.h"
#include "ToGui/0SetImGui.h"
#include "0HuiAddImGui.h"

extern "C" {
	void __fastcall ASMGetDXGISwapChain();
	uintptr_t GetDXGISwapChainRetAddr;
}

void module_InitializeAddImGui(PBYTE hmodEXE)
{
	DXGI_Initialize(hmodEXE);

	// EDF5.exe+5E1BB9
	hookGameBlockWithInt3((void*)(hmodEXE + 0x5E1BB9), (uintptr_t)ASMGetDXGISwapChain);
	WriteHookToProcess((void*)(hmodEXE + 0x5E1BB9 + 15), (void*)&nop1, 1U);
	GetDXGISwapChainRetAddr = (uintptr_t)(hmodEXE + 0x5E1BCE);

	//MessageBoxW(NULL, L"test", L"debug", MB_OK);
}
