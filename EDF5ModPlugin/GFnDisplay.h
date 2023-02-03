#pragma once

extern "C" {
void __fastcall setDamageString(uintptr_t pstr, uintptr_t pcolor);
}

// get player weapon damage
void hookGetPlayerDamage();
void __fastcall displayWeaponDamageClear();
// reset string
void __fastcall displayWeaponDamageReset();

std::wstring __fastcall FormatDamageNumber(const float dmg);
void __fastcall DMGCommonClear();
void __fastcall setChagreDamageTime(int time);
void __fastcall setDamageDisplayTime(int vstart, int vend, int time);
void __fastcall displayWeaponDamageA1();
void __fastcall displayWeaponDamageA2();
void __fastcall displayWeaponDamageB1();
void __fastcall displayWeaponDamageB2();
// reset values in real time read configuration
void __fastcall displayWeaponDamageNull();
