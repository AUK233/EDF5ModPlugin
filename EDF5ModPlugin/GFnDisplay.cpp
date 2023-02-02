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
static wchar_t DMGstrN1[] = L"               \n               \n               \n               \n               \n               \n               ";
static wchar_t DMGstrN2[] = L"               \n               \n               \n               \n               \n               \n               \n               ";

extern "C" {
extern int displayDamageIndex;
extern int displayDamageStatus;
}
/*
struct DamageString {
	uintptr_t pstr;
	uintptr_t pcolor;
};*/

std::vector<uintptr_t> pDMGstr0;
std::vector<uintptr_t> pDMGstr0C;
std::vector<uintptr_t> pDMGstr1;
std::vector<uintptr_t> pDMGstr1C;
std::vector<uintptr_t> pDMGstr2;
std::vector<uintptr_t> pDMGstr2C;

void __fastcall setDamageString(uintptr_t pstr, uintptr_t pcolor) {
	uintptr_t pText = *(uintptr_t *)(pstr + 0x60);
	UINT32 textSize = *(UINT32 *)(pstr + 0x80);
	// check that float4 is the required value
	if (*(INT64 *)(pcolor + 0x270) == 4550220892846510047 && *(INT64 *)(pcolor + 0x278) == 4692750812782960574) {
		// then back to white
		float vf[4] = {1.0f, 1.0f, 1.0f, 1.0f};
		memcpy((void *)(pcolor + 0x270), &vf, 16U);

		if (textSize == 95) {
			memcpy((void *)pText, &DMGstrN0, 192U);
			pDMGstr0.push_back(pText);
		} else if (textSize == 111) {
			memcpy((void *)pText, &DMGstrN1, 224U);
			pDMGstr1.push_back(pText);
		} else if (textSize == 127) {
			memcpy((void *)pText, &DMGstrN2, 256U);
			pDMGstr2.push_back(pText);
		}
	} else if (*(INT64 *)(pcolor + 0x270) == 4594572340047290302 && *(INT64 *)(pcolor + 0x278) == 4566650023222005727) {
		// back to white
		float vf[4] = {1.0f, 1.0f, 1.0f, 1.0f};
		memcpy((void *)(pcolor + 0x270), &vf, 16U);

		if (textSize == 95) {
			memcpy((void *)pText, &DMGstrN0, 192U);
			pDMGstr0C.push_back(pText);
		} else if (textSize == 111) {
			memcpy((void *)pText, &DMGstrN1, 224U);
			pDMGstr1C.push_back(pText);
		} else if (textSize == 127) {
			memcpy((void *)pText, &DMGstrN2, 256U);
			pDMGstr2C.push_back(pText);
		}
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
	// WriteHookToProcess((void *)(hmodEXE + 0xEC8F18), (void *)L"lyt_HudRaderM1.sgo", 36U);
}

// set damage display duration (short)
constexpr int DAMAGE_DISPLAY_TIME_S = 61;
// set damage display duration (long)
constexpr int DAMAGE_DISPLAY_TIME_L = 71;
// set rechargeable damage display duration (short)
constexpr int DAMAGE_CHARGE_TIME_S = 10;
// set rechargeable damage display duration (long)
constexpr int DAMAGE_CHARGE_TIME_L = 15;
struct Damage {
	float value;
	int time;
};
// separate damage
Damage damageNumber;
// damage group
Damage dmgNumGroup[8];

// reset string
void __fastcall displayWeaponDamageReset() {
	damage_tmp = 0;
	damageNumber.time = 0;
	for (int i = 0; i < 8; i++) {
		dmgNumGroup[i].time = 0;
	}

	for (size_t i = 0; i < pDMGstr0.size(); i++) {
		memcpy((void *)pDMGstr0[i], &DMGstrN0, 192U);
	}
	for (size_t i = 0; i < pDMGstr0C.size(); i++) {
		memcpy((void *)pDMGstr0C[i], &DMGstrN0, 192U);
	}
	for (size_t i = 0; i < pDMGstr1.size(); i++) {
		memcpy((void *)pDMGstr1[i], &DMGstrN1, 224U);
	}
	for (size_t i = 0; i < pDMGstr1C.size(); i++) {
		memcpy((void *)pDMGstr1C[i], &DMGstrN1, 224U);
	}
	for (size_t i = 0; i < pDMGstr2.size(); i++) {
		memcpy((void *)pDMGstr2[i], &DMGstrN2, 256U);
	}
	for (size_t i = 0; i < pDMGstr2C.size(); i++) {
		memcpy((void *)pDMGstr2C[i], &DMGstrN2, 256U);
	}

	displayDamageStatus = 0;
	PLOG_INFO << "Display damage number has been reset";
}

std::wstring __fastcall FormatDamageNumber(const float dmg) {
	if (dmg >= 100.0f) {
		return std::format(L"{:.0f}", dmg);
	} else if (dmg >= 10.0f) {
		return std::format(L"{:.1f}", dmg);
	} else {
		return std::format(L"{:.2f}", dmg);
	}
}

void __fastcall DMGCommonClear() {
	damageNumber.time = 0;
	for(int i = 0; i < 8; i++) {
		dmgNumGroup[i].time = 0;
	}
	damage_tmp = 0;

	pDMGstr0.clear();
	pDMGstr0C.clear();
	pDMGstr1.clear();
	pDMGstr1C.clear();
	pDMGstr2.clear();
	pDMGstr2C.clear();
}

void __fastcall setChagreDamageTime(int time) {
	if (damageNumber.time < 1) {
		damageNumber.value = -damage_tmp;
	} else {
		damageNumber.value -= damage_tmp;
	}
	damageNumber.time = time;
	damage_tmp = 0;
}

void __fastcall setDamageDisplayTime(int vstart, int vend, int time) {
	for (int i = vstart; i < vend; i++) {
		if (dmgNumGroup[i + 1].time > 0) {
			dmgNumGroup[i].value = dmgNumGroup[i + 1].value;
			dmgNumGroup[i].time = dmgNumGroup[i + 1].time + time;
		}
	}
	dmgNumGroup[vend].value = damageNumber.value;
	dmgNumGroup[vend].time = time;
}

void __fastcall displayWeaponDamageA1() {
	while (displayDamageIndex == 1) {
		playerAddress = GetPointerAddress((uintptr_t)hmodEXE, {0x0125AB68, 0x238, 0x290, 0x10});

		if (playerAddress > 0) {
			if (pDMGstr0.size() > 0) {
				if (damage_tmp != 0) {
					setChagreDamageTime(DAMAGE_DISPLAY_TIME_S);
				}

				for (size_t i = 0; i < pDMGstr0.size(); i++) {
					memcpy((void*)pDMGstr0[i], &DMGstrN0, 192U);
				}

				if (damageNumber.time > 0) {
					std::wstring displayText = FormatDamageNumber(damageNumber.value);

					size_t strofs = 0;
					size_t strsize = 44;
					if (displayText.size() < 22) {
						strofs = (22 - displayText.size()) * 2;
						strsize = displayText.size() * 2;
					}

					for (size_t i = 0; i < pDMGstr0.size(); i++) {
						memcpy((void *)(pDMGstr0[i] + strofs), displayText.c_str(), strsize);
					}
					damageNumber.time--;
				}
			}
			//
		} else {
			if (pDMGstr0.size() > 0) {
				DMGCommonClear();
			}
		}
		// 20 fps should be enough
		Sleep(50);
	}
	// If setting change, we first have to reset
	displayWeaponDamageReset();
}

void __fastcall displayWeaponDamageA2() {
	while (displayDamageIndex == 11) {
		playerAddress = GetPointerAddress((uintptr_t)hmodEXE, {0x0125AB68, 0x238, 0x290, 0x10});

		if (playerAddress > 0) {
			if (pDMGstr0.size() > 0 && pDMGstr0C.size() > 0) {
				if (damage_tmp != 0) {
					setChagreDamageTime(DAMAGE_CHARGE_TIME_L);
				}

				for (size_t i = 0; i < pDMGstr0.size(); i++) {
					memcpy((void *)pDMGstr0[i], &DMGstrN0, 192U);
				}
				for (size_t i = 0; i < pDMGstr0C.size(); i++) {
					memcpy((void *)pDMGstr0C[i], &DMGstrN0, 192U);
				}

				if (damageNumber.time > 0) {
					std::wstring displayText = FormatDamageNumber(damageNumber.value);

					size_t strofs = 0;
					size_t strsize = 44;
					if (displayText.size() < 22) {
						strofs = (22 - displayText.size()) * 2;
						strsize = displayText.size() * 2;
					}

					for (size_t i = 0; i < pDMGstr0.size(); i++) {
						memcpy((void *)(pDMGstr0[i] + strofs), displayText.c_str(), strsize);
					}
					damageNumber.time--;
				} else if (damageNumber.value > 0) {
					setDamageDisplayTime(0, 3, DAMAGE_DISPLAY_TIME_S);

					damageNumber.value = 0;
				}

				for (int i = 0; i < 4; i++) {
					if (dmgNumGroup[i].time > 0) {
						std::wstring displayText = FormatDamageNumber(dmgNumGroup[i].value);

						size_t strofs = 0;
						size_t strsize = 44;
						if (displayText.size() < 22) {
							strofs = (22 - displayText.size()) * 2;
							strsize = displayText.size() * 2;
						}
						strofs += (size_t)i * 48;

						for (size_t j = 0; j < pDMGstr0C.size(); j++) {
							memcpy((void *)(pDMGstr0C[j] + strofs), displayText.c_str(), strsize);
						}
						dmgNumGroup[i].time--;
					}
				}
				//
			}
			//
		} else {
			if (pDMGstr0.size() > 0) {
				DMGCommonClear();
			}
		}
		// 20 fps should be enough
		Sleep(50);
	}
	// If setting change, we first have to reset
	displayWeaponDamageReset();
}

void __fastcall displayWeaponDamageB1() {
	while (displayDamageIndex == 2) {
		playerAddress = GetPointerAddress((uintptr_t)hmodEXE, {0x0125AB68, 0x238, 0x290, 0x10});

		if (playerAddress > 0) {
			if (pDMGstr1.size() > 0) {
				if (damage_tmp != 0) {
					setChagreDamageTime(DAMAGE_DISPLAY_TIME_S);
				}

				for (size_t i = 0; i < pDMGstr1.size(); i++) {
					memcpy((void *)pDMGstr1[i], &DMGstrN1, 224U);
				}
				for (size_t i = 0; i < pDMGstr2.size(); i++) {
					memcpy((void *)pDMGstr2[i], &DMGstrN2, 256U);
				}

				if (damageNumber.time > 0) {
					std::wstring displayText = FormatDamageNumber(damageNumber.value);

					size_t strofs = 0;
					size_t strsize = 28;
					if (displayText.size() < 14) {
						strofs = ((14 - displayText.size()) / 2) * 2;
						strsize = displayText.size() * 2;
					}

					for (size_t i = 0; i < pDMGstr1.size(); i++) {
						memcpy((void *)(pDMGstr1[i] + strofs), displayText.c_str(), strsize);
					}
					for (size_t i = 0; i < pDMGstr2.size(); i++) {
						memcpy((void *)(pDMGstr2[i] + 224U), displayText.c_str(), strsize);
					}

					damageNumber.time--;
				}
			}
			//
		} else {
			if (pDMGstr1.size() > 0 || pDMGstr2.size() > 0) {
				DMGCommonClear();
			}
		}
		// 20 fps should be enough
		Sleep(50);
	}
	// If setting change, we first have to reset
	displayWeaponDamageReset();
}

void __fastcall displayWeaponDamageB2() {
	while (displayDamageIndex == 21) {
		playerAddress = GetPointerAddress((uintptr_t)hmodEXE, {0x0125AB68, 0x238, 0x290, 0x10});

		if (playerAddress > 0) {
			if (pDMGstr1.size() > 0) {
				if (damage_tmp != 0) {
					setChagreDamageTime(DAMAGE_CHARGE_TIME_S);
				}

				for (size_t i = 0; i < pDMGstr1.size(); i++) {
					memcpy((void *)pDMGstr1[i], &DMGstrN1, 224U);
				}
				for (size_t i = 0; i < pDMGstr1C.size(); i++) {
					memcpy((void *)pDMGstr1C[i], &DMGstrN1, 224U);
				}
				for (size_t i = 0; i < pDMGstr2.size(); i++) {
					memcpy((void *)pDMGstr2[i], &DMGstrN2, 256U);
				}
				for (size_t i = 0; i < pDMGstr2C.size(); i++) {
					memcpy((void *)pDMGstr2C[i], &DMGstrN2, 256U);
				}

				if (damageNumber.time > 0) {
					std::wstring displayText = FormatDamageNumber(damageNumber.value);

					size_t strofs = 0;
					size_t strsize = 28;
					if (displayText.size() < 14) {
						strofs = ((14 - displayText.size()) / 2) * 2;
						strsize = displayText.size() * 2;
					}

					for (size_t i = 0; i < pDMGstr1.size(); i++) {
						memcpy((void *)(pDMGstr1[i] + strofs), displayText.c_str(), strsize);
					}
					for (size_t i = 0; i < pDMGstr2.size(); i++) {
						memcpy((void *)(pDMGstr2[i] + 224U), displayText.c_str(), strsize);
					}
					damageNumber.time--;
				} else if (damageNumber.value > 0) {
					setDamageDisplayTime(0, 7, DAMAGE_DISPLAY_TIME_L);

					damageNumber.value = 0;
				}

				for (int i = 0; i < 8; i++) {
					if (dmgNumGroup[i].time > 0) {
						std::wstring displayText = FormatDamageNumber(dmgNumGroup[i].value);

						size_t strofs = 0;
						size_t strsize = 28;
						if (displayText.size() < 14) {
							strofs = ((14 - displayText.size()) / 2) * 2;
							strsize = displayText.size() * 2;
						}
						strofs += (size_t)i * 32;
						for (size_t j = 0; j < pDMGstr1C.size(); j++) {
							memcpy((void *)(pDMGstr1C[j] + strofs), displayText.c_str(), strsize);
						}

						size_t strofs2 = 32;
						strofs2 += (size_t)i * 32;
						for (size_t j = 0; j < pDMGstr2C.size(); j++) {
							memcpy((void *)(pDMGstr2C[j] + strofs2), displayText.c_str(), strsize);
						}

						dmgNumGroup[i].time--;
					}
				}
				//
			}
			//
		} else {
			if (pDMGstr1.size() > 0 || pDMGstr2.size() > 0) {
				DMGCommonClear();
			}
		}
		// 20 fps should be enough
		Sleep(50);
	}
	// If setting change, we first have to reset
	displayWeaponDamageReset();
}

// reset values in real time read configuration
void __fastcall displayWeaponDamageNull() {
	while (displayDamageIndex == 0) {
		playerAddress = GetPointerAddress((uintptr_t)hmodEXE, {0x0125AB68, 0x238, 0x290, 0x10});

		if (playerAddress > 0) {
			for (size_t i = 0; i < pDMGstr0.size(); i++) {
				memcpy((void *)pDMGstr0[i], &DMGstrN0, 192U);
			}
			for (size_t i = 0; i < pDMGstr0C.size(); i++) {
				memcpy((void *)pDMGstr0C[i], &DMGstrN0, 192U);
			}

			for (size_t i = 0; i < pDMGstr1.size(); i++) {
				memcpy((void *)pDMGstr1[i], &DMGstrN1, 224U);
			}
			for (size_t i = 0; i < pDMGstr1C.size(); i++) {
				memcpy((void *)pDMGstr1C[i], &DMGstrN1, 224U);
			}
			for (size_t i = 0; i < pDMGstr2.size(); i++) {
				memcpy((void *)pDMGstr2[i], &DMGstrN2, 256U);
			}
			for (size_t i = 0; i < pDMGstr2C.size(); i++) {
				memcpy((void *)pDMGstr2C[i], &DMGstrN2, 256U);
			}
		} else {
			if (pDMGstr0.size() > 0 || pDMGstr1.size() > 0 || pDMGstr2.size() > 0) {
				DMGCommonClear();
			}
		}
		// Execute twice per second
		Sleep(500);
	}
	// If setting change, we first have to reset
	displayWeaponDamageReset();
}

