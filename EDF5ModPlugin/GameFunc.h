#pragma once
#include "EDFPointerStruct.h"

// get game function address
void GetGameFunctions();
// get ammo function address
void GetAmmoFunctions();
// here hook all changed functions, written in c++
void hookGameFunctionsC();


extern "C"{
// new accessory functions
void __fastcall eAccessoryEnhancement(const uintptr_t p_Class);
// get accessory value
// (fetchType: 0 is Direct return, 1 is Take maximum, 2 is Take minimum)
// (fetchType: 3 is Default + Acquired, 4 is Default * Acquired)
float __fastcall ASMeGetAccessoryValue(uintptr_t p_Class, UINT32 accessoryType, float defaultValue, int fetchType);
// get accessory value
// (fetchType: 0 is Direct return, 1 is Take minimum, 2 is Take maximum)
// (fetchType: 3 is Default + Acquired, 4 is Default * Acquired)
int __fastcall ASMeGetAccessoryINT32(uintptr_t p_Class, UINT32 accessoryType, int defaultValue, int fetchType);
//
void __fastcall edfSoldierWeaponCharge(EDFWeaponPointer* pweapon);
}

//typedef void *(__fastcall *fnk391230_func)(uintptr_t);
//static fnk391230_func fnk391230_orig;
