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

#include "GameFuncSetup.h"

extern PBYTE hmodEXE;

extern "C" {
// star rating calculation
uintptr_t edf8C8C0Address;

uintptr_t edf4738B0Address;
uintptr_t edf4DC190Address;
// read sgo node name
uintptr_t edf5BDF30Address;


uintptr_t playerViewRetAddr;
void __fastcall ASMplayerViewChange();

void __fastcall ASMxgsOCgiantAnt();

uintptr_t weaponReloadEXRetAddr;
void __fastcall ASMweaponReloadEX();
uintptr_t weaponStartReloadRetAddr;
void __fastcall ASMweaponStartReload();
}

void hookGameFunctions() {
	// get star rating calculation function
	edf8C8C0Address = (uintptr_t)(hmodEXE + 0x8C8C0);

	edf4738B0Address = (uintptr_t)(hmodEXE + 0x4738B0);
	edf4DC190Address = (uintptr_t)(hmodEXE + 0x4DC190);
	// get read sgo node function
	edf5BDF30Address = (uintptr_t)(hmodEXE + 0x5BDF30);

	// allows switching of views
	playerViewRetAddr = (uintptr_t)(hmodEXE + 0x2DB0D1);
	hookGameBlock((void *)(hmodEXE + 0x2DB090), (uint64_t)ASMplayerViewChange);

	// hook GiantAnt extra features
	hookGameBlock((void *)(hmodEXE + 0x1FFD1B), (uint64_t)ASMxgsOCgiantAnt);

	// first, it need to reallocate memory
	ReallocateWeaponMemory();
	// set new readable sgo node name
	weaponReloadEXRetAddr = (uintptr_t)(hmodEXE + 0x38EF46);
	hookGameBlock((void *)(hmodEXE + 0x38EEDD), (uint64_t)ASMweaponReloadEX);
	// allows midsection reload
	weaponStartReloadRetAddr = (uintptr_t)(hmodEXE + 0x3911DF);
	hookGameBlock((void *)(hmodEXE + 0x3911CB), (uint64_t)ASMweaponStartReload);

}

// new functions require more memory
void ReallocateWeaponMemory() {
	// Allocate more memory to all weapons
	// New size must be larger than largest of them all
	// Safe size at least 0x2500
	// Must be an integer multiple of 0x10
	int newWeaponSize = 0x2520;
	// start:0x2500, size:0x20, function: extra reload types.

	// Weapon_Accessory 0x11E0
	WriteHookToProcess((void *)(hmodEXE + 0x398018), &newWeaponSize, 4U);
	// Weapon_BasicShoot 0x1200
	WriteHookToProcess((void *)(hmodEXE + 0x398588), &newWeaponSize, 4U);
	// Weapon_BasicSemiAuto 0x1200
	WriteHookToProcess((void *)(hmodEXE + 0x39863D), &newWeaponSize, 4U);
	// Weapon_ChargeShoot 0x14D0
	WriteHookToProcess((void *)(hmodEXE + 0x3997F8), &newWeaponSize, 4U);
	// Weapon_PreChargeShoot 0x1310
	WriteHookToProcess((void *)(hmodEXE + 0x3A3E48), &newWeaponSize, 4U);
	// Weapon_RadioContact 0x1500
	WriteHookToProcess((void *)(hmodEXE + 0x3A4368), &newWeaponSize, 4U);
	// Weapon_Shield 0x1230
	WriteHookToProcess((void *)(hmodEXE + 0x3A6858), &newWeaponSize, 4U);
	// Weapon_VehicleMaser 0x1FF0
	WriteHookToProcess((void *)(hmodEXE + 0x3A9438), &newWeaponSize, 4U);
	// Weapon_HomingShoot 0x11F0
	WriteHookToProcess((void *)(hmodEXE + 0x46A398), &newWeaponSize, 4U);
	// Weapon_Throw 0x1200
	WriteHookToProcess((void *)(hmodEXE + 0x46A3E8), &newWeaponSize, 4U);
	// Weapon_Swing 0x1210
	WriteHookToProcess((void *)(hmodEXE + 0x46A438), &newWeaponSize, 4U);
	// Weapon_PileBanker 0x11F0
	WriteHookToProcess((void *)(hmodEXE + 0x46A488), &newWeaponSize, 4U);
	// Weapon_ImpactHammer 0x11F0
	WriteHookToProcess((void *)(hmodEXE + 0x46A4D8), &newWeaponSize, 4U);
	// Weapon_HeavyShoot 0x1260
	WriteHookToProcess((void *)(hmodEXE + 0x46A528), &newWeaponSize, 4U);
	// Weapon_Gatling 0x1260
	WriteHookToProcess((void *)(hmodEXE + 0x46A578), &newWeaponSize, 4U);
	// Weapon_LaserMarker 0x1460
	WriteHookToProcess((void *)(hmodEXE + 0x46A5C8), &newWeaponSize, 4U);
	// Weapon_LaserMarkerCallFire 0x1AB0
	WriteHookToProcess((void *)(hmodEXE + 0x46A618), &newWeaponSize, 4U);
	// Weapon_MarkerShooter 0x11F0
	WriteHookToProcess((void *)(hmodEXE + 0x46A66D), &newWeaponSize, 4U);
	// Weapon_VehicleShoot 0x11F0
	WriteHookToProcess((void *)(hmodEXE + 0x469AD8), &newWeaponSize, 4U);
	// Weapon_VehicleRailGun 0x11F0
	WriteHookToProcess((void *)(hmodEXE + 0x46A72D), &newWeaponSize, 4U);
}
