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
#include <HookLib.h>
#include "utiliy.h"

#include "GameFunc.h"

extern PBYTE hmodEXE;
extern int weaponEnhance;

static const unsigned char intNOP32[] = {0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90};

extern "C" {
// star rating calculation
uintptr_t edf8C8C0Address;
//
uintptr_t edf3AE530Address;

uintptr_t edf4738B0Address;
uintptr_t edf4DC190Address;
// read sgo node name
uintptr_t edf5BDF30Address;
// get AmmoClass address
uintptr_t edf6136C0Address;
// get Weapon_Accessory data (int)
uintptr_t edf3983B0Address;
// get Weapon_Accessory data (float)
uintptr_t edf3982A0Address;
//
uintptr_t edf1BCBA0Address;
uintptr_t edf1ACA80Address;
uintptr_t edf6E160Address;
uintptr_t edf6E0D0Address;
uintptr_t edf70730Address;
uintptr_t vedfE940C0;
uintptr_t vedfE94130;
uintptr_t edf5C8410Address;
uintptr_t edf707B0Address;
uintptr_t edf70850Address;
uintptr_t edf1BCDF0Address;
uintptr_t edf5C6C80Address;
uintptr_t edf5C8660Address;
//
uintptr_t eGetAccessoryValueAddr;
uintptr_t eGetAccessoryINT32Addr;
//
uintptr_t rva9C6E40;
uintptr_t rva6E010;
uintptr_t rva79390;
uintptr_t rva9C850;
//
uintptr_t rva5CC830;
// read shader?
uintptr_t rva5EE9E0;
uintptr_t rva186330;
// add barga animation event
uintptr_t rva344F30;
// add giant ant animation event
uintptr_t rva204EA0;
// add giant spider animation event
uintptr_t rva225C80;
// add giant bee animation event
uintptr_t rva211070;
// Initialize laser sighting
uintptr_t rva38AEF0;
uintptr_t rva399760;
uintptr_t rva38B100;
uintptr_t rva949E0;
// Initialize Shot Indicator Circle
uintptr_t rva38A4F0;
uintptr_t rva38A960;
uintptr_t rva181DA0;
uintptr_t rva25A20;
uintptr_t rva94A70;
uintptr_t rva6170C0;
uintptr_t rva616F30;
uintptr_t vedfEBBFB8;
uintptr_t rva5E0660;
}

