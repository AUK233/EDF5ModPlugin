#include <windows.h>
#include <string>
#include <vector>
#include <format>
#include <stdexcept>
#include <list>
#include <cstdlib>

#include "utiliy.h"
#include "commonNOP.h"
#include "HUiHudWeapon.h"

extern "C" {
	void __fastcall ASMhuiHudWeaponInitialization();
	void __fastcall ASMHUiHudWeaponUpdateAmmoText();

	void __fastcall ASMHUiHudWeaponUpdateVehicleText();
	uintptr_t HUiHudWeaponUpdateVehicleTextRet;
}

void module_UpdateHUiHudWeapon(PBYTE hmodEXE)
{
	/*WriteHookToProcess((void*)(hmodEXE + 0xECB740), (void*)L"lyt_HudWeaponGuagL1.sgo", 48U);
	WriteHookToProcess((void*)(hmodEXE + 0xECB820), (void*)L"lyt_HudWeaponGuagR1.sgo", 48U);
	WriteHookToProcess((void*)(hmodEXE + 0xECB7D0), (void*)L"lyt_HudEnergyGuageR1.sgo", 48U);
	WriteHookToProcess((void*)(hmodEXE + 0xEC8F90), (void*)L"lyt_HudWeaponGuageVehicl1.sgo", 60U);*/

	// EDF5.exe+4D1017
	int newWeaponSize = 0xCD0;
	static_assert(sizeof(G_HUiHudWeapon_t) <= 0xCD0);
	WriteHookToProcessCheckECX((void*)(hmodEXE + 0x4D1017 + 1), &newWeaponSize, 4U);

	// EDF5.exe+4D1C32
	hookGameBlockWithInt3((void*)(hmodEXE + 0x4D1C32), (uintptr_t)ASMhuiHudWeaponInitialization);
	WriteHookToProcess((void*)(hmodEXE + 0x4D1C32 + 15), (void*)&nop3, 3U);

	// EDF5.exe+4D370C
	// now, it is no longer used.
	/*hookGameBlockWithInt3((void*)(hmodEXE + 0x4D370C), (uintptr_t)ASMHUiHudWeaponUpdateVehicleText);
	WriteHookToProcess((void*)(hmodEXE + 0x4D370C + 15), (void*)&nop3, 3U);
	HUiHudWeaponUpdateVehicleTextRet = (uintptr_t)(hmodEXE + 0x4D371E);*/

	// EDF5.exe+4D7110
	hookGameBlock((void*)(hmodEXE + 0x4D7110), (uintptr_t)ASMHUiHudWeaponUpdateAmmoText);
	// we still need to continue using rsi
	BYTE _r14_ = 0x4C;
	WriteHookToProcess((void*)(hmodEXE + 0x4D70B5), &_r14_, 1U);
	WriteHookToProcess((void*)(hmodEXE + 0x4D70E2), &_r14_, 1U);
}
