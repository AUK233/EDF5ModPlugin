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
#include "HuiMoreCharacterModel.h"

extern "C" {
uintptr_t vft_ModelIndexCivilian;
uintptr_t vft_ModelIndexSoldier;
void __fastcall ASMHuiMoreChaModelSelButton();
uintptr_t HuiMoreChaModelSelButtonRetAddr;

// model index is 2
EDFHuiSelectButtonVFT vft_ModelIndexVeteran;
void* __fastcall ASMHuiBoxModelIndexVeteranFuncP0(void* pSource, void* pTarget);
void __fastcall ASMHuiBoxModelIndexVeteranFuncP10(void* pSource, void* pCharacter);

// model index is 3
EDFHuiSelectButtonVFT vft_ModelIndexElite;
void* __fastcall ASMHuiBoxModelIndexEliteFuncP0(void* pSource, void* pTarget);
void __fastcall ASMHuiBoxModelIndexEliteFuncP10(void* pSource, void* pCharacter);

// model index is 4
EDFHuiSelectButtonVFT vft_ModelIndexPioneer;
void* __fastcall ASMHuiBoxModelIndexPioneerFuncP0(void* pSource, void* pTarget);
void __fastcall ASMHuiBoxModelIndexPioneerFuncP10(void* pSource, void* pCharacter);
}

void module_SetHuiMoreCharacterModel(PBYTE hmodEXE)
{
	// add selection button of character model
	// EDF5.exe+52E08C
	hookGameBlockWithInt3((void*)(hmodEXE + 0x52E08C), (uintptr_t)ASMHuiMoreChaModelSelButton);
	WriteHookToProcess((void*)(hmodEXE + 0x52E08C + 15), (void*)&nop2, 2U);
	HuiMoreChaModelSelButtonRetAddr = (uintptr_t)(hmodEXE + 0x52E0B5);
	// EDF5.exe+52DFD8
	vft_ModelIndexCivilian = (uintptr_t)(hmodEXE + 0xED3128);
	vft_ModelIndexSoldier = (uintptr_t)(hmodEXE + 0xED3198);

	// new virtual function table
	// copy original
	memcpy(&vft_ModelIndexVeteran, (void*)(vft_ModelIndexSoldier - 8), sizeof(EDFHuiSelectButtonVFT));
	memcpy(&vft_ModelIndexElite, (void*)(vft_ModelIndexSoldier - 8), sizeof(EDFHuiSelectButtonVFT));
	memcpy(&vft_ModelIndexPioneer, (void*)(vft_ModelIndexSoldier - 8), sizeof(EDFHuiSelectButtonVFT));

	// set new function
	vft_ModelIndexVeteran.copyVFT = (uintptr_t)ASMHuiBoxModelIndexVeteranFuncP0;
	vft_ModelIndexVeteran.copyVFT8 = (uintptr_t)ASMHuiBoxModelIndexVeteranFuncP0;
	vft_ModelIndexVeteran.activeButton = (uintptr_t)ASMHuiBoxModelIndexVeteranFuncP10;

	vft_ModelIndexElite.copyVFT = (uintptr_t)ASMHuiBoxModelIndexEliteFuncP0;
	vft_ModelIndexElite.copyVFT8 = (uintptr_t)ASMHuiBoxModelIndexEliteFuncP0;
	vft_ModelIndexElite.activeButton = (uintptr_t)ASMHuiBoxModelIndexEliteFuncP10;

	vft_ModelIndexPioneer.copyVFT = (uintptr_t)ASMHuiBoxModelIndexPioneerFuncP0;
	vft_ModelIndexPioneer.copyVFT8 = (uintptr_t)ASMHuiBoxModelIndexPioneerFuncP0;
	vft_ModelIndexPioneer.activeButton = (uintptr_t)ASMHuiBoxModelIndexPioneerFuncP10;

	module_ShareCharacterModelColor(hmodEXE);
}

