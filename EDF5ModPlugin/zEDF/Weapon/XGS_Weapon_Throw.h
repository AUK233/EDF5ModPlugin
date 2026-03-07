#pragma once
#include "EDFWeapon.hpp"

typedef struct G_Weapon_Throw_t : G_WeaponBase_t {
	char padOld[0x60]; // original size is 0x1200
	// new
	G_AreaRender_t indicatorCircle;
} *PG_Weapon_Throw;
#if 1
static_assert(offsetof(G_Weapon_Throw_t, indicatorCircle) == 0x1200);
#endif

void xgs_Weapon_Throw_Init(PBYTE hmodEXE);

extern "C" {
	int __fastcall xgs_Weapon_Throw_SetIndicatorPos(PG_Weapon_Throw pThis, __m128* pInGravity);
}