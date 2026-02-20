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

extern "C" {
	PG_HUiHudTextCommonContent __fastcall HUiHudCommon_GetTextContent(PG_HUiHudCommonText pText);
	size_t __fastcall HUiHudCommon_TextForFormatFloatNumber(const float number, WCHAR* destination, size_t len);
	size_t __fastcall HUiHudCommon_TextForFormatIntNumber(const int number, WCHAR* destination, size_t len);

	// 0 is ReloadTimeColor, 1 is ChargeColor, 2 is ReloadPercentColor
	__m128 __fastcall HUiHudCommon_GetWeaponTextColor(int index);
}
