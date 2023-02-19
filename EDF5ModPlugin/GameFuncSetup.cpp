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
uintptr_t __RTDynamicCastAddr;

uintptr_t playerViewRetAddr;
void __fastcall ASMplayerViewChange();

uintptr_t hookTextDisplayRetAddr;
void __fastcall ASMhookTextDisplay();

uintptr_t pickupBoxRangeFRetAddr;
uintptr_t pickupBoxRangeTRetAddr;
void __fastcall ASMpickupBoxRange();

// xgs_scene_object_class
void __fastcall ASMxgsOCgiantAnt();
void __fastcall ASMxgsOCgiantSpider();
void __fastcall ASMxgsOCmonster501();

/* For testing
uintptr_t wwwRetAddr;
void __fastcall ASMwww();
*/
}

void hookGameFunctions() {
	// first overwrite original
	OverwriteGameFunctions();
	__RTDynamicCastAddr = (uintptr_t)(hmodEXE + 0x9C8228);
	// allows switching of views
	playerViewRetAddr = (uintptr_t)(hmodEXE + 0x2DB0D1);
	hookGameBlock((void *)(hmodEXE + 0x2DB090), (uint64_t)ASMplayerViewChange);
	// get text address
	hookTextDisplayRetAddr = (uintptr_t)(hmodEXE + 0x4B15C9);
	hookGameBlock((void *)(hmodEXE + 0x4B15B4), (uint64_t)ASMhookTextDisplay);
	// add guaranteed pickup, offset is 0x198350
	pickupBoxRangeFRetAddr = (uintptr_t)(hmodEXE + 0x198F5F);
	pickupBoxRangeTRetAddr = (uintptr_t)(hmodEXE + 0x198F64);
	hookGameBlock((void *)(hmodEXE + 0x198F50), (uint64_t)ASMpickupBoxRange);

	// hook GiantAnt extra features
	hookGameBlock((void *)(hmodEXE + 0x1FFD1B), (uint64_t)ASMxgsOCgiantAnt);
	// hook GiantSpider extra features, offset is 0x21E48A
	hookGameBlock((void *)(hmodEXE + 0x21F08A), (uint64_t)ASMxgsOCgiantSpider);
	// hook Monster501 extra features
	hookGameBlock((void *)(hmodEXE + 0x263B64), (uint64_t)ASMxgsOCmonster501);
	
	// By Features
	hookHeavyArmorFunctions();
	hookWeaponFunctions();

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

	// removal GiantSpider change ammo color, offset is 0x21F8FB
	unsigned char noSpiderColorChange[] = {0xEB, 0x4F};
	WriteHookToProcess((void *)(hmodEXE + 0x2204FB), &noSpiderColorChange, 2U);
	// removal Monster501 shot interval forced to 2, then change original 1 to 2
	unsigned char nop10[] = {0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90};
	WriteHookToProcess((void *)(hmodEXE + 0x264286), &nop10, 10U);
	int m501shot = 2;
	WriteHookToProcess((void *)(hmodEXE + 0x263AF4 + 6), &m501shot, 4U);

	// normal explosion
	// time, offset is 0xEE4E74, default is 42.0f
	float explosionTime = 8.4f;
	WriteHookToProcess((void *)(hmodEXE + 0xEE6674), &explosionTime, 4U);
	// particle count, offset is 0x1AD7DE, default is 60
	int explosionNum = 90;
	WriteHookToProcess((void *)(hmodEXE + 0x1AE3DE + 2), &explosionNum, 4U);
	// smoke time, offset is 0x1ad3b7, default is 60 and 240
	int explosionST1 = 10;
	WriteHookToProcess((void *)(hmodEXE + 0x1ADFB8), &explosionST1, 4U);
	int explosionST2 = 40;
	WriteHookToProcess((void *)(hmodEXE + 0x1ADFC2), &explosionST2, 4U);
	// genocide explosion, base offset is 0x1ACA23
	// particle count, default is 80
	int GenExploNum = 100;
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

extern "C" {
// Swap boost and dash
uintptr_t edf11B24E0Address;
uintptr_t edf11B1AB0Address;
void __fastcall ASMeFencerJetSetup();
uintptr_t ofs3073C0JmpAddr;
uintptr_t ofs2E4070JmpAddr;
uintptr_t ofs2E42C0JmpAddr;
uintptr_t ofs2E43E0JmpAddr;
uintptr_t ofs2E4500JmpAddr;
void __fastcall ASMeFencerBoostAndDash();
}

void hookHeavyArmorFunctions() {
	int newFencerSize = 0x1E10;
	// start:0x1E00, size:0x10, function: extra reload types.
	// HeavyArmor 0x1C30
	WriteHookToProcess((void *)(hmodEXE + 0x2E3408), &newFencerSize, 4U);
	
	// +1BA0h, default is 240
	int newBoosterCD = 300;
	WriteHookToProcess((void *)(hmodEXE + 0x2E4D38 + 6), &newBoosterCD, 4U);
	// +1BB0h, default is 90
	int newDashCD = 120;
	WriteHookToProcess((void *)(hmodEXE + 0x2E54E4 + 6), &newDashCD, 4U);
	// 0x2E387B 0.5f to 1.25f
	unsigned char newDashInterval[] = {0xED, 0x1F};
	WriteHookToProcess((void *)(hmodEXE + 0x2E447B + 4), &newDashInterval, 2U);

	// Swap boost and dash Installation
	edf11B24E0Address = (uintptr_t)(hmodEXE + 0x11B24E0);
	edf11B1AB0Address = (uintptr_t)(hmodEXE + 0x11B1AB0);
	// offset is 0x2E3926
	hookGameBlock((void *)(hmodEXE + 0x2E4526), (uint64_t)ASMeFencerJetSetup);
	// Swap boost and dash Activate
	ofs3073C0JmpAddr = (uintptr_t)(hmodEXE + 0x307FC0);
	ofs2E4070JmpAddr = (uintptr_t)(hmodEXE + 0x2E4C70);
	ofs2E42C0JmpAddr = (uintptr_t)(hmodEXE + 0x2E4EC0);
	ofs2E43E0JmpAddr = (uintptr_t)(hmodEXE + 0x2E4FE0);
	ofs2E4500JmpAddr = (uintptr_t)(hmodEXE + 0x2E5100);
	hookGameBlock((void *)(hmodEXE + 0x2E4890), (uint64_t)ASMeFencerBoostAndDash);
}


extern "C" {
uintptr_t weaponReloadEXRetAddr;
void __fastcall ASMweaponReloadEX();
uintptr_t weaponStartReloadRetAddr;
void __fastcall ASMweaponStartReload();
// Weapon_Gatling
uintptr_t wGatlingSetupRetAddr;
void __fastcall ASMweaponGatlingSetup();
uintptr_t wGatlingShotRetAddr;
void __fastcall ASMweaponGatlingShot();
}

void hookWeaponFunctions() {
	// new weapon features
	// first, it need to reallocate memory
	ReallocateWeaponMemory();
	// set new readable sgo node name
	weaponReloadEXRetAddr = (uintptr_t)(hmodEXE + 0x38EF46);
	hookGameBlock((void *)(hmodEXE + 0x38EEDD), (uint64_t)ASMweaponReloadEX);
	// allows midsection reload
	weaponStartReloadRetAddr = (uintptr_t)(hmodEXE + 0x3911DF);
	hookGameBlock((void *)(hmodEXE + 0x3911CB), (uint64_t)ASMweaponStartReload);

	// gatling setup, offset is 0x39A0C5
	wGatlingSetupRetAddr = (uintptr_t)(hmodEXE + 0x39ACE0);
	hookGameBlock((void *)(hmodEXE + 0x39ACC5), (uint64_t)ASMweaponGatlingSetup);
	// gatling shot, offset is 0x39A7AA
	wGatlingShotRetAddr = (uintptr_t)(hmodEXE + 0x39B3B8);
	hookGameBlock((void *)(hmodEXE + 0x39B3AA), (uint64_t)ASMweaponGatlingShot);
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
