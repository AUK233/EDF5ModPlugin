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
#include "commonNOP.h"
#include "EDFbaseParameterSet.h"
#include "EDFCommonFunction.h"
#include "EDFAmmoFunction.h"
#include "EDFWeaponFunction.h"
#include "EDFSoldierClass.hpp"
#include "Class/EDFSoldierHeavyArmor.h"

void module_SetEDFSeriesFunction(PBYTE hmodEXE)
{
	module_SetFunction_AssultSoldier(hmodEXE);
	module_SetFunction_PaleWing(hmodEXE);
	module_SetFunction_Engineer(hmodEXE);

	module_SetFunction_HeavyArmor(hmodEXE);
	// EDF5.exe+EADD18, is vft+230
	uintptr_t newLoadAccessory = (uintptr_t)module_LoadAccessory_HeavyArmor;
	WriteHookToProcess((void*)(hmodEXE + 0xEADAE8 + 0x230), &newLoadAccessory, 8U);

	module_SetFunction_EDFCommon(hmodEXE);
	module_SetFunction_EDFVehicle(hmodEXE);
	module_SetFunction_EDFAmmo(hmodEXE);
	module_SetFunction_EDFWeapon(hmodEXE);
}

extern "C" {
	// ranger!
	void __fastcall ASMeAssultSoldierActionInit();
	void __fastcall ASMeAssultSoldierInitialization();
	void __fastcall ASMeArmySoldierUseAuxiliary();
	uintptr_t eArmySoldierUseAuxiliaryRetAddr;
	// vehicle_call
	uintptr_t edf2E0270Address;
	// ranger dash
	uintptr_t edf2E07C0Address;
	// Execution?
	uintptr_t edf2E18A0Address;
	// Allow change movement direction in dash
	void __fastcall ASMeArmySoldierDashTurning();
	uintptr_t eArmySoldierDashTurningRetAddr;
	//
	uintptr_t eSoldierCallSupportRetAddr;
	uintptr_t edf5F8C40Address;
	// Show 2nd support slot
	void __fastcall ASMhudShowSupportSlot2();
	uintptr_t hudShowSupportSlot2RetAddr;
}

void module_SetFunction_AssultSoldier(PBYTE hmodEXE)
{
	// ranger!
	int newRangerSize = 0x2000;
	// AssultSoldier 0x1BD0
	// start: 0x1BE0, size: 8, function: throw button timer.
	// start: 0x1BE8, size: 4, function: dash button timer.
	WriteHookToProcessCheckECX((void*)(hmodEXE + 0x2DF9C7 + 1), &newRangerSize, 4U);
	static_assert(offsetof(EDFAssultSoldier_t, ThrowButtonTimer) == 0x1BE0);
	static_assert(offsetof(EDFAssultSoldier_t, ThrowButtonCD) == 0x1BE4);
	static_assert(offsetof(EDFAssultSoldier_t, DashButtonTimer) == 0x1BE8);
	// EDF5.exe+2DFD0D
	hookGameBlockWithInt3((void*)(hmodEXE + 0x2DFD0D), (uintptr_t)ASMeAssultSoldierInitialization);
	WriteHookToProcess((void*)(hmodEXE + 0x2DFD0D + 15), (void*)&nop1, 1U);
	// EDF5.exe+2DFD0D
	hookGameBlockWithInt3((void*)(hmodEXE + 0x2E1213), (uintptr_t)ASMeAssultSoldierActionInit);
	WriteHookToProcess((void*)(hmodEXE + 0x2E1213 + 15), (void*)&nop2, 2U);
	// EDF5.exe+2E0017
	eArmySoldierUseAuxiliaryRetAddr = (uintptr_t)(hmodEXE + 0x2E00C1);
	hookGameBlockWithInt3((void*)(hmodEXE + 0x2E0017), (uintptr_t)ASMeArmySoldierUseAuxiliary);
	WriteHookToProcess((void*)(hmodEXE + 0x2E0017 + 15), (void*)&nop3, 3U);
	//
	edf2E0270Address = (uintptr_t)(hmodEXE + 0x2E0270);
	edf2E07C0Address = (uintptr_t)(hmodEXE + 0x2E07C0);
	edf2E18A0Address = (uintptr_t)(hmodEXE + 0x2E18A0);
	//
	eSoldierCallSupportRetAddr = (uintptr_t)(hmodEXE + 0x2E0368);
	edf5F8C40Address = (uintptr_t)(hmodEXE + 0x5F8C40);
	// EDF5.exe+4D7A70
	// Show 2nd support slot
	hudShowSupportSlot2RetAddr = (uintptr_t)(hmodEXE + 0x4D7A7F);
	hookGameBlockWithInt3((void*)(hmodEXE + 0x4D7A70), (uintptr_t)ASMhudShowSupportSlot2);

	// EDF5.exe+2E0A7D, movss xmm6, dword ptr [rdi+1BD0]
	BYTE getDashBaseSpeed[] = {
		0xF3, 0x0F, 0x10, 0xB7, 0xD0, 0x1B, 0x00, 0x00
	};
	WriteHookToProcess((void*)(hmodEXE + 0x2E0A7D), &getDashBaseSpeed, 8U);
	static_assert(offsetof(EDFAssultSoldier_t, BaseDashSpeed) == 0x1BD0);

	// EDF5.exe+2E094F, allow change movement direction in dash
	hookGameBlockWithInt3((void*)(hmodEXE + 0x2E094F), (uintptr_t)ASMeArmySoldierDashTurning);
	WriteHookToProcess((void*)(hmodEXE + 0x2E094F + 15), (void*)&nop6, 6U);
	eArmySoldierDashTurningRetAddr = (uintptr_t)(hmodEXE + 0x2E09B8);
	// EDF5.exe+2E0B7F, no longer need to hold on the dash.
	BYTE dashIsNoHold[] = {
		0x80, 0xBF, 0x5B, 0x0B, 0x00, 0x00, 0x00, // cmp byte ptr [rdi+B5B], 0
		0x44, 0x0F, 0x28, 0x84, 0x24, 0x90, 0x00, 0x00, 0x00,
		0x40, 0x0F, 0x95, 0xC6                    // setne sil
	};
	WriteHookToProcess((void*)(hmodEXE + 0x2E0B7F), &dashIsNoHold, 20U);

	// EDF5.exe+EAD278, is vft+230
	uintptr_t newLoadAccessory = (uintptr_t)module_LoadAccessory_AssultSoldier;
	WriteHookToProcess((void*)(hmodEXE + 0xEAD048 + 0x230), &newLoadAccessory, 8U);
}

