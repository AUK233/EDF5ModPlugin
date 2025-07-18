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
#include <cstdlib>
#include "utiliy.h"

#include "GameFuncSetup.h"
#include "LoadModule.hpp"
#include "commonNOP.h"
#include "GameFunc_Mission.h"

extern PBYTE hmodEXE;
extern int weaponEnhance;

static const unsigned char intNOP32[] = {0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90};
static const unsigned char Interruptions32[] = {0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC};

extern "C" {
uintptr_t __RTDynamicCastAddr;
uintptr_t aligned_mallocAddr;
uintptr_t aligned_freeAddr;
uintptr_t __sqrtfAddr;
uintptr_t __sinfAddr;
uintptr_t RtlEnterCriticalSection;
uintptr_t RtlLeaveCriticalSection;
//
void __fastcall ASMplayerViewChange();
uintptr_t playerViewRetAddr;

/* For testing
uintptr_t wwwRetAddr;
void __fastcall ASMwww();
*/
}

void hookGameFunctions() {
	//EDF5.exe+613a12
	// first overwrite original
	OverwriteGameFunctions();
	__RTDynamicCastAddr = (uintptr_t)(hmodEXE + 0x9C8228);
	aligned_mallocAddr = (uintptr_t)_aligned_malloc;
	aligned_freeAddr = (uintptr_t)_aligned_free;
	__sqrtfAddr = (uintptr_t)sqrtf;
	__sinfAddr = (uintptr_t)sinf;
	RtlEnterCriticalSection = (uintptr_t)EnterCriticalSection;
	RtlLeaveCriticalSection = (uintptr_t)LeaveCriticalSection;
	// allows switching of views, offset is 0x2DA490
	//hookGameBlock((void *)(hmodEXE + 0x2DB090), (uintptr_t)ASMplayerViewChange);
	//playerViewRetAddr = (uintptr_t)(hmodEXE + 0x2DB0D1);
	// EDF5.exe+2DB070
	hookGameBlockWithInt3((void*)(hmodEXE + 0x2DB070), (uintptr_t)ASMplayerViewChange);
	WriteHookToProcess((void*)(hmodEXE + 0x2DB070 + 15), (void*)&nop3, 3U);
	//playerViewRetAddr = (uintptr_t)(hmodEXE + 0x2DB0E0);

	// By Features
	hookMonsterFunctions();
	hookEDFClassFunctions();
	hookAmmoFunctions();

	if (weaponEnhance) {
	hookWeaponFunctions();
	}

	HookMissionSeriesSet(hmodEXE);

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

	// remove clear limit
	WriteHookToProcess((void*)(hmodEXE + 0x3E32A1), (void*)&nop2, 2U);
	/*
	unsigned char clearLimit1 = 0xEB;
	WriteHookToProcess((void*)(hmodEXE + 0x51230E), &clearLimit1, 1U);
	unsigned char clearLimit2[] = {
		0xE9, 0x0E, 0x01, 0x00, 0x00, 0x90
	};
	WriteHookToProcess((void*)(hmodEXE + 0x5123CF), &clearLimit2, 6U);
	*/

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
// giant spider
void __fastcall ASMxgsOCgiantSpider();
void __fastcall ASMGiantSpiderUpdateAttack();
void __fastcall ASMGiantSpiderNormalShot();
uintptr_t GiantSpiderNormalShotRetAddr;
void __fastcall ASMGiantSpiderAnimationEvent();
uintptr_t GiantSpiderAnimationEventRetAddr;
void __fastcall ASMGiantSpiderFuncP10();
uintptr_t GiantSpiderNormalShotAddr;
// giant bee
void __fastcall ASMxgsOCgiantBee();
void __fastcall ASMGiantBeeNormalShot();
uintptr_t GiantBeeNormalShotRetAddr;
void __fastcall ASMGiantBeeAnimationEvent();
uintptr_t GiantBeeAnimationEventRetAddr;
void __fastcall ASMGiantBeeFuncP10();
uintptr_t GiantBeeNormalShotAddr;
// dragon small
void __fastcall ASMxgsOCdragonSmall();
uintptr_t dragonSmallAmmoNextAddr;
uintptr_t dragonSmallAmmoRetAddr;
uintptr_t dragonSmallAmmoSetNextAddr;
uintptr_t dragonSmallAmmoSetRetAddr;
void __fastcall ASMxgsOCdragonSmallAmmo();
// monster501
void __fastcall ASMxgsOCmonster501();
// Monster501 and Monster504
void __fastcall ASMMonsterUpdateDataInMission();

}

