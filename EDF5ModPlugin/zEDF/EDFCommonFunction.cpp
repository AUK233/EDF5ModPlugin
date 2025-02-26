#include <windows.h>
#include <psapi.h>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <format>
#include <stdexcept>
#include <list>
#include <cstdlib>

#include "utiliy.h"
#include "EDFCommonFunction.h"

extern "C" {
	extern PCustomMissionData pMissionCustomData;

	uintptr_t eGetAccessoryValueAddr;
	uintptr_t eGetAccessoryINT32Addr;
	uintptr_t eLoadClassAccessoryAddr;
}

void module_EDFCommonFunctionInitialization(PBYTE hmodEXE)
{
	eGetAccessoryINT32Addr = (uintptr_t)(hmodEXE + 0x3072F0);
	eGetAccessoryValueAddr = (uintptr_t)(hmodEXE + 0x307400);
	eLoadClassAccessoryAddr = (uintptr_t)(hmodEXE + 0x303E90);
}

void __fastcall module_LoadAccessory_InMission(void* p_Class, PCustomMissionData pData)
{
	PEDFBaseClass pClass = (PEDFBaseClass)p_Class;
	pClass->WalkingSpeed *= pData->Class_SpeedX;
}

void __fastcall module_LoadAccessory_AssultSoldier(PEDFAssultSoldier pClass)
{
	func_Call303E90 LoadClassAccessory = (func_Call303E90)eLoadClassAccessoryAddr;
	LoadClassAccessory(pClass);

	module_LoadAccessory_ExtraWeapon((uintptr_t)pClass);

	PCustomMissionData pData = pMissionCustomData;
	module_LoadAccessory_InMission(pClass, pData);

	float speed = pData->Ranger_SpeedX;
	pClass->WalkingSpeed *= speed;
	speed *= pData->Ranger_DashSpeedX;
	pClass->BaseDashSpeed *= speed;
}

void __fastcall module_LoadAccessory_Engineer(PEDFBaseClass pClass)
{
	func_Call303E90 LoadClassAccessory = (func_Call303E90)eLoadClassAccessoryAddr;
	LoadClassAccessory(pClass);

	module_LoadAccessory_ExtraWeapon((uintptr_t)pClass);

	PCustomMissionData pData = pMissionCustomData;
	module_LoadAccessory_InMission(pClass, pData);
}

void __fastcall module_LoadAccessory_HeavyArmor(PEDFHeavyArmor pClass)
{
	func_Call303E90 LoadClassAccessory = (func_Call303E90)eLoadClassAccessoryAddr;
	LoadClassAccessory(pClass);

	module_LoadAccessory_ExtraWeapon((uintptr_t)pClass);

	PCustomMissionData pData = pMissionCustomData;

	int boostCount = ASMeGetAccessoryINT32((uintptr_t)pClass, 302, 2, 3);
	pClass->thruster.BoostMaxCount = boostCount;
	pClass->thruster.BoostCurrentCount = boostCount;
	float boostSpeed = ASMeGetAccessoryValue((uintptr_t)pClass, 305, 1.0f, 4);
	boostSpeed *= pData->Fencer_BoostX;
	pClass->thruster.BoostSpeed = boostSpeed;

	int dashCount = ASMeGetAccessoryINT32((uintptr_t)pClass, 300, 2, 3);
	pClass->thruster.DashMaxCount = dashCount;
	pClass->thruster.DashCurrentCount = dashCount;
	float dashInterval = ASMeGetAccessoryValue((uintptr_t)pClass, 301, 0.5f, 4);
	dashInterval *= pData->Fencer_DashX;
	pClass->thruster.DashInterval = dashInterval;

	// new
	pClass->IsDashToBoost = ASMeGetAccessoryINT32((uintptr_t)pClass, 1300, 0, 1);
	pClass->IsBoostToDash = ASMeGetAccessoryINT32((uintptr_t)pClass, 1301, 0, 1);

	float boostCD = ASMeGetAccessoryValue((uintptr_t)pClass, 1303, pData->Fencer_BoostX, 4);
	pClass->BoostRecoveryBaseTime = 240 * boostCD;

	float dashCD = ASMeGetAccessoryValue((uintptr_t)pClass, 1302, pData->Fencer_DashX, 4);
	pClass->DashRecoveryBaseTime = 90 * dashCD;

	module_LoadAccessory_InMission(pClass, pData);
}

