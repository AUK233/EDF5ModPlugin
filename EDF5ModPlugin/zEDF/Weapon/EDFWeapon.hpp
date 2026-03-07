#pragma once
#include "Base/base_BaseAmmo.hpp"
#include "Base/base_baseObject.h"

typedef void(__fastcall* CallFunc_WeaponBase_P90)(void* pThis, __m128* pPos); // vft+90

typedef struct G_AreaRender_t {
	void* vft_AreaRender;
	char pad08[0xA8];
	__m128 StartPos;
	char padC0[0x150];
} *PG_AreaRender;
#if 1
static_assert(offsetof(G_AreaRender_t, StartPos) == 0xB0);
static_assert(sizeof(G_AreaRender_t) == 0x210);
#endif

typedef struct G_ShellCaseBase_t {
	void* vft_ShellCaseBase;
	char pad08[0x118];
} *PG_ShellCaseBase;
#if 1
static_assert(sizeof(G_ShellCaseBase_t) == 0x120);
#endif

typedef struct G_WeaponFireInfo_t {
	int Type; // FireType
	char pad04[12];
	__m128 Vector; // FireVector
	int SpreadType; float SpreadWidth; int Count, Interval;
	int BurstCount, BurstInterval; float Accuracy, Recoil;
	char Se[0x40]; // FireLoadSe
	char LoadSe[0x40];
} *PG_WeaponFireInfo;
#if 1
static_assert(offsetof(G_WeaponFireInfo_t, SpreadType) == 0x20);
static_assert(sizeof(G_WeaponFireInfo_t) == 0xC0);
#endif

// EDF5.exe+38F0E0
typedef struct G_WeaponInfo_t {
	char pad00[8];
	WCHAR* Name; char pad10[0x18];
	void* pVector168; char pad30[0x30]; // this is a useful pointer.
	int ReloadType, ReloadTime, ReloadTime2;
	char pad1A4[0x18];
	float EnergyChargeRequire, EnergyChargeOriginal, EnergyChargePercent;
	int MaxAmmoCount; float AmmoOwnerMove;
	char pad98[0x80];
	void* vft_ConstantBuffer;
	char pad120[0x70];
	G_WeaponFireInfo_t Fire; char pad250[8];
	void* vft_MuzzleFlashBase; char pad260[0x40];
	G_ShellCaseBase_t ShellCase;
	int SecondaryFire_Type; char pad504[4]; //3c0
	void* SecondaryFire_Parameter;
	int LockonType, LockonTargetType, Lockon_DistributionType;
	bool Lockon_AutoTimeOut, Lockon_FireEndToClear, pad51E[2];
	float LockonAngle[4];
	float LockonRange;
	float LockonTime, LockonHoldTime; // int to float
	char pad53C[4];
	int LockonFailedTime;
	char pad404[0x4C];
} *PG_WeaponInfo;
#if 1
static_assert(offsetof(G_WeaponInfo_t, Name) == 8);
static_assert(offsetof(G_WeaponInfo_t, pVector168) == 0x28);
static_assert(offsetof(G_WeaponInfo_t, vft_ConstantBuffer) == 0x118);
static_assert(offsetof(G_WeaponInfo_t, Fire) == 0x190);
static_assert(offsetof(G_WeaponInfo_t, vft_MuzzleFlashBase) == 0x258);
static_assert(offsetof(G_WeaponInfo_t, ShellCase) == 0x2A0);
static_assert(offsetof(G_WeaponInfo_t, LockonFailedTime) == 0x400);
static_assert(sizeof(G_WeaponInfo_t) == 0x450);
#endif