void hookMonsterFunctions() {

	module_EnemyHook_Common(hmodEXE);
	module_EnemyHook_GiantAnt(hmodEXE);
	module_EnemyHook_BigNest(hmodEXE);

	// hook GiantSpider extra features, EDF5.exe+46A138
	unsigned char jmpToGiantSpiderInit[] = {
		0xFF, 0x25, 0xBA, 0xFF, 0xFF, 0xFF
	};
	WriteHookToProcess((void*)(hmodEXE + 0x46A138), &jmpToGiantSpiderInit, 6U);
	uintptr_t GiantSpiderInitaddr = (uintptr_t)ASMxgsOCgiantSpider;
	WriteHookToProcess((void*)(hmodEXE + 0x46A0F8), &GiantSpiderInitaddr, 8U);
	// old is 0x1C40, EDF5.exe+46A117
	// +1C40h(8-Bytes) is ammo call address
	// +1C48h(4-Bytes) is shot count in burst, +1C4Ch(4-Bytes) is accuracy
	// +1C50h(4-Bytes) is burst state (0 or 1)
	// +1C54h(4-Bytes) is current burst count, +1C58h(4-Bytes) is burst count
	// +1C5Ch(4-Bytes) is burst interval, +1C60h(4-Bytes) is burst interval count
	int newGiantSpiderSize = 0x1C70;
	WriteHookToProcessCheckECX((void*)(hmodEXE + 0x46A117 + 1), &newGiantSpiderSize, 4U);
	// EDF5.exe+21F092 [rax+18h], Includes difficulty update object strength.
	hookGameBlock14((void*)(hmodEXE + 0x21F092), (uintptr_t)ASMGiantSpiderUpdateAttack);
	// EDF5.exe+220498, allows modify normal shot accuracy.
	// movss xmm6, dword ptr [rbx+1C4Ch]
	unsigned char SpiderShotAccuracy[] = {
		0xF3, 0x0F, 0x10, 0xB3, 0x4C, 0x1C, 0x00, 0x00
	};
	WriteHookToProcess((void*)(hmodEXE + 0x220498), &SpiderShotAccuracy, 8U);
	// EDF5.exe+2204FB, removal GiantSpider change ammo color
	//unsigned char noSpiderColorChange[] = { 0xEB, 0x4F };
	//WriteHookToProcess((void*)(hmodEXE + 0x2204FB), &noSpiderColorChange, 2U);
	// EDF5.exe+2204FB, allows modify shot ammo.
	hookGameBlockWithInt3((void*)(hmodEXE + 0x2204FB), (uintptr_t)ASMGiantSpiderNormalShot);
	WriteHookToProcess((void*)(hmodEXE + 0x2204FB + 15), (void*)&nop4, 4U);
	GiantSpiderNormalShotRetAddr = (uintptr_t)(hmodEXE + 0x22059A);
	// EDF5.exe+21F354, Update animation events
	hookGameBlockWithInt3((void*)(hmodEXE + 0x21F354), (uintptr_t)ASMGiantSpiderAnimationEvent);
	WriteHookToProcess((void*)(hmodEXE + 0x21F354 + 15), (void*)&nop2, 2U);
	GiantSpiderAnimationEventRetAddr = (uintptr_t)(hmodEXE + 0x21F365);
	// EDF5.exe+21FA95, set continuous shot.
	hookGameBlock14((void*)(hmodEXE + 0x21F9CD), (uintptr_t)ASMGiantSpiderFuncP10);
	hookGameBlock14((void*)(hmodEXE + 0x21FA95), (uintptr_t)ASMGiantSpiderFuncP10);
	GiantSpiderNormalShotAddr = (uintptr_t)(hmodEXE + 0x220650);

	// hook GiantBee extra features, offset is 0x20A0B0
	hookGameBlock((void *)(hmodEXE + 0x20ACB0), (uintptr_t)ASMxgsOCgiantBee);
	WriteHookToProcess((void *)(hmodEXE + 0x20ACB0 + 12), (void *)&Interruptions32, 10U);
	// old is 0x1C40, EDF5.exe+46A167
	// +1C40h(8-Bytes) is ammo call address
	// +1C48h(4-Bytes) is shot count in burst, +1C4Ch(4-Bytes) is accuracy
	// +1C50h(4-Bytes) is burst state (0 or 1)
	// +1C54h(4-Bytes) is current burst count, +1C58h(4-Bytes) is burst count
	// +1C5Ch(4-Bytes) is burst interval, +1C60h(4-Bytes) is burst interval count
	int newGiantBeeSize = 0x1C70;
	WriteHookToProcessCheckECX((void*)(hmodEXE + 0x46A167 + 1), &newGiantBeeSize, 4U);
	// hook GiantBee shot, EDF5.exe+20B2E0
	hookGameBlockWithInt3((void *)(hmodEXE + 0x20B2E0), (uintptr_t)ASMGiantBeeNormalShot);
	WriteHookToProcess((void *)(hmodEXE + 0x20B2E0 + 15), (void *)&nop10, 10U);
	GiantBeeNormalShotRetAddr = (uintptr_t)(hmodEXE + 0x20B2F9);
	// allows adjustment of shot accuracy, offset is 0x20A642
	// movss xmm9, dword ptr [r14+1C4Ch]
	unsigned char beeAccuracy[] = {
		0xF3, 0x45, 0x0F, 0x10, 0x8E, 0x4C, 0x1C, 0x00, 0x00
	};
	WriteHookToProcess((void *)(hmodEXE + 0x20B242), &beeAccuracy, 9U);
	// EDF5.exe+208EBB, update animation events
	hookGameBlockWithInt3((void*)(hmodEXE + 0x208EBB), (uintptr_t)ASMGiantBeeAnimationEvent);
	WriteHookToProcess((void*)(hmodEXE + 0x208EBB + 15), (void*)&nop1, 1U);
	GiantBeeAnimationEventRetAddr = (uintptr_t)(hmodEXE + 0x208ECB);
	// EDF5.exe+209EC8, set continuous shot.
	hookGameBlockWithInt3((void*)(hmodEXE + 0x209EC8), (uintptr_t)ASMGiantBeeFuncP10);
	WriteHookToProcess((void*)(hmodEXE + 0x209EC8 + 15), (void*)&nop6, 6U);
	GiantBeeNormalShotAddr = (uintptr_t)(hmodEXE + 0x20B430);

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

	// EDF5.exe+2725DB
	// Update the monster's data in missions that affect Monster501 and Monster504
	hookGameBlock((void*)(hmodEXE + 0x2725DB), (uintptr_t)ASMMonsterUpdateDataInMission);
}

