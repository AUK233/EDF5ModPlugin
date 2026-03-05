#pragma once
#include "EDFWeapon.hpp"

extern "C" {
	void __fastcall debugAllocateMemorySize(void* ptr);
	void __fastcall debugGetWeaponName(PG_WeaponBase pWeapon);
}
