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
#include <malloc.h>
#include "utiliy.h"

#include "GameFuncSetup.h"
#include <commonNOP.h>

extern PBYTE hmodEXE;
extern int weaponEnhance;

static const unsigned char intNOP32[] = {0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90};
static const unsigned char Interruptions32[] = {0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC};

extern "C" {
uintptr_t __RTDynamicCastAddr;
uintptr_t aligned_mallocAddr;

uintptr_t playerViewRetAddr;
void __fastcall ASMplayerViewChange();

uintptr_t pickupBoxRangeFRetAddr;
uintptr_t pickupBoxRangeTRetAddr;
void __fastcall ASMpickupBoxRange();

/* For testing
uintptr_t wwwRetAddr;
void __fastcall ASMwww();
*/
}

void hookGameFunctions() {
	// first overwrite original
	OverwriteGameFunctions();
	__RTDynamicCastAddr = (uintptr_t)(hmodEXE + 0x9C8228);
	aligned_mallocAddr = (uintptr_t)_aligned_malloc;
	// allows switching of views, offset is 0x2DA490
	playerViewRetAddr = (uintptr_t)(hmodEXE + 0x2DB0D1);
	hookGameBlock((void *)(hmodEXE + 0x2DB090), (uintptr_t)ASMplayerViewChange);
	// add guaranteed pickup, offset is 0x198350
	pickupBoxRangeFRetAddr = (uintptr_t)(hmodEXE + 0x198F5F);
	pickupBoxRangeTRetAddr = (uintptr_t)(hmodEXE + 0x198F64);
	hookGameBlock((void *)(hmodEXE + 0x198F50), (uintptr_t)ASMpickupBoxRange);
	WriteHookToProcess((void *)(hmodEXE + 0x198F50 + 12), (void *)&intNOP32, 1U);

	// By Features
	hookMonsterFunctions();
	hookEDFClassFunctions();
	hookAmmoFunctions();

	if (weaponEnhance) {
	hookWeaponFunctions();
	}

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

	// forced private room creation
	// everyone, offset is 0x57F88F
	unsigned char everyone1[] = {0x82, 0x94};
	WriteHookToProcess((void *)(hmodEXE + 0x58048F + 3), &everyone1, 2U);
	unsigned char everyone2[] = {0x3C, 0x78};
	WriteHookToProcess((void *)(hmodEXE + 0x5820D5 + 3), &everyone2, 2U);
	// need password, offset is 0x57FAD0
	unsigned char password1[] = {0x41};
	WriteHookToProcess((void *)(hmodEXE + 0x5806D0 + 3), &password1, 1U);
	unsigned char password2[] = {0xDC, 0x78};
	WriteHookToProcess((void *)(hmodEXE + 0x582035 + 3), &password2, 2U);

	// minimum building destruction blast radius
	// offset is 0x141AA5
	// 3.0f to 5.0f
	unsigned char minBlastBuilding = 0xFC;
	WriteHookToProcess((void *)(hmodEXE + 0x1426A5 + 3), &minBlastBuilding, 1U);

	// normal hit smoke
	// offset is 0x1B0C23
	// 30.0f to 15.0f
	unsigned char normalHitSmokeF[] = {0xE9, 0x4D};
	WriteHookToProcess((void *)(hmodEXE + 0x1B1823 + 4), &normalHitSmokeF, 2U);
	// 60 to 20
	unsigned char normalHitSmokeI = 20;
	WriteHookToProcess((void *)(hmodEXE + 0x1B1836 + 2), &normalHitSmokeI, 1U);

	// normal explosion
	// time, offset is 0xEE4E74, default is 42.0f
	float explosionTime = 14.0f;
	WriteHookToProcess((void *)(hmodEXE + 0xEE6674), &explosionTime, 4U);
	// particle count, offset is 0x1AD7DE, default is 60
	int explosionNum = 80;
	WriteHookToProcess((void *)(hmodEXE + 0x1AE3DE + 2), &explosionNum, 4U);
	// smoke time, offset is 0x1ad3b7, default is 60 and 240
	int explosionST1 = 15;
	WriteHookToProcess((void *)(hmodEXE + 0x1ADFB8), &explosionST1, 4U);
	int explosionST2 = 60;
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
// xgs_scene_object_class
void __fastcall ASMxgsOCgiantAnt();
void __fastcall ASMxgsOCgiantSpider();
// giant bee
void __fastcall ASMxgsOCgiantBee();
uintptr_t giantBeeAmmoNextAddr;
uintptr_t giantBeeAmmoRetAddr;
void __fastcall ASMxgsOCgiantBeeAmmo();
// dragon small
void __fastcall ASMxgsOCdragonSmall();
uintptr_t dragonSmallAmmoNextAddr;
uintptr_t dragonSmallAmmoRetAddr;
uintptr_t dragonSmallAmmoSetNextAddr;
uintptr_t dragonSmallAmmoSetRetAddr;
void __fastcall ASMxgsOCdragonSmallAmmo();
// monster501
void __fastcall ASMxgsOCmonster501();
}