extern "C" {
// Railgun
void __fastcall ASMVehicle403TankMainFire();
uintptr_t Vehicle403TankMainFireRetAddr;
}



void hookEDFClassFunctions() {
	// input
	// EDF5.exe+48D7B is hold on a button
	// next is press a button
	// EDF5.exe+6dc70 releasing a pointer that is no longer useful

	// Function
	module_SetEDFSeriesFunction(hmodEXE);
	module_SetHuiMoreCharacterModel(hmodEXE);
	module_CasControllerInitialization(hmodEXE);
	module_UpdateHuiObjectClass(hmodEXE);
	module_UpdateHuiModConsole(hmodEXE);
	//module_UpdateHuiWindowSize(hmodEXE);

	// EDF5.exe+3391D5
	// Enable Railgun to be dual weapons
	Vehicle403TankMainFireRetAddr = (uintptr_t)(hmodEXE + 0x3391FF);
	hookGameBlock((void*)(hmodEXE + 0x3391D5), (uintptr_t)ASMVehicle403TankMainFire);
	WriteHookToProcess((void*)(hmodEXE + 0x3391D5 + 12), (void*)&nop2, 2U);

}


extern "C" {
void __fastcall ASMreadWeaponSgoNode();
uintptr_t readWeaponSgoNodeRetAddr;
void __fastcall ASMweaponStartReload();
uintptr_t RVA393D10;
void __fastcall ASMweaponReloadActive();
uintptr_t weaponReloadActiveRetAddr;
void __fastcall ASMweaponSetShotStatus();
uintptr_t weaponSetShotStatusRetAddr;
// Weapon_HeavyShoot
void __fastcall ASMweaponHeavyShootSetup();
// Weapon_Gatling
void __fastcall ASMweaponGatlingSetup();
uintptr_t wGatlingSetupRetAddr;
void __fastcall ASMweaponGatlingShot();

uintptr_t wGatlingShotRetAddr;
}

