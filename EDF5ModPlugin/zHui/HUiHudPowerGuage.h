#pragma once
#include "HUiHudCommonStruct.h"

void module_UpdateHUiHudPowerGuage(PBYTE hmodEXE);

// update text display
void __fastcall module_HUiHudPowerGuageFuncP10(PG_HUiHudPowerGuage pThis, void* pRDX);
// +2C
void* __fastcall module_HUiHudPowerGuage_Free(PG_HUiHudPowerGuage pThis, int isFree);
