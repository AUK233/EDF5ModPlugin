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

void module_SetEDFSeriesFunction(PBYTE hmodEXE)
{
	module_SetFunction_AssultSoldier(hmodEXE);
	module_SetFunction_PaleWing(hmodEXE);
	module_SetFunction_Engineer(hmodEXE);
	module_SetFunction_HeavyArmor(hmodEXE);

	module_SetFunction_EDFCommon(hmodEXE);
	module_SetFunction_EDFVehicle(hmodEXE);
	module_SetFunction_EDFAmmo(hmodEXE);
	module_SetFunction_EDFWeapon(hmodEXE);
}

extern "C" {
	// ranger!
	void __fastcall ASMeAssultSoldierInitialization();
	void __fastcall ASMeArmySoldierUseAuxiliary();
	uintptr_t eArmySoldierUseAuxiliaryRetAddr;
	// vehicle_call
	uintptr_t edf2E0270Address;
	// ranger dash
	uintptr_t edf2E07C0Address;
	// Execution?
	uintptr_t edf2E18A0Address;
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
	WriteHookToProcessCheckECX((void*)(hmodEXE + 0x2DF9C7 + 1), &newRangerSize, 4U);
	static_assert(offsetof(EDFAssultSoldier_t, ThrowButtonTimer) == 0x1BE0);
	static_assert(offsetof(EDFAssultSoldier_t, ThrowButtonCD) == 0x1BE4);
	// EDF5.exe+2DFD0D
	hookGameBlockWithInt3((void*)(hmodEXE + 0x2DFD0D), (uintptr_t)ASMeAssultSoldierInitialization);
	WriteHookToProcess((void*)(hmodEXE + 0x2DFD0D + 15), (void*)&nop1, 1U);
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
	// Show 2nd support slot
	hudShowSupportSlot2RetAddr = (uintptr_t)(hmodEXE + 0x4D7A7F);
	hookGameBlockWithInt3((void*)(hmodEXE + 0x4D7A70), (uintptr_t)ASMhudShowSupportSlot2);

	// EDF5.exe+2E0A7D, movss xmm6, dword ptr [rdi+1BD0]
	BYTE getDashBaseSpeed[] = {
		0xF3, 0x0F, 0x10, 0xB7, 0xD0, 0x1B, 0x00, 0x00
	};
	WriteHookToProcess((void*)(hmodEXE + 0x2E0A7D), &getDashBaseSpeed, 8U);
	static_assert(offsetof(EDFAssultSoldier_t, BaseDashSpeed) == 0x1BD0);

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
	// fencer!
	void __fastcall ASMeHeavyArmorInitialization();
	void __fastcall ASMeHeavyArmorSetDashCoolDown();
	// Swap boost and dash
	uintptr_t edf11B24E0Address;
	uintptr_t edf11B1AB0Address;
	void __fastcall ASMeFencerJetSetup();
	//
	uintptr_t ofs3073C0JmpAddr;
	uintptr_t ofs2E4070JmpAddr;
	uintptr_t ofs2E42C0JmpAddr;
	uintptr_t ofs2E43E0JmpAddr;
	uintptr_t ofs2E4500JmpAddr;
	void __fastcall ASMeFencerBoostAndDash();
}