void hookMonsterFunctions() {
	// hook GiantAnt extra features, offset is 0x1FF113
	hookGameBlock((void *)(hmodEXE + 0x1FFD13), (uintptr_t)ASMxgsOCgiantAnt);
	WriteHookToProcess((void *)(hmodEXE + 0x1FFD13 + 12), (void *)&Interruptions32, 6U);

	// hook GiantSpider extra features, offset is 0x21E48A
	hookGameBlock((void *)(hmodEXE + 0x21F08A), (uintptr_t)ASMxgsOCgiantSpider);
	WriteHookToProcess((void *)(hmodEXE + 0x21F08A + 12), (void *)&Interruptions32, 10U);
	// removal GiantSpider change ammo color, offset is 0x21F8FB
	unsigned char noSpiderColorChange[] = {0xEB, 0x4F};
	WriteHookToProcess((void *)(hmodEXE + 0x2204FB), &noSpiderColorChange, 2U);

	// hook GiantBee extra features, offset is 0x20A0B0
	hookGameBlock((void *)(hmodEXE + 0x20ACB0), (uintptr_t)ASMxgsOCgiantBee);
	WriteHookToProcess((void *)(hmodEXE + 0x20ACB0 + 12), (void *)&Interruptions32, 10U);
	// hook GiantBee Ammo, offset is 0x20A6D3
	giantBeeAmmoNextAddr = (uintptr_t)(hmodEXE + 0x20B2E0);
	giantBeeAmmoRetAddr = (uintptr_t)(hmodEXE + 0x20B2F9);
	hookGameBlock((void *)(hmodEXE + 0x20B2D3), (uintptr_t)ASMxgsOCgiantBeeAmmo);
	WriteHookToProcess((void *)(hmodEXE + 0x20B2D3 + 12), (void *)&intNOP32, 1U);
	// allows adjustment of shot accuracy, offset is 0x20A642
	unsigned char beeAccuracy[] = {
		0xF3, 0x45, 0x0F, 0x10, 0x8E, 0xA4, 0x12, 0x00, 0x00, 
		0x0F, 0x1F, 0x44, 0x00, 0x00, 0xC7, 0x44
	};
	WriteHookToProcess((void *)(hmodEXE + 0x20B242), &beeAccuracy, 16U);

	// hook DragonSmall extra features, offset is 0x1EC496
	hookGameBlock((void *)(hmodEXE + 0x1ED096), (uintptr_t)ASMxgsOCdragonSmall);
	WriteHookToProcess((void *)(hmodEXE + 0x1ED096 + 12), (void *)&Interruptions32, 10U);
	// hook DragonSmall Ammo Set, offset is 0x1F6C6C
	dragonSmallAmmoSetNextAddr = (uintptr_t)(hmodEXE + 0x1F783C);
	dragonSmallAmmoSetRetAddr = (uintptr_t)(hmodEXE + 0x1F786C);
	// hook DragonSmall Ammo, offset is 0x1EC89C
	dragonSmallAmmoNextAddr = (uintptr_t)(hmodEXE + 0x1ED4C6);
	dragonSmallAmmoRetAddr = (uintptr_t)(hmodEXE + 0x1ED4DF);
	//hookGameBlock((void *)(hmodEXE + 0x1ED49C), (uintptr_t)ASMxgsOCdragonSmallAmmo);
	//WriteHookToProcess((void *)(hmodEXE + 0x1ED49C + 12), (void *)&intNOP32, 30U);

	// hook Monster501 extra features, offset is 0x262F64
	hookGameBlock((void *)(hmodEXE + 0x263B64), (uintptr_t)ASMxgsOCmonster501);
	WriteHookToProcess((void *)(hmodEXE + 0x263B64 + 12), (void *)&Interruptions32, 4U);
	// removal Monster501 shot interval forced to 2, then change original 1 to 2
	//unsigned char nop10[] = {0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90};
	unsigned char nop10[] = {0x90, 0x66, 0x0F, 0x1F, 0x84, 0, 0, 0, 0, 0};
	WriteHookToProcess((void *)(hmodEXE + 0x264286), &nop10, 10U);
	int m501shot = 2;
	WriteHookToProcess((void *)(hmodEXE + 0x263AF4 + 6), &m501shot, 4U);
}

