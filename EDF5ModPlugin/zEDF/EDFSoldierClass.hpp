#pragma once
#include "Base/base_baseObject.h"
#include "EDFPointerStruct.hpp"

typedef int(__fastcall* CallFunc_SoldierBase_GetSoldierType)(); // vft+238

typedef struct G_HumanBase_t : G_NetworkObject_t {
	char pad410[0x10];
	int flag420, pad424[3];
	Matrix3D_t transform_human; // copy from transform
	int InputControlType; // 1 is fencer
	char pad474[0xCF4];
	PG_NetworkObject pVehicle;
	char pad1170[0x410];
} *PG_HumanBase;
#if 1
static_assert(offsetof(G_HumanBase_t, flag420) == 0x420);
static_assert(offsetof(G_HumanBase_t, transform_human) == 0x430);
static_assert(offsetof(G_HumanBase_t, InputControlType) == 0x470);
static_assert(offsetof(G_HumanBase_t, pVehicle) == 0x1168);
static_assert(sizeof(G_HumanBase_t) == 0x1580);
#endif

typedef struct G_SoldierBase_t : G_HumanBase_t {
	void* vft_SoldierBase;
	EDFStdVector_t v_WeaponPointer;
	char pad15a8[0x50];
	EDFStdVector_t v_SubWeaponSlot;
	char pad1608[0x84];
	float WeaponRecoil;
	char pad16a0[8];
	float WalkingSpeed;
	char pad16aC[0x8C];
	float CharacterCollectRange;
	float CurrentCollectRange; // It is written
	char pad1740[0x2C0];
} *PG_SoldierBase;
#if 1
static_assert(offsetof(G_SoldierBase_t, v_WeaponPointer) == 0x1590 - 8);
static_assert(offsetof(G_SoldierBase_t, v_SubWeaponSlot) == 0x1600 - 8);
static_assert(offsetof(G_SoldierBase_t, WeaponRecoil) == 0x169C);
static_assert(offsetof(G_SoldierBase_t, WalkingSpeed) == 0x16A8);
static_assert(sizeof(G_SoldierBase_t) == 0x1A00);
#endif

// ==================================================
// Fencer
typedef struct G_HeavyArmor_t : G_SoldierBase_t {
	char pad1a00[0x1A0];
	int BoostRecoveryRemainTime, BoostMaxCount, BoostCurrentCount; float BoostSpeed;
	int DashRecoveryRemainTime; float DashInterval; int DashMaxCount, DashCurrentCount;
	char pad1bc0[0x70];
	// new
	int IsDashToBoost, IsBoostToDash, BoostRecoveryBaseTime, DashRecoveryBaseTime;
	void* pSubWeapon; int SubWeaponButtonTimer, SubWeaponButtonCD;
} *PG_HeavyArmor;
#if 1
static_assert(offsetof(G_HeavyArmor_t, BoostRecoveryRemainTime) == 0x1BA0);
static_assert(offsetof(G_HeavyArmor_t, IsDashToBoost) == 0x1C30);
static_assert(offsetof(G_HeavyArmor_t, IsBoostToDash) == 0x1C34);
static_assert(offsetof(G_HeavyArmor_t, BoostRecoveryBaseTime) == 0x1C38);
static_assert(offsetof(G_HeavyArmor_t, DashRecoveryBaseTime) == 0x1C3C);
static_assert(offsetof(G_HeavyArmor_t, pSubWeapon) == 0x1C40);
static_assert(offsetof(G_HeavyArmor_t, SubWeaponButtonTimer) == 0x1C48);
#endif
// ==================================================
