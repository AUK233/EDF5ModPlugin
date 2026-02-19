#pragma once
#include "Base/SSE.hpp"

typedef void(__fastcall* CallFunc_HUiHudCommonDataFuncP10)(void* pThis, void* pRDX);
typedef void*(__fastcall* CallFunc_HUiHudCommonData_Free)(void* pThis, int isFree); // +2C

typedef struct G_HUiHudCommonData_t {
	void* vf_table;
	char pad8[0x758];
	bool bIsActivated760, pad761[7];
	void* pPlayerObject;
	BYTE pad770[0x78];
	void* pPlayerWeapon; // vehicle may not have it.
} *PHUiHudCommonData;
#if 1
static_assert(offsetof(G_HUiHudCommonData_t, bIsActivated760) == 0x760);
static_assert(offsetof(G_HUiHudCommonData_t, pPlayerObject) == 0x768);
static_assert(offsetof(G_HUiHudCommonData_t, pPlayerWeapon) == 0x7E8);
#endif

typedef struct G_HUiHudTextCommonContent_t {
	BYTE pad1[0x18];
	float fontSize[2];
	BYTE pad2[0x40];
	// maybe is wstring
	//std::wstring text;
	WCHAR* text;
	BYTE pad3[0x18];
	size_t textLength;
} *PG_HUiHudTextCommonContent;
#if 1
static_assert(offsetof(G_HUiHudTextCommonContent_t, fontSize) == 0x18);
static_assert(offsetof(G_HUiHudTextCommonContent_t, text) == 0x60);
static_assert(offsetof(G_HUiHudTextCommonContent_t, textLength) == 0x80);
#endif

typedef struct G_HUiHudCommonText_t {
	BYTE pad1[0x1A0];
	float RawPos[4];
	float RawMatrixSize[4];
	BYTE pad2[0x68];
	PG_HUiHudTextCommonContent** ppTextContent;
	BYTE pad3[0x40];
	float font_color[4];
} *PG_HUiHudCommonText;
#if 1
static_assert(offsetof(G_HUiHudCommonText_t, RawPos) == 0x1A0);
static_assert(offsetof(G_HUiHudCommonText_t, RawMatrixSize) == 0x1B0);
static_assert(offsetof(G_HUiHudCommonText_t, ppTextContent) == 0x228);
static_assert(offsetof(G_HUiHudCommonText_t, font_color) == 0x270);
#endif

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

extern "C" {
	PG_HUiHudTextCommonContent __fastcall HUiHudCommon_GetTextContent(PG_HUiHudCommonText pText);
	size_t __fastcall HUiHudCommon_TextForFormatFloatNumber(const float number, WCHAR* destination, size_t len);
	size_t __fastcall HUiHudCommon_TextForFormatIntNumber(const int number, WCHAR* destination, size_t len);

	// 0 is ReloadTimeColor, 1 is ChargeColor, 2 is ReloadPercentColor
	__m128 __fastcall HUiHudCommon_GetWeaponTextColor(int index);
}
