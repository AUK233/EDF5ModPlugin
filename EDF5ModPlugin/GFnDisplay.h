#pragma once

extern "C" {
void __fastcall setDamageString(PBYTE pstr, PBYTE pcolor);
}

// get player weapon damage
void hookGetPlayerDamage();
// reset string
void displayWeaponDamageReset();

std::wstring FormatDamageNumber(const float dmg);
void displayWeaponDamageA();
