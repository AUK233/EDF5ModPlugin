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
#include <HookLib.h>
#include "utiliy.h"

#include "GameFunc.h"
//#include "CommonData.h"

extern PBYTE hmodEXE;

static const unsigned char intNOP32[] = {0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90};

extern "C" {
// star rating calculation
uintptr_t edf8C8C0Address;
//
uintptr_t edf3AE530Address;

uintptr_t edf4738B0Address;
uintptr_t edf4DC190Address;
// read sgo node name
uintptr_t edf5BDF30Address;
// get AmmoClass address
uintptr_t edf6136C0Address;
// get Weapon_Accessory data (int)
uintptr_t edf3983B0Address;
// get Weapon_Accessory data (float)
uintptr_t edf3982A0Address;
//
uintptr_t vedf125AB68;
}

// get game function address
void GetGameFunctions() {
	// get star rating calculation function
	edf8C8C0Address = (uintptr_t)(hmodEXE + 0x8C8C0);

	edf3AE530Address = (uintptr_t)(hmodEXE + 0x3AE530);

	edf4738B0Address = (uintptr_t)(hmodEXE + 0x4738B0);
	edf4DC190Address = (uintptr_t)(hmodEXE + 0x4DC190);
	// get read sgo node function
	edf5BDF30Address = (uintptr_t)(hmodEXE + 0x5BDF30);
	// get get AmmoClass address function
	edf6136C0Address = (uintptr_t)(hmodEXE + 0x6136C0);
	// get Weapon_Accessory data (int)
	edf3983B0Address = (uintptr_t)(hmodEXE + 0x3983B0);
	// get Weapon_Accessory data (float)
	edf3982A0Address = (uintptr_t)(hmodEXE + 0x3982A0);

	//
	vedf125AB68 = (uintptr_t)(hmodEXE + 0x125AB68);
	// get ammo function address
	GetAmmoFunctions();
}

extern "C" {
// AcidBullet01
uintptr_t edf136890Address;
// FlameBullet01
uintptr_t edf14DE20Address;
// PlasmaBullet01
uintptr_t edf16DBB0Address;
// SolidBullet01
uintptr_t edf185510Address;
// SpiderStringBullet01
uintptr_t edf18AE10Address;
// SpiderStringBullet02
uintptr_t edf18DF30Address;
// PulseBullet01
uintptr_t edf171140Address;
// LaserBullet02
uintptr_t edf156FF0Address;
}
// get ammo function address
void GetAmmoFunctions() {
	edf136890Address = (uintptr_t)(hmodEXE + 0x136890);
	edf14DE20Address = (uintptr_t)(hmodEXE + 0x14DE20);
	edf16DBB0Address = (uintptr_t)(hmodEXE + 0x16DBB0);
	edf185510Address = (uintptr_t)(hmodEXE + 0x185510);
	edf18AE10Address = (uintptr_t)(hmodEXE + 0x18AE10);
	edf18DF30Address = (uintptr_t)(hmodEXE + 0x18DF30);
	edf171140Address = (uintptr_t)(hmodEXE + 0x171140);
	edf156FF0Address = (uintptr_t)(hmodEXE + 0x156FF0);
}

// here hook all changed functions, written in c++
void hookGameFunctionsC() {
	// allows weapons to be charged, offset is 0x390630
	//SetupHook(0x391230, (PVOID *)&fnk391230_orig, fnk391230_hook, "Allows weapons to be charged", 1);
	hookGameBlock((void *)(hmodEXE + 0x391230), (uint64_t)fnk391230_hook);
	WriteHookToProcess((void *)(hmodEXE + 0x391230 + 12), (void *)&intNOP32, 8U);
	//hookGameBlock14((void *)(hmodEXE + 0x391230), (uint64_t)fnk391230_hook);
}

static bool __fastcall fnk391230_hook(uintptr_t pweapon) {
	// ammo
	int *curAmmo = (int *)(pweapon + 0x8E8);
	int maxAmmo = *(int *)(pweapon + 0x1D0);
	int ROFCount = *(int *)(pweapon + 0xB40);
	// reloading phase, 0 means no
	// int rePhase = *(int *)(a1 + 0x8D4);
	// new function
	int chargeType = *(int *)(pweapon + 0x2500);
	int chargeTime = *(int *)(pweapon + 0x2508);
	int *chargeRT = (int *)(pweapon + 0x250C);
	
	if (chargeType == 2 && chargeTime > 0) {
		if (*curAmmo > 0 && *curAmmo < maxAmmo && !ROFCount) {
			if (*chargeRT <= 0) {
				*curAmmo += 1;
				*chargeRT = chargeTime;
			} else {
				*chargeRT += -1;
			}
		} else {
			*chargeRT = chargeTime;
		}
	}
	
	// Original function block
	// don't modify them, it will cause crashes when reloading
	int v2;
	int v1 = *curAmmo;
	if (v1 > 0 || *(INT64 *)(pweapon + 3128) || (!*(INT64 *)(pweapon + 3144) ? (v2 = *(int *)(pweapon + 2960)) : (v2 = *(int *)(pweapon + 420)), !v2)) {
		if (*(BYTE *)(pweapon + 224))
			return true;
	}
	if (v1 > 0 || *(INT64 *)(pweapon + 3128))
		return false;
	int v4 = *(INT64 *)(pweapon + 3144) ? *(int *)(pweapon + 420) : *(int *)(pweapon + 2960);
	return v4 && !v1 && !*(int *)(pweapon + 0xB40) && *(float *)(pweapon + 452) <= 0.0f && *(int *)(pweapon + 420) >= 0;
}