void hookWeaponFunctions() {
	// new weapon features
	// first, it need to reallocate memory
	ReallocateWeaponMemory();

	// set new readable sgo node name, offset is 0x38E2DD
	hookGameBlock((void *)(hmodEXE + 0x38EEDD), (uint64_t)ASMreadWeaponSgoNode);
	WriteHookToProcess((void *)(hmodEXE + 0x38EEDD + 12), (void *)&nop8, 8U);
	WriteHookToProcess((void*)(hmodEXE + 0x38EEDD + 12 + 8), (void*)&nop8, 8U);
	readWeaponSgoNodeRetAddr = (uintptr_t)(hmodEXE + 0x38EF46);
	// EDF5.exe+312318
	// Overwrite write to friendly damage
	unsigned char weaponSetFriendlyFire[] = {
		0x4D, 0x8B, 0x86, 0xA0, 0x15, 0x00, 0x00, // mov r8, [r14+15A0h]
		0x4D, 0x85, 0xC0,                         // test r8, r8
		0x74, 0x31,                               // je
		0x48, 0x31, 0xC9,                         // xor rcx, rcx
		0x66, 0x0F, 0x1F, 0x84, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x49, 0x8B, 0x14, 0xC9,                   // mov rdx, [r9+rcx*8]
		0x83, 0xBA, 0x14, 0x25, 0x00, 0x00, 0x00, // cmp dword ptr [rdx+2514h], 0
		0x0F, 0x95, 0xC0,                         // setne al
		0x88, 0x82, 0xAD, 0x06, 0x00, 0x00,       // mov [rdx+6AD], al
		0x48, 0xFF, 0xC1,                         // inc rcx
		0x49, 0x3B, 0xC8,                         // cmp rcx, r8
		0x7C, 0xE4,                               // jl
		0x66, 0x0F, 0x1F, 0x84, 0x00, 0x00, 0x00, 0x00, 0x00
	};
	WriteHookToProcess((void*)(hmodEXE + 0x312318), (void*)&weaponSetFriendlyFire, 61U);
	// allows midsection reload, offset is 0x3905CB
	hookGameBlockWithInt3((void *)(hmodEXE + 0x3911CB), (uint64_t)ASMweaponStartReload);
	WriteHookToProcess((void *)(hmodEXE + 0x3911CB + 15), (void *)&nop5, 5U);
	RVA393D10 = (uintptr_t)(hmodEXE + 0x393D10);
	// updated reload check, EDF5.exe+39059F
	hookGameBlock((void*)(hmodEXE + 0x39059F), (uint64_t)ASMweaponReloadActive);
	weaponReloadActiveRetAddr = (uintptr_t)(hmodEXE + 0x3905B4);
	// allows AI both hands to fire, EDF5.exe+308CFD
	hookGameBlockWithInt3((void*)(hmodEXE + 0x308CFD), (uint64_t)ASMweaponSetShotStatus);
	weaponSetShotStatusRetAddr = (uintptr_t)(hmodEXE + 0x308D1C);

	// heavy shoot setup, EDF5.exe+39BC26
	// start:0x12A0, size:0x10, function: set laser sight.
	// start:0x12D0, size:0x30, function: set indicator circle.
	hookGameBlock((void*)(hmodEXE + 0x39BC26), (uint64_t)ASMweaponHeavyShootSetup);
	WriteHookToProcess((void*)(hmodEXE + 0x39BC26 + 12), (void*)&nop10, 10U);

	// gatling setup, offset is 0x39A0C5
	// start:0x13F0, size:0x10, function: save AmmoDamage, AmmoSize, AmmoExplosion
	// start:0x1400, size:0x20, function: set pre-heat type.
	hookGameBlockWithInt3((void *)(hmodEXE + 0x39ACC5), (uint64_t)ASMweaponGatlingSetup);
	WriteHookToProcess((void *)(hmodEXE + 0x39ACC5 + 15), (void *)&nop5, 5U);
	wGatlingSetupRetAddr = (uintptr_t)(hmodEXE + 0x39ACE0);
	// gatling shot, offset is 0x39A7AA
	hookGameBlock14((void *)(hmodEXE + 0x39B3AA), (uint64_t)ASMweaponGatlingShot);
	wGatlingShotRetAddr = (uintptr_t)(hmodEXE + 0x39B3B8);
}