typedef struct G_WeaponBase_t : G_SceneObject_t {
	void* pPlayer; char padC8[0x11];
	bool bFireWeapon; // When it's 1, weapon will fire
	bool bFireCheck; // To be verified
	bool bSecondaryFire; // When it's 1, use SecondaryFire
	char padDC[4];
	bool addr224; char padE1[0xF];
	Matrix3D_t transform_weapon;
	__m128 vector130; // copy from player+0x520
	G_WeaponInfo_t Info;
	char pad590[0x1C];
	bool addr1452; // current weapon?
	char pad5AD[0x73];
	G_BulletBase_t Ammo;
	G_BulletBase_t Ammo770; // what!
	char pad8C0[0x14];
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
static_assert(offsetof(G_WeaponBase_t, Info) == 0x140);
static_assert(offsetof(G_WeaponBase_t, Info.Name) == 0x148);
static_assert(offsetof(G_WeaponBase_t, Info.pVector168) == 0x168);
static_assert(offsetof(G_WeaponBase_t, Info.ReloadType) == 0x1A0);
static_assert(offsetof(G_WeaponBase_t, Info.ReloadTime) == 0x1A4);
static_assert(offsetof(G_WeaponBase_t, Info.ReloadTime2) == 0x1A8);
static_assert(offsetof(G_WeaponBase_t, Info.EnergyChargeRequire) == 0x1C4);
static_assert(offsetof(G_WeaponBase_t, Info.MaxAmmoCount) == 0x1D0);
static_assert(offsetof(G_WeaponBase_t, Info.AmmoOwnerMove) == 0x1D4);
static_assert(offsetof(G_WeaponBase_t, Info.Fire.Type) == 0x2D0);
static_assert(offsetof(G_WeaponBase_t, Info.Fire.Vector) == 0x2E0);
static_assert(offsetof(G_WeaponBase_t, Info.Fire.SpreadType) == 0x2F0);
static_assert(offsetof(G_WeaponBase_t, Info.Fire.SpreadWidth) == 0x2F4);
static_assert(offsetof(G_WeaponBase_t, Info.Fire.Count) == 0x2F8);
static_assert(offsetof(G_WeaponBase_t, Info.Fire.Interval) == 0x2FC);
static_assert(offsetof(G_WeaponBase_t, Info.Fire.BurstCount) == 0x300);
static_assert(offsetof(G_WeaponBase_t, Info.Fire.BurstInterval) == 0x304);
static_assert(offsetof(G_WeaponBase_t, Info.Fire.Accuracy) == 0x308);
static_assert(offsetof(G_WeaponBase_t, Info.Fire.Recoil) == 0x30C);
static_assert(offsetof(G_WeaponBase_t, Info.SecondaryFire_Type) == 0x500);
static_assert(offsetof(G_WeaponBase_t, Info.SecondaryFire_Parameter) == 0x508);
static_assert(offsetof(G_WeaponBase_t, Info.LockonType) == 0x510);
static_assert(offsetof(G_WeaponBase_t, Info.LockonTargetType) == 0x514);
static_assert(offsetof(G_WeaponBase_t, Info.Lockon_DistributionType) == 0x518);
static_assert(offsetof(G_WeaponBase_t, Info.Lockon_AutoTimeOut) == 0x51C);
static_assert(offsetof(G_WeaponBase_t, Info.Lockon_FireEndToClear) == 0x51D);
static_assert(offsetof(G_WeaponBase_t, Info.LockonAngle) == 0x520);
static_assert(offsetof(G_WeaponBase_t, Info.LockonRange) == 0x530);
static_assert(offsetof(G_WeaponBase_t, Info.LockonTime) == 0x534);
static_assert(offsetof(G_WeaponBase_t, Info.LockonHoldTime) == 0x538);
static_assert(offsetof(G_WeaponBase_t, Info.LockonFailedTime) == 0x540);
static_assert(offsetof(G_WeaponBase_t, addr1452) == 0x5AC);
static_assert(offsetof(G_WeaponBase_t, Ammo) == 0x620);
static_assert(offsetof(G_WeaponBase_t, Ammo.data.Speed) == 0x694);
static_assert(offsetof(G_WeaponBase_t, Ammo.data.Alive) == 0x698);
static_assert(offsetof(G_WeaponBase_t, Ammo.data.Damage) == 0x69C);
static_assert(offsetof(G_WeaponBase_t, Ammo.data.DamageReduceMin) == 0x6A0);
static_assert(offsetof(G_WeaponBase_t, Ammo.data.DamageReduceFactor) == 0x6A4);
static_assert(offsetof(G_WeaponBase_t, Ammo.data.Explosion) == 0x6A8);
static_assert(offsetof(G_WeaponBase_t, Ammo.data.IsPenetration) == 0x6AC);
static_assert(offsetof(G_WeaponBase_t, Ammo.data.IsFriendlyFire) == 0x6AD);
static_assert(offsetof(G_WeaponBase_t, Ammo.data.IsFriendlyNonCollision) == 0x6AE);
static_assert(offsetof(G_WeaponBase_t, Ammo.data.Size) == 0x6B4);
static_assert(offsetof(G_WeaponBase_t, Ammo.data.HitSizeAdjust) == 0x6B8);
static_assert(offsetof(G_WeaponBase_t, Ammo.data.HitImpulseAdjust) == 0x6BC);
static_assert(offsetof(G_WeaponBase_t, Ammo.data.Color) == 0x6D0);
static_assert(offsetof(G_WeaponBase_t, Ammo.data.GravityFactor) == 0x6E0);
static_assert(offsetof(G_WeaponBase_t, Ammo.data.CustomParameter) == 0x6E8);
static_assert(offsetof(G_WeaponBase_t, Ammo770) == 0x770);
static_assert(offsetof(G_WeaponBase_t, reloadPhase) == 0x8D4);
static_assert(offsetof(G_WeaponBase_t, CurrentAmmoCount) == 0x8E8);
static_assert(offsetof(G_WeaponBase_t, ROFCount) == 0xB40);
static_assert(offsetof(G_WeaponBase_t, reloadTimeCount) == 0xB90);
static_assert(offsetof(G_WeaponBase_t, EnergyChargeCount) == 0xBA4);
static_assert(offsetof(G_WeaponBase_t, reloadFlag) == 0xC38);
static_assert(offsetof(G_WeaponBase_t, addr3144) == 0xC48);
static_assert(sizeof(G_WeaponBase_t) == 0x11A0);
#endif
