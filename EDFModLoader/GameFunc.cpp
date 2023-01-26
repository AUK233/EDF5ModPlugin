#include <windows.h>
#include <psapi.h>
#include <iostream>
#include <iomanip>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <format>
#include <stdexcept>
#include <list>
#include "utiliy.h"

#include "GameFunc.h"

extern PBYTE hmodEXE;

extern "C" {
uintptr_t edf4738B0Address;
uintptr_t edf4DC190Address;
uintptr_t edf5BDF30Address;

void __fastcall ASMxgsOCgiantAnt();

uintptr_t playerViewRetAddr;
void __fastcall ASMplayerViewChange();

uintptr_t wwwRetAddr1;
uintptr_t wwwRetAddr2;
uintptr_t wwwwww;
void __fastcall ASMwww();
}

void hookGameFunctions() {
	edf4738B0Address = (uintptr_t)(hmodEXE + 0x4738B0);
	edf4DC190Address = (uintptr_t)(hmodEXE + 0x4DC190);
	// get read sgo node function
	edf5BDF30Address = (uintptr_t)(hmodEXE + 0x5BDF30);

	// hook GiantAnt extra features
	hookGameBlock((void *)(hmodEXE + 0x1FFD1B), (uint64_t)ASMxgsOCgiantAnt);

	// allows switching of views
	playerViewRetAddr = (uintptr_t)(hmodEXE + 0x2DB0D1);
	hookGameBlock((void *)(hmodEXE + 0x2DB090), (uint64_t)ASMplayerViewChange);

	
	wwwRetAddr1 = (uintptr_t)(hmodEXE + 0x4DC505);
	wwwRetAddr2 = wwwRetAddr1 + 2;
	wwwwww = (uintptr_t)(hmodEXE + 0x125ABD0);
	//hookGameBlock((void *)(hmodEXE + 0x4DC4D7), (uint64_t)ASMwww);

}