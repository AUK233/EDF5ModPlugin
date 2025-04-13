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
#include "XGS_VehicleBegaruta.h"

extern "C" {

void __fastcall ASMVehicle504CustomizeAIAimBone();
uintptr_t Vehicle504CustomizeAIAimBoneRetAddr;

uintptr_t pVehicleBegarutaAIAimFunc;
uintptr_t pVehicleBegarutaAIAimActiveAddr;
void __fastcall ASMVehicleBegaruta_AIAim();

}

void xgs_VehicleBegaruta_Init(PBYTE hmodEXE)
{
	// EDF5.exe+363123
	hookGameBlockWithInt3((void*)(hmodEXE + 0x363123), (uintptr_t)ASMVehicle504CustomizeAIAimBone);
	WriteHookToProcess((void*)(hmodEXE + 0x363123 + 15), (void*)&nop6, 6U);
	Vehicle504CustomizeAIAimBoneRetAddr = (uintptr_t)(hmodEXE + 0x36313D);

	// EDF5.exe+364910, vtf+178
	pVehicleBegarutaAIAimFunc = (uintptr_t)(hmodEXE + 0x364910);
	//
	pVehicleBegarutaAIAimActiveAddr = (uintptr_t)(hmodEXE + 0x3637C8);
	// EDF5.exe+EB66A0
	uintptr_t VehicleBegaruta_AIAim = (uintptr_t)ASMVehicleBegaruta_AIAim;
	WriteHookToProcess((void*)(hmodEXE + 0xEB6528 + 0x178), &VehicleBegaruta_AIAim, 8U);

	// These are modified original functions.
	// To enable the rotation of the sub-turrets.
	
	//EDF5.exe+353ECC
	unsigned char ofs3532CC[] = {
		0x74, 0x3C
	};
	WriteHookToProcess((void*)(hmodEXE + 0x353ECC), &ofs3532CC, 2U);

	//EDF5.exe+3613E8
	WriteHookToProcess((void*)(hmodEXE + 0x3613E8), (void*)&nop5, 5U);

	// Aim active ==============================================================
	// EDF5.exe+363785
	//mov eax, dword ptr[rcx + 1B18]
	//mov dword ptr[rbp + 10], eax
	//xor eax, eax
	//mov dword ptr[rbp + 14], eax
	unsigned char ofs362B85[] = {
		0x8B, 0x81, 0x18, 0x1B, 0x00, 0x00,
		0x89, 0x45, 0x10,
		0x31, 0xC0,
		0x89, 0x45, 0x14
	};
	WriteHookToProcess((void*)(hmodEXE + 0x363785), &ofs362B85, 14U);
	WriteHookToProcess((void*)(hmodEXE + 0x363FBC), (void*)&nop4, 4U);
	WriteHookToProcess((void*)(hmodEXE + 0x363FBC + 4), (void*)&nop8, 8U);

	// EDF5.exe+3638D8
	// movsxd r15, dword ptr [rbp+10]
	unsigned char ofs362CD8[] = {
		0x3E, 0x4C, 0x63, 0x7D, 0x10,
		0x66, 0x90
	};
	WriteHookToProcess((void*)(hmodEXE + 0x3638D8), &ofs362CD8, 7U);

	// EDF5.exe+363DE6
	// mov edx, dword ptr [rbp+14]
	unsigned char ofs3631E6[] = {
		0x8B, 0x55, 0x14,
		0x0F, 0x1F, 0x80, 0x00, 0x00, 0x00, 0x00,
		0x0F, 0x1F, 0x84, 0x00, 0x00, 0x00, 0x00, 0x00
	};
	WriteHookToProcess((void*)(hmodEXE + 0x363DE6), &ofs3631E6, 18U);

	// EDF5.exe+363EB1
	// mov rdx, rsi
	unsigned char ofs3632B1[] = {
		0x48, 0x89, 0xF2,
		0x0F, 0x1F, 0x40, 0x00
	};
	WriteHookToProcess((void*)(hmodEXE + 0x363EB1), &ofs3632B1, 7U);

	//EDF5.exe+363CB4
	unsigned char ofs3630B4[] = {
		0x75, 0x14
	};
	WriteHookToProcess((void*)(hmodEXE + 0x363CB4), &ofs3630B4, 2U);
	// End ==============================================================
}
