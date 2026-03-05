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

#include "plog/Log.h"
#include "CommonCustomStructure.hpp"
#include "utiliy.h"
#include "commonNOP.h"
#include "XGS_WeaponBase.h"

void __fastcall debugAllocateMemorySize(void* ptr)
{
	size_t MemerySize = _aligned_msize(ptr, 0x10, 0);
	PLOG_INFO << "allocated memory size: 0x" << std::hex << MemerySize;
}

void __fastcall debugGetWeaponName(PG_WeaponBase pWeapon) {
	wchar_t wstr[64];
	wcscpy_s(wstr, pWeapon->Info.Name);
	PLOG_INFO << "weapon name: " << wstr;

	return debugAllocateMemorySize(pWeapon);
}
