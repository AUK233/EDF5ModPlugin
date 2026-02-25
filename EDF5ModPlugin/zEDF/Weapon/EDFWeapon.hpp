#pragma once
#include "Base/base_baseObject.h"

typedef struct G_WeaponBase_t : G_SceneObject_t {
	void* pPlayer; char padC8[0x11];
	bool bFireWeapon; // When it's 1, weapon will fire
	bool bFireCheck; // To be verified
	bool bSecondaryFire; // When it's 1, use SecondaryFire
	char padDC[4];
	bool addr224; char padE1[0x67];
	WCHAR* WeaponName; char pad150[0x50];
	int reloadType, reloadTime, reloadTime2;
	char pad1A4[0x18];
	float EnergyChargeRequire, EnergyChargeOriginal, EnergyChargePercent; int MaxAmmoCount;
	char pad1D4[0x11C];
	int FireSpreadType; float FireSpreadWidth; int FireCount;
	char pad2FC[0x204];
	int SecondaryFire_Type; char pad504[4];
	void* SecondaryFire_Parameter;
	int LockonType, LockonTargetType, Lockon_DistributionType;
	bool Lockon_AutoTimeOut, Lockon_FireEndToClear, pad51E[2];
	float LockonAngle[4];
	float LockonRange;
	float LockonTime, LockonHoldTime; // int to float
	char pad53C[4];
	int LockonFailedTime; char pad544[0x68];
	bool addr1452; // current weapon?
	char pad5AD[0xEF];
	float AmmoDamage, AmmoDamageReduceMin, AmmoDamageReduceFactor, AmmoExplosion;
	bool AmmoIsPenetration; // 0 is no, 1 is has
	bool AmmoIsFriendlyFire; // 0 is none, 1 is has
	bool AmmoIsFriendlyNonCollision; // 0 is has, 1 is none
	char pad6AF[5];
	float AmmoSize, AmmoHitSizeAdjust, AmmoHitImpulseAdjust; char pad6C0[0x10];
	float AmmoColor[4]; char pad6E0[0x4];
	void* Ammo_CustomParameter; char pad6F0[0x1E4];
	int reloadPhase; // reloading phase, 0 means no
	char pad42[0x10];
	int CurrentAmmoCount; char pad8EC[0x254];
	int ROFCount; char padB44[0x4C];
	int reloadTimeCount; char padB94[0x10];
	float EnergyChargeCount; char padBA8[0x88];
	void* addr3120;
	void* reloadFlag; // maybe
	char pad8[0x8];
	void* addr3144;
	char padC50[0x550];
} *PG_WeaponBase;
#if 1
static_assert(offsetof(G_WeaponBase_t, pPlayer) == 0xC0);
static_assert(offsetof(G_WeaponBase_t, bFireWeapon) == 0xD9);
static_assert(offsetof(G_WeaponBase_t, bFireCheck) == 0xDA);
static_assert(offsetof(G_WeaponBase_t, bSecondaryFire) == 0xDB);
static_assert(offsetof(G_WeaponBase_t, addr224) == 0xE0);
static_assert(offsetof(G_WeaponBase_t, WeaponName) == 0x148);
static_assert(offsetof(G_WeaponBase_t, reloadType) == 0x1A0);
static_assert(offsetof(G_WeaponBase_t, reloadTime) == 0x1A4);
static_assert(offsetof(G_WeaponBase_t, reloadTime2) == 0x1A8);
static_assert(offsetof(G_WeaponBase_t, EnergyChargeRequire) == 0x1C4);
static_assert(offsetof(G_WeaponBase_t, MaxAmmoCount) == 0x1D0);
static_assert(offsetof(G_WeaponBase_t, FireSpreadType) == 0x2F0);
static_assert(offsetof(G_WeaponBase_t, FireSpreadWidth) == 0x2F4);
static_assert(offsetof(G_WeaponBase_t, FireCount) == 0x2F8);
static_assert(offsetof(G_WeaponBase_t, SecondaryFire_Type) == 0x500);
static_assert(offsetof(G_WeaponBase_t, SecondaryFire_Parameter) == 0x508);
static_assert(offsetof(G_WeaponBase_t, LockonType) == 0x510);
static_assert(offsetof(G_WeaponBase_t, LockonTargetType) == 0x514);
static_assert(offsetof(G_WeaponBase_t, Lockon_DistributionType) == 0x518);
static_assert(offsetof(G_WeaponBase_t, Lockon_AutoTimeOut) == 0x51C);
static_assert(offsetof(G_WeaponBase_t, Lockon_FireEndToClear) == 0x51D);
static_assert(offsetof(G_WeaponBase_t, LockonAngle) == 0x520);
static_assert(offsetof(G_WeaponBase_t, LockonRange) == 0x530);
static_assert(offsetof(G_WeaponBase_t, LockonTime) == 0x534);
static_assert(offsetof(G_WeaponBase_t, LockonHoldTime) == 0x538);
static_assert(offsetof(G_WeaponBase_t, LockonFailedTime) == 0x540);
static_assert(offsetof(G_WeaponBase_t, addr1452) == 0x5AC);
static_assert(offsetof(G_WeaponBase_t, AmmoDamage) == 0x69C);
static_assert(offsetof(G_WeaponBase_t, AmmoDamageReduceMin) == 0x6A0);
static_assert(offsetof(G_WeaponBase_t, AmmoDamageReduceFactor) == 0x6A4);
static_assert(offsetof(G_WeaponBase_t, AmmoExplosion) == 0x6A8);
static_assert(offsetof(G_WeaponBase_t, AmmoIsPenetration) == 0x6AC);
static_assert(offsetof(G_WeaponBase_t, AmmoIsFriendlyFire) == 0x6AD);
static_assert(offsetof(G_WeaponBase_t, AmmoIsFriendlyNonCollision) == 0x6AE);
static_assert(offsetof(G_WeaponBase_t, AmmoSize) == 0x6B4);
static_assert(offsetof(G_WeaponBase_t, AmmoHitSizeAdjust) == 0x6B8);
static_assert(offsetof(G_WeaponBase_t, AmmoHitImpulseAdjust) == 0x6BC);
static_assert(offsetof(G_WeaponBase_t, AmmoColor) == 0x6D0);
static_assert(offsetof(G_WeaponBase_t, Ammo_CustomParameter) == 0x6E8);
static_assert(offsetof(G_WeaponBase_t, reloadPhase) == 0x8D4);
static_assert(offsetof(G_WeaponBase_t, CurrentAmmoCount) == 0x8E8);
static_assert(offsetof(G_WeaponBase_t, ROFCount) == 0xB40);
static_assert(offsetof(G_WeaponBase_t, reloadTimeCount) == 0xB90);
static_assert(offsetof(G_WeaponBase_t, EnergyChargeCount) == 0xBA4);
static_assert(offsetof(G_WeaponBase_t, reloadFlag) == 0xC38);
static_assert(offsetof(G_WeaponBase_t, addr3144) == 0xC48);
static_assert(sizeof(G_WeaponBase_t) == 0x11A0);
#endif