extern "C" {
void __fastcall ASMammoSolidBullet01();

void __fastcall ASMammoSolidExpBullet01();
uintptr_t ammoSolidExpBullet01RetAddr;
//
void __fastcall ASMreadSolidPelletBullet01();

uintptr_t ammoLaserBullet01RetAddr;
void __fastcall ASMammoLaserBullet01();
uintptr_t ammoLaserBullet01HitRetAddr;
void __fastcall ASMammoLaserBullet01Hit();

uintptr_t ammoLaserBullet02RetAddr;
void __fastcall ASMammoLaserBullet02();
uintptr_t ammoLaserBullet02BlastRetAddr;
void __fastcall ASMammoLaserBullet02Blast();

void __fastcall ASMreadMissileBullet01();
uintptr_t readMissileBullet01RetAddr;
void __fastcall ASMammoMissileBullet01Blast();
uintptr_t ammoMissileBullet01BlastRetAddr;

void __fastcall ASMammoRocketBullet01();
uintptr_t ammoRocketBullet01RetAddr;

void __fastcall ASMammoSentryGunBulletOpen01();
uintptr_t ammoSentryGunBullet01Initialize;
uintptr_t ammoSentryGunBullet01FuncP0;
void __fastcall ASMammoSentryGunBulletOpen01Init();
uintptr_t ammoSentryGunBulletOpen01InitRetAddr;
void __fastcall ASMammoSentryGunBulletOpen01Shot();
uintptr_t ammoSentryGunBulletOpen01ShotTRetAddr;
uintptr_t ammoSentryGunBulletOpen01ShotFRetAddr;
}

