#pragma once

// size is 0xC20, Initialize at edf.dll+F488E
typedef struct XGS_System_Camera_t {
	void* vf_table;
	char pad8[0x288];
	void* pGameObject;
} *PXGS_System_Camera;
#if 1
static_assert(offsetof(XGS_System_Camera_t, pGameObject) == 0x290);
#endif

typedef struct XGS_System_Player_t {
	void* pad0;
	PXGS_System_Camera pCamera;
	char pad10[0x50];
	int PlayerID;
	char pad64[0x124];
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
