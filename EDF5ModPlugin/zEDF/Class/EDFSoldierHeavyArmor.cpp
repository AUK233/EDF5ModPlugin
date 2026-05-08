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
#include "EDFSoldierHeavyArmor.h"

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

	// virtual function
	uintptr_t eHeavyArmorFunc10;
	void __fastcall ASMeHeavyArmorFunc10(PG_HeavyArmor pIn, void* pRDX);
	uintptr_t eHeavyArmorFunc1B8;
	uintptr_t eHeavyArmorFunc1C8; // jump
	void __fastcall ASMeHeavyArmorFunc1C8(PG_HeavyArmor pIn, int iEDX);
}

void module_SetFunction_HeavyArmor(PBYTE hmodEXE)
{
	// fencer!
	int newFencerSize = 0x2000;
	// HeavyArmor 0x1C30
	// start:0x1C30, size:0x10, function: swap types.
	// start:0x1C40, size:0x10, function: sub weapon.
	WriteHookToProcessCheckECX((void*)(hmodEXE + 0x2E3407 + 1), &newFencerSize, 4U);
	//WriteHookToProcess((void *)(hmodEXE + 0x2E4229 + 1), &newFencerSize, 4U);
	//WriteHookToProcess((void *)(hmodEXE + 0xC61AC9 + 1), &newFencerSize, 4U);
	// EDF5.exe+2E41CE
	hookGameBlockWithInt3((void*)(hmodEXE + 0x2E41CE), (uintptr_t)ASMeHeavyArmorInitialization);
	WriteHookToProcess((void*)(hmodEXE + 0x2E41CE + 15), (void*)&nop3, 3U);

	// vft, EDF5.exe+EADAE8
	auto vft_fencer = (uintptr_t)(hmodEXE + 0xEADAE8);
	// allow use jump key to activate sub-weapon
	auto vft10 = vft_fencer + 0x10;
	eHeavyArmorFunc10 = *(uintptr_t*)vft10;
	auto newFunc10 = (uintptr_t)ASMeHeavyArmorFunc10;
	WriteHookToProcess((void*)vft10, &newFunc10, 8U);
	//
	auto vft1B8 = vft_fencer + 0x1B8;
	eHeavyArmorFunc1B8 = *(uintptr_t*)vft1B8;
	auto vft1C8 = vft_fencer + 0x1C8;
	eHeavyArmorFunc1C8 = *(uintptr_t*)vft1C8;
	auto newFunc1C8 = (uintptr_t)ASMeHeavyArmorFunc1C8;
	WriteHookToProcess((void*)vft1C8, &newFunc1C8, 8U);

	// +1BA0h, default is 240, EDF5.exe+2E4D38
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
