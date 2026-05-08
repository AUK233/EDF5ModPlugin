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
// wing diver
typedef struct G_PaleWing_t : G_SoldierBase_t {
	char pad1a00[0x2E0];
	float DeadWeight; // Affect recoil.
	float ChargeSpeed, EmergencyChargeSpeed;
	float FlightConsumption, BoostConsumption;
	float WeaponChargeSpeed;
	bool IsFlying, pad1cf9[3];
	float WDWalkSpeed, pad1D00, FlyingSpeed;
	float TakeoffSpeed; // affect height.
	float FlightDamping;
	float BoostSpeed[4]; // 0 is Sideways, 1 is Forward, 2 is Backward, 3 is always is 1.0f
} *PG_PaleWing;
static_assert(offsetof(G_PaleWing_t, DeadWeight) == 0x1CE0);
static_assert(offsetof(G_PaleWing_t, ChargeSpeed) == 0x1CE4);
static_assert(offsetof(G_PaleWing_t, FlightConsumption) == 0x1CEC);
static_assert(offsetof(G_PaleWing_t, BoostConsumption) == 0x1CF0);
static_assert(offsetof(G_PaleWing_t, WeaponChargeSpeed) == 0x1CF4);
static_assert(offsetof(G_PaleWing_t, IsFlying) == 0x1CF8);
static_assert(offsetof(G_PaleWing_t, FlyingSpeed) == 0x1D04);
static_assert(offsetof(G_PaleWing_t, TakeoffSpeed) == 0x1D08);
static_assert(offsetof(G_PaleWing_t, FlightDamping) == 0x1D0C);
static_assert(offsetof(G_PaleWing_t, BoostSpeed) == 0x1D10);
// ==================================================

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
