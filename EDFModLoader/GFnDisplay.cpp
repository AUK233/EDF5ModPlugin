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
#include "utiliy.h"

#include "GFnDisplay.h"

extern PBYTE hmodEXE;

static wchar_t DMGstrN0[] = L"                       \n                       \n                       \n                       ";
wchar_t DMGstr0[] = L"                       \n                       \n                       \n                       ";

extern "C" {
uintptr_t pDMGstr0 = (uintptr_t)&DMGstr0;
}

void __fastcall setDamageString(PBYTE pstr, PBYTE pcolor) {

	if (*(uintptr_t *)(pstr + 0x60) != pDMGstr0) {
		// check that float4 is the required value
		if (*(INT64 *)(pcolor + 0x270) == 4550220892846510047 && *(INT64 *)(pcolor + 0x278) == 4692750812782960574) {
			// then back to white
			float vf[4] = {1.0f, 1.0f, 1.0f, 1.0f};
			memcpy((pcolor + 0x270), &vf, 16U);

			*(uintptr_t *)(pstr + 0x60) = pDMGstr0;
		}
	}
}

extern "C" {
extern uintptr_t playerAddress;
extern float damage_tmp;
}

constexpr auto DAMAGE_DISPLAY_TIME = 60;
struct Damage {
	float value;
	int time;
};
Damage damageNumber;

void displayWeaponDamage() {
	while (1) {
		//playerAddress = GetPointerAddress((uintptr_t)hmodEXE, {0x0125AB68, 0x238, 0x290, 0x10});

		if (playerAddress > 0) {
			if (damage_tmp != 0) {
				// gameTime - damageNumber.time > DAMAGE_DISPLAY_TIME
				if (damageNumber.time < 1) {
					damageNumber.value = -damage_tmp;
					// gameTime = 0;
				} else {
					damageNumber.value -= damage_tmp;
				}
				// Why +1, because the following will be immediately subtracted.
				damageNumber.time = DAMAGE_DISPLAY_TIME + 1;
				damage_tmp = 0;
			}

			memcpy((void *)pDMGstr0, &DMGstrN0, 192U);
			if (damageNumber.time > 0) {
				std::wstring displayText;
				if (damageNumber.value >= 100.0f) {
					displayText = std::format(L"{:.0f}", damageNumber.value);
				} else if (damageNumber.value >= 10.0f) {
					displayText = std::format(L"{:.1f}", damageNumber.value);
				} else {
					displayText = std::format(L"{:.2f}", damageNumber.value);
				}

				size_t strofs = 0;
				size_t strsize = 44;
				if (displayText.size() < 22) {
					strofs = (22 - displayText.size()) * 2;
					strsize = displayText.size() * 2;
				}
				
				memcpy((void *)(pDMGstr0 + strofs), displayText.c_str(), strsize);
				damageNumber.time--;
			}

		} else {
			if (damageNumber.time != 0) {
				damageNumber.time = 0;
			}
		}

		// gameTime++;
		// 20 fps should be enough
		Sleep(50);
	}
}


