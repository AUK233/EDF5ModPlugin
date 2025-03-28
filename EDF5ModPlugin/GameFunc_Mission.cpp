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
#include <cstdlib>

#include "commonNOP.h"
#include "utiliy.h"
#include "CommonCustomStructure.hpp"
#include "EDFPointerStruct.hpp"

#include "GameFunc_Mission.h"

extern "C" {
PCustomMissionData pMissionCustomData;

void __fastcall ASMloadMissionTimelineLayer();
uintptr_t loadMissionTimelineLayerRetAddr;
void __fastcall ASMcheckMissionEndCondition();
//
void __fastcall ASMpickupBoxRange();
uintptr_t pickupBoxRangeFRetAddr;
uintptr_t pickupBoxRangeTRetAddr;
//
void __fastcall ASMreadMissionSavaData();
uintptr_t readMissionSavaDataRetAddr;
}

void HookMissionSeriesSet(PBYTE hmodEXE)
{
	// EDF5.exe+114862
	// load customized mission data at the same time as the mission is loaded
	hookGameBlockWithInt3((void*)(hmodEXE + 0x114862), (uintptr_t)ASMloadMissionTimelineLayer);
	WriteHookToProcess((void*)(hmodEXE + 0x114862 + 15), (void*)&nop2, 2U);
	loadMissionTimelineLayerRetAddr = (uintptr_t)(hmodEXE + 0x1148B6);
	//
	pMissionCustomData = (PCustomMissionData)_aligned_malloc(sizeof(CustomMissionData_t), 16U);
	CustomMissionData_initialization(pMissionCustomData);

	// EDF5.exe+114C51
	// box counting after mission completion
	BYTE updateMissionClear[] = {
		0x48, 0x8B, 0x1D, 0x80, 0x5F, 0x14, 0x01, // mov rbx, [EDF5.exe+125ABD8]
		0x83, 0xFE, 0x01,                         // cmp esi, 1
		0x74, 0x31                                // je
	};
	WriteHookToProcess((void*)(hmodEXE + 0x114C51), &updateMissionClear, 12U);
	//
	hookGameBlockWithInt3((void*)(hmodEXE + 0x114C8E), (uintptr_t)ASMcheckMissionEndCondition);
	WriteHookToProcess((void*)(hmodEXE + 0x114C8E + 15), (void*)&nop1, 1U);

	// EDF5.exe+199139
	// relaunch the accumulated boxes after mission failure
	BYTE updateMissionFailBox[]= {
		0x41, 0x83, 0xF9, 0x02, // cmp r9d, 2
		0x77, 0x38,             // ja
		0xF3, 0x0F, 0x10, 0x0D, 0xE9, 0xD2, 0xD4, 0x00, // movss xmm1, 1.0f
		0xEB, 0x2E,             // jmp
		0x0F, 0x1F, 0x80, 0x00, 0x00, 0x00, 0x00,
		0x0F, 0x1F, 0x84, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x0F, 0x1F, 0x84, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x0F, 0x1F, 0x84, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x0F, 0x1F, 0x84, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x0F, 0x1F, 0x80, 0x00, 0x00, 0x00, 0x00
	};
	WriteHookToProcess((void*)(hmodEXE + 0x199139), &updateMissionFailBox, 62U);

	// add guaranteed pickup, offset is 0x198350
	hookGameBlock((void*)(hmodEXE + 0x198F50), (uintptr_t)ASMpickupBoxRange);
	WriteHookToProcess((void*)(hmodEXE + 0x198F50 + 12), (void*)&nop1, 1U);
	pickupBoxRangeFRetAddr = (uintptr_t)(hmodEXE + 0x198F5F);
	pickupBoxRangeTRetAddr = (uintptr_t)(hmodEXE + 0x198F64);

	// EDF5.exe+909DD
	// Sync of offline and online mission progress
	hookGameBlock((void*)(hmodEXE + 0x909DD), (uintptr_t)ASMreadMissionSavaData);
	readMissionSavaDataRetAddr = (uintptr_t)(hmodEXE + 0x90A1A);
}

void __fastcall CustomMissionData_InitializationMore(PCustomMissionData pData)
{
	pData->Class_SpeedX = 1.0f;
	pData->Ranger_DashSpeedX = 1.0f;
	pData->PaleWing_WeaponChargeX = 1.0f;
	pData->PaleWing_BoostSpeedX = 1.0f;
	pData->AirRaider_DelayX = 1.0f;
	pData->AirRaider_DamageX = 1.0f;
	pData->Fencer_DashX = 1.0f;
	pData->Fencer_BoostX = 1.0f;
}

void __fastcall CustomMissionData_initialization(PCustomMissionData pData)
{
	pData->armorBox = 16;
	pData->weaponBox = 4;
	pData->Ranger_SpeedX = 1.0f;
	pData->PaleWing_ChargeX = 1.0f;
	pData->AirRaider_CreditX = 1.0f;
	pData->Fencer_ChargeX = 1.0f;
	CustomMissionData_InitializationMore(pData);
}

