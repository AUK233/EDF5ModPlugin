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
uintptr_t edf5BDF30Address;

void __fastcall ASMxgsOCgiantAnt();

uintptr_t playerViewRetAddr;
void __fastcall ASMplayerViewChange();
}

void hookGameFunctions() {
	// get read sgo node function
	edf5BDF30Address = (uintptr_t)(hmodEXE + 0x5BDF30);

	// hook GiantAnt extra features
	hookGameBlock((void *)(hmodEXE + 0x1FFD1B), (uint64_t)ASMxgsOCgiantAnt);

	//
	playerViewRetAddr = (uintptr_t)(hmodEXE + 0x2DB0D1);
	hookGameBlock((void *)(hmodEXE + 0x2DB090), (uint64_t)ASMplayerViewChange);
}