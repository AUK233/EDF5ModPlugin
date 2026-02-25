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
	extern int Config_DisplayDamageType;

	CallFunc_HUiHudCommonDataFuncP10 HUiHudBaseFuncP10_4B2FB0;

	void __fastcall ASMreadHUiHudPowerGuage();
	uintptr_t readHUiHudPowerGuageRet;
	void __fastcall ASMhuiHudPowerGuageFuncP10();


	uintptr_t* vft_HUiHudPowerGuage;
	CallFunc_HUiHudCommonData_Free HUiHudPowerGuage_Free;
}

void module_UpdateHUiHudPowerGuage(PBYTE hmodEXE)
{
	// EDF5.exe+4B2FB0
	HUiHudBaseFuncP10_4B2FB0 = (CallFunc_HUiHudCommonDataFuncP10)(hmodEXE + 0x4B2FB0);
	WriteHookToProcess((void*)(hmodEXE + 0xEC8F50), (void*)L"lyt_HudPowerGuageM1.sgo", 48U);

	// old is 0XB20
	int newHPSize = 0xD00;
	static_assert(sizeof(G_HUiHudPowerGuage_t) < 0xD00);
	WriteHookToProcessCheckECX((void*)(hmodEXE + 0x4CAAF7 + 1), &newHPSize, 4U);
	// EDF5.exe+4CB4CC
	hookGameBlockWithInt3((void*)(hmodEXE + 0x4CB4CC), (uintptr_t)ASMreadHUiHudPowerGuage);
	WriteHookToProcess((void*)(hmodEXE + 0x4CB4CC + 15), (void*)&nop3, 3U);
	readHUiHudPowerGuageRet = (uintptr_t)(hmodEXE + 0x4CB4DE);
	// EDF5.exe+4CC914, is vtf+10
	// but now inside it, to prevent the game crash
	hookGameBlockWithInt3((void*)(hmodEXE + 0x4CC914), (uintptr_t)ASMhuiHudPowerGuageFuncP10);
	WriteHookToProcess((void*)(hmodEXE + 0x4CC914 + 15), (void*)&nop6, 6U);

	// set vf table, EDF5.exe+ECAF90
	vft_HUiHudPowerGuage = (uintptr_t*)(hmodEXE + 0xECAF90);

	// Save original Free function
	HUiHudPowerGuage_Free = (CallFunc_HUiHudCommonData_Free)vft_HUiHudPowerGuage[5];
	// Hook Free function
	uintptr_t addrToHook = (uintptr_t)module_HUiHudPowerGuage_Free;
	WriteHookToProcess((void*)&vft_HUiHudPowerGuage[5], &addrToHook, 8U);
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

void __fastcall module_HUiHudPowerGuageFuncP10_add(PG_HUiHudPowerGuage pThis, void* pRDX)
{
	// ok, now it is always present.
	auto pPlayer = (PG_SoldierBase)pThis->pPlayerObject;

	// show fencer's dash count
	if (pPlayer->InputControlType) {
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
	// don't forget it.
	HUiHudBaseFuncP10_4B2FB0(pThis, pRDX);

	// ================================================================
	// check is who will display
	if (!Config_DisplayDamageType) {
		pPlayer->f34InflictDamage = 0;
	} else {
		using namespace DigitRenderer;
		auto pGlobal = GetLocalCurrentPlayersPointer();
		auto bIsSplitScreen = GetIsSplitScreen();

		auto globalPlayer = pGlobal[bIsSplitScreen];
		if (pPlayer != globalPlayer) return;
		
		DigitProcessor_ProcessData();
		togui_MainDisplayInMission();
	}
}
