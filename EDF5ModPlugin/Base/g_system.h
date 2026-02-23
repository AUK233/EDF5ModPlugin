#pragma once
#include "SSE.hpp"

// size is 
typedef struct XGS_System_Camera_t {
	void* vf_table;
	void* pCamera; // yeah, pointer to self
	char pad10[0x50];
	int PlayerID;
	char pad64[0xBC];
	Matrix4x6_t mat120;
	bool bShowHUD, pad181[3]; int pad184;
	void* pointer188; char pad190[0x10];
	Matrix3D_t CameraTransform;
	char pad1e0[0xB0];
	void* pGameObject; char pad298[0x18];
	Matrix3D_t playerTransform; // Read-only, it will be overwritten by other functions.
	MatrixCamera_t playerCamera; // Read-only, it will be overwritten by other functions.
	char pad310[0x80];
	Matrix3D_t mat3d390;
	Matrix3D_t mat3d3D0; // copy from CameraTransform
	char pad410[0x18];
	float playerCurrentTime;
} *PXGS_System_Camera;
#if 1
static_assert(offsetof(XGS_System_Camera_t, PlayerID) == 0x60);
static_assert(offsetof(XGS_System_Camera_t, mat120) == 0x120);
static_assert(offsetof(XGS_System_Camera_t, bShowHUD) == 0x180);
static_assert(offsetof(XGS_System_Camera_t, CameraTransform) == 0x1A0);
static_assert(offsetof(XGS_System_Camera_t, pGameObject) == 0x290);
static_assert(offsetof(XGS_System_Camera_t, playerTransform) == 0x2B0);
static_assert(offsetof(XGS_System_Camera_t, playerCamera) == 0x2F0);
static_assert(offsetof(XGS_System_Camera_t, mat3d390) == 0x390);
static_assert(offsetof(XGS_System_Camera_t, playerCurrentTime) == 0x428);
#endif

typedef struct XGS_System_Player_t {
	void* pad0;
	PXGS_System_Camera pCamera;
	char pad10[0x50];
	char pad60[0x128];
} *PXGS_System_Player;
#if 1
static_assert(sizeof(XGS_System_Player_t) == 0x188);
#endif

// EDF5.exe+125AB68
typedef struct XGS_System_t {
	void* vf_table; // is EDF5.exe+EE0600 
	char pad8[0x218];
	void* pPlayerInfoWhenLoading;
	size_t PlayerInfoWhenLoadingSize; // It will be reset when loading is complete.
	XGS_System_Player_t player[4]; // really?
} *PXGS_System;
#if 1
static_assert(offsetof(XGS_System_t, pPlayerInfoWhenLoading) == 0x220);
static_assert(offsetof(XGS_System_t, player) == 0x230);
#endif

void __fastcall XGS_SystemFunction_Initialize(PBYTE hmodEXE);

PXGS_System __fastcall XGS_GetXGSSystemPointer();
PXGS_System_Player __fastcall XGS_GetSystemPlayerPointer(PXGS_System pSystem, int index);
