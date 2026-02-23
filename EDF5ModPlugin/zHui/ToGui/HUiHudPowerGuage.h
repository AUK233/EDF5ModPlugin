#pragma once
#include "../HUiHudCommonStruct.h"

typedef struct G_HUiHudPowerGuage_t : G_HUiHudCommonData_t {
	BYTE pad7F0[0x130];
	// aka Current HP
	PG_HUiHudCommonText PowText;
	void* PowTextCheck;
	// aka Max HP
	PG_HUiHudCommonText DefText;
	void* DefTextCheck;
	BYTE pad2[0x2C0];
	// new
	PG_HUiHudCommonText TextFencerDash;
	void* TextFencerDashCheck;
	float TextFencerDashColor[4];
	PG_HUiHudCommonText TextFencerBoost;
	void* TextFencerBoostCheck;
	float TextFencerBoostColor[4];
	BYTE padEnd[0x10];
} *PG_HUiHudPowerGuage;
#if 1
static_assert(offsetof(G_HUiHudPowerGuage_t, PowText) == 0x920);
static_assert(offsetof(G_HUiHudPowerGuage_t, PowTextCheck) == 0x928);
static_assert(offsetof(G_HUiHudPowerGuage_t, DefText) == 0x930);
static_assert(offsetof(G_HUiHudPowerGuage_t, DefTextCheck) == 0x938);
static_assert(offsetof(G_HUiHudPowerGuage_t, TextFencerDash) == 0xC00);
static_assert(offsetof(G_HUiHudPowerGuage_t, TextFencerDashCheck) == 0xC08);
static_assert(offsetof(G_HUiHudPowerGuage_t, TextFencerDashColor) == 0xC10);
static_assert(offsetof(G_HUiHudPowerGuage_t, TextFencerBoost) == 0xC20);
static_assert(offsetof(G_HUiHudPowerGuage_t, TextFencerBoostCheck) == 0xC28);
static_assert(offsetof(G_HUiHudPowerGuage_t, TextFencerBoostColor) == 0xC30);
#endif

void module_UpdateHUiHudPowerGuage(PBYTE hmodEXE);

// +2C
void* __fastcall module_HUiHudPowerGuage_Free(PG_HUiHudPowerGuage pThis, int isFree);

extern "C"{
	// update text display
	void __fastcall module_HUiHudPowerGuageFuncP10_add(PG_HUiHudPowerGuage pThis, void* pRDX);
}