void hookAmmoFunctions() {

	// hook SolidBullet01
	// EDF5.exe+1852A7
	hookGameBlockWithInt3((void *)(hmodEXE + 0x1852A7), (uintptr_t)ASMammoSolidBullet01);
	WriteHookToProcess((void*)(hmodEXE + 0x1852A7 + 15), (void*)&nop8, 8U);
	// old is 0x710, now up to 0xA10
	// start:0x800, size:0x210, function: tail smoke.
	int newSolidBullet01Size = 0xA10;
	WriteHookToProcessCheckECX((void*)(hmodEXE + 0x185297 + 1), &newSolidBullet01Size, 4U);

	// hook SolidExpBullet01
	// offset is 0x187637
	hookGameBlock((void *)(hmodEXE + 0x188237), (uintptr_t)ASMammoSolidExpBullet01);
	WriteHookToProcess((void *)(hmodEXE + 0x188237 + 12), (void *)&nop7, 7U);
	ammoSolidExpBullet01RetAddr = (uintptr_t)(hmodEXE + 0x188262);
	// explosion effect, offset is 0x1880DD
	int hitFxNum = 10;
	// particle count
	WriteHookToProcess((void *)(hmodEXE + 0x188CDD + 3), &hitFxNum, 4U);
	// particle time
	WriteHookToProcess((void *)(hmodEXE + 0x188CE4 + 3), &hitFxNum, 4U);
	// particle speed x3.0f to x0.75f
	unsigned char hitFxSpeed[] = {0xB1, 0xD6};
	WriteHookToProcess((void *)(hmodEXE + 0x188CF3 + 4), &hitFxSpeed, 2U);
	// EDF5.exe+188A1A, remove hit brightness judgment
	unsigned char hitLightness[] = {
		0xF3, 0x0F, 0x11, 0x93, 0xD8, 0x06, 0x00, 0x00, // movss dword ptr [rbx+6D8], xmm2
		0xEB, 0x1B,										// jmp
		0x66, 0x66, 0x0F, 0x1F, 0x84, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x66, 0x66, 0x0F, 0x1F, 0x84, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x0F, 0x1F, 0x80, 0x00, 0x00, 0x00, 0x00
	};
	WriteHookToProcess((void*)(hmodEXE + 0x188A1A), &hitLightness, 37U);

	// hook SolidPelletBullet01
	// EDF5.exe+185485
	hookGameBlock((void*)(hmodEXE + 0x185485), (uintptr_t)ASMreadSolidPelletBullet01);
	WriteHookToProcess((void*)(hmodEXE + 0x185485 + 12), (void*)&nop9, 9U);
	// UP to 0x730
	int newSolidPelletBullet01Size = 0x730;
	WriteHookToProcessCheckECX((void*)(hmodEXE + 0x18543C + 1), &newSolidPelletBullet01Size, 4U);

	// hook LaserBullet01
	// offset is 0x155421
	ammoLaserBullet01RetAddr = (uintptr_t)(hmodEXE + 0x156038);
	hookGameBlock((void *)(hmodEXE + 0x156021), (uintptr_t)ASMammoLaserBullet01);
	WriteHookToProcess((void*)(hmodEXE + 0x156021 + 12), (void*)&nop2, 2U);
	WriteHookToProcess((void*)(hmodEXE + 0x156021 + 12 + 2), (void*)&nop9, 9U);
	// hit FX, offset is 0x15893D
	ammoLaserBullet01HitRetAddr = (uintptr_t)(hmodEXE + 0x15954B);
	hookGameBlock((void *)(hmodEXE + 0x15953D), (uintptr_t)ASMammoLaserBullet01Hit);
	WriteHookToProcess((void *)(hmodEXE + 0x15953D + 12), (void*)&nop2, 2U);

	// hook LaserBullet02
	// EDF5.exe+157193
	unsigned char LaserBullet02rdi2rdx[] = { 0x15, 0x55 };
	WriteHookToProcess((void*)(hmodEXE + 0x157193 + 2), &LaserBullet02rdi2rdx[0], 1U);
	WriteHookToProcess((void*)(hmodEXE + 0x15719A + 2), &LaserBullet02rdi2rdx[1], 1U);
	// offset is 0x156682
	ammoLaserBullet02RetAddr = (uintptr_t)(hmodEXE + 0x15729D);
	hookGameBlock((void *)(hmodEXE + 0x157282), (uintptr_t)ASMammoLaserBullet02);
	WriteHookToProcess((void *)(hmodEXE + 0x157282 + 12), (void *)&nop5, 5U);
	// explosion effect, offset is 0x1572B5
	ammoLaserBullet02BlastRetAddr = (uintptr_t)(hmodEXE + 0x157F18);
	hookGameBlock((void *)(hmodEXE + 0x157EB5), (uintptr_t)ASMammoLaserBullet02Blast);
	WriteHookToProcess((void *)(hmodEXE + 0x157EB5 + 12), (void *)&nop8, 8U);
	WriteHookToProcess((void*)(hmodEXE + 0x157EB5 + 12 + 8), (void*)&nop8, 8U);

	// hook MissileBullet01
	// EDF5.exe+160EEF
	hookGameBlock((void*)(hmodEXE + 0x160EEF), (uintptr_t)ASMreadMissileBullet01);
	WriteHookToProcess((void*)(hmodEXE + 0x160EEF + 12), (void*)&nop2, 2U);
	readMissileBullet01RetAddr = (uintptr_t)(hmodEXE + 0x160F0D);
	// explosion effect, offset is 0x1613B3
	ammoMissileBullet01BlastRetAddr = (uintptr_t)(hmodEXE + 0x161FEC);
	hookGameBlock((void *)(hmodEXE + 0x161FB3), (uintptr_t)ASMammoMissileBullet01Blast);
	WriteHookToProcess((void *)(hmodEXE + 0x161FB3 + 12), (void *)&nop10, 10U);
	WriteHookToProcess((void*)(hmodEXE + 0x161FB3 + 12 + 10), (void*)&nop8, 8U);
	// expanded guidance type, offset is 0x161545
	unsigned char MB01GuideType[] = {3, 0x77};
	WriteHookToProcess((void *)(hmodEXE + 0x162145 + 2), &MB01GuideType, 2U);

	// hook RocketBullet01
	// EDF5.exe+17292F
	hookGameBlock((void*)(hmodEXE + 0x17292F), (uintptr_t)ASMammoRocketBullet01);
	WriteHookToProcess((void*)(hmodEXE + 0x17292F + 12), (void*)&nop2, 2U);
	ammoRocketBullet01RetAddr = (uintptr_t)(hmodEXE + 0x172959);

	// hook SentryGunBullet_Open01
	// EDF5.exe+46AF60
	hookGameBlockWithInt3((void*)(hmodEXE + 0x46AF60), (uintptr_t)ASMammoSentryGunBulletOpen01);
	ammoSentryGunBullet01Initialize = (uintptr_t)(hmodEXE + 0x1736C0);
	ammoSentryGunBullet01FuncP0 = (uintptr_t)(hmodEXE + 0x173C50);
	// old is 0xB70
	int newSentryGunBulletOpen01Size = 0xB80;
	WriteHookToProcessCheckECX((void*)(hmodEXE + 0x1747D1 + 1), &newSentryGunBulletOpen01Size, 4U);
	// EDF5.exe+17526C
	hookGameBlockWithInt3((void*)(hmodEXE + 0x17526C), (uintptr_t)ASMammoSentryGunBulletOpen01Init);
	WriteHookToProcess((void*)(hmodEXE + 0x17526C + 15), (void*)&nop3, 3U);
	ammoSentryGunBulletOpen01InitRetAddr = (uintptr_t)(hmodEXE + 0x17527E);
	// EDF5.exe+1756EF
	hookGameBlockWithInt3((void*)(hmodEXE + 0x1756EF), (uintptr_t)ASMammoSentryGunBulletOpen01Shot);
	WriteHookToProcess((void*)(hmodEXE + 0x1756EF + 15), (void*)&nop3, 3U);
	ammoSentryGunBulletOpen01ShotTRetAddr = (uintptr_t)(hmodEXE + 0x175713);
	ammoSentryGunBulletOpen01ShotFRetAddr = (uintptr_t)(hmodEXE + 0x1757A1);
}

