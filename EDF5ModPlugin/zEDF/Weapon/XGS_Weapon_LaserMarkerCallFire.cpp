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
#include "XGS_Weapon_LaserMarkerCallFire.h"

extern "C" {
	extern PCustomMissionData pMissionCustomData;
	void __fastcall ASMxgs_Weapon_LaserMarkerCallFireInit();
}

void xgs_Weapon_LaserMarkerCallFire_Init(PBYTE hmodEXE)
{
	// EDF5.exe+3A0F68
	hookGameBlockWithInt3((void*)(hmodEXE + 0x3A0F68), (uintptr_t)ASMxgs_Weapon_LaserMarkerCallFireInit);
	WriteHookToProcess((void*)(hmodEXE + 0x3A0F68 + 15), (void*)&nop3, 3U);
}
