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

#include "GameFunc.h"

// here hook all changed functions, written in c++
void hookGameFunctionsC() {
	// allows weapons to be charged
	SetupHook(0x391230, (PVOID *)&fnk391230_orig, fnk391230_hook, "test", 1);
}

static bool __fastcall fnk391230_hook(intptr_t a1) {
	// ammo
	int *curAmmo = (int *)(a1 + 0x8E8);
	int maxAmmo = *(int *)(a1 + 0x1D0);
	int ROFCount = *(int *)(a1 + 0xB40);
	// reloading phase, 0 means no
	// int rePhase = *(int *)(a1 + 0x8D4);
	// new function
	int chargeType = *(int *)(a1 + 0x2100);
	int chargeTime = *(int *)(a1 + 0x2108);
	int *chargeRT = (int *)(a1 + 0x210C);
	
	if (chargeType == 2 && chargeTime > 0) {
		if (*curAmmo > 0 && *curAmmo < maxAmmo && !ROFCount) {
			if (*chargeRT <= 0) {
				*curAmmo += 1;
				*chargeRT = chargeTime;
			} else {
				*chargeRT += -1;
			}
		} else {
			*chargeRT = chargeTime;
		}
	}
	
	// old
	// don't modify them, it will cause crashes when reloading
	int v2;
	int v1 = *curAmmo;
	if (v1 > 0 || *(INT64 *)(a1 + 3128) || (!*(INT64 *)(a1 + 3144) ? (v2 = *(int *)(a1 + 2960)) : (v2 = *(int *)(a1 + 420)), !v2)) {
		if (*(BYTE *)(a1 + 224))
			return 1;
	}
	if (v1 > 0 || *(INT64 *)(a1 + 3128))
		return 0;
	int v4 = *(INT64 *)(a1 + 3144) ? *(int *)(a1 + 420) : *(int *)(a1 + 2960);
	return v4 && !v1 && !*(int *)(a1 + 0xB40) && *(float *)(a1 + 452) <= 0.0 && *(int *)(a1 + 420) >= 0;
}