// get game function address
void GetGameFunctions() {

	rva38A4F0 = (uintptr_t)(hmodEXE + 0x38A4F0);
	rva38A960 = (uintptr_t)(hmodEXE + 0x38A960);
	rva181DA0 = (uintptr_t)(hmodEXE + 0x181DA0);
	rva25A20 = (uintptr_t)(hmodEXE + 0x25A20);
	rva94A70 = (uintptr_t)(hmodEXE + 0x94A70);
	rva6170C0 = (uintptr_t)(hmodEXE + 0x6170C0);
	rva616F30 = (uintptr_t)(hmodEXE + 0x616F30);
	vedfEBBFB8 = (uintptr_t)(hmodEXE + 0xEBBFB8);
	rva5E0660 = (uintptr_t)(hmodEXE + 0x5E0660);

	// get star rating calculation function
	edf8C8C0Address = (uintptr_t)(hmodEXE + 0x8C8C0);

	edf3AE530Address = (uintptr_t)(hmodEXE + 0x3AE530);

	edf4738B0Address = (uintptr_t)(hmodEXE + 0x4738B0);
	edf4DC190Address = (uintptr_t)(hmodEXE + 0x4DC190);
	// get read sgo node function
	edf5BDF30Address = (uintptr_t)(hmodEXE + 0x5BDF30);
	// get get AmmoClass address function
	edf6136C0Address = (uintptr_t)(hmodEXE + 0x6136C0);
	// get Weapon_Accessory data (int)
	edf3983B0Address = (uintptr_t)(hmodEXE + 0x3983B0);
	// get Weapon_Accessory data (float)
	edf3982A0Address = (uintptr_t)(hmodEXE + 0x3982A0);
	// error
	edf1BCBA0Address = (uintptr_t)(hmodEXE + 0x1BCBA0);
	edf1ACA80Address = (uintptr_t)(hmodEXE + 0x1ACA80);
	edf6E160Address = (uintptr_t)(hmodEXE + 0x6E160);
	edf6E0D0Address = (uintptr_t)(hmodEXE + 0x6E0D0);
	edf70730Address = (uintptr_t)(hmodEXE + 0x70730);
	vedfE940C0 = (uintptr_t)(hmodEXE + 0xE940C0);
	vedfE94130 = (uintptr_t)(hmodEXE + 0xE94130);
	edf5C8410Address = (uintptr_t)(hmodEXE + 0x5C8410);
	edf707B0Address = (uintptr_t)(hmodEXE + 0x707B0);
	edf70850Address = (uintptr_t)(hmodEXE + 0x70850);
	edf1BCDF0Address = (uintptr_t)(hmodEXE + 0x1BCDF0);
	edf5C6C80Address = (uintptr_t)(hmodEXE + 0x5C6C80);
	edf5C8660Address = (uintptr_t)(hmodEXE + 0x5C8660);

	// 
	eGetAccessoryValueAddr = (uintptr_t)(hmodEXE + 0x307400);
	eGetAccessoryINT32Addr = (uintptr_t)(hmodEXE + 0x3072F0);

	//
	rva9C6E40 = (uintptr_t)(hmodEXE + 0x9C6E40);
	rva6E010 = (uintptr_t)(hmodEXE + 0x6E010);
	rva79390 = (uintptr_t)(hmodEXE + 0x79390);
	rva9C850 = (uintptr_t)(hmodEXE + 0x9C850);
	rva186330 = (uintptr_t)(hmodEXE + 0x186330);
	rva5CC830 = (uintptr_t)(hmodEXE + 0x5CC830);
	rva5EE9E0 = (uintptr_t)(hmodEXE + 0x5EE9E0);

	// Add Animation Event
	rva344F30 = (uintptr_t)(hmodEXE + 0x344F30);
	rva204EA0 = (uintptr_t)(hmodEXE + 0x204EA0);
	rva211070 = (uintptr_t)(hmodEXE + 0x211070);
	rva225C80 = (uintptr_t)(hmodEXE + 0x225C80);

	// Initialize laser sighting
	rva38AEF0 = (uintptr_t)(hmodEXE + 0x38AEF0);
	rva399760 = (uintptr_t)(hmodEXE + 0x399760);
	rva38B100 = (uintptr_t)(hmodEXE + 0x38B100);
	rva949E0 = (uintptr_t)(hmodEXE + 0x949E0);

	// get ammo function address
	GetAmmoFunctions();
}

