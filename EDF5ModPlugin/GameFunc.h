#pragma once
#include "EDFPointerStruct.hpp"

// get game function address
void GetGameFunctions();
// get ammo function address
void GetAmmoFunctions();
// here hook all changed functions, written in c++
void hookGameFunctionsC();


extern "C"{
//
void __fastcall edfSoldierWeaponCharge(EDFWeaponPointer* pweapon);
//
float __fastcall GetAmmoRandomDamageFactor(float minDmg, float maxDmg);
}

//typedef void *(__fastcall *fnk391230_func)(uintptr_t);
//static fnk391230_func fnk391230_orig;