extern "C" {
// ranger!
void __fastcall ASMeArmySoldierUseAuxiliary();
uintptr_t eArmySoldierUseAuxiliaryRetAddr;
// vehicle_call
uintptr_t edf2E0270Address;
// ranger dash
uintptr_t edf2E07C0Address;
// Execution?
uintptr_t edf2E18A0Address;
//
uintptr_t eSoldierCallSupportRetAddr;
uintptr_t edf5F8C40Address;
// Show 2nd support slot
void __fastcall ASMhudShowSupportSlot2();
uintptr_t hudShowSupportSlot2RetAddr;
// air raider!
void __fastcall ASMeEngineerUseAuxiliary();
uintptr_t eEngineerUseAuxiliaryRetAddr;
uintptr_t edf2E2E30Address;
// fencer!
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
// General
void __fastcall ASMeAccessoryEnhancement();
uintptr_t eAccessoryEnhancementRetAddr;

// Railgun
void __fastcall ASMVehicle403TankMainFire();
uintptr_t Vehicle403TankMainFireRetAddr;
// Barga
void __fastcall ASMVehicle501AnimationEvent();
}

void hookEDFClassFunctions() {
	// ranger!
	// offset is 0x2DF417
	eArmySoldierUseAuxiliaryRetAddr = (uintptr_t)(hmodEXE + 0x2E00C1);
	hookGameBlock((void *)(hmodEXE + 0x2E0017), (uint64_t)ASMeArmySoldierUseAuxiliary);
	WriteHookToProcess((void *)(hmodEXE + 0x2E0017 + 12), (void *)&intNOP32, 8U);
	//
	edf2E0270Address = (uintptr_t)(hmodEXE + 0x2E0270);
	edf2E07C0Address = (uintptr_t)(hmodEXE + 0x2E07C0);
	edf2E18A0Address = (uintptr_t)(hmodEXE + 0x2E18A0);
	//
	eSoldierCallSupportRetAddr = (uintptr_t)(hmodEXE + 0x2E0368);
	edf5F8C40Address = (uintptr_t)(hmodEXE + 0x5F8C40);
	// Show 2nd support slot
	hudShowSupportSlot2RetAddr = (uintptr_t)(hmodEXE + 0x4D7A7F);
	hookGameBlock((void *)(hmodEXE + 0x4D7A70), (uint64_t)ASMhudShowSupportSlot2);
	WriteHookToProcess((void *)(hmodEXE + 0x4D7A70 + 12), (void *)&intNOP32, 3U);

	// air raider!
	// offset is 0x2E197A
	eEngineerUseAuxiliaryRetAddr = (uintptr_t)(hmodEXE + 0x2E25FC);
	hookGameBlock((void *)(hmodEXE + 0x2E257A), (uint64_t)ASMeEngineerUseAuxiliary);
	WriteHookToProcess((void *)(hmodEXE + 0x2E257A + 12), (void *)&intNOP32, 6U);
	edf2E2E30Address = (uintptr_t)(hmodEXE + 0x2E2E30);

	// wing diver!
	// Flying Speed, default is 0.4f
	unsigned char newWDFlying[] = {0x51, 0xE5};
	float WDspeedFly = 0.27f;
	WriteHookToProcess((void *)(hmodEXE + 0x2F6F65 + 7), &WDspeedFly, 4U);
	WriteHookToProcess((void *)(hmodEXE + 0x2F848B + 4), &newWDFlying[0], 1U);
	// Takeoff Speed, default is 0.007f
	float WDspeedTakeoff = 0.005f;
	WriteHookToProcess((void *)(hmodEXE + 0x2F6F7B + 7), &WDspeedTakeoff, 4U);
	WriteHookToProcess((void *)(hmodEXE + 0x2F84D3 + 4), &newWDFlying[1], 1U);
	// Flight Consumption, default is 0.25f
	// now it is 0.2f
	unsigned char newWDFlyEnergy[] = {0x51, 0x9A};
	WriteHookToProcess((void *)(hmodEXE + 0x2F7263 + 4), &newWDFlyEnergy[0], 1U);
	WriteHookToProcess((void *)(hmodEXE + 0x2F861A + 4), &newWDFlyEnergy[1], 1U);

	// fencer!
	int newFencerSize = 0x2000;
	// start:0x1E00, size:0x10, function: swap types.
	// HeavyArmor 0x1C30
	WriteHookToProcess((void *)(hmodEXE + 0x2E3408), &newFencerSize, 4U);
	//WriteHookToProcess((void *)(hmodEXE + 0x2E4229 + 1), &newFencerSize, 4U);
	//WriteHookToProcess((void *)(hmodEXE + 0xC61AC9 + 1), &newFencerSize, 4U);
	
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
	// offset is 0x2E37C4, remove old boost speed
	unsigned char removeBoostSpeed[] = {
		0x89, 0xBB, 0xA4, 0x1B, 0x00, 0x00, // mov dword ptr [rbx+1BA4h], edi
		0x89, 0xBB, 0xA8, 0x1B, 0x00, 0x00, // mov dword ptr [rbx+1BA8h], edi
	    0x41, 0x8B, 0xFE,                   // mov edi, r14d
		0xEB, 0x1E,                         // jmp
		0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90,
		0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90,
	    0x48                                // jmp target: cmp
	};
	WriteHookToProcess((void *)(hmodEXE + 0x2E43BC), &removeBoostSpeed, 48U);
	// offset is 0x2E37BC
	hookGameBlock((void *)(hmodEXE + 0x2E4526), (uint64_t)ASMeFencerJetSetup);
	WriteHookToProcess((void *)(hmodEXE + 0x2E4526 + 12), (void *)&Interruptions32, 20U);
	// Swap boost and dash Activate, offset is 0x2E3C90
	ofs3073C0JmpAddr = (uintptr_t)(hmodEXE + 0x307FC0);
	ofs2E4070JmpAddr = (uintptr_t)(hmodEXE + 0x2E4C70);
	ofs2E42C0JmpAddr = (uintptr_t)(hmodEXE + 0x2E4EC0);
	ofs2E43E0JmpAddr = (uintptr_t)(hmodEXE + 0x2E4FE0);
	ofs2E4500JmpAddr = (uintptr_t)(hmodEXE + 0x2E5100);
	hookGameBlock((void *)(hmodEXE + 0x2E4890), (uint64_t)ASMeFencerBoostAndDash);
	WriteHookToProcess((void *)(hmodEXE + 0x2E4890 + 12), (void *)&intNOP32, 2U);

	// General
	// Add new accessory functions, offset is 0x303DB4
	hookGameBlock((void *)(hmodEXE + 0x3049B4), (uint64_t)ASMeAccessoryEnhancement);
	WriteHookToProcess((void *)(hmodEXE + 0x3049B4 + 12), (void *)&intNOP32, 2U);
	eAccessoryEnhancementRetAddr = (uintptr_t)(hmodEXE + 0x3049C2);

	// EDF5.exe+3391D5
	// Enable Railgun to be dual weapons
	Vehicle403TankMainFireRetAddr = (uintptr_t)(hmodEXE + 0x3391FF);
	hookGameBlock((void*)(hmodEXE + 0x3391D5), (uint64_t)ASMVehicle403TankMainFire);
	WriteHookToProcess((void*)(hmodEXE + 0x3391D5 + 12), (void*)&nop2, 2U);

	// EDF5.exe+33D000
	// Allow Barga to use the weapon
	hookGameBlock((void*)(hmodEXE + 0x33D000), (uint64_t)ASMVehicle501AnimationEvent);
	WriteHookToProcess((void*)(hmodEXE + 0x33D000 + 12), (void*)&nop4, 4U);
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
	// set new readable sgo node name, offset is 0x38E2DD
	weaponReloadEXRetAddr = (uintptr_t)(hmodEXE + 0x38EF46);
	hookGameBlock((void *)(hmodEXE + 0x38EEDD), (uint64_t)ASMweaponReloadEX);
	WriteHookToProcess((void *)(hmodEXE + 0x38EEDD + 12), (void *)&intNOP32, 16U);
	// allows midsection reload, offset is 0x3905CB
	weaponStartReloadRetAddr = (uintptr_t)(hmodEXE + 0x3911DF);
	hookGameBlock((void *)(hmodEXE + 0x3911CB), (uint64_t)ASMweaponStartReload);
	WriteHookToProcess((void *)(hmodEXE + 0x3911CB + 12), (void *)&intNOP32, 2U);

	// gatling setup, offset is 0x39A0C5
	wGatlingSetupRetAddr = (uintptr_t)(hmodEXE + 0x39ACE0);
	hookGameBlock((void *)(hmodEXE + 0x39ACC5), (uint64_t)ASMweaponGatlingSetup);
	WriteHookToProcess((void *)(hmodEXE + 0x39ACC5 + 12), (void *)&intNOP32, 8U);
	// gatling shot, offset is 0x39A7AA
	wGatlingShotRetAddr = (uintptr_t)(hmodEXE + 0x39B3B8);
	hookGameBlock((void *)(hmodEXE + 0x39B3AA), (uint64_t)ASMweaponGatlingShot);
	//WriteHookToProcess((void *)(hmodEXE + 0x39B3AA + 12), (void *)&intNOP32, 2U);
}

