#pragma once

extern "C" {
uintptr_t __fastcall setDamageString(uintptr_t pstr, uintptr_t pcolor, uintptr_t rspBackup);
}

// get player weapon damage
void hookGetPlayerDamage();
// fast get address
uintptr_t __fastcall GetPlayerAddress();

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
