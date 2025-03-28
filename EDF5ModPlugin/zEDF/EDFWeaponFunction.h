#pragma once

void module_SetFunction_EDFWeapon(PBYTE hmodEXE);

extern "C" {
	int __fastcall module_WeaponFunction_SetDelayInt32(int frame);
}
