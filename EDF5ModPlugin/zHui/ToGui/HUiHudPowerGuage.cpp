#include <windows.h>
#include <string>
#include <vector>
#include <format>
#include <stdexcept>
#include <list>
#include <cstdlib>

#include "utiliy.h"
#include "commonNOP.h"

#include "0SetImGui.h"
#include "1DigitProcessor.h"

#include "HUiHudPowerGuage.h"

extern "C" {
	void __fastcall ASMreadHUiHudPowerGuage();
	uintptr_t readHUiHudPowerGuageRet;

	uintptr_t* vft_HUiHudPowerGuage;
	CallFunc_HUiHudCommonDataFuncP10 HUiHudPowerGuageFuncP10;
	CallFunc_HUiHudCommonData_Free HUiHudPowerGuage_Free;
}

void module_UpdateHUiHudPowerGuage(PBYTE hmodEXE)
{
	WriteHookToProcess((void*)(hmodEXE + 0xEC8F50), (void*)L"lyt_HudPowerGuageM1.sgo", 48U);

	// old is 0XB20
	int newHPSize = 0xD00;
	static_assert(sizeof(G_HUiHudPowerGuage_t) < 0xD00);
	WriteHookToProcessCheckECX((void*)(hmodEXE + 0x4CAAF7 + 1), &newHPSize, 4U);
	// EDF5.exe+4CB4CC
	hookGameBlock((void*)(hmodEXE + 0x4CB4CC), (uintptr_t)ASMreadHUiHudPowerGuage);
	WriteHookToProcess((void*)(hmodEXE + 0x4CB4CC + 12), (void*)&nop6, 6U);
	readHUiHudPowerGuageRet = (uintptr_t)(hmodEXE + 0x4CB4DE);

	// set vf table, EDF5.exe+ECAF90
	vft_HUiHudPowerGuage = (uintptr_t*)(hmodEXE + 0xECAF90);

	// Save original +10 function
	HUiHudPowerGuageFuncP10 = (CallFunc_HUiHudCommonDataFuncP10)vft_HUiHudPowerGuage[2];
	// Hook +10 function
	uintptr_t addrToHook = (uintptr_t)module_HUiHudPowerGuageFuncP10;
	WriteHookToProcess((void*)&vft_HUiHudPowerGuage[2], &addrToHook, 8U);

	// Save original Free function
	HUiHudPowerGuage_Free = (CallFunc_HUiHudCommonData_Free)vft_HUiHudPowerGuage[5];
	// Hook Free function
	addrToHook = (uintptr_t)module_HUiHudPowerGuage_Free;
	WriteHookToProcess((void*)&vft_HUiHudPowerGuage[5], &addrToHook, 8U);
}

void __fastcall module_HUiHudPowerGuageFuncP10(PG_HUiHudPowerGuage pThis, void* pRDX)
{
	auto pPlayer = (PG_SoldierBase)pThis->pPlayerObject;

	// show fencer's dash count
	if (pPlayer && pThis->bIsActivated760 && pPlayer->InputControlType) {
		auto fencer = (PG_HeavyArmor)pPlayer;

		auto pDash = pThis->TextFencerDash;
		if (pDash) {
			auto pText = HUiHudCommon_GetTextContent(pDash);
			ZeroMemory(&pText->text, 16U);
			if (fencer->DashCurrentCount > 0) {
				memcpy(&pDash->font_color, &pThis->TextFencerDashColor, 16U);
				HUiHudCommon_TextForFormatIntNumber(fencer->DashCurrentCount, (WCHAR*)&pText->text, 7);
			} else {
				auto ReloadTimeColor = HUiHudCommon_GetWeaponTextColor(0);
				_mm_store_ps(pDash->font_color, ReloadTimeColor);
				float remainTime = fencer->DashRecoveryRemainTime / 60.0f;
				HUiHudCommon_TextForFormatFloatNumber(remainTime, (WCHAR*)&pText->text, 7);
			}
		}

		auto pBoost = pThis->TextFencerBoost;
		if (pBoost) {
			auto pText = HUiHudCommon_GetTextContent(pBoost);
			ZeroMemory(&pText->text, 16U);
			if (fencer->BoostCurrentCount > 0) {
				memcpy(&pBoost->font_color, &pThis->TextFencerBoostColor, 16U);
				HUiHudCommon_TextForFormatIntNumber(fencer->BoostCurrentCount, (WCHAR*)&pText->text, 7);
			} else {
				auto ReloadTimeColor = HUiHudCommon_GetWeaponTextColor(0);
				_mm_store_ps(pBoost->font_color, ReloadTimeColor);
				float remainTime = fencer->BoostRecoveryRemainTime / 60.0f;
				HUiHudCommon_TextForFormatFloatNumber(remainTime, (WCHAR*)&pText->text, 7);
			}
		}
		// end
	}

	// it must now be placed at the end, because it has a function that must be executed in the final step.
	HUiHudPowerGuageFuncP10(pThis, pRDX);
	if (!pPlayer) return;
	if (!pThis->bIsActivated760) return;

	// ================================================================
	// check is who will display
	using namespace DigitRenderer;
	auto pGlobal = GetLocalCurrentPlayersPointer();
	auto bIsSplitScreen = GetIsSplitScreen();

	auto globalPlayer = pGlobal[bIsSplitScreen];
	if (pPlayer != globalPlayer) return;

	DigitProcessor_ProcessData();
	togui_MainDisplayInMission();
}

void* __fastcall module_HUiHudPowerGuage_Free(PG_HUiHudPowerGuage pThis, int isFree)
{
	if (isFree & 1) {
		// clear global player pointer
		auto pObject = pThis->pPlayerObject;

		if (pObject) {
			using namespace DigitRenderer;
			auto pGlobal = GetLocalCurrentPlayersPointer();

			if (pObject == pGlobal[0] || pObject == pGlobal[1]) {
				DigitProcessor_ClearData(0);
				DigitProcessor_ClearData(1);
			}
		}
		// end
	}

	return HUiHudPowerGuage_Free(pThis, isFree);
}
