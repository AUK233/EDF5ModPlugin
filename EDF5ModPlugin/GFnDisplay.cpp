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

const wchar_t DMGstrN0[] = L"                       \n                       \n                       \n                        ";
const wchar_t DMGstrN1[] = L"               \n               \n               \n               \n               \n               \n                ";
const wchar_t DMGstrN2[] = L"               \n               \n               \n               \n               \n               \n               \n        ";

//static wchar_t DMGstrN1RS[] = L"Init:Damage1...............\n...........................\n...........................\n...........................";
//static wchar_t DMGstrN2RS[] = L"Init:Damage1..................0...............................\n...............................0................................";

extern "C" {
extern int displayDamageIndex;
extern int displayDamageStatus;
}
/*
struct DamageString {
	uintptr_t pstr;
	uintptr_t pcolor;
};*/

// fs suffix is resized text.
uintptr_t pDMGstr0 = 0;
uintptr_t pDMGstr0C = 0;
uintptr_t pDMGstr0fs = 0;
float DMGstr0fs = 1.0f;
// on class weapon
uintptr_t pDMGstr1 = 0;
uintptr_t pDMGstr1C = 0;
uintptr_t pDMGstr1fs = 0;
float DMGstr1fs = 1.0f;
// on vehicle weapon
uintptr_t pDMGstr2 = 0;
uintptr_t pDMGstr2C = 0;
uintptr_t pDMGstr2fs = 0;
float DMGstr2fs = 1.0f;

uintptr_t __fastcall setDamageString(uintptr_t pstr, uintptr_t pcolor, uintptr_t rspBackup) {
	uintptr_t pText = *(uintptr_t *)(pstr + 0x60);
	UINT32 textSize = *(UINT32 *)(pstr + 0x80);
	// check that float4 is the required value
	// ok, now memcpy is a multiple of 16
	if (*(INT32 *)(pcolor + 0x270) == 0x3FAD4FDF && *(INT32 *)(pcolor + 0x274) == 0x3F25A1CB && *(INT32 *)(pcolor + 0x278) == 0x3F5A9FBE && *(INT32 *)(pcolor + 0x27C) == 0x41200000) {
		// if (*(INT64 *)(pcolor + 0x270) == 4550220892846510047 && *(INT64 *)(pcolor + 0x278) == 4692750812782960574)
		// then back to white
		float vf[4] = {1.0f, 1.0f, 1.0f, 1.0f};
		memcpy((void *)(pcolor + 0x270), &vf, 16U);

		if (textSize == 96U) {
			memcpy((void *)pText, &DMGstrN0, 192U);
			if (pDMGstr0 == 0) {
				pDMGstr0fs = pstr + 0x18;
				DMGstr0fs = *(float *)(pstr + 0x18);
				pDMGstr0 = pText;
			}
		} else if (textSize == 112U) {
			memcpy((void *)pText, &DMGstrN1, 224U);
			if (pDMGstr1 == 0) {
				pDMGstr1fs = pstr + 0x18;
				DMGstr1fs = *(float *)(pstr + 0x18);
				pDMGstr1 = pText;
			}
		} else if (textSize == 120U) {
			memcpy((void *)pText, &DMGstrN2, 240U);
			if (pDMGstr2 == 0) {
				pDMGstr2fs = pstr + 0x18;
				DMGstr2fs = *(float *)(pstr + 0x18);
				pDMGstr2 = pText;
			}
		}
	} else if (*(INT32 *)(pcolor + 0x270) == 0x3F5A9FBE && *(INT32 *)(pcolor + 0x274) == 0x3FC33333 && *(INT32 *)(pcolor + 0x278) == 0x3FAD4FDF && *(INT32 *)(pcolor + 0x27C) == 0x3F600000) {
		// if (*(INT64 *)(pcolor + 0x270) == 4594572340047290302 && *(INT64 *)(pcolor + 0x278) == 4566650023222005727)
		// back to white
		float vf[4] = {1.0f, 1.0f, 1.0f, 1.0f};
		memcpy((void *)(pcolor + 0x270), &vf, 16U);

		if (textSize == 96U) {
			memcpy((void *)pText, &DMGstrN0, 192U);
			if (pDMGstr0C == 0) {
				pDMGstr0C = pText;
			}
		} else if (textSize == 112U) {
			memcpy((void *)pText, &DMGstrN1, 224U);
			if (pDMGstr1C == 0) {
				pDMGstr1C = pText;
			}
		} else if (textSize == 120U) {
			memcpy((void *)pText, &DMGstrN2, 240U);
			if (pDMGstr2C == 0) {
				pDMGstr2C = pText;
			}
		}
	}

	//PLOG_INFO << "Initializing the damage string is complete! get rsp: " << std::hex << rspBackup;
	return rspBackup;
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
	hookGameBlock14((void *)(hmodEXE + 0x2DB641), (uint64_t)ASMrecordPlayerDamage);
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

void __fastcall displayWeaponDamageClear() {
	
	if (pDMGstr0 > 0) {
		memcpy((void *)pDMGstr0, &DMGstrN0, 192U);
		memcpy((void *)pDMGstr0fs, &DMGstr0fs, 4U);
		memcpy((void *)(pDMGstr0fs + 4), &DMGstr0fs, 4U);
	}
	if (pDMGstr0C > 0) {
		memcpy((void *)pDMGstr0C, &DMGstrN0, 192U);
	}
	if (pDMGstr1 > 0) {
		memcpy((void *)pDMGstr1, &DMGstrN1, 224U);
		memcpy((void *)pDMGstr1fs, &DMGstr1fs, 4U);
		memcpy((void *)(pDMGstr1fs + 4), &DMGstr1fs, 4U);
	}
	if (pDMGstr1C > 0) {
		memcpy((void *)pDMGstr1C, &DMGstrN1, 224U);
	}
	if (pDMGstr2 > 0) {
		memcpy((void *)pDMGstr2, &DMGstrN2, 240U);
		memcpy((void *)pDMGstr2fs, &DMGstr2fs, 4U);
		memcpy((void *)(pDMGstr2fs + 4), &DMGstr2fs, 4U);
	}
	if (pDMGstr2C > 0) {
		memcpy((void *)pDMGstr2C, &DMGstrN2, 240U);
	}
}

// reset string
void __fastcall displayWeaponDamageReset() {
	damage_tmp = 0;
	damageNumber.value = 0;
	damageNumber.time = 0;
	for (int i = 0; i < 8; i++) {
		dmgNumGroup[i].time = 0;
	}

	displayWeaponDamageClear();

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
	damageNumber.value = 0;
	damageNumber.time = 0;
	for(int i = 0; i < 8; i++) {
		dmgNumGroup[i].time = 0;
	}
	damage_tmp = 0;

	pDMGstr0C = 0;
	pDMGstr0fs = 0;
	DMGstr0fs = 1.0f;
	pDMGstr0 = 0;

	pDMGstr1C = 0;
	pDMGstr1fs = 0;
	DMGstr1fs = 1.0f;
	pDMGstr1 = 0;

	pDMGstr2C = 0;
	pDMGstr2fs = 0;
	DMGstr2fs = 1.0f;
	pDMGstr2 = 0;
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
			dmgNumGroup[i].time = dmgNumGroup[i + 1].time;
		}
	}
	dmgNumGroup[vend].value = damageNumber.value;
	dmgNumGroup[vend].time = time + 30;
}