extern "C" {
	// wing diver!
	void __fastcall ASMePaleWingInitialization();
	uintptr_t vft_EAF348;
	void __fastcall ASMePaleWingLoadAccessory();
	// initialize emergency charge FX
	uintptr_t edf2FDB10Address;
	//
	void __fastcall ASMePaleWingActivateRechargeFX();
	uintptr_t edf2FBA30Address;
}

void module_SetFunction_PaleWing(PBYTE hmodEXE)
{
	// wing diver!
	int newPaleWingSize = 0x2000;
	// PaleWing 0x1DF0
	// start: 0x1E00, size: 0x18, function: extra emergency recharge
	WriteHookToProcessCheckECX((void*)(hmodEXE + 0x2F6D17 + 1), &newPaleWingSize, 4U);
	// EDF5.exe+2F7F46
	hookGameBlockWithInt3((void*)(hmodEXE + 0x2F7F46), (uintptr_t)ASMePaleWingInitialization);
	WriteHookToProcess((void*)(hmodEXE + 0x2F7F46 + 15), (void*)&nop3, 3U);
	vft_EAF348 = (uintptr_t)(hmodEXE + 0xEAF348);
	// initialize emergency charge FX
	edf2FDB10Address = (uintptr_t)(hmodEXE + 0x2FDB10);
	// EDF5.exe+2FB964, activate emergency recharge FX
	hookGameBlockWithInt3((void*)(hmodEXE + 0x2FB964), (uintptr_t)ASMePaleWingActivateRechargeFX);
	WriteHookToProcess((void*)(hmodEXE + 0x2FB964 + 15), (void*)&nop2, 2U);
	edf2FBA30Address = (uintptr_t)(hmodEXE + 0x2FBA30);
	// EDF5.exe+2F863E
	hookGameBlockWithInt3((void*)(hmodEXE + 0x2F863E), (uintptr_t)ASMePaleWingLoadAccessory);

	// Flying Speed, default is 0.4f
	//unsigned char newWDFlying[] = {0x51, 0xE5};
	//float WDspeedFly = 0.27f;
	// up to 2x
	unsigned char newWDFlying[] = { 0xB9, 0xE5 };
	float WDspeedFly = 0.55f;
	WriteHookToProcess((void*)(hmodEXE + 0x2F6F65 + 7), &WDspeedFly, 4U);
	WriteHookToProcess((void*)(hmodEXE + 0x2F848B + 4), &newWDFlying[0], 1U);
	// Takeoff Speed, default is 0.007f
	// float WDspeedTakeoff = 0.005f;
	//WriteHookToProcess((void *)(hmodEXE + 0x2F6F7B + 7), &WDspeedTakeoff, 4U);
	//WriteHookToProcess((void *)(hmodEXE + 0x2F84D3 + 4), &newWDFlying[1], 1U);

	// Flight Consumption, default is 0.25f
	// now it is 0.2f
	//unsigned char newWDFlyEnergy[] = {0x51, 0x9A
	// up to 0.4f
	unsigned char newWDFlyEnergy[] = { 0xB1, 0xFA };
	WriteHookToProcess((void*)(hmodEXE + 0x2F7263 + 4), &newWDFlyEnergy[0], 1U);
	WriteHookToProcess((void*)(hmodEXE + 0x2F861A + 4), &newWDFlyEnergy[1], 1U);
	// Emergency Charge, default is 0.2f
	// now it is 0.3f, EDF5.exe+2F85D3
	unsigned char newWDEmergencyCharge[] = { 0x11, 0xDD };
	WriteHookToProcess((void*)(hmodEXE + 0x2F85D3 + 4), &newWDEmergencyCharge, 2U);
	// EDF5.exe+2F724F
	unsigned char newWDEmergencyChargeInit = 0x95;
	WriteHookToProcess((void*)(hmodEXE + 0x2F724F + 4), &newWDEmergencyChargeInit, 1U);
}

