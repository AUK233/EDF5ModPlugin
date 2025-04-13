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
#include "EnemyHook_BigNest.h"

extern "C" {
	void __fastcall ASMxgsOCBigNestInitialization();

	void __fastcall ASMBigNestDamageAction();
	uintptr_t BigNestDeathAnimationPlayAddr;
	uintptr_t rvaE8E3F8;
}

void module_EnemyHook_BigNest(PBYTE hmodEXE)
{
	// EDF5.exe+46A2F7, old is 0xCE0
	// +0CE0h (4-Bytes), death fade time (float)
	// +0CE8h (8-Bytes), sgo string pointer
	int newBigNestSize = 0xCF0;
	WriteHookToProcessCheckECX((void*)(hmodEXE + 0x46A2F7 + 1), &newBigNestSize, 4U);

	// EDF5.exe+29A407
	hookGameBlockWithInt3((void*)(hmodEXE + 0x29A407), (uintptr_t)ASMxgsOCBigNestInitialization);
	WriteHookToProcess((void*)(hmodEXE + 0x29A407 + 15), (void*)&nop4, 4U);
	BigNestDeathAnimationPlayAddr = (uintptr_t)(hmodEXE + 0x29C5B0);
	rvaE8E3F8 = (uintptr_t)(hmodEXE + 0xE8E3F8);

	// EDF5.exe+29B9D8
	hookGameBlockWithInt3((void*)(hmodEXE + 0x29B9D8), (uintptr_t)ASMBigNestDamageAction);
	WriteHookToProcess((void*)(hmodEXE + 0x29B9D8 + 15), (void*)&nop1, 1U);

	// EDF5.exe+29BBE2
	// comiss xmm0, dword ptr [rbx+CE0]
	unsigned char DeathFadeTime[] = {
		0x0F, 0x2F, 0x83, 0xE0, 0x0C, 0x00, 0x00
	};
	WriteHookToProcess((void*)(hmodEXE + 0x29BBE2), &DeathFadeTime, 7U);
}
