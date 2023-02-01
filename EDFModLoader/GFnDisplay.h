#pragma once

extern "C" {
void __fastcall setDamageString(PBYTE pstr, PBYTE pcolor);
}

// get player weapon damage
void hookGetPlayerDamage();
void displayWeaponDamage();