void __fastcall displayWeaponDamageA1() {
	while (displayDamageIndex == 1) {
		playerAddress = GetPointerAddress((uintptr_t)hmodEXE, {0x0125AB68, 0x238, 0x290, 0x10});

		if (playerAddress > 0) {
			if (pDMGstr0 > 0) {
				if (damage_tmp != 0) {
					setChagreDamageTime(DAMAGE_DISPLAY_TIME_S);
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
			}
			//
		} else {
			if (pDMGstr0 > 0) {
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
			if (pDMGstr0 > 0 && pDMGstr0C > 0) {
				if (damage_tmp != 0) {
					setChagreDamageTime(DAMAGE_CHARGE_TIME_L);
				}

				memcpy((void *)pDMGstr0, &DMGstrN0, 192U);
				memcpy((void *)pDMGstr0C, &DMGstrN0, 192U);

				if (damageNumber.time > 0) {
					std::wstring displayText = FormatDamageNumber(damageNumber.value);

					size_t strofs = 0;
					size_t strsize = 44;
					if (displayText.size() < 22) {
						strsize = displayText.size() * 2;
					}

					float fontSize = 0.2f * damageNumber.time;
					fontSize += (DMGstr0fs * 1.5f);

					memcpy((void *)pDMGstr0, displayText.c_str(), strsize);
					memcpy((void *)pDMGstr0fs, &fontSize, 4U);
					memcpy((void *)(pDMGstr0fs + 4), &fontSize, 4U);

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

						memcpy((void *)(pDMGstr0C + strofs), displayText.c_str(), strsize);
						dmgNumGroup[i].time--;
					}
				}
				//
			}
			//
		} else {
			if (pDMGstr0 > 0) {
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
			if (pDMGstr1 > 0 || pDMGstr2 > 0) {
				if (damage_tmp != 0) {
					setChagreDamageTime(DAMAGE_DISPLAY_TIME_S);
				}

				if (pDMGstr1 > 0) {
					memcpy((void *)pDMGstr1, &DMGstrN1, 224U);

					if (damageNumber.time > 0) {
						std::wstring displayText = FormatDamageNumber(damageNumber.value);

						size_t strofs = 0;
						size_t strsize = 28;
						if (displayText.size() < 14) {
							strofs = ((14 - displayText.size()) / 2) * 2;
							strsize = displayText.size() * 2;
						}
						//strofs += 128U;

						memcpy((void *)(pDMGstr1 + strofs), displayText.c_str(), strsize);
						
					}
				} 
				if (pDMGstr2 > 0) {
					memcpy((void *)pDMGstr2, &DMGstrN2, 240U);

					if (damageNumber.time > 0) {
						std::wstring displayText = FormatDamageNumber(damageNumber.value);

						size_t strsize = 28;
						if (displayText.size() < 14) {
							strsize = displayText.size() * 2;
						}

						memcpy((void *)pDMGstr2, displayText.c_str(), strsize);
					}
				}

				if (damageNumber.time > 0) {
					damageNumber.time--;
				}
			}
			//
		} else {
			if (pDMGstr1 > 0 || pDMGstr2 > 0) {
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
			if ((pDMGstr1 > 0 && pDMGstr1C > 0) || (pDMGstr2 > 0 && pDMGstr2C > 0)) {
				if (damage_tmp != 0) {
					setChagreDamageTime(DAMAGE_CHARGE_TIME_S);
				}

				if (pDMGstr1 > 0) {
					memcpy((void *)pDMGstr1, &DMGstrN1, 224U);
					memcpy((void *)pDMGstr1C, &DMGstrN1, 224U);
				}
				if (pDMGstr2 > 0) {
					memcpy((void *)pDMGstr2, &DMGstrN2, 240U);
					memcpy((void *)pDMGstr2C, &DMGstrN2, 240U);
				}

				if (damageNumber.time > 0) {
					std::wstring displayText = FormatDamageNumber(damageNumber.value);

					size_t strsize = 28;
					if (displayText.size() < 14) {
						strsize = displayText.size() * 2;
					}

					if (pDMGstr1 > 0) {
						float fontSize = 0.1f * damageNumber.time;
						fontSize += (DMGstr1fs * 1.5f);

						memcpy((void *)pDMGstr1, displayText.c_str(), strsize);
						memcpy((void *)pDMGstr1fs, &fontSize, 4U);
						memcpy((void *)(pDMGstr1fs + 4), &fontSize, 4U);
					}

					if (pDMGstr2 > 0) {
						float fontSize = 0.15f * damageNumber.time;
						fontSize += (DMGstr2fs * 1.5f);

						memcpy((void *)pDMGstr2, displayText.c_str(), strsize);
						memcpy((void *)pDMGstr2fs, &fontSize, 4U);
						memcpy((void *)(pDMGstr2fs + 4), &fontSize, 4U);
					}

					damageNumber.time--;
				} else if (damageNumber.value > 0) {
					setDamageDisplayTime(0, 6, DAMAGE_DISPLAY_TIME_L);
					damageNumber.value = 0;
				}

				for (int i = 0; i < 7; i++) {
					if (dmgNumGroup[i].time > 0) {
						std::wstring displayText = FormatDamageNumber(dmgNumGroup[i].value);

						size_t strofs = 0;
						size_t strsize = 28;
						if (displayText.size() < 14) {
							strofs = ((14 - displayText.size()) / 2) * 2;
							strsize = displayText.size() * 2;
						}
						strofs += (size_t)i * 32;
						if (pDMGstr1C > 0) {
							memcpy((void *)(pDMGstr1C + strofs), displayText.c_str(), strsize);
						}

						size_t strofs2 = (size_t)i * 32;
						if (pDMGstr2C > 0) {
							memcpy((void *)(pDMGstr2C + strofs2), displayText.c_str(), strsize);
						}

						dmgNumGroup[i].time--;
					}
				}
				//
			}
			//
		} else {
			if (pDMGstr1 > 0 || pDMGstr2 > 0) {
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
			displayWeaponDamageClear();
		} else {
			if (pDMGstr0 > 0 || pDMGstr1 > 0 || pDMGstr2 > 0) {
				DMGCommonClear();
			}
		}
		// Execute twice per second
		Sleep(500);
	}
	// If setting change, we first have to reset
	displayWeaponDamageReset();
}

