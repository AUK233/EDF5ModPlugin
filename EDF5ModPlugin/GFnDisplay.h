#pragma once
#include "EDFPointerStruct.hpp"


extern "C" {
HUiHudTextContentPointer* __fastcall ASMgetHUiHudTextContentPointer(void* pText);

void __fastcall hookSleep(DWORD time);
void __fastcall debugAllocateMemorySize(void* ptr);
void __fastcall debugGetWeaponName(EDFWeaponPointer* pWeapon);
size_t __fastcall TextForFormatFloatNumber(const float number, WCHAR *destination, size_t len);
size_t __fastcall TextForFormatFloatNumber2(const float number, WCHAR *destination, size_t len);
size_t __fastcall TextForFormatIntNumber(const int number, WCHAR* destination, size_t len);

size_t __fastcall eTextForWeaponReloadTime(EDFWeaponStruct *pweapon, WCHAR *destination, EDFColor4Pointer *pcolor);
//void __fastcall eDisplaySoldierWeaponAmmo(HUiHudWeaponPointer* p);
void __fastcall eDisplayFencerBoostAndDash(HUiHudPowerGuagePointer* p, PFencerBoostAndDash fencer);

void __fastcall eDisplaySoldierWeaponDamage(HUiHudWeaponPointer* p);
void __fastcall eDisplayVehicleWeaponDamage(HUiHudWeaponPointer* p);

// fast get address
void __fastcall ASMgetPlayerAddress(uintptr_t startAddr, void* target);
}
void WINAPI getPlayerWeaponDamage();
std::wstring __fastcall FormatDamageNumber(const float dmg);
void __fastcall setDamageDisplayTime(int vstart, int vend, int time);

void hookHUDEnhancement();