extern "C" {
uintptr_t ammoSolidBullet01RetAddr;
void __fastcall ASMammoSolidBullet01();

uintptr_t ammoSolidExpBullet01RetAddr;
void __fastcall ASMammoSolidExpBullet01();

uintptr_t ammoLaserBullet01RetAddr;
void __fastcall ASMammoLaserBullet01();
uintptr_t ammoLaserBullet01HitRetAddr;
void __fastcall ASMammoLaserBullet01Hit();

uintptr_t ammoLaserBullet02RetAddr;
void __fastcall ASMammoLaserBullet02();
uintptr_t ammoLaserBullet02BlastRetAddr;
void __fastcall ASMammoLaserBullet02Blast();

uintptr_t ammoMissileBullet01BlastRetAddr;
void __fastcall ASMammoMissileBullet01Blast();
}

void hookAmmoFunctions() {
	// hook SolidExpBullet01, can't use it now
	// offset is 0x184BE2
	ammoSolidBullet01RetAddr = (uintptr_t)(hmodEXE + 0x1857F0);
	//hookGameBlock((void *)(hmodEXE + 0x1857E2), (uintptr_t)ASMammoSolidBullet01);
	//WriteHookToProcess((void *)(hmodEXE + 0x1857E2 + 12), (void *)&intNOP32, 2U);

	// hook SolidExpBullet01
	// offset is 0x187637
	ammoSolidExpBullet01RetAddr = (uintptr_t)(hmodEXE + 0x188262);
	hookGameBlock((void *)(hmodEXE + 0x188237), (uintptr_t)ASMammoSolidExpBullet01);
	WriteHookToProcess((void *)(hmodEXE + 0x188237 + 12), (void *)&intNOP32, 7U);
	// explosion effect, offset is 0x1880DD
	int hitFxNum = 10;
	// particle count
	WriteHookToProcess((void *)(hmodEXE + 0x188CDD + 3), &hitFxNum, 4U);
	// particle time
	WriteHookToProcess((void *)(hmodEXE + 0x188CE4 + 3), &hitFxNum, 4U);
	// particle speed x3.0f to x0.75f
	unsigned char hitFxSpeed[] = {0xB1, 0xD6};
	WriteHookToProcess((void *)(hmodEXE + 0x188CF3 + 4), &hitFxSpeed, 2U);

	// hook LaserBullet01
	// offset is 0x155421
	ammoLaserBullet01RetAddr = (uintptr_t)(hmodEXE + 0x156038);
	hookGameBlock((void *)(hmodEXE + 0x156021), (uintptr_t)ASMammoLaserBullet01);
	WriteHookToProcess((void *)(hmodEXE + 0x156021 + 12), (void *)&intNOP32, 11U);
	// hit FX, offset is 0x15893D
	ammoLaserBullet01HitRetAddr = (uintptr_t)(hmodEXE + 0x15954B);
	hookGameBlock((void *)(hmodEXE + 0x15953D), (uintptr_t)ASMammoLaserBullet01Hit);
	WriteHookToProcess((void *)(hmodEXE + 0x15953D + 12), (void *)&intNOP32, 2U);

	// hook LaserBullet02
	// offset is 0x156682
	ammoLaserBullet02RetAddr = (uintptr_t)(hmodEXE + 0x15729D);
	hookGameBlock((void *)(hmodEXE + 0x157282), (uintptr_t)ASMammoLaserBullet02);
	WriteHookToProcess((void *)(hmodEXE + 0x157282 + 12), (void *)&intNOP32, 5U);
	// explosion effect, offset is 0x1572B5
	ammoLaserBullet02BlastRetAddr = (uintptr_t)(hmodEXE + 0x157F18);
	hookGameBlock((void *)(hmodEXE + 0x157EB5), (uintptr_t)ASMammoLaserBullet02Blast);
	WriteHookToProcess((void *)(hmodEXE + 0x157EB5 + 12), (void *)&intNOP32, 16U);

	// hook MissileBullet01
	// explosion effect, offset is 0x1613B3
	ammoMissileBullet01BlastRetAddr = (uintptr_t)(hmodEXE + 0x161FEC);
	hookGameBlock((void *)(hmodEXE + 0x161FB3), (uintptr_t)ASMammoMissileBullet01Blast);
	WriteHookToProcess((void *)(hmodEXE + 0x161FB3 + 12), (void *)&intNOP32, 18U);
	// expanded guidance type, offset is 0x161545
	unsigned char MB01GuideType[] = {3, 0x77};
	WriteHookToProcess((void *)(hmodEXE + 0x162145 + 2), &MB01GuideType, 2U);
}

