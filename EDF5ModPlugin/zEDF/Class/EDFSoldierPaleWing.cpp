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
}

void __fastcall module_LoadAccessory_PaleWing(PG_PaleWing pClass) {
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
	__m128 newBoostSpeed = { 2.5, 3, 2, 1 };
	newBoostSpeed = _mm_mul_ps(boostSpeed, newBoostSpeed);
	_mm_store_ps(pClass->BoostSpeed, newBoostSpeed);

	float deadWeight = EDFSoldier_GetAccessoryValue(pClass, 210, 1.0f, 0);
	pClass->DeadWeight = deadWeight;
	float weaponChargeSpeed = EDFSoldier_GetAccessoryValue(pClass, 206, 1.0f, 0);
	pClass->WeaponChargeSpeed = weaponChargeSpeed;

	float currentEnergy = PaleWing_GetAccessoryCurrentEnergy(pClass, 100.0f);
	pClass->TotalEnergy = currentEnergy;
	pClass->CurrentEnergy = currentEnergy;
	float csEnergy = currentEnergy / 100.0f;

	float chargeSpeed = EDFSoldier_GetAccessoryValue(pClass, 208, 1.0f, 0);
	pClass->ChargeSpeed = csEnergy * chargeSpeed * 0.2f; // old is 0.1
	float emergencyChargeSpeed = EDFSoldier_GetAccessoryValue(pClass, 209, 1.0f, 0);
	pClass->EmergencyChargeSpeed = csEnergy * emergencyChargeSpeed * 0.45f; // old is 0.2

	// now only consumption is calculated based on base energy.
	float baseEnergy = PaleWing_GetAccessoryCoreBaseEnergy(pClass);
	csEnergy = baseEnergy / 100.0f;

	float boostConsumption = EDFSoldier_GetAccessoryValue(pClass, 201, 1.0f, 0);
	pClass->BoostConsumption = csEnergy * boostConsumption * 9.0f; // old is 3
	float flightConsumption = EDFSoldier_GetAccessoryValue(pClass, 205, 1.0f, 0);
	pClass->FlightConsumption = csEnergy * flightConsumption * 0.6f; // old is 0.25

	module_LoadAccessory_ExtraWeapon((uintptr_t)pClass);
	module_LoadAccessoryInMission_PaleWing(pClass);
}