extern "C" {
// Set insect ammo type
uintptr_t InsectAmmoType[14 + 1];
uintptr_t giantBeeAmmoSetRetAddr;
uintptr_t AVSceneObjectAddress;
void __fastcall ASMInsectPlasmaBullet01();
uintptr_t AVPlasmaBullet01Address;
void __fastcall ASMInsectSolidBullet01();
uintptr_t AVSolidBullet01Address;
void __fastcall ASMInsectPulseBullet01();
uintptr_t AVPulseBullet01Address;
void __fastcall ASMInsectSolidExpBullet01();
uintptr_t AVSolidExpBullet01Address;
void __fastcall ASMInsectHomingLaserBullet01();
uintptr_t AVHomingLaserBullet01Address;
void __fastcall ASMInsectLaserBullet02();
uintptr_t AVLaserBullet02Address;
void __fastcall ASMInsectLightningBullet01();
uintptr_t AVLightningBullet01Address;
void __fastcall ASMInsectLaserBullet01();
uintptr_t AVLaserBullet01Address;
void __fastcall ASMInsectRocketBullet01();
uintptr_t AVRocketBullet01Address;
void __fastcall ASMInsectBeamBullet01();
uintptr_t AVBeamBullet01Address;
uintptr_t InsectSpiderStringBullet02;

// Delayed explosion
uintptr_t edf47D950Address;
// Genocide explosion
uintptr_t edf1AE7A0Address;
// Normal explosion
uintptr_t edf1AD970Address;
}
// get ammo function address
void GetAmmoFunctions() {
	// Set insect ammo type
	InsectAmmoType[0] = 0;
	// Get insect ammo return address
	giantBeeAmmoSetRetAddr = (uintptr_t)(hmodEXE + 0x21120C);
	AVSceneObjectAddress = (uintptr_t)(hmodEXE + 0x11A49C0);
	// AcidBullet01
	InsectAmmoType[1] = (uintptr_t)(hmodEXE + 0x204FC0);
	// FlameBullet01
	InsectAmmoType[2] = (uintptr_t)(hmodEXE + 0x1F77F0);
	// PlasmaBullet01
	InsectAmmoType[3] = (uintptr_t)ASMInsectPlasmaBullet01;
	AVPlasmaBullet01Address = (uintptr_t)(hmodEXE + 0x11AA2B8);
	// SolidBullet01
	InsectAmmoType[4] = (uintptr_t)ASMInsectSolidBullet01;
	AVSolidBullet01Address = (uintptr_t)(hmodEXE + 0x11A9740);
	// PulseBullet01
	InsectAmmoType[5] = (uintptr_t)ASMInsectPulseBullet01;
	AVPulseBullet01Address = (uintptr_t)(hmodEXE + 0x11AA380);
	// SolidExpBullet01
	InsectAmmoType[6] = (uintptr_t)ASMInsectSolidExpBullet01;
	AVSolidExpBullet01Address = (uintptr_t)(hmodEXE + 0x11AAD98);
	// HomingLaserBullet01
	InsectAmmoType[7] = (uintptr_t)ASMInsectHomingLaserBullet01;
	AVHomingLaserBullet01Address = (uintptr_t)(hmodEXE + 0x11A9C10);
	// LaserBullet02
	InsectAmmoType[8] = (uintptr_t)ASMInsectLaserBullet02;
	AVLaserBullet02Address = (uintptr_t)(hmodEXE + 0x11A9D40);
	// FlameBullet02
	InsectAmmoType[9] = (uintptr_t)(hmodEXE + 0x205220);
	// NeedleBullet01
	InsectAmmoType[10] = (uintptr_t)(hmodEXE + 0x211190);
	// LightningBullet01
	InsectAmmoType[11] = (uintptr_t)ASMInsectLightningBullet01;
	AVLightningBullet01Address = (uintptr_t)(hmodEXE + 0x11AA000);
	// LaserBullet01
	InsectAmmoType[12] = (uintptr_t)ASMInsectLaserBullet01;
	AVLaserBullet01Address = (uintptr_t)(hmodEXE + 0x11A9CB8);
	// RocketBullet01
	InsectAmmoType[13] = (uintptr_t)ASMInsectRocketBullet01;
	AVRocketBullet01Address = (uintptr_t)(hmodEXE + 0x11AA3D0);
	// BeamBullet01
	InsectAmmoType[14] = (uintptr_t)ASMInsectBeamBullet01;
	AVBeamBullet01Address = (uintptr_t)(hmodEXE + 0x11A9538);

	InsectSpiderStringBullet02 = (uintptr_t)(hmodEXE + 0x225DA0);
	// Delayed explosion
	edf47D950Address = (uintptr_t)(hmodEXE + 0x47D950);
	edf1AE7A0Address = (uintptr_t)(hmodEXE + 0x1AE7A0);
	edf1AD970Address = (uintptr_t)(hmodEXE + 0x1AD970);
}

extern "C" {
	// checking the status of weapons fire
	// 0 is can shoot, 1 is not
	uintptr_t rva391230;
	void __fastcall ASMrva391230();
}

// here hook all changed functions, written in c++
void hookGameFunctionsC() {
	rva391230 = (uintptr_t)(hmodEXE + 0x391230);
	if (weaponEnhance) {
		// EDF5.exe+4D3402 is the location of the HUD check.
		// EDF5.exe+4B96B1 is checking the sights.
		BYTE ofs4D2802[] = { 0xE8, 0xBC, 0xDE, 0xEB, 0xFF};
		WriteHookToProcess((void*)(hmodEXE + 0x4D3402), &ofs4D2802, 5U);
		// allows weapons to be charged, offset is 0x390630
		//SetupHook(0x391230, (PVOID *)&fnk391230_orig, fnk391230_hook, "Allows weapons to be charged", 1);
		// now override a useless position.
		hookGameBlock((void *)(hmodEXE + 0x3912C3), (uint64_t)ASMrva391230);
	}
}