extern "C" {
	// air raider!
	void __fastcall ASMeEngineerInitialization();
	void __fastcall ASMeEngineerUseAuxiliary();
	uintptr_t eEngineerUseAuxiliaryRetAddr;
	uintptr_t edf2E2E30Address;
}

void module_SetFunction_Engineer(PBYTE hmodEXE)
{
	// air raider!
	int newAirRaiderSize = 0x2000;
	// Engineer 0x1A70
	// start: 0x1AE0, size: 8, function: throw button timer.
	WriteHookToProcessCheckECX((void*)(hmodEXE + 0x2E2057 + 1), &newAirRaiderSize, 4U);
	// EDF5.exe+2E2347
	hookGameBlockWithInt3((void*)(hmodEXE + 0x2E2347), (uintptr_t)ASMeEngineerInitialization);
	WriteHookToProcess((void*)(hmodEXE + 0x2E2347 + 15), (void*)&nop1, 1U);
	// offset is 0x2E197A
	eEngineerUseAuxiliaryRetAddr = (uintptr_t)(hmodEXE + 0x2E25FC);
	hookGameBlockWithInt3((void*)(hmodEXE + 0x2E257A), (uintptr_t)ASMeEngineerUseAuxiliary);
	WriteHookToProcess((void*)(hmodEXE + 0x2E257A + 15), (void*)&nop3, 3U);
	edf2E2E30Address = (uintptr_t)(hmodEXE + 0x2E2E30);

	// EDF5.exe+EAD650, is vft+230
	uintptr_t newLoadAccessory = (uintptr_t)module_LoadAccessory_Engineer;
	WriteHookToProcess((void*)(hmodEXE + 0xEAD420 + 0x230), &newLoadAccessory, 8U);
}

extern "C" {
	// General
	void __fastcall ASMeAccessoryEnhancement();
	//
}

void module_SetFunction_EDFCommon(PBYTE hmodEXE)
{
	// Add new accessory functions, EDF5.exe+304AE5
	//hookGameBlockWithInt3((void*)(hmodEXE + 0x304AE5), (uintptr_t)ASMeAccessoryEnhancement);
	//WriteHookToProcess((void*)(hmodEXE + 0x304AE5 + 15), (void*)&nop1, 1U);
	module_EDFCommonFunctionInitialization(hmodEXE);
	// ofs+3039fd, is 500
}


#include "Vehicle/XGS_VehicleBegaruta.h"

extern "C" {
	// Barga
	void __fastcall ASMVehicle501AnimationEvent();
}

void module_SetFunction_EDFVehicle(PBYTE hmodEXE)
{
	// EDF5.exe+33D000
	// Allow Barga to use the weapon
	hookGameBlockWithInt3((void*)(hmodEXE + 0x33D000), (uintptr_t)ASMVehicle501AnimationEvent);
	WriteHookToProcess((void*)(hmodEXE + 0x33D000 + 15), (void*)&nop5, 5U);
	// EDF5.exe+3B9695 
	// Allow Barga to directly attack anchor
	int BargaDamageFlag = 0x47; // old is 7, 0x40 is hit anchor
	WriteHookToProcess((void*)(hmodEXE + 0x3B9695 + 6), &BargaDamageFlag, 4U);

	xgs_VehicleBegaruta_Init(hmodEXE);
}
