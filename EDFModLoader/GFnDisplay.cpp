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
static wchar_t DMGstr0[] = L"                       \n                       \n                       \n                       ";

extern "C" {
static uintptr_t pDMGstr0 = (uintptr_t)&DMGstr0;
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
void __fastcall ASMrecordPlayerDamage();
void __fastcall ASMwww1();
uintptr_t playerDmgRetAddress;
uintptr_t playerAddress;
float damage_tmp = 0.0f;
}

void __fastcall ASMwww1() {
	playerAddress = GetPointerAddress((uintptr_t)hmodEXE, {0x0125AB68, 0x238, 0x290, 0x10});
}

// get player weapon damage
void hookGetPlayerDamage() {

	// Then, get the damage
	void *originalFunctionAddr = (void *)(sigscan(L"EDF5.exe", "\xF3\x0F\x58\x87\xFC\x01\x00\x00", "xxxxxxxx"));
	playerDmgRetAddress = (uint64_t)originalFunctionAddr + 0x8;

	void *memoryBlock = AllocatePageNearAddress(originalFunctionAddr);

	uint8_t hookFunction[] = {
	    0x48, 0xB8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // mov rax, addr
	    0xFF, 0xE0                                                  // jmp rax
	};
	uint64_t addrToJumpTo64 = (uint64_t)ASMrecordPlayerDamage;

	memcpy(&hookFunction[2], &addrToJumpTo64, sizeof(addrToJumpTo64));
	memcpy(memoryBlock, hookFunction, sizeof(hookFunction));

	uint8_t jmpInstruction[5] = {0xE9, 0x0, 0x0, 0x0, 0x0};
	const uint64_t relAddr = (uint64_t)memoryBlock - ((uint64_t)originalFunctionAddr + sizeof(jmpInstruction));
	memcpy(jmpInstruction + 1, &relAddr, 4);

	WriteHookToProcess(originalFunctionAddr, jmpInstruction, sizeof(jmpInstruction));
}

constexpr auto DAMAGE_DISPLAY_TIME = 60;
struct Damage {
	float value;
	int time;
};
Damage damageNumber;

void displayWeaponDamage() {
	while (1) {
		playerAddress = GetPointerAddress((uintptr_t)hmodEXE, {0x0125AB68, 0x238, 0x290, 0x10});

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


