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
#include "zEDF/EDFCommonFunction.h"

#include "EDFSoldierPaleWing.h"

extern "C" {
	extern uintptr_t eLoadClassAccessoryAddr;
	// wing diver!
	void __fastcall ASMePaleWingInitialization();
	uintptr_t vft_EAF348;
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


	// vft, EDF5.exe+EAF0B8
	auto vft_wingDiver = (uintptr_t)(hmodEXE + 0xEAF0B8);
	//vft + 230
	uintptr_t newLoadAccessory = (uintptr_t)module_LoadAccessory_PaleWing;
	WriteHookToProcess((void*)(vft_wingDiver + 0x230), &newLoadAccessory, 8U);

	// Flying Speed, default is 0.4f
	//unsigned char newWDFlying[] = {0x51, 0xE5};
	//float WDspeedFly = 0.27f;
	// up to 2x
	//unsigned char newWDFlying[] = { 0xB9, 0xE5 };
	//float WDspeedFly = 0.55f;
	//WriteHookToProcess((void*)(hmodEXE + 0x2F6F65 + 7), &WDspeedFly, 4U);
	//WriteHookToProcess((void*)(hmodEXE + 0x2F848B + 4), &newWDFlying[0], 1U);
	
	// Takeoff Speed, default is 0.007f
	// float WDspeedTakeoff = 0.005f;
	//WriteHookToProcess((void *)(hmodEXE + 0x2F6F7B + 7), &WDspeedTakeoff, 4U);
	//WriteHookToProcess((void *)(hmodEXE + 0x2F84D3 + 4), &newWDFlying[1], 1U);

	// Flight Consumption, default is 0.25f
	// now it is 0.2f
	//unsigned char newWDFlyEnergy[] = {0x51, 0x9A
	// up to 0.4f
	//unsigned char newWDFlyEnergy[] = { 0xB1, 0xFA };
	//WriteHookToProcess((void*)(hmodEXE + 0x2F7263 + 4), &newWDFlyEnergy[0], 1U);
	//WriteHookToProcess((void*)(hmodEXE + 0x2F861A + 4), &newWDFlyEnergy[1], 1U);
	// Emergency Charge, default is 0.2f
	// now it is 0.3f, EDF5.exe+2F85D3
	//unsigned char newWDEmergencyCharge[] = { 0x11, 0xDD };
	//WriteHookToProcess((void*)(hmodEXE + 0x2F85D3 + 4), &newWDEmergencyCharge, 2U);
	// EDF5.exe+2F724F
	//unsigned char newWDEmergencyChargeInit = 0x95;
	//WriteHookToProcess((void*)(hmodEXE + 0x2F724F + 4), &newWDEmergencyChargeInit, 1U);
}

void __fastcall module_LoadAccessory_PaleWing(PG_PaleWing pClass)
{
	// EDF5.exe+2F8450
	func_Call303E90 LoadClassAccessory = (func_Call303E90)eLoadClassAccessoryAddr;
	LoadClassAccessory(pClass);

	float flySpeed = EDFSoldier_GetAccessoryValue(pClass, 202, 1.0f, 0);
	pClass->FlyingSpeed = flySpeed * 0.4f;
	float flightDamping = EDFSoldier_GetAccessoryValue(pClass, 203, 1.0f, 0);
	pClass->FlightDamping = flightDamping * 0.01f;
	float takeoffSpeed = EDFSoldier_GetAccessoryValue(pClass, 204, 1.0f, 0);
	pClass->TakeoffSpeed = takeoffSpeed * 0.007f;

	__m128 boostSpeedDefault = { 1, 1, 1, 1 };
	__m128 boostSpeed;
	PaleWing_GetAccessoryBoostSpeed(pClass, &boostSpeed, 0, &boostSpeedDefault);
	_mm_store_ps(pClass->BoostSpeed, boostSpeed);

	float deadWeight = EDFSoldier_GetAccessoryValue(pClass, 210, 1.0f, 0);
	pClass->DeadWeight = deadWeight;
	float weaponChargeSpeed = EDFSoldier_GetAccessoryValue(pClass, 206, 1.0f, 0);
	pClass->WeaponChargeSpeed = weaponChargeSpeed;

	float currentEnergy = PaleWing_GetAccessoryCurrentEnergy(pClass, 100.0f);
	pClass->TotalEnergy = currentEnergy;
	pClass->CurrentEnergy = currentEnergy;
	float csEnergy = currentEnergy / 100.0f;

	// ok, skip base energy gain now to set recharge speed to a fixed value.

	float chargeSpeed = EDFSoldier_GetAccessoryValue(pClass, 208, 1.0f, 0);
	pClass->ChargeSpeed = csEnergy * chargeSpeed * 0.1f;
	float emergencyChargeSpeed = EDFSoldier_GetAccessoryValue(pClass, 209, 1.0f, 0);
	pClass->EmergencyChargeSpeed = csEnergy * emergencyChargeSpeed * 0.2f;

	float boostConsumption = EDFSoldier_GetAccessoryValue(pClass, 201, 1.0f, 0);
	pClass->BoostConsumption = csEnergy * boostConsumption * 3.0f;
	float flightConsumption = EDFSoldier_GetAccessoryValue(pClass, 205, 1.0f, 0);
	pClass->FlightConsumption = csEnergy * flightConsumption * 0.25f;

	module_LoadAccessory_ExtraWeapon((uintptr_t)pClass);
	module_LoadAccessoryInMission_PaleWing(pClass);
}