// new functions require more memory
void ReallocateWeaponMemory() {
	// Allocate more memory to all weapons
	// New size must be larger than largest of them all
	// Safe size at least 0x2500
	// Must be an integer multiple of 0x10
	int newWeaponSize = 0x2600;
	// start:0x2500, size:0x20, function: extra reload types.

	// Weapon_Accessory 0x11E0
	WriteHookToProcessCheckECX((void *)(hmodEXE + 0x398018), &newWeaponSize, 4U);
	//WriteHookToProcess((void *)(hmodEXE + 0x398268 + 1), &newWeaponSize, 4U);
	//WriteHookToProcess((void *)(hmodEXE + 0xC65789 + 1), &newWeaponSize, 4U);
	// Weapon_BasicShoot 0x1200
	WriteHookToProcessCheckECX((void *)(hmodEXE + 0x398588), &newWeaponSize, 4U);
	//WriteHookToProcess((void *)(hmodEXE + 0x399269 + 1), &newWeaponSize, 4U);
	// Weapon_BasicSemiAuto 0x1200
	WriteHookToProcessCheckECX((void *)(hmodEXE + 0x39863D), &newWeaponSize, 4U);
	//WriteHookToProcess((void *)(hmodEXE + 0xC657F9 + 1), &newWeaponSize, 4U);
	// Weapon_ChargeShoot 0x14D0
	WriteHookToProcessCheckECX((void *)(hmodEXE + 0x3997F8), &newWeaponSize, 4U);
	//WriteHookToProcess((void *)(hmodEXE + 0x399E19 + 1), &newWeaponSize, 4U);
	//WriteHookToProcess((void *)(hmodEXE + 0xC65919 + 1), &newWeaponSize, 4U);
	// Weapon_PreChargeShoot 0x1310
	WriteHookToProcessCheckECX((void *)(hmodEXE + 0x3A3E48), &newWeaponSize, 4U);
	//WriteHookToProcess((void *)(hmodEXE + 0x3A3FC6 + 1), &newWeaponSize, 4U);
	//WriteHookToProcess((void *)(hmodEXE + 0xC65BC9 + 1), &newWeaponSize, 4U);
	// Weapon_RadioContact 0x1500
	WriteHookToProcessCheckECX((void *)(hmodEXE + 0x3A4368), &newWeaponSize, 4U);
	//WriteHookToProcess((void *)(hmodEXE + 0x26D359 + 1), &newWeaponSize, 4U);
	//WriteHookToProcess((void *)(hmodEXE + 0x3A4949 + 1), &newWeaponSize, 4U);
	//WriteHookToProcess((void *)(hmodEXE + 0xC65C19 + 1), &newWeaponSize, 4U);
	// Weapon_Shield 0x1230
	WriteHookToProcessCheckECX((void *)(hmodEXE + 0x3A6858), &newWeaponSize, 4U);
	//WriteHookToProcess((void *)(hmodEXE + 0x3A6BE7 + 1), &newWeaponSize, 4U);
	//WriteHookToProcess((void *)(hmodEXE + 0xC65CF9 + 1), &newWeaponSize, 4U);
	// Weapon_VehicleMaser 0x1FF0
	WriteHookToProcessCheckECX((void *)(hmodEXE + 0x3A9438), &newWeaponSize, 4U);
	//WriteHookToProcess((void *)(hmodEXE + 0x3AAB99 + 1), &newWeaponSize, 4U);
	//WriteHookToProcess((void *)(hmodEXE + 0xC65DC9 + 1), &newWeaponSize, 4U);
	// Weapon_HomingShoot 0x11F0
	WriteHookToProcessCheckECX((void *)(hmodEXE + 0x46A398), &newWeaponSize, 4U);
	//WriteHookToProcess((void *)(hmodEXE + 0x3ABB86 + 1), &newWeaponSize, 4U);
	// Weapon_Throw 0x1200
	WriteHookToProcessCheckECX((void *)(hmodEXE + 0x46A3E8), &newWeaponSize, 4U);
	//WriteHookToProcess((void *)(hmodEXE + 0x3A8D57 + 1), &newWeaponSize, 4U);
	// Weapon_Swing 0x1210
	WriteHookToProcessCheckECX((void *)(hmodEXE + 0x46A438), &newWeaponSize, 4U);
	//WriteHookToProcess((void *)(hmodEXE + 0x3A8547 + 1), &newWeaponSize, 4U);
	//WriteHookToProcess((void *)(hmodEXE + 0x353589 + 1), &newWeaponSize, 4U);
	//WriteHookToProcess((void *)(hmodEXE + 0xC6EE99 + 1), &newWeaponSize, 4U);
	// Weapon_PileBanker 0x11F0
	WriteHookToProcessCheckECX((void *)(hmodEXE + 0x46A488), &newWeaponSize, 4U);
	//WriteHookToProcess((void *)(hmodEXE + 0x398A17 + 1), &newWeaponSize, 4U);
	// Weapon_ImpactHammer 0x1320
	WriteHookToProcessCheckECX((void *)(hmodEXE + 0x46A4D8), &newWeaponSize, 4U);
	//WriteHookToProcess((void *)(hmodEXE + 0x39D057 + 1), &newWeaponSize, 4U);
	//WriteHookToProcess((void *)(hmodEXE + 0xC6EEB9 + 1), &newWeaponSize, 4U);
	// Weapon_HeavyShoot 0x1260
	WriteHookToProcessCheckECX((void *)(hmodEXE + 0x46A528), &newWeaponSize, 4U);
	//WriteHookToProcess((void *)(hmodEXE + 0x39BC87 + 1), &newWeaponSize, 4U);
	//WriteHookToProcess((void *)(hmodEXE + 0xC6EED9 + 1), &newWeaponSize, 4U);

	// Weapon_Gatling 0x1290
	WriteHookToProcessCheckECX((void *)(hmodEXE + 0x46A578), &newWeaponSize, 4U);
	//WriteHookToProcess((void *)(hmodEXE + 0x39AEC9 + 1), &newWeaponSize, 4U);
	//WriteHookToProcess((void *)(hmodEXE + 0xC6EEF9 + 1), &newWeaponSize, 4U);
	// start:0x1400, size:0x20, function: set pre-heat type.

	// Weapon_LaserMarker 0x1460
	WriteHookToProcessCheckECX((void *)(hmodEXE + 0x46A5C8), &newWeaponSize, 4U);
	//WriteHookToProcess((void *)(hmodEXE + 0x3A0868 + 1), &newWeaponSize, 4U);
	//WriteHookToProcess((void *)(hmodEXE + 0xC6EF19 + 1), &newWeaponSize, 4U);
	// Weapon_LaserMarkerCallFire 0x1AB0
	WriteHookToProcessCheckECX((void *)(hmodEXE + 0x46A618), &newWeaponSize, 4U);
	//WriteHookToProcess((void *)(hmodEXE + 0x3A0FF6 + 1), &newWeaponSize, 4U);
	//WriteHookToProcess((void *)(hmodEXE + 0xC6EF39 + 1), &newWeaponSize, 4U);
	// Weapon_MarkerShooter 0x11F0
	WriteHookToProcessCheckECX((void *)(hmodEXE + 0x46A66D), &newWeaponSize, 4U);
	//WriteHookToProcess((void *)(hmodEXE + 0xC6EE79 + 1), &newWeaponSize, 4U);
	// Weapon_VehicleShoot 0x11F0
	WriteHookToProcessCheckECX((void *)(hmodEXE + 0x46A6D8), &newWeaponSize, 4U);
	//WriteHookToProcess((void *)(hmodEXE + 0x3A3817 + 1), &newWeaponSize, 4U);
	// Weapon_VehicleRailGun 0x11F0
	WriteHookToProcessCheckECX((void *)(hmodEXE + 0x46A72D), &newWeaponSize, 4U);
}