void module_ShareCharacterModelColor(PBYTE hmodEXE)
{
	// mov eax, dword ptr [rsi+76C]
	BYTE eaxTo0_6[] = {
		0x31, 0xC0,            // xor eax, eax
		0x0F, 0x1F, 0x40, 0x00 // nop
	};
	// EDF5.exe+50B2AE, write to player color
	WriteHookToProcess((void*)(hmodEXE + 0x50B2AE), &eaxTo0_6, 6U);
	// EDF5.exe+50AF03
	WriteHookToProcess((void*)(hmodEXE + 0x50AF03), &eaxTo0_6, 6U);
	// EDF5.exe+50E489, read color in menu
	WriteHookToProcess((void*)(hmodEXE + 0x50E489), &eaxTo0_6, 6U);
	WriteHookToProcess((void*)(hmodEXE + 0x50E506), &eaxTo0_6, 6U);
	// EDF5.exe+50EB87, write to color index
	WriteHookToProcess((void*)(hmodEXE + 0x50EB87), &eaxTo0_6, 6U);

	BYTE r9dTo0_8[] = {
		0x45, 0x31, 0xC9,            // xor r9d, r9d
		0x0F, 0x1F, 0x44, 0x00, 0x00 // nop
	};
	// EDF5.exe+304F4B, read player color in offline
	WriteHookToProcess((void*)(hmodEXE + 0x304F4B), &r9dTo0_8, 8U);
	// EDF5.exe+304EE4, read player color in online
	WriteHookToProcess((void*)(hmodEXE + 0x304EE4), &r9dTo0_8, 8U);

	// EDF5.exe+58D22C, read player color in menu
	BYTE RbpToRcx[] = {
		0x48, 0x89, 0xE9,
		0x90
	};
	WriteHookToProcess((void*)(hmodEXE + 0x58D22C), &RbpToRcx, 4U);
	int i188x4 = 0x188 * 4;
	WriteHookToProcess((void*)(hmodEXE + 0x58D23C + 3), &i188x4, 4U);

	// EDF5.exe+8FE34, setting player colors to default
	BYTE R13ToRcx[] = {
		0x4C, 0x89, 0xE9,
		0x90
	};
	WriteHookToProcess((void*)(hmodEXE + 0x8FE34), &R13ToRcx, 4U);
	WriteHookToProcess((void*)(hmodEXE + 0x8FE38 + 3), &i188x4, 4U);


	// EDF5.exe+50E3C1, preview the selected color in menu
	// imul rcx, r9, 4
	BYTE ignoreR10[] = {
		0x49, 0x6B, 0xC9, 0x04
	};
	WriteHookToProcess((void*)(hmodEXE + 0x50E3C1), &ignoreR10, 4U);

	// EDF5.exe+50F0CA
	BYTE r11dTo0[] = {
		0x45, 0x31, 0xDB,
		0x0F, 0x1F, 0x40, 0x00
	};
	WriteHookToProcess((void*)(hmodEXE + 0x50F0CA), &r11dTo0, 7U);
	// EDF5.exe+50EBDE
	WriteHookToProcess((void*)(hmodEXE + 0x50EBDE), &r11dTo0, 7U);

	// EDF5.exe+58D367
	BYTE esiTo0[] = {
		0x31, 0xF6,
		0x0F, 0x1F, 0x80, 0x00, 0x00, 0x00, 0x00
	};
	WriteHookToProcess((void*)(hmodEXE + 0x58D367), &esiTo0, 9U);

	// EDF5.exe+50C839
	//WriteHookToProcess((void*)(hmodEXE + 0x50C839), (void*)&nop9, 9U);
}

extern "C" {
extern uintptr_t edf4738B0Address;
extern uintptr_t edf6D8D0Address;
extern uintptr_t edf9C71C0Address;
extern uintptr_t edf607E0Address;
}

void __fastcall module_HuiPushChaModelSelButton(void* v125ABD0, void* pButtonVector, void* pCharacter) {
	func_Call4738B0 getTextWString = (func_Call4738B0)edf4738B0Address;
	WCHAR* pWstr;

	EDFHuiChgDlgMsgButton button[3];
	EDFHuiChgDlgMsgVector v_button[3];

	button[0].vf_table = &vft_ModelIndexVeteran.copyVFT;
	button[0].pCharacter = pCharacter;
	button[0].pVFTable = &button[0].vf_table;
	//pWstr = getTextWString(v125ABD0, L"ModelIndexCivilian");
	pWstr = getTextWString(v125ABD0, L"ModelIndexVeteran");
	ASMHuiSetCharacterModelButton(&v_button[0], pWstr, &button[0]);

	button[1].vf_table = &vft_ModelIndexElite.copyVFT;
	button[1].pCharacter = pCharacter;
	button[1].pVFTable = &button[1].vf_table;
	//pWstr = getTextWString(v125ABD0, L"ModelIndexSoldier");
	pWstr = getTextWString(v125ABD0, L"ModelIndexElite");
	ASMHuiSetCharacterModelButton(&v_button[1], pWstr, &button[1]);

	button[2].vf_table = &vft_ModelIndexPioneer.copyVFT;
	button[2].pCharacter = pCharacter;
	button[2].pVFTable = &button[2].vf_table;
	pWstr = getTextWString(v125ABD0, L"ModelIndexPioneer");
	ASMHuiSetCharacterModelButton(&v_button[2], pWstr, &button[2]);

	func_Call6D8D0 pushButtonToHui = (func_Call6D8D0)edf6D8D0Address;
	pushButtonToHui(pButtonVector, &v_button[0]);
	pushButtonToHui(pButtonVector, &v_button[1]);
	pushButtonToHui(pButtonVector, &v_button[2]);

	func_Call9C71C0 clearButtonVector = (func_Call9C71C0)edf9C71C0Address;
	clearButtonVector(&v_button[0], sizeof(EDFHuiChgDlgMsgVector), 3, (void*)edf607E0Address);
}
