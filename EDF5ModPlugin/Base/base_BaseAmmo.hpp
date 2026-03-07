#pragma once
#include "SSE.hpp"

// EDF5.exe+141970
typedef struct G_BulletCommonData_t {
	char pad00[0x44];
	float Speed; int Alive; float Damage;
	float DamageReduceMin, DamageReduceFactor, Explosion;
	bool IsPenetration; // 0 is no, 1 is has
	bool IsFriendlyFire; // 0 is none, 1 is has
	bool IsFriendlyNonCollision; // 0 is has, 1 is none
	char pad6AF[5];
	float Size, HitSizeAdjust, HitImpulseAdjust; char pad6C0[0x10];
	float Color[4];
	float GravityFactor;
	char pad94[4];
	void* CustomParameter;
	char padA0[0x40];
} *PG_BulletCommonData;
#if 1
static_assert(offsetof(G_BulletCommonData_t, Damage) == 0x4C);
static_assert(offsetof(G_BulletCommonData_t, CustomParameter) == 0x98);
static_assert(sizeof(G_BulletCommonData_t) == 0xE0);
#endif

// EDF5.exe+A6B70
typedef struct G_BulletBase_t {
	void* vf_table;
	char pad8[0x28];
	G_BulletCommonData_t data;
	char pad110[0x40];
} *PG_BulletBase;
#if 1
static_assert(offsetof(G_BulletBase_t, data) == 0x30);
static_assert(sizeof(G_BulletBase_t) == 0x150);
#endif


typedef struct xgs_DamageData_t {
	char pad00[0x10];
	void* pFather;
	char pad18[0xC];
	int TeamID; int pad28[2];
	__m128 pos; __m128 pad40;
	float damage;
	char pad54[0x24];
	__m128* pHitPos;
} *Pxgs_DamageData;
#if 1
static_assert(offsetof(xgs_DamageData_t, pFather) == 0x10);
static_assert(offsetof(xgs_DamageData_t, TeamID) == 0x24);
static_assert(offsetof(xgs_DamageData_t, pos) == 0x30);
static_assert(offsetof(xgs_DamageData_t, damage) == 0x50);
static_assert(offsetof(xgs_DamageData_t, pHitPos) == 0x78);
//static_assert(sizeof(xgs_DamageData_t) == 160);
#endif
