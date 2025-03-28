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
#include "zEnemy/EnemyHook_GiantAnt.h"

extern "C" {
	__declspec(align(16)) LPCWSTR GiantAntDeathBoneList[4] = {
		L"kosi", L"hara", L"siri", L"kao"
	};

	void __fastcall ASMxgsOCgiantAnt();
	void __fastcall ASMGiantAntUpdateAttack();
	void __fastcall ASMGiantAntAnimationEvent();
	uintptr_t GiantAntAnimationEventRetAddr;
	void __fastcall ASMGiantAntNormalShot();
	uintptr_t GiantAntNormalShotRetAddr;
	uintptr_t GiantAntNormalShotFireRetAddr;
	void __fastcall ASMGiantAntFuncP10();
	uintptr_t GiantAntNormalShotAddr;

	void __fastcall ASMGiantAntDeathReaction();
	uintptr_t GiantAntDeathReactionRetAddr;
}

void module_EnemyHook_GiantAnt(PBYTE hmodEXE)
{

	// old is 0x1930
	// +1930h(8-Bytes) is ammo call address
	// +1938h(4-Bytes) is burst count, +193Ch(4-Bytes) is accuracy
	// +1940h(4-Bytes) is burst state (0 or 1), +1944h(4-Bytes) is current burst count
	// +1948h(4-Bytes) is shot count in new burst
	// +194Ch(4-Bytes) is burst interval, +1950h(4-Bytes) is burst interval count
	// +1960h(32-Bytes) is bone pointer for death
	int newGiantAntSize = 0x1980;
	WriteHookToProcessCheckECX((void*)(hmodEXE + 0x46A0C7 + 1), &newGiantAntSize, 4U);

	// hook GiantAnt extra features, EDF5.exe+46A0E8
	unsigned char jmpToGiantAntInit[] = {
		0xFF, 0x25, 0xBA, 0xFF, 0xFF, 0xFF
	};
	WriteHookToProcess((void*)(hmodEXE + 0x46A0E8), &jmpToGiantAntInit, 6U);
	uintptr_t GiantAntInitaddr = (uintptr_t)ASMxgsOCgiantAnt;
	WriteHookToProcess((void*)(hmodEXE + 0x46A0A8), &GiantAntInitaddr, 8U);

	// EDF5.exe+1FFD13 [rax+18h], Includes difficulty update object strength.
	hookGameBlock((void*)(hmodEXE + 0x1FFD13), (uintptr_t)ASMGiantAntUpdateAttack);
	WriteHookToProcess((void*)(hmodEXE + 0x1FFD13 + 12), (void*)&nop3, 3U);
	// EDF5.exe+1FE2BB, Update animation events
	hookGameBlockWithInt3((void*)(hmodEXE + 0x1FE2BB), (uintptr_t)ASMGiantAntAnimationEvent);
	WriteHookToProcess((void*)(hmodEXE + 0x1FE2BB + 15), (void*)&nop2, 2U);
	GiantAntAnimationEventRetAddr = (uintptr_t)(hmodEXE + 0x1FE2CC);
	// EDF5.exe+20024D, allows modify normal shot accuracy.
	// movss xmm6, dword ptr [rsi+193Ch]
	unsigned char AntShotAccuracy[] = {
		0xF3, 0x0F, 0x10, 0xB6, 0x3C, 0x19, 0x00, 0x00
	};
	WriteHookToProcess((void*)(hmodEXE + 0x20024D), &AntShotAccuracy, 8U);
	// EDF5.exe+2002EB, allows modify normal shot ammo.
	hookGameBlockWithInt3((void*)(hmodEXE + 0x2002EB), (uintptr_t)ASMGiantAntNormalShot);
	WriteHookToProcess((void*)(hmodEXE + 0x2002EB + 15), (void*)&nop10, 10U);
	GiantAntNormalShotRetAddr = (uintptr_t)(hmodEXE + 0x200304);
	GiantAntNormalShotFireRetAddr = (uintptr_t)(hmodEXE + 0x205041);
	// EDF5.exe+1FFD3D, allows modify continuous shot count.
	unsigned char AntBurstCount[] = {
		0x8B, 0x81, 0x38, 0x19, 0x00, 0x00, // mov eax, [rcx+1938h]
		0x89, 0x81, 0xB0, 0x13, 0x00, 0x00, // mov [rcx+13B0h], eax
		0xC3								// ret
	};
	WriteHookToProcess((void*)(hmodEXE + 0x1FFD3D), &AntBurstCount, 13U);
	// EDF5.exe+1FEAEA, set new continuous shot.
	hookGameBlockWithInt3((void*)(hmodEXE + 0x1FEAEA), (uintptr_t)ASMGiantAntFuncP10);
	WriteHookToProcess((void*)(hmodEXE + 0x1FEAEA + 15), (void*)&nop5, 5U);
	GiantAntNormalShotAddr = (uintptr_t)(hmodEXE + 0x1FFF40);

	// EDF5.exe+201389, update death reaction bone
	hookGameBlockWithInt3((void*)(hmodEXE + 0x201389), (uintptr_t)ASMGiantAntDeathReaction);
	WriteHookToProcess((void*)(hmodEXE + 0x201389 + 15), (void*)&nop2, 2U);
	GiantAntDeathReactionRetAddr = (uintptr_t)(hmodEXE + 0x20139A);
	// EDF5.exe+201709, read death reaction bone
	BYTE movRCX[] = {0x48, 0x8B, 0x8F};
	int ofsRCX = 0x1960;
	WriteHookToProcess((void*)(hmodEXE + 0x201709), (void*)&movRCX, 3U);
	WriteHookToProcess((void*)(hmodEXE + 0x201709 + 3), (void*)&ofsRCX, 4U);
	ofsRCX = 0x1968;
	WriteHookToProcess((void*)(hmodEXE + 0x201753), (void*)&movRCX, 3U);
	WriteHookToProcess((void*)(hmodEXE + 0x201753 + 3), (void*)&ofsRCX, 4U);
	ofsRCX = 0x1970;
	WriteHookToProcess((void*)(hmodEXE + 0x201798), (void*)&movRCX, 3U);
	WriteHookToProcess((void*)(hmodEXE + 0x201798 + 3), (void*)&ofsRCX, 4U);
	ofsRCX = 0x1978;
	WriteHookToProcess((void*)(hmodEXE + 0x2017DD), (void*)&movRCX, 3U);
	WriteHookToProcess((void*)(hmodEXE + 0x2017DD + 3), (void*)&ofsRCX, 4U);
}
