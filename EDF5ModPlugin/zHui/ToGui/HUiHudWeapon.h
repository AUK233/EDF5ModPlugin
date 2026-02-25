#pragma once
#include "../HUiHudCommonStruct.h"

typedef struct G_HUiHudWeapon_t : G_HUiHudBase_t {
	char pad11[0x228];
	PG_HUiHudCommonText TextNumeric;
	void* TextNumericCheck;
	PG_HUiHudCommonText TextWingEnergy;
	void* TextWingEnergyCheck;
	PG_HUiHudCommonText WingChargeGuage;
	void* WingChargeGuageCheck;
	char pad2[0x1E8];
	bool IsVehicle; // 0 is soldiers
	bool HasVehicleWeaponUI; // 0 is no?
	char padC32[0x8E];
	// next is new
	int Hud_WeaponAlignType; // 0 is none, 1 is left, 2 is upper, 3 is right
	char padCC4[0xC]; //align to 16
} *PG_HUiHudWeapon;
#if 1
static_assert(offsetof(G_HUiHudWeapon_t, TextNumeric) == 0xA18);
static_assert(offsetof(G_HUiHudWeapon_t, TextNumericCheck) == 0xA20);
static_assert(offsetof(G_HUiHudWeapon_t, TextWingEnergy) == 0xA28);
static_assert(offsetof(G_HUiHudWeapon_t, TextWingEnergyCheck) == 0xA30);
static_assert(offsetof(G_HUiHudWeapon_t, WingChargeGuage) == 0xA38);
static_assert(offsetof(G_HUiHudWeapon_t, WingChargeGuageCheck) == 0xA40);
static_assert(offsetof(G_HUiHudWeapon_t, IsVehicle) == 0xC30);
static_assert(offsetof(G_HUiHudWeapon_t, HasVehicleWeaponUI) == 0xC31);
static_assert(offsetof(G_HUiHudWeapon_t, Hud_WeaponAlignType) == 0xCC0);
#endif

void module_UpdateHUiHudWeapon(PBYTE hmodEXE);