void __fastcall CustomMissionData_SetToCurrentMission(uintptr_t pSGONode, int nodeNum)
{
	PCustomMissionData pData = pMissionCustomData;
	// If necessary parameters are missing, reset to default
	if (nodeNum <= 4) {
		CustomMissionData_initialization(pData);
		return;
	}

	uintptr_t SGONode = pSGONode + (4 * 12);
	int dataOffset = CustomMissionData_GetSGOValue(SGONode, 0);
	SGONode += dataOffset;
	pData->armorBox = CustomMissionData_GetSGOValue(SGONode, 0);
	pData->weaponBox = CustomMissionData_GetSGOValue(SGONode, 1);

	int nodeCount = CustomMissionData_GetSGONodeCount(pSGONode, 4);
	if (nodeCount < 3) {
		pData->Ranger_SpeedX = 1.0f;
		pData->PaleWing_ChargeX = 1.0f;
		pData->AirRaider_CreditX = 1.0f;
		pData->Fencer_ChargeX = 1.0f;
		CustomMissionData_InitializationMore(pData);
	}
	else {
		uintptr_t factorNode = SGONode + (2 * 12);
		int nodeValue = CustomMissionData_GetSGOValue(factorNode, 0);
		int nodeType = CustomMissionData_GetSGONodeType(factorNode, 0);
		if (nodeType == 1) {
			CustomMissionData_SetToPresetValue(pData, nodeValue);
		} else {
			factorNode += nodeValue;
			*(int*)&pData->Ranger_SpeedX = CustomMissionData_GetSGOValue(factorNode, 0);
			*(int*)&pData->PaleWing_ChargeX = CustomMissionData_GetSGOValue(factorNode, 1);
			*(int*)&pData->AirRaider_CreditX = CustomMissionData_GetSGOValue(factorNode, 2);
			*(int*)&pData->Fencer_ChargeX = CustomMissionData_GetSGOValue(factorNode, 3);
			CustomMissionData_InitializationMore(pData);
		}

		if (nodeCount >= 4) {
			*(int*)&pData->Class_SpeedX = CustomMissionData_GetSGOValue(SGONode, 3);
		}
	}
}

int __fastcall CustomMissionData_GetSGONodeType(uintptr_t pSGONode, int nodeIndex)
{
	int value = *(int*)(pSGONode + (nodeIndex * 12));
	return value;
}

int __fastcall CustomMissionData_GetSGONodeCount(uintptr_t pSGONode, int nodeIndex)
{
	int value = *(int*)(pSGONode + (nodeIndex * 12) + 4);
	return value;
}

int __fastcall CustomMissionData_GetSGOValue(uintptr_t pSGONode, int nodeIndex)
{
	int value = *(int*)(pSGONode + (nodeIndex * 12) + 8);
	return value;
}

void __fastcall CustomMissionData_SetToPresetValue(PCustomMissionData pData, int presetType)
{
	pData->Ranger_SpeedX = 1.0f;
	pData->PaleWing_ChargeX = 1.0f;
	pData->AirRaider_CreditX = 1.0f;
	pData->Fencer_ChargeX = 1.0f;
	CustomMissionData_InitializationMore(pData);
	switch (presetType)
	{
	case 101: {
		// most difficult battlefield
		pData->Ranger_SpeedX = 0.8f;
		pData->Ranger_DashSpeedX = 0.9f;

		pData->PaleWing_ChargeX = 0.75f;
		pData->PaleWing_BoostSpeedX = 0.75f;
		pData->PaleWing_WeaponChargeX = 1.6f;

		pData->AirRaider_CreditX = 5.0f;
		pData->AirRaider_DelayX = 5.0f;
		pData->AirRaider_DamageX = 0.5f;

		pData->Fencer_ChargeX = 1.6f;
		pData->Fencer_DashX = 0.75f;
		pData->Fencer_BoostX = 0.75f;
		return;
	}
	case 102: {

		pData->Ranger_SpeedX = 0.9f;
		pData->Ranger_DashSpeedX = 0.9f;

		pData->PaleWing_ChargeX = 0.8f;
		pData->PaleWing_BoostSpeedX = 0.8f;
		pData->PaleWing_WeaponChargeX = 1.5f;

		pData->AirRaider_CreditX = 4.0f;
		pData->AirRaider_DelayX = 4.0f;
		pData->AirRaider_DamageX = 0.75f;

		pData->Fencer_ChargeX = 1.5f;
		pData->Fencer_DashX = 0.8f;
		pData->Fencer_BoostX = 0.8f;

		return;
	}
	case 103: {
		pData->AirRaider_CreditX = 5.0f;
		return;
	}
	default: {
		return;
	}
	}
}

void __fastcall CustomMissionData_MissionClearSetup(Pedf125ABD8 p125ABD8)
{
	PCustomMissionData pData = pMissionCustomData;
	// weapon
	UINT32 BonusWeapon = p125ABD8->TotalWeaponBox - p125ABD8->AcquiredWeaponBox;
	BonusWeapon /= 2;
	BonusWeapon += pData->weaponBox;
	p125ABD8->TotalWeaponBox += BonusWeapon;
	p125ABD8->AcquiredWeaponBox += BonusWeapon;
	// armor
	UINT32 BonusArmor = p125ABD8->TotalArmorBox - p125ABD8->AcquiredArmorBox;
	BonusArmor /= 2;
	BonusArmor += pData->armorBox;
	p125ABD8->AcquiredArmorBox += BonusArmor;
	p125ABD8->TotalArmorBox += BonusArmor;
}

float __fastcall CustomMissionData_GetCustomFactor(int index)
{
	PCustomMissionData pData = pMissionCustomData;
	switch (index)
	{
	case 0:
		return pData->AirRaider_CreditX;
	case 1:
		return pData->AirRaider_DamageX;
	case 2:
		return pData->AirRaider_DelayX;
	default:
		break;
	}
	return 0.0f;
}