void module_SetFunction_HeavyArmor(PBYTE hmodEXE)
{
	// fencer!
	int newFencerSize = 0x2000;
	// HeavyArmor 0x1C30
	// start:0x1C30, size:0x10, function: swap types.
	WriteHookToProcessCheckECX((void*)(hmodEXE + 0x2E3408), &newFencerSize, 4U);
	static_assert(offsetof(EDFHeavyArmor_t, IsDashToBoost) == 0x1C30);
	static_assert(offsetof(EDFHeavyArmor_t, IsBoostToDash) == 0x1C34);
	//WriteHookToProcess((void *)(hmodEXE + 0x2E4229 + 1), &newFencerSize, 4U);
	//WriteHookToProcess((void *)(hmodEXE + 0xC61AC9 + 1), &newFencerSize, 4U);
	// EDF5.exe+2E41CE
	hookGameBlockWithInt3((void*)(hmodEXE + 0x2E41CE), (uintptr_t)ASMeHeavyArmorInitialization);
	WriteHookToProcess((void*)(hmodEXE + 0x2E41CE + 15), (void*)&nop3, 3U);

	// EDF5.exe+EADD18, is vft+230
	uintptr_t newLoadAccessory = (uintptr_t)module_LoadAccessory_HeavyArmor;
	WriteHookToProcess((void*)(hmodEXE + 0xEADAE8 + 0x230), &newLoadAccessory, 8U);

	// +1BA0h, default is 240, EDF5.exe+2E4D38
	static_assert(offsetof(EDFHeavyArmor_t, BoostRecoveryBaseTime) == 0x1C38);
	//mov eax, dword ptr[rbx + 1C38]
	//mov dword ptr[rdi + 10], eax
	//nop
	BYTE newBoostCD[] = {
		0x8B, 0x83, 0x38, 0x1C, 0x00, 0x00,
		0x89, 0x47, 0x10,
		0x90
	};
	WriteHookToProcess((void*)(hmodEXE + 0x2E4D38), &newBoostCD, 10U);
	// +1BB0h, default is 90, EDF5.exe+2E54E4
	static_assert(offsetof(EDFHeavyArmor_t, DashRecoveryBaseTime) == 0x1C3C);
	hookGameBlockWithInt3((void*)(hmodEXE + 0x2E54E4), (uintptr_t)ASMeHeavyArmorSetDashCoolDown);
	WriteHookToProcess((void*)(hmodEXE + 0x2E54E4 + 15), (void*)&nop5, 5U);
	// EDF5.exe+2E53BB, 60 to 90
	BYTE newDashSpeed[] = {
		0xF3, 0x0F, 0x10, 0x0D, 0x09, 0x13, 0xC0, 0x00
	};
	WriteHookToProcess((void*)(hmodEXE + 0x2E53BB), &newDashSpeed, 8U);
	// 0x2E387B 0.5f to 1.25f
	//unsigned char newDashInterval[] = { 0xED, 0x1F };
	//WriteHookToProcess((void*)(hmodEXE + 0x2E447B + 4), &newDashInterval, 2U);

	// Swap boost and dash Installation
	edf11B24E0Address = (uintptr_t)(hmodEXE + 0x11B24E0);
	edf11B1AB0Address = (uintptr_t)(hmodEXE + 0x11B1AB0);
	// EDF5.exe+2E43BC, remove old boost speed
	unsigned char removeBoostSpeed[] = {
		0x89, 0xBB, 0xA4, 0x1B, 0x00, 0x00, // mov dword ptr [rbx+1BA4h], edi
		0x89, 0xBB, 0xA8, 0x1B, 0x00, 0x00, // mov dword ptr [rbx+1BA8h], edi
		0x41, 0x8B, 0xFE,                   // mov edi, r14d
		0xEB, 0x1E,                         // jmp
		0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90,
		0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90,
		0x48                                // jmp target: cmp
	};
	WriteHookToProcess((void*)(hmodEXE + 0x2E43BC), &removeBoostSpeed, 48U);
	// EDF5.exe+2E4526
	hookGameBlockWithInt3((void*)(hmodEXE + 0x2E4526), (uintptr_t)ASMeFencerJetSetup);
	WriteHookToProcess((void*)(hmodEXE + 0x2E4526 + 15), (void*)&nop1, 1U);

	// Swap boost and dash Activate, EDF5.exe+2E4890 is vft+30
	ofs3073C0JmpAddr = (uintptr_t)(hmodEXE + 0x307FC0);
	ofs2E4070JmpAddr = (uintptr_t)(hmodEXE + 0x2E4C70);
	ofs2E42C0JmpAddr = (uintptr_t)(hmodEXE + 0x2E4EC0);
	ofs2E43E0JmpAddr = (uintptr_t)(hmodEXE + 0x2E4FE0);
	ofs2E4500JmpAddr = (uintptr_t)(hmodEXE + 0x2E5100);
	//hookGameBlock((void*)(hmodEXE + 0x2E4890), (uintptr_t)ASMeFencerBoostAndDash);
	//WriteHookToProcess((void*)(hmodEXE + 0x2E4890 + 12), (void*)&nop2, 2U);
	// EDF5.exe+EADB18
	uintptr_t newFencerBoostAndDash = (uintptr_t)ASMeFencerBoostAndDash;
	WriteHookToProcess((void*)(hmodEXE + 0xEADAE8 + 0x30), &newFencerBoostAndDash, 8U);
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

extern "C" {
	// Barga
	void __fastcall ASMVehicle501AnimationEvent();

	// Combat Frame
	void __fastcall ASMVehicle504CustomizeAIAimBone();
	uintptr_t Vehicle504CustomizeAIAimBoneRetAddr;
}

void module_SetFunction_EDFVehicle(PBYTE hmodEXE)
{
	// EDF5.exe+33D000
	// Allow Barga to use the weapon
	hookGameBlockWithInt3((void*)(hmodEXE + 0x33D000), (uintptr_t)ASMVehicle501AnimationEvent);
	WriteHookToProcess((void*)(hmodEXE + 0x33D000 + 15), (void*)&nop5, 5U);

	// EDF5.exe+363123
	hookGameBlockWithInt3((void*)(hmodEXE + 0x363123), (uintptr_t)ASMVehicle504CustomizeAIAimBone);
	WriteHookToProcess((void*)(hmodEXE + 0x363123 + 15), (void*)&nop6, 6U);
	Vehicle504CustomizeAIAimBoneRetAddr = (uintptr_t)(hmodEXE + 0x36313D);
}
