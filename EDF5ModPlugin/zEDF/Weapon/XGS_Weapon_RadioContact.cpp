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

#include "CommonCustomStructure.hpp"
#include "utiliy.h"
#include "commonNOP.h"
#include "XGS_Weapon_RadioContact.h"

extern "C" {
	extern PCustomMissionData pMissionCustomData;
	void __fastcall ASMxgs_Weapon_RadioContactInit();
}

void xgs_Weapon_RadioContact_Init(PBYTE hmodEXE)
{
	// EDF5.exe+3A490C
	hookGameBlockWithInt3((void*)(hmodEXE + 0x3A490C), (uintptr_t)ASMxgs_Weapon_RadioContactInit);
	WriteHookToProcess((void*)(hmodEXE + 0x3A490C + 15), (void*)&nop4, 4U);
}
