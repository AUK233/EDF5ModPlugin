#pragma once
#include "EDFPointerStruct.hpp"
#include "CommonCustomStructure.hpp"

typedef void(__fastcall* func_Call303E90)(void* pClass);

// ================================================================

void module_EDFCommonFunctionInitialization(PBYTE hmodEXE);
void __fastcall module_LoadAccessory_InMission(void* p_Class, PCustomMissionData pData);

void __fastcall module_LoadAccessory_AssultSoldier(PEDFAssultSoldier pClass);
void __fastcall module_LoadAccessory_Engineer(PEDFBaseClass pClass);
void __fastcall module_LoadAccessory_HeavyArmor(PEDFHeavyArmor pClass);

extern "C" {
	void __fastcall module_LoadAccessory_PaleWing(PEDFPaleWing pClass);
	// new accessory functions
	int __fastcall module_LoadAccessory_ExtraWeapon(const uintptr_t p_Class);
	// get accessory value
	// (fetchType: 0 is Direct return, 1 is Take maximum, 2 is Take minimum)
	// (fetchType: 3 is Default + Acquired, 4 is Default * Acquired)
	float __fastcall ASMeGetAccessoryValue(uintptr_t p_Class, UINT32 accessoryType, float defaultValue, int fetchType);
	// get accessory value
	// (fetchType: 0 is Direct return, 1 is Take maximum, 2 is Take minimum)
	// (fetchType: 3 is Default + Acquired, 4 is Default * Acquired)
	int __fastcall ASMeGetAccessoryINT32(uintptr_t p_Class, UINT32 accessoryType, int defaultValue, int fetchType);
}