#include <windows.h>
#include <psapi.h>
#include <iostream>
#include <iomanip>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <format>
#include <stdexcept>
#include <list>

#include <plog/Log.h>
#include <plog/Initializers/RollingFileInitializer.h>
#include "utiliy.h"

#include "GFnDisplay.h"

extern PBYTE hmodEXE;

static wchar_t DMGstrN0[] = L"                       \n                       \n                       \n                       ";
static wchar_t DMGstr0[] = L"                       \n                       \n                       \n                       ";
static wchar_t DMGstr1[] = L"                       \n                       \n                       \n                       ";


extern "C" {
extern int displayDamageIndex;
extern int displayDamageStatus;
}

uintptr_t pDMGstr0 = (uintptr_t)&DMGstr0;
uintptr_t pDMGstr1 = (uintptr_t)&DMGstr1;

void __fastcall setDamageString(PBYTE pstr, PBYTE pcolor) {

	// check that float4 is the required value
	if (*(INT64 *)(pcolor + 0x270) == 4550220892846510047 && *(INT64 *)(pcolor + 0x278) == 4692750812782960574) {
		// then back to white
		float vf[4] = {1.0f, 1.0f, 1.0f, 1.0f};
		memcpy((pcolor + 0x270), &vf, 16U);

		*(uintptr_t *)(pstr + 0x60) = pDMGstr0;
	} else if (*(INT64 *)(pcolor + 0x270) == 4594572340047290302 && *(INT64 *)(pcolor + 0x278) == 4566650023222005727) {
		// back to white
		float vf[4] = {1.0f, 1.0f, 1.0f, 1.0f};
		memcpy((pcolor + 0x270), &vf, 16U);

		*(uintptr_t *)(pstr + 0x60) = pDMGstr1;
	}
}

extern "C" {
void __fastcall ASMrecordPlayerDamage();
uintptr_t playerDmgRetAddress;
uintptr_t playerAddress = 0;
float damage_tmp = 0;
}

// get player weapon damage
void hookGetPlayerDamage() {
	// file offset = 0x2DAA41
	playerDmgRetAddress = (uintptr_t)(hmodEXE + 0x2DB659);
	hookGameBlock((void *)(hmodEXE + 0x2DB641), (uint64_t)ASMrecordPlayerDamage);
	// we still override original radar
	// use it to ensure that this function is executed correctly
	WriteHookToProcess((void *)(hmodEXE + 0xEC8F18), (void *)L"lyt_HudRaderM1.sgo", 36U);
}

constexpr auto DAMAGE_DISPLAY_TIME_S = 61;
struct Damage {
	float value;
	int time;
};
Damage damageNumber;

// reset string
void displayWeaponDamageReset() {
	damage_tmp = 0;
	damageNumber.time = 0;
	memcpy((void *)pDMGstr0, &DMGstrN0, 192U);
	displayDamageStatus = 0;
	PLOG_INFO << "Display damage number has been reset";
}

std::wstring FormatDamageNumber(const float dmg) {
	if (dmg >= 100.0f) {
		return std::format(L"{:.0f}", dmg);
	} else if (dmg >= 10.0f) {
		return std::format(L"{:.1f}", dmg);
	} else {
		return std::format(L"{:.2f}", dmg);
	}
}

void displayWeaponDamageA() {
	while (displayDamageIndex == 1) {
		playerAddress = GetPointerAddress((uintptr_t)hmodEXE, {0x0125AB68, 0x238, 0x290, 0x10});

		if (playerAddress > 0) {
			if (damage_tmp != 0) {
				if (damageNumber.time < 1) {
					damageNumber.value = -damage_tmp;
				} else {
					damageNumber.value -= damage_tmp;
				}
				damageNumber.time = DAMAGE_DISPLAY_TIME_S;
				damage_tmp = 0;
			}

			memcpy((void *)pDMGstr0, &DMGstrN0, 192U);
			if (damageNumber.time > 0) {
				std::wstring displayText = FormatDamageNumber(damageNumber.value);

				size_t strofs = 0;
				size_t strsize = 44;
				if (displayText.size() < 22) {
					strofs = (22 - displayText.size()) * 2;
					strsize = displayText.size() * 2;
				}
				
				memcpy((void *)(pDMGstr0 + strofs), displayText.c_str(), strsize);
				damageNumber.time--;
			}
			//
		} else {
			if (damageNumber.time != 0) {
				damageNumber.time = 0;
				damage_tmp = 0;
			}
		}
		// 20 fps should be enough
		Sleep(50);
	}
	// If setting change, we first have to reset
	displayWeaponDamageReset();
}

