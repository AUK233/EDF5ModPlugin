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

#include "GameFunc_vftable.h"
#include <commonNOP.h>

// size is 0x60
extern "C" {
	uintptr_t vftable_SolidPelletBullet01[12] = { 0 };
	void __fastcall ASMammoSolidPelletBullet01CheckPT();
}

void __fastcall CreateNewVirtualTablesOfAmmo(uintptr_t hmodEXE)
{
	memcpy(&vftable_SolidPelletBullet01[0], (void*)(hmodEXE + 0xE9C068 - 8), 0x60);
	// Check bullet penetration. EDF5.exe+186870
	// should is 0x10 + 8
	vftable_SolidPelletBullet01[3] = (uintptr_t)ASMammoSolidPelletBullet01CheckPT;
}

// size is 0x110
extern "C" {
	uintptr_t vftable_WeaponHeavyShoot[34] = { 0 };
	void __fastcall ASMweaponHeavyShootFuncP0();
	uintptr_t weaponHeavyShootFuncP0Ret;
	void __fastcall ASMweaponHeavyShootFuncP10();
	uintptr_t weaponHeavyShootFuncP10Call;
	void __fastcall ASMweaponHeavyShootFuncFree();
	uintptr_t weaponHeavyShootFuncFreeRet;
}

void __fastcall CreateNewVirtualTablesOfWeapon(uintptr_t hmodEXE)
{
	memcpy(&vftable_WeaponHeavyShoot[0], (void*)(hmodEXE + 0xEBA480 - 8), 0x110);
	// EDF5.exe+38FF90
	// should is 0 + 8
	vftable_WeaponHeavyShoot[1] = (uintptr_t)ASMweaponHeavyShootFuncP0;
	weaponHeavyShootFuncP0Ret = (uintptr_t)(hmodEXE + 0x3AE1D0);
	// EDF5.exe+38FF90
	// should is 0x10 + 8
	vftable_WeaponHeavyShoot[3] = (uintptr_t)ASMweaponHeavyShootFuncP10;
	weaponHeavyShootFuncP10Call = (uintptr_t)(hmodEXE + 0x3900F0);
	// EDF5.exe+39BC60
	// should is 0x48 + 8
	vftable_WeaponHeavyShoot[10] = (uintptr_t)ASMweaponHeavyShootFuncFree;
	weaponHeavyShootFuncFreeRet = (uintptr_t)(hmodEXE + 0x39BC6A);
}

extern "C" {
	uintptr_t vftable_WeaponIndicatorLine[3] = { 0 };
	void __fastcall ASMweaponIndicatorLineFuncP8();
}

void __fastcall CreateNewVirtualTablesOfOther(uintptr_t hmodEXE)
{
	memcpy(&vftable_WeaponIndicatorLine[0], (void*)(hmodEXE + 0xEB8DC0 - 8), 0x18);
	// EDF5.exe+38ABD0
	// should is 8 + 8
	vftable_WeaponIndicatorLine[2] = (uintptr_t)ASMweaponIndicatorLineFuncP8;
}

extern "C" {
// void __cdecl Umbra::Object::set(enum Umbra::Object::Property, bool)
uintptr_t jmp_Umbra_Object_set;

// laser sight +0
uintptr_t rva38B6A0;
// laser sight +0x10
uintptr_t rva38B390;
// indicator
uintptr_t rva3B67C0;
uintptr_t rva3B4F40;
uintptr_t rva3B5460;
uintptr_t rva9DD10;
uintptr_t m4x4_118C710;
uintptr_t rva5EE2A0;
// collision detection
uintptr_t rvaAB720;
uintptr_t rvaABED0;
uintptr_t rva10EA000;
// havok
uintptr_t hktCEAF78;
}

void __fastcall CreateNewVirtualTables(uintptr_t hmodEXE)
{
	// initialization required
	jmp_Umbra_Object_set = *(uintptr_t*)(hmodEXE + 0xC97FD0);

	// laser sight 
	rva38B6A0 = (uintptr_t)(hmodEXE + 0x38B6A0);
	rva38B390 = (uintptr_t)(hmodEXE + 0x38B390);
	// indicator
	rva3B67C0 = (uintptr_t)(hmodEXE + 0x3B67C0);
	rva3B4F40 = (uintptr_t)(hmodEXE + 0x3B4F40);
	rva3B5460 = (uintptr_t)(hmodEXE + 0x3B5460);
	rva9DD10 = (uintptr_t)(hmodEXE + 0x9DD10);
	m4x4_118C710 = (uintptr_t)(hmodEXE + 0x118C710);
	rva5EE2A0 = (uintptr_t)(hmodEXE + 0x5EE2A0);
	// collision detection
	rvaAB720 = (uintptr_t)(hmodEXE + 0xAB720);
	rvaABED0 = (uintptr_t)(hmodEXE + 0xABED0);
	rva10EA000 = (uintptr_t)(hmodEXE + 0x10EA000);
	// havok
	hktCEAF78 = (uintptr_t)(hmodEXE + 0xCEAF78);

	CreateNewVirtualTablesOfAmmo(hmodEXE);
	CreateNewVirtualTablesOfWeapon(hmodEXE);
	CreateNewVirtualTablesOfOther(hmodEXE);
}