void __fastcall module_LoadAccessory_PaleWing(PEDFPaleWing pClass)
{
	module_LoadAccessory_ExtraWeapon((uintptr_t)pClass);

	PCustomMissionData pData = pMissionCustomData;
	module_LoadAccessory_InMission(pClass, pData);

	pClass->ChargeSpeed *= pData->PaleWing_ChargeX;
	pClass->EmergencyChargeSpeed *= pData->PaleWing_ChargeX;
	//
	pClass->WeaponChargeSpeed *= pData->PaleWing_WeaponChargeX;
	//
	pClass->BoostSidewaySpeed *= pData->PaleWing_BoostSpeedX;
	pClass->BoostForwardSpeed *= pData->PaleWing_BoostSpeedX;
	pClass->BoostBackwardSpeed *= pData->PaleWing_BoostSpeedX;
}

int __fastcall module_LoadAccessory_ExtraWeapon(const uintptr_t p_Class) {
	int hasExtraParameter = 0;
	uintptr_t p_weapon = *(uintptr_t*)(p_Class + 0x1590);
	uintptr_t num_weapon = *(uintptr_t*)(p_Class + 0x15A0);
	uintptr_t p_weaponValue;
	EDFWeaponPointer* pWeapon;
	float getScale;
	int getIntValue;

	// Update AmmoCount
	getScale = ASMeGetAccessoryValue(p_Class, 1001, 1.0f, 0);
	if (getScale != 1.0f) {
		for (uintptr_t i = p_weapon; i != (p_weapon + (num_weapon * 8)); i += 8) {
			p_weaponValue = *(uintptr_t*)i;
			int curAmmo = *(int*)(p_weaponValue + 0x8E8);
			if (curAmmo > 1) {
				curAmmo *= getScale;
				if (curAmmo < 1) {
					curAmmo = 1;
				}
				// Total ammo
				*(int*)(p_weaponValue + 0x1D0) = curAmmo;
				// Current Ammo
				*(int*)(p_weaponValue + 0x8E8) = curAmmo;
			}
		}
	}
	// Update ReloadTime
	getScale = ASMeGetAccessoryValue(p_Class, 1002, 1.0f, 0);
	if (getScale != 1.0f) {
		for (uintptr_t i = p_weapon; i != (p_weapon + (num_weapon * 8)); i += 8) {
			p_weaponValue = *(uintptr_t*)i;
			int reloadTime = *(int*)(p_weaponValue + 0x1A4);
			if (reloadTime > 0) {
				reloadTime *= getScale;
				if (reloadTime < 1) {
					reloadTime = 1;
				}
				*(int*)(p_weaponValue + 0x1A4) = reloadTime;
				// reload time count
				*(int*)(p_weaponValue + 0xB90) = reloadTime;
			}
		}
	}
	// Update FireInterval
	getScale = ASMeGetAccessoryValue(p_Class, 1003, 1.0f, 0);
	if (getScale != 1.0f) {
		for (uintptr_t i = p_weapon; i != (p_weapon + (num_weapon * 8)); i += 8) {
			p_weaponValue = *(uintptr_t*)i;
			int iROF = *(int*)(p_weaponValue + 0x2FC);
			if (iROF > 0) {
				iROF *= getScale;
				if (iROF < 1) {
					iROF = 1;
				}
				*(int*)(p_weaponValue + 0x2FC) = iROF;
			}
		}
	}

	// float
	// Update AmmoDamage
	getScale = ASMeGetAccessoryValue(p_Class, 1050, 1.0f, 0);
	if (getScale != 1.0f) {
		for (uintptr_t i = p_weapon; i != (p_weapon + (num_weapon * 8)); i += 8) {
			p_weaponValue = *(uintptr_t*)i;
			float ammoDamage = *(float*)(p_weaponValue + 0x69C);
			if (ammoDamage > 1.0f) {
				ammoDamage *= getScale;
				*(float*)(p_weaponValue + 0x69C) = ammoDamage;
			}
		}
	}
	// Update (-)AmmoDamage
	getScale = ASMeGetAccessoryValue(p_Class, 1150, 1.0f, 0);
	if (getScale != 1.0f) {
		for (uintptr_t i = p_weapon; i != (p_weapon + (num_weapon * 8)); i += 8) {
			p_weaponValue = *(uintptr_t*)i;
			float ammoDamage = *(float*)(p_weaponValue + 0x69C);
			if (ammoDamage < 0.0f) {
				ammoDamage *= getScale;
				*(float*)(p_weaponValue + 0x69C) = ammoDamage;
			}
		}
	}
	// Update AmmoSpeed
	getScale = ASMeGetAccessoryValue(p_Class, 1051, 1.0f, 0);
	if (getScale != 1.0f) {
		for (uintptr_t i = p_weapon; i != (p_weapon + (num_weapon * 8)); i += 8) {
			p_weaponValue = *(uintptr_t*)i;
			float ammoSpeed = *(float*)(p_weaponValue + 0x694);
			if (ammoSpeed > 0.0f) {
				ammoSpeed *= getScale;
				*(float*)(p_weaponValue + 0x694) = ammoSpeed;
			}
		}
	}
	// Update AmmoExplosion
	getScale = ASMeGetAccessoryValue(p_Class, 1052, 1.0f, 0);
	if (getScale != 1.0f) {
		for (uintptr_t i = p_weapon; i != (p_weapon + (num_weapon * 8)); i += 8) {
			p_weaponValue = *(uintptr_t*)i;
			float blastRadius = *(float*)(p_weaponValue + 0x6A8);
			if (blastRadius > 0.0f) {
				blastRadius *= getScale;
				*(float*)(p_weaponValue + 0x6A8) = blastRadius;
			}
		}
	}
	// Update AmmoSize
	getScale = ASMeGetAccessoryValue(p_Class, 1060, 1.0f, 0);
	if (getScale != 1.0f) {
		for (uintptr_t i = p_weapon; i != (p_weapon + (num_weapon * 8)); i += 8) {
			p_weaponValue = *(uintptr_t*)i;
			float ammoSize = *(float*)(p_weaponValue + 0x6B4);
			if (ammoSize > 0.0f) {
				ammoSize *= getScale;
				*(float*)(p_weaponValue + 0x6B4) = ammoSize;
			}
		}
	}

	// Special
	// change Zoom
	getIntValue = ASMeGetAccessoryINT32(p_Class, 1310, 0, 0);
	if (getIntValue > 0) {
		for (uintptr_t i = p_weapon; i != (p_weapon + (num_weapon * 8)); i += 8) {
			p_weaponValue = *(uintptr_t*)i;
			int sType = *(int*)(p_weaponValue + 0x500);
			if (sType == 1) {
				if (getIntValue == 1) {
					*(int*)(p_weaponValue + 0x500) = 4;
				}
				else {
					*(int*)(p_weaponValue + 0x500) = 5;
				}
			}
		}
	}
	// change Shield Reflect
	getIntValue = ASMeGetAccessoryINT32(p_Class, 1311, 0, 0);
	if (getIntValue > 0) {
		for (uintptr_t i = p_weapon; i != (p_weapon + (num_weapon * 8)); i += 8) {
			p_weaponValue = *(uintptr_t*)i;
			int sType = *(int*)(p_weaponValue + 0x500);
			if (sType == 6) {
				if (getIntValue == 1) {
					*(int*)(p_weaponValue + 0x500) = 4;
				}
				else {
					*(int*)(p_weaponValue + 0x500) = 5;
				}
			}
		}
	}

	// Update Recoil
	getScale = ASMeGetAccessoryValue(p_Class, 909, 1.0f, 0);
	if (getScale != 1.0f) {
		p_weaponValue = p_Class + 0x169C;
		*(float*)p_weaponValue *= getScale;
	}

	// change LockonTargetType to 0
	getIntValue = ASMeGetAccessoryINT32(p_Class, 1012, 0, 0);
	if (getIntValue > 0) {
		for (uintptr_t i = p_weapon; i != (p_weapon + (num_weapon * 8)); i += 8) {
			p_weaponValue = *(uintptr_t*)i;
			pWeapon = (EDFWeaponPointer*)p_weaponValue;
			pWeapon->LockonTargetType = 0;
		}
	}
	// Update LockonAngle width
	getScale = ASMeGetAccessoryValue(p_Class, 912, 1.0f, 0);
	if (getScale != 1.0f) {
		for (uintptr_t i = p_weapon; i != (p_weapon + (num_weapon * 8)); i += 8) {
			p_weaponValue = *(uintptr_t*)i;
			pWeapon = (EDFWeaponPointer*)p_weaponValue;
			pWeapon->LockonAngle.x *= getScale;
		}
	}
	// Update LockonAngle height
	getScale = ASMeGetAccessoryValue(p_Class, 913, 1.0f, 0);
	if (getScale != 1.0f) {
		for (uintptr_t i = p_weapon; i != (p_weapon + (num_weapon * 8)); i += 8) {
			p_weaponValue = *(uintptr_t*)i;
			pWeapon = (EDFWeaponPointer*)p_weaponValue;
			pWeapon->LockonAngle.y *= getScale;
		}
	}

	return hasExtraParameter;
}
