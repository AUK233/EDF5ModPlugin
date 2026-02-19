#pragma once
#include "SSE.hpp"

typedef struct G_SceneObject_t {
	void* vf_table;
	char pad8[0x8];
	void* pSelf; // what?! always seems to point self
	char pad18[0x18];
	BYTE flag30[3], pad33;
	float f34InflictDamage; // our value, originally unused
	CRITICAL_SECTION CriticalSection38; // verification required
	char pad60[0x10];
	Matrix3D_t transform;
	void* pointerB0;
	char padB8[8]; // Useless 8-bytes
} *PG_SceneObject;
#if 1
static_assert(offsetof(G_SceneObject_t, flag30) == 0x30);
static_assert(offsetof(G_SceneObject_t, f34InflictDamage) == 0x34);
static_assert(offsetof(G_SceneObject_t, transform) == 0x70);
static_assert(sizeof(G_SceneObject_t) == 0xC0);
#endif

typedef struct G_NetworkObject_t : G_SceneObject_t {
	void* vft_NetworkObject;
	char padC8[0x38];
	MatrixCamera_t base_camera; // modify it will change view.
	char pad120[0x60];
	MatrixCamera_t mapping_camera; // from EDF5.exe+2E01C7
	char pad1A0[0x50];
	float unk1F0, MinHP, MaxHP, CurrentHP;
	BYTE pad200[8]; // +4 always is 100.0f
	float TotalEnergy, CurrentEnergy;
	float game_object_destroy_score_adjust, CurrentTime; // time is seconds, does not increase when paused
	char pad218[8];
	int game_object_vehicle_mask;
	char pad224[0x14];
	int i_Time238; // EDF5.exe+2DA986 unknown function
	char pad23C[0xA4];
	float game_object_encount[2];
	char pad2E8[0x88];
	UINT32 FrameInSecond, FrameRate; // if FrameInSecond >= FrameRate, FrameInSecond = 0;
	char pad378[0x40];
	float game_object_formation[2];
	char pad3C0[0x50];
} *PG_NetworkObject;
#if 1
static_assert(offsetof(G_NetworkObject_t, base_camera) == 0x100);
static_assert(offsetof(G_NetworkObject_t, mapping_camera) == 0x180);
static_assert(offsetof(G_NetworkObject_t, unk1F0) == 0x1F0);
static_assert(offsetof(G_NetworkObject_t, game_object_destroy_score_adjust) == 0x210);
static_assert(offsetof(G_NetworkObject_t, CurrentTime) == 0x214);
static_assert(offsetof(G_NetworkObject_t, game_object_vehicle_mask) == 0x220);
static_assert(offsetof(G_NetworkObject_t, i_Time238) == 0x238);
static_assert(offsetof(G_NetworkObject_t, game_object_encount) == 0x2E0);
static_assert(offsetof(G_NetworkObject_t, FrameInSecond) == 0x370);
static_assert(offsetof(G_NetworkObject_t, game_object_formation) == 0x3B8);
static_assert(sizeof(G_NetworkObject_t) == 0x410);
#endif

void __fastcall XGS_BaseObject_Initialize(PBYTE hmodEXE);
