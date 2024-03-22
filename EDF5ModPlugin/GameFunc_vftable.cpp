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
#include "CommonData.h"
#include <commonNOP.h>
#include "Resource/LoadShaders.hpp"

#include <fstream>
#include <vector>

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
	// AcidBullet01
	uintptr_t edf136890Address;
	// FlameBullet01
	uintptr_t edf14DE20Address;
	// FlameBullet02
	uintptr_t edf14E9B0Address;
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
	// SolidExpBullet01
	uintptr_t edf187EC0Address;
	// HomingLaserBullet01
	uintptr_t edf150AD0Address;
	// LightningBullet01
	uintptr_t edf15FD90Address;
	// LaserBullet01
	uintptr_t edf155B80Address;
	// RocketBullet01
	uintptr_t edf1727E0Address;
	// BeamBullet01
	uintptr_t edf139950Address;
	// NeedleBullet01
	uintptr_t edf168F00Address;

	uintptr_t InsectAmmoType_Func[14 + 1];
	UINT32 InsectAmmoType_Size[14 + 1];
}

void __fastcall CreateNewAmmoIndex(uintptr_t hmodEXE)
{
	// initialization
	edf136890Address = (uintptr_t)(hmodEXE + 0x136890);
	edf14DE20Address = (uintptr_t)(hmodEXE + 0x14DE20);
	edf14E9B0Address = (uintptr_t)(hmodEXE + 0x14E9B0);
	edf16DBB0Address = (uintptr_t)(hmodEXE + 0x16DBB0);
	edf185510Address = (uintptr_t)(hmodEXE + 0x185510);
	edf18AE10Address = (uintptr_t)(hmodEXE + 0x18AE10);
	edf18DF30Address = (uintptr_t)(hmodEXE + 0x18DF30);
	edf171140Address = (uintptr_t)(hmodEXE + 0x171140);
	edf156FF0Address = (uintptr_t)(hmodEXE + 0x156FF0);
	edf187EC0Address = (uintptr_t)(hmodEXE + 0x187EC0);
	edf150AD0Address = (uintptr_t)(hmodEXE + 0x150AD0);
	edf15FD90Address = (uintptr_t)(hmodEXE + 0x15FD90);
	edf155B80Address = (uintptr_t)(hmodEXE + 0x155B80);
	edf1727E0Address = (uintptr_t)(hmodEXE + 0x1727E0);
	edf139950Address = (uintptr_t)(hmodEXE + 0x139950);
	edf168F00Address = (uintptr_t)(hmodEXE + 0x168F00);

	// Initialize insect ammo
	InsectAmmoType_Func[0] = 0;
	InsectAmmoType_Size[0] = 0;
	// AcidBullet01
	InsectAmmoType_Func[1] = edf136890Address;
	InsectAmmoType_Size[1] = AcidBullet01Size;
	// FlameBullet01
	InsectAmmoType_Func[2] = edf14DE20Address;
	InsectAmmoType_Size[2] = FlameBullet01Size;
	// PlasmaBullet01
	InsectAmmoType_Func[3] = edf16DBB0Address;
	InsectAmmoType_Size[3] = PlasmaBullet01Size;
	// SolidBullet01
	InsectAmmoType_Func[4] = edf185510Address;
	InsectAmmoType_Size[4] = SolidBullet01Size;
	// PulseBullet01
	InsectAmmoType_Func[5] = edf171140Address;
	InsectAmmoType_Size[5] = PulseBullet01Size;
	// SolidExpBullet01
	InsectAmmoType_Func[6] = edf187EC0Address;
	InsectAmmoType_Size[6] = SolidExpBullet01Size;
	// HomingLaserBullet01
	InsectAmmoType_Func[7] = edf150AD0Address;
	InsectAmmoType_Size[7] = HomingLaserBullet01Size;
	// LaserBullet02
	InsectAmmoType_Func[8] = edf156FF0Address;
	InsectAmmoType_Size[8] = LaserBullet02Size;
	// FlameBullet02
	InsectAmmoType_Func[9] = edf14E9B0Address;
	InsectAmmoType_Size[9] = FlameBullet02Size;
	// NeedleBullet01
	InsectAmmoType_Func[10] = edf168F00Address;
	InsectAmmoType_Size[10] = NeedleBullet01Size;
	// LightningBullet01
	InsectAmmoType_Func[11] = edf15FD90Address;
	InsectAmmoType_Size[11] = LightningBullet01Size;
	// LaserBullet01
	InsectAmmoType_Func[12] = edf155B80Address;
	InsectAmmoType_Size[12] = LaserBullet01Size;
	// RocketBullet01
	InsectAmmoType_Func[13] = edf1727E0Address;
	InsectAmmoType_Size[13] = RocketBullet01Size;
	// BeamBullet01
	InsectAmmoType_Func[14] = edf139950Address;
	InsectAmmoType_Size[14] = BeamBullet01Size;
	// Initialize end
}