void __fastcall eAccessoryEnhancement(const uintptr_t p_Class) {
	uintptr_t p_weapon = *(uintptr_t *)(p_Class + 0x1590);
	uintptr_t num_weapon = *(uintptr_t *)(p_Class + 0x15A0);
	uintptr_t p_weaponValue;
	float getScale;
	int getIntValue;
	// Update AmmoCount
	getScale = ASMeGetAccessoryValue(p_Class, 1001, 1.0f, 0);
	if (getScale != 1.0f) {
		for (uintptr_t i = p_weapon; i != (p_weapon + (num_weapon * 8)); i += 8) {
			p_weaponValue = *(uintptr_t *)i;
			int totalAmmo = *(int *)(p_weaponValue + 0x1D0);
			if (totalAmmo > 0) {
				totalAmmo *= getScale;
				if (totalAmmo < 1) {
					totalAmmo = 1;
				}
				*(int *)(p_weaponValue + 0x1D0) = totalAmmo;
				// Current Ammo
				*(int *)(p_weaponValue + 0x8E8) = totalAmmo;
			}
		}
	}
	// Update ReloadTime
	getScale = ASMeGetAccessoryValue(p_Class, 1002, 1.0f, 0);
	if (getScale != 1.0f) {
		for (uintptr_t i = p_weapon; i != (p_weapon + (num_weapon * 8)); i += 8) {
			p_weaponValue = *(uintptr_t *)i;
			int reloadTime = *(int *)(p_weaponValue + 0x1A4);
			if (reloadTime > 0) {
				reloadTime *= getScale;
				if (reloadTime < 1) {
					reloadTime = 1;
				}
				*(int *)(p_weaponValue + 0x1A4) = reloadTime;
				// reload time count
				*(int *)(p_weaponValue + 0xB90) = reloadTime;
			}
		}
	}
	// Update FireInterval
	getScale = ASMeGetAccessoryValue(p_Class, 1003, 1.0f, 0);
	if (getScale != 1.0f) {
		for (uintptr_t i = p_weapon; i != (p_weapon + (num_weapon * 8)); i += 8) {
			p_weaponValue = *(uintptr_t *)i;
			int iROF = *(int *)(p_weaponValue + 0x2FC);
			if (iROF > 0) {
				iROF *= getScale;
				if (iROF < 1) {
					iROF = 1;
				}
				*(int *)(p_weaponValue + 0x2FC) = iROF;
			}
		}
	}

	// float
	// Update AmmoDamage
	getScale = ASMeGetAccessoryValue(p_Class, 1050, 1.0f, 0);
	if (getScale != 1.0f) {
		for (uintptr_t i = p_weapon; i != (p_weapon + (num_weapon * 8)); i += 8) {
			p_weaponValue = *(uintptr_t *)i;
			float ammoDamage = *(float *)(p_weaponValue + 0x69C);
			if (ammoDamage > 1.0f) {
				ammoDamage *= getScale;
				*(float *)(p_weaponValue + 0x69C) = ammoDamage;
			}
		}
	}
	// Update (-)AmmoDamage
	getScale = ASMeGetAccessoryValue(p_Class, 1150, 1.0f, 0);
	if (getScale != 1.0f) {
		for (uintptr_t i = p_weapon; i != (p_weapon + (num_weapon * 8)); i += 8) {
			p_weaponValue = *(uintptr_t *)i;
			float ammoDamage = *(float *)(p_weaponValue + 0x69C);
			if (ammoDamage < 0.0f) {
				ammoDamage *= getScale;
				*(float *)(p_weaponValue + 0x69C) = ammoDamage;
			}
		}
	}
	// Update AmmoSpeed
	getScale = ASMeGetAccessoryValue(p_Class, 1051, 1.0f, 0);
	if (getScale != 1.0f) {
		for (uintptr_t i = p_weapon; i != (p_weapon + (num_weapon * 8)); i += 8) {
			p_weaponValue = *(uintptr_t *)i;
			float ammoSpeed = *(float *)(p_weaponValue + 0x694);
			if (ammoSpeed > 0.0f) {
				ammoSpeed *= getScale;
				*(float *)(p_weaponValue + 0x694) = ammoSpeed;
			}
		}
	}
	// Update AmmoExplosion
	getScale = ASMeGetAccessoryValue(p_Class, 1052, 1.0f, 0);
	if (getScale != 1.0f) {
		for (uintptr_t i = p_weapon; i != (p_weapon + (num_weapon * 8)); i += 8) {
			p_weaponValue = *(uintptr_t *)i;
			float blastRadius = *(float *)(p_weaponValue + 0x6A8);
			if (blastRadius > 0.0f) {
				blastRadius *= getScale;
				*(float *)(p_weaponValue + 0x6A8) = blastRadius;
			}
		}
	}
	// Update AmmoSize
	getScale = ASMeGetAccessoryValue(p_Class, 1060, 1.0f, 0);
	if (getScale != 1.0f) {
		for (uintptr_t i = p_weapon; i != (p_weapon + (num_weapon * 8)); i += 8) {
			p_weaponValue = *(uintptr_t *)i;
			float ammoSize = *(float *)(p_weaponValue + 0x6B4);
			if (ammoSize > 0.0f) {
				ammoSize *= getScale;
				*(float *)(p_weaponValue + 0x6B4) = ammoSize;
			}
		}
	}

	// Special
	// change Zoom
	getIntValue = ASMeGetAccessoryINT32(p_Class, 1310, 0, 0);
	if (getIntValue > 0) {
		for (uintptr_t i = p_weapon; i != (p_weapon + (num_weapon * 8)); i += 8) {
			p_weaponValue = *(uintptr_t *)i;
			int sType = *(int *)(p_weaponValue + 0x500);
			if (sType == 1) {
				if (getIntValue == 1) {
					*(int *)(p_weaponValue + 0x500) = 4;
				} else {
					*(int *)(p_weaponValue + 0x500) = 5;
				}
			}
		}
	}
	// change Shield Reflect
	getIntValue = ASMeGetAccessoryINT32(p_Class, 1311, 0, 0);
	if (getIntValue > 0) {
		for (uintptr_t i = p_weapon; i != (p_weapon + (num_weapon * 8)); i += 8) {
			p_weaponValue = *(uintptr_t *)i;
			int sType = *(int *)(p_weaponValue + 0x500);
			if (sType == 6) {
				if (getIntValue == 1) {
					*(int *)(p_weaponValue + 0x500) = 4;
				} else {
					*(int *)(p_weaponValue + 0x500) = 5;
				}
			}
		}
	}

	// Update Recoil
	getScale = ASMeGetAccessoryValue(p_Class, 909, 1.0f, 0);
	if (getScale != 1.0f) {
		p_weaponValue = p_Class + 0x169C;
		*(float *)p_weaponValue *= getScale;
	}
}

