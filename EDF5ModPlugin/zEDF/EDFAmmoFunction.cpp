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
#include "EDFAmmoFunction.h"


void module_SetFunction_EDFAmmo(PBYTE hmodEXE)
{
	module_EDFAmmo_SmokeCandleBullet(hmodEXE);
}

extern "C" {
	extern PCustomMissionData pMissionCustomData;
	void __fastcall ASMammoSmokeCandleBulletInit();
	uintptr_t ammoSmokeCandleBulletInitRetAddr;

	void __fastcall ASMammoSmokeCandleBullet01ofs182060();
	uintptr_t ammoSmokeCandleBullet01justCallVeh;
	uintptr_t ammoSmokeCandleBullet01ofs182168;
	uintptr_t rva181ED0;
}

void module_EDFAmmo_SmokeCandleBullet(PBYTE hmodEXE)
{
	// Common
	// EDF5.exe+180F6D
	hookGameBlockWithInt3((void*)(hmodEXE + 0x180F6D), (uintptr_t)ASMammoSmokeCandleBulletInit);
	WriteHookToProcess((void*)(hmodEXE + 0x180F6D + 15), (void*)&nop10, 10U);
	ammoSmokeCandleBulletInitRetAddr = (uintptr_t)(hmodEXE + 0x180F86);

	// hook SmokeCandleBullet01
	// EDF5.exe+182CBE, allowed to directly deploy vehicles
	hookGameBlockWithInt3((void*)(hmodEXE + 0x182CBE), (uintptr_t)ASMammoSmokeCandleBullet01ofs182060);
	WriteHookToProcess((void*)(hmodEXE + 0x182CBE + 15), (void*)&nop7, 7U);
	ammoSmokeCandleBullet01justCallVeh = (uintptr_t)(hmodEXE + 0x182CE6);
	ammoSmokeCandleBullet01ofs182168 = (uintptr_t)(hmodEXE + 0x182D68);
	rva181ED0 = (uintptr_t)(hmodEXE + 0x181ED0);
}

float __fastcall module_SmokeCandleBullet_SetDelaySecond(float frame)
{
	float value = frame / 60.0f;
	value *= pMissionCustomData->AirRaider_DelayX;
	return value;
}
