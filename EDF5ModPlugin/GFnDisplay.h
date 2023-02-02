#pragma once

extern "C" {
void __fastcall setDamageString(PBYTE pstr, PBYTE pcolor);
}

// get player weapon damage
void hookGetPlayerDamage();
// reset string
void displayWeaponDamageReset();

std::wstring FormatDamageNumber(const float dmg);
void DMGCommonClear();
void setChagreDamageTime(int time);
void setDamageDisplayTime(int vstart, int vend, int time);
void displayWeaponDamageA();
void displayWeaponDamageB();
void displayWeaponDamageC();
// reset values in real time read configuration
void displayWeaponDamageNull();