void __fastcall edfSoldierWeaponCharge(EDFWeaponPointer* pweapon) {
	if (pweapon->reloadPadType == 2 || pweapon->reloadPadType == 3) {
		if (pweapon->chargeTime > 0) {
			if (pweapon->curAmmo > 0 && pweapon->curAmmo < pweapon->maxAmmo && !pweapon->ROFCount) {
				if (pweapon->chargeTimeCount <= 0) {
					pweapon->curAmmo += 1;
					pweapon->chargeTimeCount = pweapon->chargeTime;
				} else {
					pweapon->chargeTimeCount += -1;
				}
			} else {
				pweapon->chargeTimeCount = pweapon->chargeTime;
			}
		}
	}
	// end
}


bool __fastcall fnk391230_hook(const uintptr_t pweapon) {
	// ammo
	int *curAmmo = (int *)(pweapon + 0x8E8);
	int maxAmmo = *(int *)(pweapon + 0x1D0);
	int ROFCount = *(int *)(pweapon + 0xB40);
	// reloading phase, 0 means no
	// int rePhase = *(int *)(a1 + 0x8D4);
	// new function
	int reloadPadType = *(int *)(pweapon + 0x2500);
	int chargeTime = *(int *)(pweapon + 0x2508);
	int *chargeRT = (int *)(pweapon + 0x250C);
	if (reloadPadType == 2 || reloadPadType == 3) {
		if (chargeTime > 0) {
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
	}

	// Original function block
	// Attempts to modify it failed.
	int v2;
	int v1 = *curAmmo;
	if (v1 > 0 || *(INT64 *)(pweapon + 3128) || (!*(INT64 *)(pweapon + 3144) ? (v2 = *(int *)(pweapon + 2960)) : (v2 = *(int *)(pweapon + 420)), !v2)) {
		if (*(BYTE *)(pweapon + 224))
			return true;
	}
	if (v1 > 0 || *(INT64 *)(pweapon + 3128))
		return false;
	int v4 = *(INT64 *)(pweapon + 3144) ? *(int *)(pweapon + 420) : *(int *)(pweapon + 2960);
	return v4 && !v1 && !*(int *)(pweapon + 0xB40) && *(float *)(pweapon + 452) <= 0.0f && *(int *)(pweapon + 420) >= 0;
}