extern "C" {
	// void __cdecl Umbra::Object::set(enum Umbra::Object::Property, bool)
	uintptr_t jmp_Umbra_Object_set;
	// static class Umbra::Object * __cdecl Umbra::Object::create(class Umbra::Model *)
	uintptr_t jmp_Umbra_Object_create;
	// void __cdecl Umbra::Object::setBitmask(unsigned int)
	uintptr_t jmp_Umbra_Object_setBitmask;
	// void __cdecl Umbra::Object::setCell(class Umbra::Cell *)
	uintptr_t jmp_Umbra_Object_setCell;
	// void __cdecl Umbra::Object::setRenderCost(enum Umbra::Object::RenderCost)
	uintptr_t jmp_Umbra_Object_setRenderCost;
	// void __cdecl Umbra::ReferenceCount::release(void) const>
	uintptr_t jmp_Umbra_ReferenceCount_release;
	// void __cdecl Umbra::ReferenceCount::setUserPointer(void *)
	uintptr_t jmp_Umbra_ReferenceCount_setUserPointer;
}

void __fastcall GetUmbraFunctionAddress(uintptr_t hmodEXE)
{
	jmp_Umbra_Object_set = *(uintptr_t*)(hmodEXE + 0xC97FD0);
	jmp_Umbra_Object_create = *(uintptr_t*)(hmodEXE + 0xC98058);
	jmp_Umbra_Object_setBitmask = *(uintptr_t*)(hmodEXE + 0xC97FC0);
	jmp_Umbra_Object_setCell = *(uintptr_t*)(hmodEXE + 0xC980D8);
	jmp_Umbra_Object_setRenderCost = *(uintptr_t*)(hmodEXE + 0xC980E8);
	jmp_Umbra_ReferenceCount_release = *(uintptr_t*)(hmodEXE + 0xC980E0);
	jmp_Umbra_ReferenceCount_setUserPointer = *(uintptr_t*)(hmodEXE + 0xC980D0);
}

extern "C" {

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

// only test
uintptr_t testDhaihfiahfia;
}

std::vector<char> afawfawfwafw;

void __fastcall CreateNewVirtualTables(uintptr_t hmodEXE)
{
	// initialization required
	GetUmbraFunctionAddress(hmodEXE);
	CreateNewAmmoIndex(hmodEXE);

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

	/*
	std::ifstream file(L"snd_test.sgo", std::ios::binary | std::ios::ate | std::ios::in);

	std::streamsize size = file.tellg();
	file.seekg(0, std::ios::beg);

	afawfawfwafw.resize(size);
	if (file.read(afawfawfwafw.data(), size)) {
		testDhaihfiahfia = (uintptr_t)afawfawfwafw.data();
		
		// EDF5.exe+3B52D1
		unsigned char dawdawfawf= 0x8B;
		WriteHookToProcess((void*)(hmodEXE + 0x3B52D1 + 1), &dawdawfawf, 1U);
		uintptr_t dafawf = (uintptr_t)afawfawfwafw.data();
		WriteHookToProcess((void*)(hmodEXE + 0x114B650), &dafawf, 8U);
		
	}
	file.close();*/
}