// new functions require more memory
void ReallocateWeaponMemory() {
	// Allocate more memory to all weapons
	// New size must be larger than largest of them all
	// Safe size at least 0x2500
	// Must be an integer multiple of 0x10
	// start:0x2500, size:0x20, function: extra reload types.
	int newWeaponSize = 0x2600;

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
	// start:0x12A0, size:0x10, function: set laser sight.
	WriteHookToProcessCheckECX((void *)(hmodEXE + 0x46A528), &newWeaponSize, 4U);
	//WriteHookToProcess((void *)(hmodEXE + 0x39BC87 + 1), &newWeaponSize, 4U);
	//WriteHookToProcess((void *)(hmodEXE + 0xC6EED9 + 1), &newWeaponSize, 4U);

	// Weapon_Gatling 0x1290
	// start:0x1400, size:0x20, function: set pre-heat type.
	WriteHookToProcessCheckECX((void *)(hmodEXE + 0x46A578), &newWeaponSize, 4U);
	//WriteHookToProcess((void *)(hmodEXE + 0x39AEC9 + 1), &newWeaponSize, 4U);
	//WriteHookToProcess((void *)(hmodEXE + 0xC6EEF9 + 1), &newWeaponSize, 4U);

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
	// Weapon_VehicleMaser 0x1FF0
	WriteHookToProcessCheckECX((void*)(hmodEXE + 0x3A9438), &newWeaponSize, 4U);
	//WriteHookToProcessCheckEDX((void*)(hmodEXE + 0x3AAB99 + 1), &newWeaponSize, 4U);
	//WriteHookToProcess((void *)(hmodEXE + 0xC65DC9 + 1), &newWeaponSize, 4U);
}
