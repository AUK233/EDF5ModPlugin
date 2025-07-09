#pragma once
#include "CommonCustomStructure.hpp"
#include "EDFPointerStruct.hpp"

void HookMissionSeriesSet(PBYTE hmodEXE);

void __fastcall CustomMissionData_SetWeaponIconColor();
void __fastcall CustomMissionData_InitializationMore(PCustomMissionData pData);
extern "C" {
void __fastcall CustomMissionData_initialization(PCustomMissionData pData);
void __fastcall CustomMissionData_SetToCurrentMission(uintptr_t pSGONode, int nodeNum);
int __fastcall CustomMissionData_GetSGONodeType(uintptr_t pSGONode, int nodeIndex);
int __fastcall CustomMissionData_GetSGONodeCount(uintptr_t pSGONode, int nodeIndex);
int __fastcall CustomMissionData_GetSGOValue(uintptr_t pSGONode, int nodeIndex);
void __fastcall CustomMissionData_SetToPresetValue(PCustomMissionData pData, int presetType);
void __fastcall CustomMissionData_MissionClearSetup(Pedf125ABD8 p125ABD8);
// use cpp to get the value, not assembly to read it
float __fastcall CustomMissionData_GetCustomFactor(int index);
}
