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
uintptr_t playerViewRetAddr;
void __fastcall ASMplayerViewChange();

uintptr_t hookTextDisplayRetAddr;
void __fastcall ASMhookTextDisplay();

uintptr_t pickupBoxRangeFRetAddr;
uintptr_t pickupBoxRangeTRetAddr;
void __fastcall ASMpickupBoxRange();

// xgs_scene_object_class
void __fastcall ASMxgsOCgiantAnt();
void __fastcall ASMxgsOCmonster501();

uintptr_t weaponReloadEXRetAddr;
void __fastcall ASMweaponReloadEX();
uintptr_t weaponStartReloadRetAddr;
void __fastcall ASMweaponStartReload();
// Weapon_Gatling
uintptr_t wGatlingSetupRetAddr;
void __fastcall ASMweaponGatlingSetup();
uintptr_t wGatlingShotRetAddr;
void __fastcall ASMweaponGatlingShot();

/* For testing
uintptr_t wwwRetAddr;
void __fastcall ASMwww();
*/
}

void hookGameFunctions() {
	// first overwrite original
	OverwriteGameFunctions();
	// allows switching of views
	playerViewRetAddr = (uintptr_t)(hmodEXE + 0x2DB0D1);
	hookGameBlock14((void *)(hmodEXE + 0x2DB090), (uint64_t)ASMplayerViewChange);
	// get text address
	hookTextDisplayRetAddr = (uintptr_t)(hmodEXE + 0x4B15C9);
	hookGameBlock14((void *)(hmodEXE + 0x4B15B4), (uint64_t)ASMhookTextDisplay);
	// add guaranteed pickup, offset is 0x198350
	pickupBoxRangeFRetAddr = (uintptr_t)(hmodEXE + 0x198F5F);
	pickupBoxRangeTRetAddr = (uintptr_t)(hmodEXE + 0x198F64);
	hookGameBlock14((void *)(hmodEXE + 0x198F50), (uint64_t)ASMpickupBoxRange);

	// hook GiantAnt extra features
	hookGameBlock14((void *)(hmodEXE + 0x1FFD1B), (uint64_t)ASMxgsOCgiantAnt);
	// hook Monster501 extra features
	hookGameBlock14((void *)(hmodEXE + 0x263B64), (uint64_t)ASMxgsOCmonster501);

	// new weapon features
	// first, it need to reallocate memory
	ReallocateWeaponMemory();
	// set new readable sgo node name
	weaponReloadEXRetAddr = (uintptr_t)(hmodEXE + 0x38EF46);
	hookGameBlock14((void *)(hmodEXE + 0x38EEDD), (uint64_t)ASMweaponReloadEX);
	// allows midsection reload
	weaponStartReloadRetAddr = (uintptr_t)(hmodEXE + 0x3911DF);
	hookGameBlock14((void *)(hmodEXE + 0x3911CB), (uint64_t)ASMweaponStartReload);

	// gatling setup, offset is 0x39A0C5
	wGatlingSetupRetAddr = (uintptr_t)(hmodEXE + 0x39ACE0);
	hookGameBlock14((void *)(hmodEXE + 0x39ACC5), (uint64_t)ASMweaponGatlingSetup);
	// gatling shot, offset is 0x39A7AA
	wGatlingShotRetAddr = (uintptr_t)(hmodEXE + 0x39B3B8);
	hookGameBlock14((void *)(hmodEXE + 0x39B3AA), (uint64_t)ASMweaponGatlingShot);

	/* For testing
	wwwRetAddr = (uintptr_t)(hmodEXE + 0x);
	hookGameBlock((void *)(hmodEXE + 0x), (uint64_t)ASMwww);
	*/
}

// here is the part overwritten with hex
void OverwriteGameFunctions() {
	// increase pickup box limit to 4096, offset is 0x1984CB
	int maxBoxes = 0x1000;
	WriteHookToProcess((void *)(hmodEXE + 0x1990CC), &maxBoxes, 4U);
	WriteHookToProcess((void *)(hmodEXE + 0x1990EC), &maxBoxes, 4U);
	// removal Monster501 shot interval forced to 2, then change original 1 to 2
	unsigned char nop10[10] = {0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90};
	WriteHookToProcess((void *)(hmodEXE + 0x264286), &nop10, 10U);
	int m501shot = 2;
	WriteHookToProcess((void *)(hmodEXE + 0x263AF4 + 6), &m501shot, 4U);

	// normal explosion
	// time, offset is 0xEE4E74, default is 42.0f
	float explosionTime = 6.3f;
	WriteHookToProcess((void *)(hmodEXE + 0xEE6674), &explosionTime, 4U);
	// particle count, offset is 0x1AD7DE, default is 60
	int explosionNum = 90;
	WriteHookToProcess((void *)(hmodEXE + 0x1AE3DE + 2), &explosionNum, 4U);
	// smoke time, offset is 0x1ad3b7, default is 60 and 240
	int explosionST1 = 9;
	WriteHookToProcess((void *)(hmodEXE + 0x1ADFB8), &explosionST1, 4U);
	int explosionST2 = 36;
	WriteHookToProcess((void *)(hmodEXE + 0x1ADFC2), &explosionST2, 4U);
	// genocide explosion, base offset is 0x1ACA23
	// particle count, default is 80
	int GenExploNum = 40;
	WriteHookToProcess((void *)(hmodEXE + 0x1AD624), &GenExploNum, 4U);
	// lifetime large, default is 930 and 990
	int GenExploLargeT1 = 330;
	WriteHookToProcess((void *)(hmodEXE + 0x1AD71F + 3), &GenExploLargeT1, 4U);
	int GenExploLargeT2 = 390;
	WriteHookToProcess((void *)(hmodEXE + 0x1AD726 + 3), &GenExploLargeT2, 4U);
	// lifetime small, default is 330 and 390
	int GenExploSmallT1 = 80;
	WriteHookToProcess((void *)(hmodEXE + 0x1AD808 + 3), &GenExploSmallT1, 4U);
	int GenExploSmallT2 = 110;
	WriteHookToProcess((void *)(hmodEXE + 0x1AD80F + 3), &GenExploSmallT2, 4U);
	/*
	// particle speed, offset is 0x1ADF4E
	unsigned char GenExploByte[3] = {0xCA, 0x7A, 0xF9};
	// 1.8f to 18.0f
	WriteHookToProcess((void *)(hmodEXE + 0x1AEB4E + 4), &GenExploByte[0], 2U);
	// 3.2f to 32.0f
	WriteHookToProcess((void *)(hmodEXE + 0x1AEB5B + 4), &GenExploByte[2], 1U);
	*/
	unsigned char GenExploByte = 0xFD;
	// 0.005f to 0.07f
	WriteHookToProcess((void *)(hmodEXE + 0x1AEB3B + 4), &GenExploByte, 1U);
	// particle size, offset is 0xEE4E64, default is 35.0f
	float GenExploSize = 21.0f;
	WriteHookToProcess((void *)(hmodEXE + 0xEE6664), &GenExploSize, 4U);
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
	// Weapon_Gatling 0x1290
	WriteHookToProcess((void *)(hmodEXE + 0x46A578), &newWeaponSize, 4U);
	// start:0x1400, size:0x20, function: set pre-heat type.

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
