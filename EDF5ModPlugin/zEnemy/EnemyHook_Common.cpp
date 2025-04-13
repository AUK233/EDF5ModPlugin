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
#include "EnemyHook_Common.h"

extern "C" {
	void __fastcall ASMxgsOCGeneratorPollDestroyRagdollInit();
}

void module_EnemyHook_Common(PBYTE hmodEXE)
{
	// insectbase no knockout if insectbase_Type != 0
	// EDF5.exe+261DC5
	unsigned char InsectbaseKnockout[] = {
		0x83, 0xBB, 0x18, 0x04, 0x00, 0x00, 0x00,
		0x75, 0x0A
	};
	WriteHookToProcess((void*)(hmodEXE + 0x261DC5), &InsectbaseKnockout, 9U);


	// EDF5.exe+1FBB44
	hookGameBlockWithInt3((void*)(hmodEXE + 0x1FBB44), (uintptr_t)ASMxgsOCGeneratorPollDestroyRagdollInit);
	WriteHookToProcess((void*)(hmodEXE + 0x1FBB44 + 15), (void*)&nop2, 2U);
	// EDF5.exe+1FBC92, death fade out time
}
